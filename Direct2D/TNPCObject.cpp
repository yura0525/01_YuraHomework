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

	//��ġ���̳� �浹�ڽ� ����.
	return xObject::Frame();
}

TNPCObject::TNPCObject()
{
	m_fSpeed = 150.0f;			//NPC �̵� �ӵ�
	m_iHP = 1;
}


TNPCObject::~TNPCObject()
{
}
