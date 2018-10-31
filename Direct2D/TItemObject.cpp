#include "TItemObject.h"


bool TItemObject::Frame()
{
	if (IsDead())	return true;

	int iHalfY = m_rtDraw.bottom / 2;

	if ((m_pos.y + iHalfY) > g_rtClient.bottom)
	{
		SetDead();
	}

	m_pos.x += m_fDir[0] * m_fSpeed * g_fSecPerFrame;
	m_pos.y += m_fDir[1] * m_fSpeed * g_fSecPerFrame;

	return xObject::Frame();
}

TItemObject::TItemObject()
{
}


TItemObject::~TItemObject()
{
}
