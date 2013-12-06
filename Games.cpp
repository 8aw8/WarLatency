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
	hSemaphore = hSemaphore = CreateSemaphore(NULL, 1, 1, NULL);
}

CGames::~CGames(void)
{
	CloseHandle (hSemaphore);
}

void CGames::eventFromClient(CClient *client)
{		
	if (client->game_mode!=0)
	{
	  if (WaitForSingleObject(hSemaphore, 30000) == WAIT_FAILED) return;

	char* winStr = "You have pressed the spacebar first and WIN! \n"; //Вы нажали пробел первым и победили
	char* falseStr = "You had not time to lose!!! \n"; //Вы не успели и проиграли

	char* winStr2 = "Your opponent rushed and you WIN! \n"; //Ваш противник поспешил и вы выйграли
	char* falseStr2 = "You were in a hurry and lost!!! \n"; // Вы поспешили и проиграли

		
	CClient *actionClient;
	CClient *otherClient;

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
			printf("Client %d is WIN!!! \n", actionClient->m_socket);
			printf("Client %d is false!!! \n", otherClient->m_socket);
		    actionClient->SendData(winStr,   strlen(winStr));
			otherClient->SendData(falseStr, strlen(falseStr));
	}
	else
	{
		    printf("Client %d is WIN!!! \n", otherClient->m_socket);
			printf("Client %d is false!!! \n", actionClient->m_socket);
		    otherClient->SendData(winStr2,   strlen(winStr2));
			actionClient->SendData(falseStr2, strlen(falseStr2));
	}

	  actionClient->game_mode=0; 
	  otherClient->game_mode=0;

	
	if(hSemaphore != NULL)
    ReleaseSemaphore(hSemaphore, 1, NULL);
	
	Sleep(1000);
	client1->Realize();
	client2->Realize();
	StopLoop = TRUE;
	}// if (client->game_mode!=0)
}

DWORD CGames::ThreadFunc(void)
{
	srand((unsigned int)time(NULL));

	char* startGameStr = "Client is start game.  Press the spacebar when you see the number 3 \n"; //��������� ������. ������� ������, ����� ������� ����� 3
		
	client1->SendData(startGameStr, strlen(startGameStr));		
	client2->SendData(startGameStr, strlen(startGameStr));		
		
   StopLoop= FALSE;
   char numStr[2];

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
