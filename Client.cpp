
#include "afxcoll.h"
#include <iostream>
#include "Client.h"
#include "Games.h"


CClient::CClient(SOCKET socket) :CMyThread()
{	
	m_socket=socket;
	StopingThread=FALSE;
	RecvBufferSize=8192;
	command[0]=0;
	commandSize=0;	
	type=1;//Class CClient
	game_mode=0;// Game mode: 0 - Select game; 1 - Game started;
}
CClient::~CClient(void)
{
  
}
void CClient::Realize()
{
	StopLoop = TRUE;
	shutdown(m_socket, SD_BOTH);
	closesocket(m_socket);
}

DWORD CClient::ThreadFunc()
{	
	CClient* localClient = (CClient*)threadParam; //Получение указателя на область данных, с которой будет работать поток, обычно текущий экземпляр класса.

	int BytesRecv = -1;     
	printf("Clients Socket= %d\n",localClient->m_socket);

//Цикл обработки потока 
//Слушает сокет,при появлении данных в сокете запускает обработчик OnRecvPacket.
   StopLoop= FALSE;
  while ((!localClient->StopLoop))
   {
           Sleep(60);               
		   BytesRecv = recv(localClient->m_socket, localClient->SendBuffer, localClient->RecvBufferSize, 0);		
           if (BytesRecv<=0)
           {
			   if(handlErr(WSAGetLastError())!=0)//disconect
				   {
					   localClient->StopLoop=TRUE;
					   printf("Client %d for ip %s is disconnected.\n",m_hThread, IP_Addr);

				   }
			   if (BytesRecv==0)                   
                   {
						  localClient->StopLoop=TRUE;
			              printf("Client %d for ip %s is disconnected.\n",m_hThread, IP_Addr);                         
                   }//disconect        
           }
		   else
		   {  
			  if (BytesRecv>0) OnRecvPacket(localClient->SendBuffer, BytesRecv);//Запуск обработчика получения данных из сокета			   
		   }		   		   
  }// while (!StopLoop)         
  
  return 0;
}

char* CClient::setCommand(char* buffer, int BufferSize)
{
	int i = 0;
	int endCommand = -1;

	while (i<BufferSize)
	{
		if (buffer[i]=='\n')
		{
			commandSize=commandSize+i;
			endCommand=0;
			i=BufferSize;
		}						
		else 
			command[i+commandSize] = buffer[i];
		i++;
	}

	if (endCommand==0)
	{
		command[commandSize]=0;
		commandSize=0;
		return command;
	}
	else
	{
		commandSize = commandSize+BufferSize;
		command[commandSize]=0;
		return NULL;
	}	

	return NULL;
}


void CClient::startGame(void)
{
	char *str1 = "Client not found. \n\r";
		
	//Получение рабочего клиента из числа подключившихся
	CMyThread *_client_ = clientPool->getRandomClient(this);
	if (_client_==NULL)
	{
		this->SendData(str1, strlen(str1));
	}
	else
	{
		CClient *gameClient = dynamic_cast< CClient* >(_client_);
		CGames *games = new CGames(this,gameClient);//Создание и запуск потока обрабатывающего игру
		clientPool->addClient(games);//Добавление в пулл потоков 
			  this->runGames=games;//Сохранить указатель на класс игры у играющих клиентов
		gameClient->runGames=games;//Сохранить указатель на класс игры у играющих клиентов
		games->Execute(games);//Запуск игры
	}
}


void CClient::OnRecvPacket(char* buffer, int BufferSize)
{	
	buffer[BufferSize]=0;

  if (game_mode==0)

//Обработка данных , полученных из сокета
//Из сокета прийти может все что угодно. для этого нужен обработчик вводимого буфера
	if (setCommand(buffer, BufferSize)!=NULL)
	{	
		switch (atoi(command))
		{
		case 1:					
			 break;				
		case 2:
			startGame();
			 break;				
		case 9:
			{		
				Realize();//Закрытие конекта отключение клиента.
			}
			 break;	
		default:
			std::cout << "Command not found!!!" <<std::endl;
			 break;
	    }//switch 
	}
	
	if (game_mode==1)// Клиент в игре 
	{
		CGames *games =dynamic_cast< CGames* >(runGames);

		int endCommand = -1;
		int i=0;
// Проверка команды во время игры поиск пробела в водимых данных
	   while (i<BufferSize)
       {
	       if (buffer[i]==' ')
		   {
			  endCommand=0;
			  i=BufferSize;
		   }								  
		   i++;
	   }//while

	   if (endCommand==0)  games->eventFromClient(this); //Ответ клиента на событие в игре, обрабатывается в потоке клиента
	} 
}

void CClient::SendData(char* buffer, int bufferSize) 
{
	send(m_socket, buffer, bufferSize,0); 
}

void CClient::EchoClient()
{ 
}

int CClient::handlErr(const int err)
{
    int res=0;
    switch(err)     
    {
         case WSANOTINITIALISED     //A successful WSAStartup call must occur before using this function.
              :res = err; break;
         case WSAENETDOWN          //The network subsystem has failed.
              :res = err; break;
         case WSAENOTCONN          //The socket is not connected.
              :res = err; break;
         case WSAENOTSOCK          //The descriptor is not a socket. 
              :res = err; break;
         case WSAESHUTDOWN          //The socket has been shut down; it is not possible to receive on a socket after shutdown has been invoked with how set to SD_RECEIVE or SD_BOTH.
              :res = err; break;
         case WSAECONNABORTED     //The virtual circuit was terminated due to a time-out or other failure. The application should close the socket as it is no longer usable.
              :res = err; break;
         case WSAETIMEDOUT          //The connection has been dropped because of a network failure or because the peer system failed to respond.
              :res = err; break;
         case WSAECONNRESET          //The virtual circuit was reset by the remote side executing a hard or abortive close. The application should close the socket as it is no longer usable. On a UPD-datagram socket this error would indicate that a previous send operation resulted in an ICMP Port Unreachable message;
              :res = err; break;
         
         //case WSAEFAULT:printf(" The buf parameter is not completely contained in a valid part of the user address space. "); break;
         //case WSAEINTR:printf(" The (blocking) call was canceled through WSACancelBlockingCall. "); break;
         //case WSAEINPROGRESS:printf(" A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function. "); break;
         //case WSAENETRESET:printf("The connection has been broken due to the keep-alive activity detecting a failure while the operation was in progress. "); break;          
         //case WSAEOPNOTSUPP:printf("MSG_OOB was specified, but the socket is not stream-style such as type SOCK_STREAM, OOB data is not supported in the communication domain associated with this socket, or the socket is unidirectional and supports only send operations. "); break;
         //case WSAEWOULDBLOCK:printf("The socket is marked as nonblocking and the receive operation would block. "); break;
         //case WSAEMSGSIZE:printf("The message was too large to fit into the specified buffer and was truncated. "); break;
         //case WSAEINVAL:printf("The socket has not been bound with bind, or an unknown flag was specified, or MSG_OOB was specified for a socket with SO_OOBINLINE enabled or (for byte stream sockets only) len was zero or negative. "); break;
         
         default: res =0; break;
    }
    return res;
} 


