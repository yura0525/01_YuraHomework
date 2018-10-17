#include "TNPCObject.h"

bool TNPCObject::Frame()
{
	int iHalfY = m_rtDraw.bottom / 2;

	if ((m_pos.y + iHalfY) > g_rtClient.bottom)
	{
		SetDead();
	}

	m_pos.x += m_fDir[0] * m_fSpeed * g_fSecPerFrame;
	m_pos.y += m_fDir[1] * m_fSpeed * g_fSecPerFrame;

	//위치값이나 충돌박스 수정.
	return xObject::Frame();
}

TNPCObject::TNPCObject()
{
	m_fSpeed = 150.0f;			//NPC 이동 속도
	m_iHP = 1;
}


TNPCObject::~TNPCObject()
{
}
