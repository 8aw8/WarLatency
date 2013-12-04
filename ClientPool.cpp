#include "ClientPool.h"
#include "Client.h"
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
  for (int i = 0; i< VectorClient.size();i++ )
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

void CClientPool::addClient(CMyThread* client)	
{
	VectorClient.push_back(client);
}

void CClientPool::printClients()
{
  CClient* client;

  for (int i = 0; i< VectorClient.size();i++ )
  {
    client=dynamic_cast< CClient* >(VectorClient[i]);
	client->EchoClient();	
  }//for  
}

void CClientPool::deleteAllThread()
{
   CMyThread* client;
  for (int i = 0; i< VectorClient.size();i++ )
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
}

int CClientPool::countClients(void)
{
	int countClient=0;
	CMyThread* client_;
	int i = 0;
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

	for (int i = 0; i< VectorClient.size();i++ )
    {
		CMyThread *client_=VectorClient[i];	
		CClient *gameClient;
		if (gameClient=dynamic_cast< CClient* >(client_))
			if ((gameClient!=client) && (gameClient->game_mode==0))
			{
				vc.push_back(gameClient);
			}//if (gameClient!=client)
	}//for

	srand(time(NULL));
	
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

	 for (int i = 0; i< vc.size();i++ )
	 {    
		vc.erase(vc.begin()+i);
     }

	printf("countClients=%d \n", countClients());

	return resClient;
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