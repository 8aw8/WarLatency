#pragma once
#include "winsock2.h"

class CPoolSocket
{
public:
	
	SOCKET socket;
	BOOL isUsed;
	char IP_Addr[16];

	CPoolSocket(void)
	{
		socket = 0;
		isUsed = FALSE;		

	}

	CPoolSocket(SOCKET poolSocket, BOOL is_Used)
	{
		socket = poolSocket;
		isUsed = is_Used;
	}

	virtual ~CPoolSocket(void)
	{
	}
};

