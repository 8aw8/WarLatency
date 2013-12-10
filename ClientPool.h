#pragma once
#include "mythread.h"
#include "PoolSocket.h"
#include <vector>

 using namespace std;


class CClientPool :public CMyThread
{
public:
    vector <CMyThread*> VectorClient;
	vector <CMyThread*>::iterator vc_Iter;
	
	vector <CPoolSocket> VectorSocket;
	vector <CPoolSocket>::iterator vs_Iter;

	vector <CMyThread*> workClient;
	vector <CMyThread*>::iterator wc_Iter;

	CPoolSocket poolSocket;

	BOOL StopingThread;
	
	CClientPool(void);
	virtual ~CClientPool(void);	
	
		void deleteAllThread(void);
		void addClient(CMyThread* client);
		void addListenSocket(SOCKET socket);
		void printClients(void);
  CMyThread* getRandomClient(CMyThread *client);	
        void listenPoolSocket();
       DWORD ThreadFunc();

private:
	 
	int countClients(void);
	void deleteNotWorkingThread(void);

};

