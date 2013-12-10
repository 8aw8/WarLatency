#pragma once
#include "mythread.h"
#include "PoolSocket.h"
#include <vector>

using namespace std;

//����� �������� ������� ������� ���������������� �� � ��������� ������������

class CClientPool :public CMyThread
{
public:
	
	//�������, �������� ��� ���������� ������
	vector <CMyThread*> VectorClient;
	vector <CMyThread*>::iterator vc_Iter;
		
	//�������, �������� ��������� �� ��� ���������� ������� ������, �������� 
	// ����� ��������� ������ CGames
	vector <CMyThread*> workClient;
	vector <CMyThread*>::iterator wc_Iter;
	

	BOOL StopingThread;

	CClientPool(void);
	virtual ~CClientPool(void);	

	void deleteAllThread(void);//�������� ���� ������� �������, ����������� ��� �������� �������
	void addClient(CMyThread* client);// �������� � ���� �������, ����������� ��� ��� ����������� �� CMyThread 	
	void printClients(void);
	CMyThread* getRandomClient(CMyThread *client);	// ��������� ��������� �� ������� ������
	DWORD ThreadFunc();

private:
	int countClients(void);
	void deleteNotWorkingThread(void);//�����, ��������� ��������� ������, ���������� ������

};

