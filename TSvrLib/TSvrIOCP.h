#pragma once
#include "TServerStd.h"
#include "TSvrObject.h"
#include "TWorkThread.h"


class TSvrIOCP : public TSingleton<TSvrIOCP>, public TSvrObject
{
	friend class TSingleton<TSvrIOCP>;
public:
	HANDLE m_hIOCP;
	TWorkThread m_WorkerThread[WORKER_THREAD_COUNT];
public:
	bool Init();
	bool Release();
	void Add(HANDLE hClientSocket, ULONG_PTR dwReturnKey);
public:
	TSvrIOCP();
	virtual ~TSvrIOCP();
};

#define I_IOCP TSvrIOCP::GetInstance()