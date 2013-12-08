#pragma once
#include "mythread.h"
#include "Client.h"

class CGames :public CMyThread
{
public:
	int type;
	BOOL StopingThread;
	CClient *client1;
	CClient *client2;

	int currentNumber;
	BOOL StopLoop;
	HANDLE hSemaphore;
		
	CGames(void);
	CGames(CClient *_client1, CClient *_client2);	
	~CGames(void);

	void eventFromClient(CClient *client);
	void Realize(); 

protected:	
	DWORD ThreadFunc();
};

