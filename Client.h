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

	//Переменны, обслуживающие работу с сокетами  
	SOCKET m_socket;	
	char IP_Addr[16];
	int RecvBufferSize;


	CClient(SOCKET socket);
	~CClient(void);
	void Realize(void);//Служебный метод используется для очистки данных экземпляра класса не используя деструктор. 

		
	CClientPool *clientPool; //Указатель на класс пула потоков
	CMyThread *runGames; // Указатель на класс запущенной игры
	
	DWORD ThreadFunc();//Метод выполняемый при запуске потока
	void EchoClient();//Печать информации о клиенте 

	void OnRecvPacket(char* buffer, int BufferSize);//Метод обработчик, срабатываемый при появлении данных в сокете
	void SendData(char* buffer, int bufferSize);// Метод отправки данных клиенту	
	void startGame(void);//Метод запускающий игру, вызывается в OnRecvPacket 

private:
	
	char SendBuffer[8192];//Память для окна получения данных. По умолчанию в TCP/IP 8k.
	char command[256];//Максимальная длинна команды

	int commandSize;	
	BOOL StopLoop;       
	
	int handlErr(const int err);//Обработка ошибок подключения
	char* setCommand(char* buffer, int BufferSize);//Парсинг вводимого буфера на предмет ввода ENTER
};
