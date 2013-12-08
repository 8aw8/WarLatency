// WarLatency.cpp: определяет точку входа для консольного приложения.
//
//#include <windows.h>
//#include <time.h>
//#define _AFXDLL

#include "afxcoll.h"

#include <stdio.h>
#include <conio.h>
#include "Server.h"
#include "ClientPool.h"
//#include <ctype.h>

#include <locale.h>



int main(int argc, char **argv)
{
	
	setlocale(LC_ALL, "rus");
    printf("%s", "Привет, мир! \n");

   CClientPool* clientPool = new CClientPool();	  

   clientPool->Execute();
   
   CServer* server = new CServer(12345);
   server->clientPool = clientPool;
   server->StartWaitConnect();

  _getch();
  
   server->StopWaitConnect();
   server->RealiseServer();

   clientPool->Terminate();
   clientPool->deleteAllThread();
  
  
   delete server; 
   delete clientPool;

   _getch();
   return 0;
}

