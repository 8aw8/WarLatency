#pragma once
#include "mythread.h"
#include "PoolSocket.h"
#include <vector>

using namespace std;

//Класс хранящий очереди потоков синхронизирующий их и удаляющий отработанные

class CClientPool :public CMyThread
{
public:
	
	//Очередь, хранящая все запущенные потоки
	vector <CMyThread*> VectorClient;
	vector <CMyThread*>::iterator vc_Iter;
		
	//Очередь, хранящая указатели на все запущенные рабочие потоки, исключая 
	// поток экземпляр класса CGames
	vector <CMyThread*> workClient;
	vector <CMyThread*>::iterator wc_Iter;
	

	BOOL StopingThread;

	CClientPool(void);
	virtual ~CClientPool(void);	

	void deleteAllThread(void);//Удаление всех рабочих потоков, срабатывает при закрытии сервера
	void addClient(CMyThread* client);// Добавить в пулл потоков, добавляется все что наследуется от CMyThread 	
	void printClients(void);
	CMyThread* getRandomClient(CMyThread *client);	// Получение указателя на рабочий конект
	DWORD ThreadFunc();

private:
	int countClients(void);
	void deleteNotWorkingThread(void);//Метод, удаляющий нерабочие потоки, чистельщик мусора

};

