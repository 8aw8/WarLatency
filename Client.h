#pragma once
#include "winsock2.h"
#include "MyThread.h"
#include "ClientPool.h"
#include <atlstr.h>
#include <vector>
//#include "StdAfx.h"
//#include "DBThread.h"


using namespace std;

struct _HeadPacket
{
  char prefix[4];
  int PacketType;
  int CodCommand;
  int Request_ID;
  int PacketSize; 
};

struct _buffer
{ 
   char* buffer;
   int SizeBuffer;
};
struct _ConnStr
{
	CString Driver;
	CString DSNname;
    CString Desc;
	CString Server;
	CString Database;
    CString User;
    CString Password;
    CString	UseUserDSN;
	CString UserDSN;
	CString UseUSER;
	CString UsePassword;
};


class CClient : public CMyThread
{
public:
	int type;
	int game_mode;

	CClient(SOCKET socket);
	~CClient(void);
	void Realize(void);

	SOCKET m_socket;
 	_HeadPacket HeadPacket;
      BOOL StopingThread;
	  char IP_Addr[16];
   int RecvBufferSize;

	  CClientPool *clientPool;
	  CMyThread *runGames;
 	
//CDBThread a;
  vector <void*> DBTVector;
  vector <void*>::iterator v_Iter;
    
//	virtual BOOL onConnected();
   DWORD ThreadFunc();
    void EchoClient();
	BOOL WorkTypePacket(_HeadPacket head);
	BOOL TrueMDISPacket(_HeadPacket head);
	 int handlErr(const int err);
    void PrintHeadPacket(_HeadPacket head);
   char* setCommand(char* buffer, int BufferSize);

   	void OnRecvPacket(char* buffer, int BufferSize);
	void SendData(char* buffer, int bufferSize);
	void CClient::getClients();	
	void startGame(void);

private:
	 // char* SendBuffer;
	 // char* command; 	  

    char SendBuffer[8192];
	char command[256];

	  int commandSize;	
	  BOOL StopLoop;
       
};
