#pragma once
#include "winsock2.h"
#include "MyThread.h"
#include "ClientPool.h"
#include <vector>

using namespace std;

class CClient : public CMyThread
{
public:
	int type;
	int game_mode;

	//���������, ������������� ������ � ��������  
	SOCKET m_socket;	
	char IP_Addr[16];
	int RecvBufferSize;


	CClient(SOCKET socket);
	~CClient(void);
	void Realize(void);//��������� ����� ������������ ��� ������� ������ ���������� ������ �� ��������� ����������. 

		
	CClientPool *clientPool; //��������� �� ����� ���� �������
	CMyThread *runGames; // ��������� �� ����� ���������� ����
	
	DWORD ThreadFunc();//����� ����������� ��� ������� ������
	void EchoClient();//������ ���������� � ������� 

	void OnRecvPacket(char* buffer, int BufferSize);//����� ����������, ������������� ��� ��������� ������ � ������
	void SendData(char* buffer, int bufferSize);// ����� �������� ������ �������	
	void startGame(void);//����� ����������� ����, ���������� � OnRecvPacket 

private:
	
	char SendBuffer[8192];//������ ��� ���� ��������� ������. �� ��������� � TCP/IP 8k.
	char command[256];//������������ ������ �������

	int commandSize;	
	BOOL StopLoop;       
	
	int handlErr(const int err);//��������� ������ �����������
	char* setCommand(char* buffer, int BufferSize);//������� ��������� ������ �� ������� ����� ENTER
};
