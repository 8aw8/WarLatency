// MyThread.cpp: implementation of the CMyThread class.
//
//////////////////////////////////////////////////////////////////////

//#define _AFXDLL

#include "MyThread.h"

unsigned __stdcall ThreadProc(void* lpParameter)
{
	CMyThread *pMyThread=(CMyThread *)lpParameter;	  
	
   DWORD result=pMyThread->ThreadFunc();     

	_endthreadex(0);
	return 0;
}


CMyThread::CMyThread()
{
	type=-1;
	m_hThread=NULL;	
	hSemaphore = CreateSemaphore(NULL, 1, 1, NULL);
}

CMyThread::~CMyThread()
{
   if (m_hThread!=NULL) CloseHandle(m_hThread);  
   m_hThread=NULL;
   CloseHandle(hSemaphore);  
}

HANDLE CMyThread::getThreadHandle()
{
	return m_hThread;
}


BOOL CMyThread::Execute(void *param)
{
	threadParam = param;
	if (m_hThread)
	{
		return FALSE;
	}
   
	m_hThread = (HANDLE)_beginthreadex( NULL, 0, &ThreadProc, param, 0, &m_dwID);

	DWORD dwExitCode=0;
	while (dwExitCode!=STILL_ACTIVE)
	{
	  GetExitCodeThread(m_hThread,&dwExitCode);
	}

    return (m_hThread==NULL)?FALSE:TRUE;
}

BOOL CMyThread::isActive()
{
	DWORD dwExitCode;
    GetExitCodeThread(m_hThread,&dwExitCode);
	if (dwExitCode==STILL_ACTIVE) 
		return TRUE; 
	else 
		return FALSE;
	return FALSE;
}

DWORD CMyThread::Suspend(void)
{
  return SuspendThread(m_hThread);
}

DWORD CMyThread::Resume(void)
{
 return ResumeThread(m_hThread);
}


DWORD CMyThread::Terminate(BOOL bCritical)
{
	DWORD dwExitCode;
	Sleep(100);
	GetExitCodeThread(m_hThread,&dwExitCode);

	TerminateThread(m_hThread,0);
	return dwExitCode;
}



