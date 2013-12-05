
//#define _AFXDLL

//#include "stdafx.h"
#include "afxcoll.h"
#include ".\server.h"
#include "winsock2.h"
#include <stdio.h>
#include "MyThread.h"
#include "Client.h"

#include <locale.h>

/*
void CharToWChar(const char * Text,wchar_t * &res)
{
	size_t convert=0;
	size_t bytes = strlen(Text)*2;
	res = (wchar_t*) calloc(bytes,sizeof(wchar_t*));
	setlocale(LC_CTYPE, "Russian_Russia.1251");
	mbstowcs(res,Text,bytes);
	//return res;
}
void WCharToChar(const wchar_t * Text,char * &Res)
{
	size_t convert=0;
	size_t bytes = wcslen(Text)*2;
	Res = (char *)calloc(bytes,1);
	setlocale(LC_CTYPE, "Russian_Russia.1251");
	wcstombs(Res, Text, bytes);	
}

LPWSTR StringToUnicode(LPCSTR sString)
{
    int iLength;
    iLength = MultiByteToWideChar
                (
                CP_ACP,
                0,
                sString,
                -1,
                NULL,
                0
                );
    if (iLength)
    {
        LPWSTR szWideCharStr = new WCHAR[iLength];
        MultiByteToWideChar
                (
                CP_ACP,
                0,
                sString,
                -1,
                szWideCharStr,
                iLength
                );
        return szWideCharStr;
    }
    return NULL;
}

*/

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
	Sleep(120);
	clientPool->deleteAllThread();	
    shutdown(ListenSocket, SD_BOTH);
	closesocket(ListenSocket);  
	WSACleanup();
}


bool CServer::BindListenSocket()
{ 
	  bool res=true;
  service.sin_family = AF_INET;
 // service.sin_addr.s_addr = inet_addr(Server_ip);
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
  while (!StopThread)
  {
	AcceptSocket=WaitConnect();
	while (AcceptSocket==SOCKET_ERROR)
	{
        AcceptSocket=WaitConnect();
		Sleep(60);
	}    
	   if (!StopThread) onConnected();
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

//  wchar_t * buf;
//  CharToWChar(menuStr, buf);

  CClient* client = new CClient(AcceptSocket);
	  strcpy_s(client->IP_Addr,inet_ntoa(send_service.sin_addr));
	 // strcpy(client->IP_Addr,inet_ntoa(send_service.sin_addr));
	  client->SendData(menuStr, strlen(menuStr));	  
	   
	//  client->SendData((char *)buf, strlen((menuStr))*2);	  

	  client->clientPool=clientPool;
	  client->Execute();
	  clientPool->addClient(client);
	  clientPool->addListenSocket(AcceptSocket);

    return TRUE;
}
BOOL CServer::StartWaitConnect()
{
	printf("Start Listen server on port %d\n",this->m_port);
	return Execute();
}

BOOL CServer::StopWaitConnect()
{
	StopThread=TRUE;
 //	terminate();
    return TRUE;
}
