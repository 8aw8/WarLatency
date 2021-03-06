//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYTHREAD_H__6B892D2F_9DF8_11D5_A3EE_E2D14EB90E01__INCLUDED_)
#define AFX_MYTHREAD_H__6B892D2F_9DF8_11D5_A3EE_E2D14EB90E01__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include <windows.h>
//#include <strsafe.h>

#include "afxdb.h"

using namespace std;


class CMyThread  
{
private:

public:
	CMyThread();	
	virtual ~CMyThread();

	int type;
	BOOL StopingThread;
	DWORD Terminate(BOOL bCritical=FALSE);//�������������� ���������� ������
	DWORD Suspend(void);//��������� ������. 
	DWORD Resume(void);//������������� ������ ������
	BOOL Execute(void *param);	//������ ������

	HANDLE getThreadHandle();
	BOOL  isActive();

protected:	
	HANDLE hSemaphore;

	void *threadParam;// ��������� �� ������ ������������ � ����� ��� ������
	unsigned int m_dwID;
	LPDWORD lpThreadId;	
	virtual DWORD ThreadFunc()=0;// ����������� ������� ������ ��� ��������������� � �������-��������
	friend unsigned __stdcall ThreadProc(void* lpParameter);
	HANDLE m_hThread;		
};

#endif //!defined(AFX_MYTHREAD_H__6B892D2F_9DF8_11D5_A3EE_E2D14EB90E01__INCLUDED_)