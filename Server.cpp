
//#define _AFXDLL

//#include "stdafx.h"
#include "afxcoll.h"
#include "Server.h"
#include "winsock2.h"
#include <stdio.h>
#include "MyThread.h"
#include "Client.h"


CServer::CServer(u_short port): CMyThread()
{
	this->InitServer(port);
}

CServer::CServer(){}

int CServer::InitServer(u_short port)
{
  StopThread=FALSE;
  AcceptSocket=SOCKET_ERROR;
  m_port=port;   
  WSADATA wsaData;
  int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
  
  if (iResult != NO_ERROR)
  {
    printf("Error at WSAStartup()\n");
	closesocket(ListenSocket); 
	return -1;
  };
  if (!BindListenSocket()) 
  { 
   printf("port and soket not binding/n");
   closesocket(ListenSocket); 
   return -1;
  }
  return 0;
}

CServer::~CServer(void)
{
	RealiseServer();
}
void CServer::RealiseServer()
{	
	StopThread=TRUE;
	Sleep(120);//Ожидание завершения потока для довыполнения всех процедур в потоке
	clientPool->deleteAllThread();	
    shutdown(ListenSocket, SD_BOTH);//Дисконект на стороне сервера
	closesocket(ListenSocket);  
	WSACleanup();
}


BOOL CServer::BindListenSocket()
{ 
	  BOOL res=TRUE;
  service.sin_family = AF_INET;
  service.sin_addr.s_addr=htonl(INADDR_ANY);
  service.sin_port = htons(m_port);

   ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (ListenSocket == INVALID_SOCKET) {
    printf("Error at socket(): %ld\n", WSAGetLastError());
	closesocket(ListenSocket); 
    WSACleanup();
	res=false; 
  }
            
  if (bind(ListenSocket,(SOCKADDR*) &service,sizeof(service)) == SOCKET_ERROR) 
  {
    printf("bind() failed.\n");
    closesocket(ListenSocket);
	res=false;
   }
  
  if (listen( ListenSocket, SOMAXCONN) == SOCKET_ERROR) //SOMAXCONN
   {
   printf("Error listening on socket.\n");
	res=false;
   }

   int iMode = 1;  ioctlsocket(ListenSocket, FIONBIO, (u_long FAR*) &iMode);
	
   return res;
  }
SOCKET CServer::WaitConnect()
{
   	int size=sizeof(send_service);
	AcceptSocket = accept(ListenSocket, (SOCKADDR*)&send_service, &size);
   return AcceptSocket;
}

DWORD CServer::ThreadFunc()
{ 
// Цикл потока, ожидающий конекты
  while (!StopThread)
  {
	AcceptSocket=WaitConnect();
	while (AcceptSocket==SOCKET_ERROR)
	{
        AcceptSocket=WaitConnect();
		Sleep(60);
	}    
	   if (!StopThread) onConnected(); //Запуск обработчика при появлении конекта
  }
   printf("Server thread stop!!!\n");
	return 0;
}
BOOL CServer::onConnected()
{
   printf("-----------------------------------------\n");
   printf("----------New connect--------------------\n");
   printf("-----------------------------------------\n");
   printf("Connected is %s\n", inet_ntoa(send_service.sin_addr));
   printf("Wait sending message...\n");

  char* menuStr = "List gamers: 1\r\nStart game:  2\r\nQuit:        9\r\n";   
  
  CClient* client = new CClient(AcceptSocket);
	  strcpy_s(client->IP_Addr,inet_ntoa(send_service.sin_addr));
	
	  client->SendData(menuStr, strlen(menuStr));	  
	  client->clientPool=clientPool;//Присвоение указателя на текущий рабочий пулл создаваемому клиенту
	  client->Execute(client);
	  clientPool->addClient(client);

    return TRUE;
}
//Запуск потока ожидания конектов
BOOL CServer::StartWaitConnect()
{
	printf("Start Listen server on port %d\n",this->m_port);
	return Execute(this);
}

//Остановка потока
//Не использую terminate для возможности отработки всех команд в цикле потока

BOOL CServer::StopWaitConnect()
{
	StopThread=TRUE; 
    return TRUE;
}
