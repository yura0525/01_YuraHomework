#include "THeroObject.h"
#include "xInput.h"
#include "TCollision.h"

POINT	g_pHeroPos;
const int g_HERO_DAMAGE_TIME_GAP = 2.0f;
const int g_Hero_MAXHP = 100;
const int g_INIT_HERO_POSX = 550;

bool THeroObject::Frame()
{
	int iHalfX = m_rtDraw.right / 2;
	int iHalfY = m_rtDraw.bottom / 2;

	//Hero의 움직임 처리. 화면영역 밖으로 넘어가는것을 막는다.
	if (g_Input.bFront)
	{
		m_pos.y += -1 * g_fSecPerFrame * 300.0f;
	}
	if (g_Input.bBack)
	{
		m_pos.y += 1 * g_fSecPerFrame * 300.0f;
	}
	if (g_Input.bLeft)
	{
		m_pos.x += -1 * g_fSecPerFrame * 300.0f;
	}
	if (g_Input.bRight)
	{
		m_pos.x += 1 * g_fSecPerFrame * 300.0f;
	}

	if ((m_pos.y - iHalfY) < g_rtClient.top)
		m_pos.y = g_rtClient.top + iHalfY;

	if ((m_pos.y + iHalfY) > g_rtClient.bottom)
		m_pos.y = g_rtClient.bottom - iHalfY;

	if ((m_pos.x - iHalfX) < g_rtClient.left)
		m_pos.x = g_rtClient.left + iHalfX;

	if ((m_pos.x + iHalfX) > g_rtClient.right)
		m_pos.x = g_rtClient.right - iHalfX;

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
	m_fDamageTimeGap = g_HERO_DAMAGE_TIME_GAP;
}

THeroObject::~THeroObject()
{
}

bool THeroMgr::Init()
{
	return m_Hero.Create(g_pd3dDevice, 150, 100, (g_rtClient.right / 2), g_INIT_HERO_POSX, 0, 0, 150, 100, L"vertexshader.txt", L"../data/Resource/Hero.png");
}
bool THeroMgr::Frame()
{
	return m_Hero.Frame();
}

bool THeroMgr::Render()
{
	return m_Hero.Render();
}

bool THeroMgr::Release()
{
	return m_Hero.Release();
}

void THeroMgr::Reset()
{
	m_Hero.SetPosition((g_rtClient.right / 2), g_INIT_HERO_POSX);
	m_Hero.SetMAXHP(g_Hero_MAXHP);
}

THeroMgr::THeroMgr()
{

}

THeroMgr::~THeroMgr()
{

}