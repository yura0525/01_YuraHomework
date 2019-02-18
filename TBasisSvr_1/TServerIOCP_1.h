#pragma once
#include "TServerObj_1.h"
#include "TWorkThread_1.h"

#define WORKER_THREAD_COUNT 3

class TServerIOCP_1 : public TSingleton<TServerIOCP_1>, public TServerObj_1
{
private:
	friend class TSingleton<TServerIOCP_1>;
public:
	HANDLE			m_hIOCP;
	TWorkThread_1	m_WorkerThread[WORKER_THREAD_COUNT];
public:
	bool			Init();
	bool			Run();
	bool			Release();
	//ULONG_PTR 32bit, 64bit ¡÷¿«
	void			AddHandleToIOCP(HANDLE hClientSocket, ULONG_PTR dwReturnKey);

public:
	TServerIOCP_1();
	virtual ~TServerIOCP_1();
};

#define I_ServerIOCP TServerIOCP_1::GetInstance()