#include "stdafx.h"
#include "TThread.h"

unsigned int WINAPI HandleRunner(LPVOID param)
{
	TThread* pThread = (TThread*)param;

	if (pThread)
		pThread->Run();

	return 0;
}
void TThread::CreateThread()
{
	m_hThread = _beginthreadex(NULL, 0, HandleRunner, (LPVOID)this, 0, &m_iThreadID);
}
void TThread::Run()
{

}

TThread::TThread()
{
	CreateThread();
}


TThread::~TThread()
{
	CloseHandle((HANDLE)m_hThread);
}
