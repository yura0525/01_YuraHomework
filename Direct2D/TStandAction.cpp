#include "TStandAction.h"

bool TStandAction::Process(xObject* pTarget)
{
	RECT rt;
	rt.left = (m_pOwner->m_pos.x) - (m_pOwner->m_fAttackRadius);
	rt.top = (m_pOwner->m_pos.y) - (m_pOwner->m_fAttackRadius);
	rt.right = (m_pOwner->m_pos.x) + (m_pOwner->m_fAttackRadius);
	rt.bottom = (m_pOwner->m_pos.y) + (m_pOwner->m_fAttackRadius);

	m_pOwner->m_rtAttackRect.left = rt.left;
	m_pOwner->m_rtAttackRect.top = rt.top;
	m_pOwner->m_rtAttackRect.right = rt.right;
	m_pOwner->m_rtAttackRect.bottom = rt.bottom;

	//딜레이를 주려고 조건이 만족하면 EVENT_PATROL
	if (rand() % 100 == 5)
	{
		m_pOwner->SetTransition(EVENT_PATROL);
	}

	//공격Rect안에 들어오면 EVENT_FINDTARGET
	if (TCollision::RectInRect(rt, pTarget->m_rtCollision))
	{
		m_pOwner->SetTransition(EVENT_FINDTARGET);
	}
	else
	{
		if (m_pOwner->m_dwCurrentState == STATE_MOVE)
		{
			m_pOwner->SetTransition(EVENT_LOSTTARGET);
		}
	}
	return true;
}

TStandAction::TStandAction(TEnemy * pOwner) : TEnemyState(pOwner)
{
}


TStandAction::~TStandAction()
{
}
