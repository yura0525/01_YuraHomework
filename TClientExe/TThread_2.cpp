#include "TThread_2.h"

TThread_2::TThread_2(void) : m_bStarted(false)
{
}
TThread_2::~TThread_2(void)
{
	if (m_hThread != NULL)
	{
		CloseHandle((HANDLE)m_hThread);
		m_hThread = NULL;
	}
}
unsigned int  WINAPI TThread_2::HandleRunner(LPVOID parameter)
{
	TThread_2* pThread = (TThread_2*)parameter;
	pThread->Run();
	return 0;
}
void TThread_2::CreateThread()
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
