#include "THeroObject.h"
#include "xInput.h"
#include "TCollision.h"

POINT	g_pHeroPos;
const int g_HeroDamageTimeGap = 2.0f;

bool THeroObject::Frame()
{
	int iHalfX = m_rtDraw.right / 2;
	int iHalfY = m_rtDraw.bottom / 2;

	//Hero의 움직임 처리. 화면영역 밖으로 넘어가는것을 막는다.
	if (I_Input.m_KeyState[DIK_W])
	{
		m_pos.y += -1 * g_fSecPerFrame * 300.0f;
		if ((m_pos.y - iHalfY) < g_rtClient.top)
		{
			m_pos.y = g_rtClient.top + iHalfY;
		}
	}
	if (I_Input.m_KeyState[DIK_S])
	{
		m_pos.y += 1 * g_fSecPerFrame * 300.0f;
		if ((m_pos.y + iHalfY) > g_rtClient.bottom)
		{
			m_pos.y = g_rtClient.bottom - iHalfY;
		}
	}
	if (I_Input.m_KeyState[DIK_A])
	{
		m_pos.x += -1 * g_fSecPerFrame * 300.0f;
		if ((m_pos.x - iHalfX) < g_rtClient.left)
		{
			m_pos.x = g_rtClient.left + iHalfY;
		}
	}
	if (I_Input.m_KeyState[DIK_D])
	{
		m_pos.x += 1 * g_fSecPerFrame * 300.0f;
		if ((m_pos.x + iHalfX) > g_rtClient.right)
		{
			m_pos.x = g_rtClient.right - iHalfX;
		}
	}

	//위치값이나 충돌박스 수정.
	return xObject::Frame();
}

bool THeroObject::Render()
{
	xObject::Render();

	// fade out<--> fadeIn
	/*if (bFadeStart && FadeOut())
	{
		bFadeStart = false;
	}

	if (!bFadeStart && FadeIn())
	{
		bFadeStart = true;
	}*/

	g_pHeroPos.x = m_pos.x;
	g_pHeroPos.y = m_pos.y;
	return true;
}
bool THeroObject::FadeOut()
{
	m_fAlpha -= g_fSecPerFrame * 100;
	if (m_fAlpha < 0.0f)
	{
		m_fAlpha = 0.0f;
		return true;
	}

	return false;
}
bool THeroObject::FadeIn()
{
	m_fAlpha += g_fSecPerFrame * 100;
	if (m_fAlpha > 255.0f)
	{
		m_fAlpha = 255.0f;
		return true;
	}

	return false;
}

THeroObject::THeroObject()
{
	bFadeStart = false;
	m_fLastDamageTime = g_fGameTimer;
	m_fDamageTimeGap = g_HeroDamageTimeGap;
}

THeroObject::~THeroObject()
{
}
