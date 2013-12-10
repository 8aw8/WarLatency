#pragma once
#include "mythread.h"
#include "Client.h"

// ласс обработки игры 
//–еализует игру 2 клиентов

class CGames :public CMyThread
{
public:
	int type;
	BOOL StopingThread;
	CClient *client1;
	CClient *client2;

	int currentNumber;//„исло отсылаемое клиенту
	
	HANDLE hSemaphore;

	CGames(void);
	CGames(CClient *_client1, CClient *_client2);	
	~CGames(void);

	void eventFromClient(CClient *client);//ќбработчик событи€, возникающего от клиентов 
	void Realize(); // ”даление данных экземпл€ра класса без запуска деструктора

protected:	
	BOOL StopLoop;
	DWORD ThreadFunc();
};

