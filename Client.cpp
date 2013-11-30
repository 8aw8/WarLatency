//#define _AFXDLL

#include "afxcoll.h"

//#include "stdafx.h"
#include ".\client.h"
//#include ".\server.h"
#include <iostream>
//#include "ServiceDiagnostics.h"
//#include "ReplicPacket.h"
//#include "DBThread.h"
//#include "inifile.h"
//#include "PacketParser.h"

int RecvBufferSize=1024;


//#include <Pkfuncs.h>

/*
void printPacket(char* Packet, int Len)
{
    printf("packet: ");
    for(int i=0; i<Len; i++)
         printf(" %d",Packet[i]);
    printf("\n");
} 
*/

CClient::CClient(SOCKET socket) :CMyThread()
{
	m_socket=socket;
	StopingThread=FALSE;
}
CClient::~CClient(void)
{
  //Realize();
}
void CClient::Realize()
{
	shutdown(m_socket, SD_BOTH);
	closesocket(m_socket);
	//CloseHandle(m_hThread);
}

DWORD CClient::ThreadFunc()
{
  int BytesSend = -1; 
    
  //printf("Clients Socket= %d\n",m_socket);
  BOOL StopLoop= FALSE;

//  CPacketParser parser; 
//  char* SendBuffer = new char[RecvBufferSize]; 
//  MDIS_packet* pack;

 // while (!StopLoop)
 // {
	
//  }// while (!StopLoop)	 
  
  //InterlockedIncrement((long*)&StopingThread);			   
//  delete [] SendBuffer;
  this->Realize();
  StopingThread=TRUE;
  
  return 0;
}

void CClient::OnRecvPacket(char* buffer, int BufferSize, _HeadPacket head)
{
	//0x200, 0x201, 0x202, 0x700
//	_HeadPacket OutHead;
	
	printf("-----------BODY PACKET------------------\n");
	
}


void CClient::EchoClient()
{
  printf("This is client socket = %d \n", m_socket);
}
BOOL CClient::WorkTypePacket(_HeadPacket head)
{
	BOOL res=FALSE;
    if (TrueMDISPacket(head))
	{
       // printf("%s\n",head.prefix);
	  if (head.PacketType==0x10)
	  {
		switch (head.CodCommand)
		{
		case 0x100:
			  res=TRUE;
			 break;
		case 0x101:
			  res=TRUE;
			 break;
		case 0x200:
			  res=TRUE;
			 break;
		case 0x201:
			  res=TRUE;
			 break;
		case 0x202:
			  res=TRUE;
			 break;
		case 0x700:
			  res=TRUE;
			 break;
		default:
			 res=FALSE;
			 break;
	    }//switch (head.CodCommand)
	  }//if
	}//if
	return res;
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

BOOL CClient::TrueMDISPacket(_HeadPacket head)
{
	char MDIS[4];
	MDIS[0]='M';
	MDIS[1]='D';
	MDIS[2]='I';
	MDIS[3]='S';
	int i=0;
	
	while (i<=3)
	{
		if (!(head.prefix[i]==MDIS[i])) return FALSE;
		i++;
	}
	return TRUE;
}
void CClient::PrintHeadPacket(_HeadPacket head)
{
	     printf("//======= Send/Reseive ====\n");
		 printf("  Prefix: %s \n",head.prefix);
		 printf("  Type: 0x%x \n",head.PacketType);
		 printf("  Command: 0x%x \n",head.CodCommand);
		 printf("  Request ID: 0x%x \n",head.Request_ID);
		 printf("  Data Size: %ld \n",head.PacketSize);
	     printf("//========================\n");
}
