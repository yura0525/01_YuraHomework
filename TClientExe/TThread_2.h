#pragma once
#include "TBasisStd.h"

class TThread_2
{
public:
	unsigned int	m_hThread;
	unsigned int	m_iThreadID;
	bool			m_bStarted;
public:
	void		CreateThread();
	static unsigned int  WINAPI HandleRunner(LPVOID parameter);
	virtual bool	Run()
	{
		return true;
	}
public:
	TThread_2(void);
	virtual ~TThread_2(void);
};
