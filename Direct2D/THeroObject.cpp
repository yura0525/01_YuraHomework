#include "THeroObject.h"
#include "xInput.h"
#include "TCollision.h"
#include "TGameDataLoad.h"

POINT	g_pHeroPos;

bool THeroObject::Frame()
{
	int iHalfX = m_rtDraw.right / 2;
	int iHalfY = m_rtDraw.bottom / 2;

	//Hero의 움직임 처리. 화면영역 밖으로 넘어가는것을 막는다.
	if (g_Input.bFront)
		m_pos.y += -1 * g_fSecPerFrame * 300.0f;

	if (g_Input.bBack)
		m_pos.y += 1 * g_fSecPerFrame * 300.0f;

	if (g_Input.bLeft)
		m_pos.x += -1 * g_fSecPerFrame * 300.0f;

	if (g_Input.bRight)
		m_pos.x += 1 * g_fSecPerFrame * 300.0f;

	//벽에 닿았을때 처리.
	if ((m_pos.y - iHalfY) < g_rtClient.top)
		m_pos.y = g_rtClient.top + iHalfY;

	if ((m_pos.y + iHalfY) > g_rtClient.bottom)
		m_pos.y = g_rtClient.bottom - iHalfY;

	if ((m_pos.x - iHalfX) < g_rtClient.left)
		m_pos.x = g_rtClient.left + iHalfX;


	if ((m_pos.x + iHalfX) > g_rtClient.right)
		m_pos.x = g_rtClient.right - iHalfX;

	g_pHeroPos.x = m_pos.x;
	g_pHeroPos.y = m_pos.y;

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
	m_fDamageTimeGap = I_GameDataLoad.g_HERO_DAMAGE_TIME_GAP;
}

THeroObject::~THeroObject()
{
}

bool THeroMgr::Init()
{
	m_HPBar.SetOwner(&m_Hero);
	m_HPBar.Create(g_pd3dDevice, 80, 19, (g_rtClient.right / 2), (I_GameDataLoad.g_INIT_HERO_POSY + I_GameDataLoad.g_INIT_HERO_HP_GAP_POSY),
		0, 0, 80, 19, L"vertexshader.txt", L"../data/Resource/HPBK.png");
	
	m_Hero.Create(g_pd3dDevice, 150, 100, (g_rtClient.right / 2), I_GameDataLoad.g_INIT_HERO_POSY,
		0, 0, 150, 100, L"vertexshader.txt", L"../data/Resource/Hero.png");

	m_Hero.SetMAXHP(I_GameDataLoad.g_HERO_MAXHP);

	//LifeIcon
	m_pLifeIcon = new xObject[I_GameDataLoad.g_STAGE_MAX_LEVEL];
	int lifeIconWidth = 40;
	for (int i = 0; i < I_GameDataLoad.g_STAGE_MAX_LEVEL; i++)
	{
		m_pLifeIcon[i].Create(g_pd3dDevice, 1024, 768, (lifeIconWidth * (i+1)), 50,
			2, 2, 35, 35, L"vertexshader.txt", L"../data/Resource/hero_1.png");
	}
	
	return true;
}
bool THeroMgr::Frame()
{
	for (int i = 0; i < I_GameDataLoad.g_STAGE_MAX_LEVEL; i++)
		m_pLifeIcon[i].Frame();

	m_HPBar.Frame();
	return m_Hero.Frame();
}

bool THeroMgr::Render()
{
	for (int i = 0; i < I_GameDataLoad.g_STAGE_MAX_LEVEL; i++)
		m_pLifeIcon[i].Render();

	m_HPBar.Render();
	return m_Hero.Render();
}

bool THeroMgr::Release()
{
	for (int i = 0; i < I_GameDataLoad.g_STAGE_MAX_LEVEL; i++)
	{
		m_pLifeIcon[i].Release();

	}

	SAFE_DELETE_ARRAY(m_pLifeIcon);

	m_HPBar.Release();
	return m_Hero.Release();
}

void THeroMgr::Reset()
{
	if (g_iCurrentLevel != 0)
	{
		m_pLifeIcon[I_GameDataLoad.g_STAGE_MAX_LEVEL - g_iCurrentLevel].SetDead();
	}

	m_Hero.SetPosition((g_rtClient.right / 2), I_GameDataLoad.g_INIT_HERO_POSY);
	m_Hero.m_fLastDamageTime = g_fGameTimer;
	m_Hero.SetMAXHP(I_GameDataLoad.g_HERO_MAXHP);

	m_HPBar.SetPosition((g_rtClient.right / 2), (I_GameDataLoad.g_INIT_HERO_POSY - I_GameDataLoad.g_INIT_HERO_HP_GAP_POSY));
}

THeroMgr::THeroMgr() : m_HPBar(NULL)
{

}

THeroMgr::~THeroMgr()
{

}