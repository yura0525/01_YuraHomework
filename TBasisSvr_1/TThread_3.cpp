#include "TThread_3.h"

unsigned int  WINAPI TThread_3::HandleRunner(LPVOID parameter)
{
	TThread_3* pThread = (TThread_3*)parameter;
	pThread->Run();
	return 0;
}
void TThread_3::CreateThread()
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


TThread_3::TThread_3() : m_bStarted(false)
{
}


TThread_3::~TThread_3()
{
	if (m_hThread != NULL)
	{
		CloseHandle((HANDLE)m_hThread);
		m_hThread = NULL;
	}
}
