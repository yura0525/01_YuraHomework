#pragma once
#include "xObject.h"
#include "TEnemyState.h"
#include "TFSMMgr.h"
#include "TFiniteStateMachine.h"
class TEnemy : public xObject
{
public:
	TEnemyState *			m_pAction;
	TEnemyState*			m_pActionList[STATE_COUNT];

	TFiniteStateMachine*	m_pFSM;
	int						m_iFSMID;
	DWORD					m_dwCurrentState;

public:
	void SetFSM(int id)
	{
		m_iFSMID = id;
		m_pFSM = I_FSMMgr.GetPtr(m_iFSMID);
	}
public:
	bool Move(TPoint pos);
	bool Move();
	bool Frame()	{	return true;	}
	bool Process(xObject* pTarget);

	DWORD SetTransition(DWORD dwEvent);

public:
	TEnemy();
	virtual ~TEnemy();
};

