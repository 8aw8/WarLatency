//#pragma once
//#include <stdio.h>
#include "winsock2.h"
#include "MyThread.h"
#include <vector>

#include "Client.h"
#include "ClientPool.h"

using namespace std;

class CServer: public CMyThread 
{
public:
	CServer();
	CServer(u_short port);
	int InitServer(u_short port);
	virtual ~CServer(void);	
	void RealiseServer();//������� ������ ������ ��� ������� �����������
	CClientPool *clientPool;//��������� �� ��� �������, ������������� ������

	BOOL StartWaitConnect();
    BOOL StopWaitConnect();			
	
private:		
	BOOL StopThread;

	u_short m_port;
	SOCKET ListenSocket;
	SOCKET AcceptSocket;
	sockaddr_in service;
	sockaddr_in send_service;

	BOOL BindListenSocket();
	
	SOCKET WaitConnect();	
	virtual BOOL onConnected();//���������� ������� ������� � ������� �������

protected:
	DWORD ThreadFunc();
};
