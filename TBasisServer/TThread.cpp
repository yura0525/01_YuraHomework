#include "TThread.h"
#include <windows.h>
void TThread::CreateThread()
{
	if (m_bStarted == true)
		return;
	m_hThread = _beginthreadex(NULL, 0, HandleRunner, (LPVOID)this, 0, &m_iThreadID);
	m_bStarted = true;
}
bool TThread::Run()
{
	return true;
}
unsigned int WINAPI TThread::HandleRunner(LPVOID parameter)
{
	TThread* pThread = (TThread*)parameter;
	if (pThread)
		pThread->Run();
	return 0;
}

TThread::TThread()
{
	m_bStarted = false;
}


TThread::~TThread()
{
	CloseHandle((HANDLE)m_hThread);
}
