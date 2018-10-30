#include "THPBarObject.h"


bool THPBarObject::Create(ID3D11Device* pd3dDevice, float texMaxU, float texMaxV, float xPos, float yPos, DWORD left, DWORD top, DWORD right, DWORD bottom,
	T_STR szShaderName, T_STR szTexName, T_STR VSFunc, T_STR PSFunc)
{
	m_BKObject.Create(pd3dDevice, texMaxU, texMaxV, xPos, yPos, left, top, right, bottom, szShaderName, szTexName, VSFunc, PSFunc);
	xObject::Create(pd3dDevice, texMaxU, texMaxV, xPos, yPos, left, top, g_HPBAR_WIDTH, g_HPBAR_HEIGHT, szShaderName,
		L"../data/Resource/HPBar.png", VSFunc, PSFunc);
	
	return true;
}

bool  THPBarObject::Frame()
{
	if (m_pOwner == NULL || m_pOwner->IsDead())
		return true;

	float percent = ((float)(m_pOwner->m_iHP) / (m_pOwner->m_iMAXHP));

	m_rtDraw.right = g_HPBAR_WIDTH * percent;
	
	float newXPos = m_pOwner->m_pos.x - ((g_HPBAR_WIDTH - m_rtDraw.right) /2);
	float newYPos = m_pOwner->m_pos.y + g_INIT_HERO_HP_POSY;

	SetPosition(newXPos, newYPos, m_rtDraw.left, m_rtDraw.top, m_rtDraw.right, m_rtDraw.bottom);

	m_BKObject.SetPosition(m_pOwner->m_pos.x, newYPos);
	m_BKObject.Frame();
	return xObject::Frame();
}

bool THPBarObject::Render()
{
	if (m_pOwner == NULL || m_pOwner->IsDead())
		return true;

	m_BKObject.Render();
	return xObject::Render();
}

THPBarObject::THPBarObject(xObject* pOwner) : m_pOwner(pOwner)
{
}


THPBarObject::~THPBarObject()
{
}
