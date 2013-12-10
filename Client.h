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
	
	
	SOCKET m_socket;
        BOOL StopingThread;
	char IP_Addr[16];
        int RecvBufferSize;

	CClientPool *clientPool;
	CMyThread *runGames;
 	

  	vector <void*> DBTVector;
  	vector <void*>::iterator v_Iter;
    

	CClient(SOCKET socket);
	~CClient(void);
	void Realize(void);
   
        
	void OnRecvPacket(char* buffer, int BufferSize);
	void SendData(char* buffer, int bufferSize);
	void CClient::getClients();	
	void EchoClient();
	
	void startGame(void);
       char* setCommand(char* buffer, int BufferSize);
	
	
        int handlErr(const int err);
	DWORD ThreadFunc();

private:

    char SendBuffer[8192];
	char command[256];

	  int commandSize;	
	  BOOL StopLoop;       
};
