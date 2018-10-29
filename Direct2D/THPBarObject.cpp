#include "THPBarObject.h"

const int g_HERO_DAMAGE_TIME_GAP = 2.0f;

bool THPBarObject::Create(ID3D11Device* pd3dDevice, float texMaxU, float texMaxV, float xPos, float yPos, DWORD left, DWORD top, DWORD right, DWORD bottom,
	T_STR szShaderName, T_STR szTexName, T_STR VSFunc, T_STR PSFunc)
{
	m_BKObject.Create(pd3dDevice, texMaxU, texMaxV, xPos, yPos, left, top, right, bottom, szShaderName, szTexName, VSFunc, PSFunc);
	xObject::Create(pd3dDevice, texMaxU, texMaxV, xPos, yPos, left, top, 94, 25, szShaderName, 
		L"../data/Resource/HPBar.png", VSFunc, PSFunc);
	return true;
}

bool  THPBarObject::Frame()
{
	if (IsDead())
		return true;

	float percent = ((float)m_iHP / m_iMAXHP);

	//여기해야한다. 
	/*m_rtDraw.right = m_rtDraw.right * percent;
	SetTexture(m_rtDraw.left, m_rtDraw.top, m_rtDraw.right, m_rtDraw.bottom);

	TCHAR	csBuffer[256];
	_stprintf_s(csBuffer, L"THPBarObject::FRAME() m_iHP = %d, m_iMAXHP  = %d, percent = %f, m_rtDraw.right = %d\n",
		m_iHP, m_iMAXHP, percent, m_rtDraw.right);

	OutputDebugString(csBuffer);*/

	m_BKObject.Frame();
	return xObject::Frame();
}

bool THPBarObject::Render()
{
	if (IsDead())
		return true;

	m_BKObject.Render();
	return xObject::Render();
}

THPBarObject::THPBarObject()
{
	m_fDamageTimeGap = g_HERO_DAMAGE_TIME_GAP;
}


THPBarObject::~THPBarObject()
{
}
