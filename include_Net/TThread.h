#pragma once
#include "TSvrObject.h"
class TThread : public TSvrObject
{
public:
	unsigned int m_hThread;
	unsigned int m_iThreadID;
public:
	void CreateThread();
	virtual void Run();

public:
	TThread();
	virtual ~TThread();
};

