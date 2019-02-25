#pragma once
#include "TThread.h"
class TWorkThread : public TThread
{
public:
	bool		m_bLoop;
public:
	bool		Run() override;

public:
	TWorkThread();
	virtual ~TWorkThread();
};

