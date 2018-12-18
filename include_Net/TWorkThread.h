#pragma once
#include "TThread.h"
#define WORKER_THREAD_COUNT 4

class TWorkThread :	public TThread
{
public:
	void Run();
public:
	TWorkThread();
	virtual ~TWorkThread();
};

