#include "Games.h"
#include "Client.h"
#include <stdlib.h>
#include <time.h>


CGames::CGames(void)
{
	type=2;
}

CGames::CGames(CClient *_client1, CClient *_client2)
{
	type=2;
	client1 = _client1;
	client2 = _client2;
	StopingThread = FALSE;
	client1->game_mode=1;//Game on client 1 started;
	client2->game_mode=1;//Game on client 2 started;
    hSemaphore = CreateSemaphore(NULL, 1, 1, NULL);
}

CGames::~CGames(void)
{
	
}

void CGames::Realize()
{
	CloseHandle(hSemaphore);
}

//Обработчик вызывается клиентом в момент обработки события от клиента
// обрабатывается семафор, не дающий одновременно вызвать обработчик несколькими клиентами

void CGames::eventFromClient(CClient *client)
{		
if (WaitForSingleObject(hSemaphore, 30000) == WAIT_FAILED) return;

	if (client->game_mode!=0)
	{	  

	char* winStr = "You have pressed the spacebar first and WIN! \n"; //Вы нажали пробел первым и победили
	char* falseStr = "You had not time to lose!!! \n"; //Вы не успели и проиграли

	char* winStr2 = "Your opponent rushed and you WIN! \n"; //Ваш противник поспешил и вы выйграли
	char* falseStr2 = "You were in a hurry and lost!!! \n"; // Вы поспешили и проиграли
			
	CClient *actionClient;
	CClient *otherClient;

//Выбор активного клиента, сделавшего ход
	if (client == client1)
	{
		actionClient = client1;
		otherClient = client2;
	}
	else
	{
		actionClient = client2;
		otherClient = client1;
	}
	
	if (currentNumber==3) 
	{
		printf("Client %d is WIN!!! \n", actionClient->getThreadHandle());
		printf("Client %d is false!!! \n", otherClient->getThreadHandle());
		    actionClient->SendData(winStr,   strlen(winStr));
			otherClient->SendData(falseStr, strlen(falseStr));
	}
	else
	{
		printf("Client %d is WIN!!! \n", otherClient->getThreadHandle());
		printf("Client %d is false!!! \n", actionClient->getThreadHandle());
		    otherClient->SendData(winStr2,   strlen(winStr2));
			actionClient->SendData(falseStr2, strlen(falseStr2));
	}

	  actionClient->game_mode=0; 
	  otherClient->game_mode=0;
	StopLoop = TRUE;

	Sleep(1000);
	client1->Realize();
	client2->Realize();
	
	}// if (client->game_mode!=0)

	if(hSemaphore != NULL) ReleaseSemaphore(hSemaphore, 1, NULL);
}

DWORD CGames::ThreadFunc(void)
{
	srand((unsigned int)time(NULL));

	printf("START GAME %d and %d. \n", client1->getThreadHandle(), client2->getThreadHandle());

	char* startGameStr = "Client is start game. \r\nPress the spacebar when you see the number 3 \n\r"; //��������� ������. ������� ������, ����� ������� ����� 3

//Сообщения клиентам о начале игры	
	client1->SendData(startGameStr, strlen(startGameStr));		
	client2->SendData(startGameStr, strlen(startGameStr));		
		
   StopLoop= FALSE;
   char numStr[2];

   //Цикл потока
   while ((!StopLoop))
   {
	   currentNumber = rand()%3+1;	
	    _itoa_s(currentNumber,numStr,10);	   
	  int delay = rand()%2000+2000;

	  client1->SendData(numStr, 2);
	  client1->SendData("\r\n",2);

	  client2->SendData(numStr, 2);
	  client2->SendData("\r\n",2);
	  
	   Sleep(delay);	
   }
   StopingThread = TRUE;
	
	return 0;
}
