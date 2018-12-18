#include "stdafx.h"
#include "TSvrIOCP.h"

bool TSvrIOCP::Init()
{
	m_hIOCP = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, WORKER_THREAD_COUNT);
	for (int iThread = 0; iThread < WORKER_THREAD_COUNT; iThread++)
	{
		m_WorkerThread[iThread].CreateThread();
	}
	return true;
}

bool TSvrIOCP::Release()
{
	return true;
}
void TSvrIOCP::Add(HANDLE hClientSocket, ULONG_PTR dwReturnKey)
{
	m_hIOCP = ::CreateIoCompletionPort(hClientSocket, m_hIOCP, dwReturnKey, 0);
}

TSvrIOCP::TSvrIOCP()
{
}


TSvrIOCP::~TSvrIOCP()
{
}
