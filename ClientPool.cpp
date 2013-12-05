#include "ClientPool.h"
#include "PoolSocket.h"
#include "Client.h"
#include "Games.h"
#include <stdlib.h>
#include <time.h>

CClientPool::CClientPool(void)
{
	StopingThread=FALSE;
}


CClientPool::~CClientPool(void)
{
  //	deleteAllThread();
}

void CClientPool::deleteNotWorkingThread()
{
  CMyThread* client;

  for (unsigned int i = 0; i< VectorClient.size();i++ )
  {
  //  client=dynamic_cast< CClient* >(VectorClient[i]);
	  client = VectorClient[i];
	if (!client->isActive())
//	if (client->StopingThread) 
	{
	//	client->Realize();		
		client->Terminate();
		delete client;

		VectorClient.erase(VectorClient.begin()+i);
		printf("ClientPool deleting client\n");
	}
  }    
}

void CClientPool::addListenSocket(SOCKET socket)
{
	CPoolSocket poolSocket(socket, TRUE);
	VectorSocket.push_back(poolSocket);
}

void CClientPool::addClient(CMyThread* client)	
{	
	VectorClient.push_back(client);
}

void CClientPool::printClients()
{
  CClient* client;

  for (unsigned int i = 0; i< VectorClient.size();i++ )
  {
    client=dynamic_cast< CClient* >(VectorClient[i]);
	client->EchoClient();	
  }//for  
}

void CClientPool::deleteAllThread()
{
   CMyThread* client;
  for (unsigned int i = 0; i< VectorClient.size();i++ )
  {
    //client=dynamic_cast< CClient* >(VectorClient[i]);	  
	client=VectorClient[i];	  
	//client->Realize();
	client->Terminate();	
	printf("Thread %d is closed.\n", client->getThreadHandle() ); 
	//printf("Thread is closed.\n"); 
	delete client;
	VectorClient.erase(VectorClient.begin()+i);
  }

  for (unsigned int i = 0; i< VectorSocket.size();i++ )
	{
		VectorSocket.erase(VectorSocket.begin()+i);		
	}
}

int CClientPool::countClients(void)
{
	int countClient=0;
	CMyThread* client_;
	unsigned int i = 0;
	while (i<VectorClient.size())
	{
		client_=VectorClient[i];	
		CClient *client;
		if (client=dynamic_cast< CClient* >(client_))
		{
			countClient++;				
		}		
		i++;
	}
	return countClient;
}

CMyThread* CClientPool::getRandomClient(CMyThread *client)
{
	vector <CMyThread*> vc;
	vector <CMyThread*>::iterator vc_Iter1;
	CClient *resClient;

	for (unsigned int i = 0; i< VectorClient.size();i++ )
    {
		CMyThread *client_=VectorClient[i];	
		CClient *gameClient;
		if (gameClient=dynamic_cast< CClient* >(client_))
			if ((gameClient!=client) && (gameClient->game_mode==0))
			{
				vc.push_back(gameClient);
			}//if (gameClient!=client)
	}//for

	srand((unsigned int)time(NULL));
	
	if (vc.size() == 0) 
	{	
		resClient=NULL;
	}
	else
	{
		int count = vc.size();
		int posClient = rand()%count;
		resClient = dynamic_cast< CClient* >( vc[posClient] );

		printf("Random position client is %d from %d \n", posClient, vc.size());

		//resClient = dynamic_cast< CClient* >( vc[0]);
	}
	
	 for (unsigned int i = 0; i< vc.size();i++ )
	 {    
		vc.erase(vc.begin()+i);
     }

	printf("countClients=%d \n", countClients());

	return resClient;
}

void CClientPool::listenPoolSocket()
{
/*	
	for (unsigned int i = 0; i< VectorSocket.size();i++ )
    {
        VectorSocket[i]
	  int BytesRecv = recv(m_socket, buffer, RecvBufferSize, 0);
		//   printf("Byte recive %d \n", BytesSend);
           if (BytesRecv<=0)
           {
			   if(WSAGetLastError()!=0)//disconect
				   {
					   printf("Client %d for ip %s is disconnected.\n",m_socket, IP_Addr);

				   }
			   if (BytesRecv==0)                   
                   {
						  printf("Client %d for ip %s is disconnected.\n",m_socket, IP_Addr);                          
                   }//disconect        
           }//if
	}//for 
*/
}


DWORD CClientPool::ThreadFunc()
{	
	while (1)
	{
		deleteNotWorkingThread();
		Sleep(1000);		
	}
		
	return 0;
}