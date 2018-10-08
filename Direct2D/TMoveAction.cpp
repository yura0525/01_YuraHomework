#include "TMoveAction.h"

bool TMoveAction::Process(xObject_2D* pTarget)
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

	if (TCollision::RectInRect(rt, pTarget->m_rtCollision))
	{
		m_pOwner->SetTransition(EVENT_FINDTARGET);
	}

	if (rand() % 100 == 5)
	{
		m_pOwner->SetTransition(EVENT_STOPWALK);
	}

	m_pOwner->Move();
	return true;
}

TMoveAction::TMoveAction(TEnemy * pOwner) : TEnemyState(pOwner)
{
}


TMoveAction::~TMoveAction()
{
}
