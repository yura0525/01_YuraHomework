#include "THeroObject.h"
#include "TInput.h"
#include "TCollision.h"

bool THeroObject::Frame()
{
	//Hero의 움직임 처리
	if (I_Input.Key('W'))
	{
		m_pos.y += -1 * g_fSecPerFrame * 300.0f;
	}
	if (I_Input.Key('S'))
	{
		m_pos.y += 1 * g_fSecPerFrame * 300.0f;
	}
	if (I_Input.Key('A'))
	{
		m_pos.x += -1 * g_fSecPerFrame * 300.0f;
	}
	if (I_Input.Key('D'))
	{
		m_pos.x += 1 * g_fSecPerFrame * 300.0f;
	}

	//위치값이나 충돌박스 수정.
	TObject::Frame();

	//m_posDraw.x = m_pos.x - (m_rtDraw.right / 2);
	//m_posDraw.y = m_pos.y - (m_rtDraw.bottom / 2);

	//m_rtCollision.left = m_posDraw.x;
	//m_rtCollision.top = m_posDraw.y;
	//m_rtCollision.right = m_rtCollision.left + m_rtDraw.right;
	//m_rtCollision.bottom = m_rtCollision.top + m_rtDraw.bottom;

	return true;
}

void THeroObject::ProcessDamage(int damage)
{
	TFlightObject::ProcessDamage(damage);
}

THeroObject::THeroObject()
{
}

THeroObject::~THeroObject()
{
}
