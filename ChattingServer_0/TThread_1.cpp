#include "TThread_1.h"

void TThread_1::CreateThread()
{
	if (m_bStarted == true) return;
	// 유저 당 1개 스레드 붙임.
	//unsigned int dwThread;
	//int hThreadID = 
	m_hThread = _beginthreadex(NULL, 0, HandleRunner,
		(LPVOID)this, 0, &m_iThreadID);

	//m_hThread = ::CreateThread( 0, 0,  HandleRunner, this, 0, &m_iThreadID );
	m_bStarted = true;
}
unsigned int WINAPI TThread_1::HandleRunner(LPVOID parameter)
{
	TThread_1* pThread = (TThread_1*)parameter;
	pThread->Run();
	return 0;
}

TThread_1::TThread_1() : m_bStarted(false)
{
}


TThread_1::~TThread_1()
{
	if (m_hThread != NULL)
	{
		CloseHandle((HANDLE)m_hThread);
		m_hThread = NULL;
	}
}
