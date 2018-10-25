#include "TNPCObject.h"

bool TNPCObject::Init()
{
	m_fSpeed = 150.0f;			//NPC �̵� �ӵ�
	m_iHP = 1;
	m_fDir[0] = 0.0f;
	m_fDir[1] = 1.0f;
	return xObject::Init();
}

bool TNPCObject::Frame()
{
	if (IsDead())
		return false;

	int iHalfY = m_rtDraw.bottom / 2;

	if ((m_pos.y - iHalfY) >= g_rtClient.bottom)
	{
		SetDead();
		return false;
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
	m_fDir[0] = 0.0f;
	m_fDir[1] = 1.0f;
}


TNPCObject::~TNPCObject()
{
}
