// MyThread.cpp: implementation of the CMyThread class.
//
//////////////////////////////////////////////////////////////////////

//#define _AFXDLL

//#include "stdafx.h"
#include "MyThread.h"
// #include "afxdb.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
unsigned __stdcall ThreadProc(void* lpParameter)

//extern DWORD WINAPI ThreadProc(LPVOID lpParameter)
{
	CMyThread *pMyThread=(CMyThread *)lpParameter;
	  
//	  pMyThread->CriticalSection = new CRITICAL_SECTION; //printf("New %ld bytes",sizeof(CRITICAL_SECTION));
//	  InitializeCriticalSection(pMyThread->CriticalSection);
//     EnterCriticalSection(pMyThread->CriticalSection);
//	  printf("sizeof(*pMyThread) %ld\n",sizeof(*pMyThread));
	
 DWORD result=pMyThread->ThreadFunc();
     
       
 //      LeaveCriticalSection(pMyThread->CriticalSection); 
//	   DeleteCriticalSection(pMyThread->CriticalSection);
//       delete pMyThread->CriticalSection;
   
   // ExitThread(0);

	_endthreadex(0);

	return 0;
}


CMyThread::CMyThread()
{
	type=-1;
	m_hThread=NULL;	
}

CMyThread::~CMyThread()
{
//   if (m_hThread!=NULL) CloseHandle(m_hThread);
  
  // 	ExitThread(0);
	//  Terminate(FALSE);
}

HANDLE CMyThread::getThreadHandle()
{
	return m_hThread;
}


BOOL CMyThread::Execute()
{
	if (m_hThread)
	{
		return FALSE;
	}
   //CMyThread* Thread = new CMyThread;
   
	//CMyThread* pData = (CMyThread*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(*this));
//	printf("sizeof(this)=%ld\n",sizeof(*this));

	//pData=this;

//	m_hThread=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThreadProc,this,0,&m_dwID);
	//m_hThread=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThreadProc,pData,0,&m_dwID);
    m_hThread = (HANDLE)_beginthreadex( NULL, 0, &ThreadProc, this, 0, &m_dwID);
    ResumeThread(m_hThread);

//	if (m_hThread!=NULL) 
//          SetThreadPriority(m_hThread, THREAD_PRIORITY_LOWEST); 

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

DWORD CMyThread::Terminate(BOOL bCritical)
{
	DWORD dwExitCode;

	
	
	Sleep(100);
	GetExitCodeThread(m_hThread,&dwExitCode);

	TerminateThread(m_hThread,0);

	if (dwExitCode==STILL_ACTIVE)
	{
		while (dwExitCode==STILL_ACTIVE)
		{
			GetExitCodeThread(m_hThread,&dwExitCode);
		
		}
	//	TerminateThread(m_hThread,0);
	//	_endthreadex( 0 );
		
	}
	else 
	{
    	GetExitCodeThread(m_hThread,&dwExitCode);	 
	//    TerminateThread(m_hThread,0);
	//	_endthreadex(0);
	//	ExitThread(0);
	}
	if (m_hThread!=NULL) CloseHandle(m_hThread);
	m_hThread=NULL;
	return dwExitCode;
	//}
}



