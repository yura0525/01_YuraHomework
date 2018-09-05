#include "TNPCObject.h"

bool TNPCObject::Frame()
{
	int iHalfX = m_rtDraw.right / 2;
	int iHalfY = m_rtDraw.bottom / 2;

	if ((m_pos.y + iHalfY) > g_rtClient.bottom)
	{
		m_bDead = true;
	}

	m_pos.x += m_fDir[0] * m_fSpeed * g_fSecPerFrame;
	m_pos.y += m_fDir[1] * m_fSpeed * g_fSecPerFrame;

	m_posDraw.x = m_pos.x - iHalfX;
	m_posDraw.y = m_pos.y - iHalfY;

	m_rtCollision.left	= m_posDraw.x;
	m_rtCollision.top	= m_posDraw.y;
	m_rtCollision.right = m_rtCollision.left + m_rtDraw.right;
	m_rtCollision.bottom = m_rtCollision.top + m_rtDraw.bottom;

	return true;
}

TNPCObject::TNPCObject()
{
	m_fSpeed = 150.0f;			//NPC 이동 속도
	m_iHP = 1;
}


TNPCObject::~TNPCObject()
{
}
