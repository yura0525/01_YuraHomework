#pragma once
#include "TStd.h"

enum TEnemyActoinState
{
	STATE_STAND = 0,
	STATE_MOVE,
	STATE_ATTACK,
	STATE_COUNT,
};

enum TEnemyEventState
{
	EVENT_PATROL = 0,
	EVENT_FINDTARGET,
	EVENT_LOSTTARGET,
	EVENT_STOPWALK,
	EVENT_COUNT,
};

class TFiniteState
{
	//event ---->output
	std::map<DWORD, DWORD>	m_mapOutState;
public:
	DWORD					m_dwCurrentState;

public:
	void	AddTransition(DWORD dwEvent, DWORD dwOutput);
	DWORD	GetState(DWORD dwEvent);
public:
	TFiniteState();
	virtual ~TFiniteState();
};

class TFiniteStateMachine
{
public:
	TFiniteStateMachine();
	virtual ~TFiniteStateMachine();
};

