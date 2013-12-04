#pragma once
#include "mythread.h"
#include <vector>

 using namespace std;

class CClientPool :public CMyThread
{
public:
    vector <CMyThread*> VectorClient;
	vector <CMyThread*>::iterator vc_Iter;

	BOOL StopingThread;
	
	CClientPool(void);
	virtual ~CClientPool(void);
	
		void deleteNotWorkingThread(void);
		void deleteAllThread(void);
		void addClient(CMyThread* client);
		void printClients(void);
  CMyThread* getRandomClient(CMyThread *client);	
       DWORD ThreadFunc();

private:
	int countClients(void);
};

