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

}

void CClientPool::deleteNotWorkingThread()
{
	CMyThread* client;
	CClient *workClient;
	CGames *games;

	for (unsigned int i = 0; i< VectorClient.size();i++ )
	{
		client = VectorClient[i];

		if (!client->isActive()) //Если поток завершился
		{
			HANDLE idClient =  client->getThreadHandle();		

			VectorClient.erase(VectorClient.begin()+i);
			if (workClient = dynamic_cast< CClient* >(client)) 
			{
				workClient->Realize();
				printf("ClientPool deleting client %d\n", idClient);	
			}
			if (games = dynamic_cast< CGames* >(client))
			{
				games->Realize();
				printf("ClientPool deleting games %d\n", idClient);				
			}

			client->Terminate();
			delete client;

		}
	}    
}

//Обработка добавления потока в пулл потоков
//Реализована синхронизация семафорами
void CClientPool::addClient(CMyThread* client)	
{	
	if (WaitForSingleObject(hSemaphore, 30000) == WAIT_FAILED) return;
	VectorClient.push_back(client);
	if(hSemaphore != NULL) ReleaseSemaphore(hSemaphore, 1, NULL);
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
	CClient *workClient;
	CGames *games;

	for (unsigned int i = 0; i< VectorClient.size();i++ )
	{
		client=VectorClient[i];	
		HANDLE idClient =  client->getThreadHandle();	
		client->Terminate();	
		
		if (workClient = dynamic_cast< CClient* >(client)) 
		{
			workClient->Realize();
			printf("ClientPool deleting client %d\n", idClient);	
		}
		if (games = dynamic_cast< CGames* >(client))
		{
			games->Realize();
			printf("ClientPool deleting games %d\n", idClient);				
		}
		delete client;
		VectorClient.erase(VectorClient.begin()+i);
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
	if (WaitForSingleObject(hSemaphore, 30000) == WAIT_FAILED) return NULL;

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

	srand((unsigned int)time(NULL));//Инициализация генератора случайных чисел

	if (vc.size() == 0) 
	{	
		resClient=NULL;
	}
	else
	{
		int count = vc.size();
		int posClient = rand()%count;
		resClient = dynamic_cast< CClient* >( vc[posClient] );

		printf("Random position client is %d from %d \n\r", posClient, vc.size());
	}

	for (unsigned int i = 0; i< vc.size();i++ )
	{    
		vc.erase(vc.begin()+i);
	}

	printf("countClients=%d \n", countClients());

	if(hSemaphore != NULL) ReleaseSemaphore(hSemaphore, 1, NULL);

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
