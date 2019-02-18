#pragma once
#include <process.h>
#include "TServerObj_1.h"
class TThread_3 : public TServerObj_1
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
	TThread_3();
	virtual ~TThread_3();
};

