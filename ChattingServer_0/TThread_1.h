#pragma once
#include "TServerStd.h"
#include <process.h>

class TThread_1
{
public:
	unsigned int	m_hThread;
	unsigned int	m_iThreadID;
	bool			m_bStarted;
public:
	void			CreateThread();
	static unsigned int WINAPI HandleRunner(LPVOID parameter);
	virtual bool	Run()
	{
		return true;
	}

public:
	TThread_1();
	virtual ~TThread_1();
};

