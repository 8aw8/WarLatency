#pragma once
#include "mythread.h"
#include "PoolSocket.h"
#include <vector>

 using namespace std;
/*
class CLSocket
{
public:
	SOCKET listenSocket;
	BOOL isUsed;
	
	CLSocket(void);
	CLSocket(SOCKET _socket);
	virtual ~CLSocket(void);
};
*/

class CClientPool :public CMyThread
{
public:
    vector <CMyThread*> VectorClient;
	vector <CMyThread*>::iterator vc_Iter;
	
	vector <CPoolSocket> VectorSocket;
	vector <CPoolSocket>::iterator vs_Iter;

	vector <CMyThread*> workClient;

	CPoolSocket poolSocket;
	
//	CLSocket *listenSocket;

	BOOL StopingThread;
	
	CClientPool(void);
	virtual ~CClientPool(void);
	
		void deleteNotWorkingThread(void);
		void deleteAllThread(void);
		void addClient(CMyThread* client);
		void addListenSocket(SOCKET socket);
		void printClients(void);
  CMyThread* getRandomClient(CMyThread *client);	
        void listenPoolSocket();
       DWORD ThreadFunc();

private:
	int countClients(void);
};

