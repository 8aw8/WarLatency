// WarLatency.cpp: определяет точку входа для консольного приложения.
//
//#include <windows.h>
//#include <time.h>
//#define _AFXDLL

#include "afxcoll.h"

#include <stdio.h>
#include <conio.h>
#include "Server.h"
//#include <ctype.h>




int main(int argc, char **argv)
{
	printf("Hello world \n");

	CServer* server = new CServer(12345);
    server->StartWaitConnect();


//	server->RealiseServer();
   
//	delete server; 


	_getch();

	//server->StopWaitConnect();
	server->RealiseServer();

    delete [] server; 
}

