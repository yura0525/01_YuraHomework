#pragma once
#include "TStd.h"

enum TEnemyActionState
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
	//event -> output
	std::map<DWORD, DWORD>		m_mapOutState;

public:
	DWORD						m_dwCurrentState;
public:
	void AddTransition(DWORD dwEvent, DWORD dwOutput);
	DWORD GetState(DWORD dwEvent);

public:
	TFiniteState();
	virtual ~TFiniteState();
};

class TFiniteStateMachine
{
public:
	std::map<DWORD, TFiniteState*>		m_mapState;
	T_STR	m_szLoadFile;

public:
	void AddStateTransition(DWORD dwState, DWORD dwEvent, DWORD dwOutState);
	DWORD StateTransition(DWORD dwState, DWORD dwEvent);

	bool Load(T_STR szLoadFile);
	bool Release();

public:
	int GetIndex(T_STR szState);

public:
	TFiniteStateMachine();
	virtual ~TFiniteStateMachine();
};

