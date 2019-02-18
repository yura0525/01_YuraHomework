#include "TServerIOCP_1.h"

bool TServerIOCP_1::Init()
{
	m_hIOCP = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, WORKER_THREAD_COUNT);

	for (int iThread = 0; iThread < WORKER_THREAD_COUNT; iThread++)
	{
		m_WorkerThread[iThread].CreateThread();
	}
	return true;
}
bool TServerIOCP_1::Run()
{
	return true;
}
bool TServerIOCP_1::Release()
{
	return true;
}
void TServerIOCP_1::AddHandleToIOCP(HANDLE hClientSocket, ULONG_PTR dwReturnKey)
{
	::CreateIoCompletionPort(hClientSocket, m_hIOCP, dwReturnKey, 0);
}

TServerIOCP_1::TServerIOCP_1()
{
	m_hIOCP = NULL;
}


TServerIOCP_1::~TServerIOCP_1()
{
}
