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

int main(int argc, char **argv)
{
   CClientPool* clientPool = new CClientPool();	  

   clientPool->Execute(clientPool);
   
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

