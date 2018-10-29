#include "TEffectObject.h"
#include "TCollision.h"
#include "THeroObject.h"
#include "TNPCObject.h"
#include "xInput.h"

const float g_EffectTimeGap = 0.1f;
bool TEffectObject::Init()
{
	m_iCurrentSprite = 0;
	m_iIndexSprite = 0;
	m_fSpriteTime = 1.0f;
	m_fLifeTime = 10.0f;
	m_fOffSet = 1.0f;

	m_fDir[0] = 0.0f;
	m_fDir[1] = -1.0f;
	m_fSpeed = 100.0f;

	return xObject::Init();
}
bool TEffectObject::Frame()
{
	if (IsDead())	return true;

	m_fLifeTime -= g_fSecPerFrame;

	if (m_fLifeTime < 0.0f)
	{
		m_fLifeTime = 10.0f;
		SetDead();
	}

	int iHalfY = m_rtDraw.bottom / 2;

	if ((m_pos.y + iHalfY) > g_rtClient.bottom)
	{
		SetDead();
	}
	if ((m_pos.y - iHalfY) < g_rtClient.top)
	{
		SetDead();
	}

	m_pos.x += m_fDir[0] * m_fSpeed * g_fSecPerFrame;
	m_pos.y += m_fDir[1] * m_fSpeed * g_fSecPerFrame;

	return xObject::Frame();
}

TEffectObject::TEffectObject(xObject*	pOwner) : m_pOwner(pOwner)
{
	m_iCurrentSprite = 0;
	m_iIndexSprite = 0;
	m_fSpriteTime = 1.0f;
	m_fLifeTime = 10.0f;
	m_fOffSet = 1.0f;

	m_fDir[0] = 0.0f;
	m_fDir[1] = -1.0f;
	m_fSpeed = 100.0f;
}


TEffectObject::~TEffectObject()
{
}


bool TEffectMgr::Init()
{
	SpriteDataLoad(L"../data/Resource/SpriteList.txt");
	return true;
}

bool TEffectMgr::Frame()
{
	m_fAngle += g_fSecPerFrame * 100.0f;

	if (I_Input.m_CurrentMouseState.rgbButtons[0] == KEY_PUSH)
	{
		static float fAddTime = 0.0f;
		fAddTime += g_fSecPerFrame;

		if (fAddTime >= g_EffectTimeGap)
		{
			AddEffectByHero();
			fAddTime -= g_EffectTimeGap;
		}
	}

	DeleteEffectList();

	list<TEffectObject*>::iterator iter;
	for (iter = m_effectObjListByHero.begin(); iter != m_effectObjListByHero.end(); iter++)
	{
		TEffectObject* pEffectObject = (*iter);
		pEffectObject->m_fOffSet = 1.0f / m_rtSpriteList[pEffectObject->m_iIndexSprite].size();
		pEffectObject->m_fSpriteTime += g_fSecPerFrame;

		if (pEffectObject->m_fSpriteTime >= pEffectObject->m_fOffSet)
		{
			pEffectObject->m_iCurrentSprite++;
			if (!m_rtSpriteList.empty())
			{
				if (pEffectObject->m_iCurrentSprite >= m_rtSpriteList[pEffectObject->m_iIndexSprite ].size())
				{
					pEffectObject->m_iCurrentSprite = 0;
				}
			}

			pEffectObject->m_fSpriteTime -= pEffectObject->m_fOffSet;
		}

		if (!m_rtSpriteList.empty())
		{
			RECT rt = m_rtSpriteList[pEffectObject->m_iIndexSprite][pEffectObject->m_iCurrentSprite];
			pEffectObject->SetTexture(rt.left, rt.top, rt.right, rt.bottom);

			pEffectObject->m_fAngle = m_fAngle;
			pEffectObject->Frame();
		}
	}
	return true;
}

bool TEffectMgr::Render()
{
	list<TEffectObject*>::iterator iter;
	for (iter = m_effectObjListByHero.begin(); iter != m_effectObjListByHero.end(); iter++)
	{
		(*iter)->Render();
	}

	for (iter = m_effectObjListByNPC.begin(); iter != m_effectObjListByNPC.end(); iter++)
	{
		(*iter)->Render();
	}
	return true;
}

bool TEffectMgr::Release()
{
	//m_effectObjListByHero
	list<TEffectObject*>::iterator iter;
	for (iter = m_effectObjListByHero.begin(); iter != m_effectObjListByHero.end(); iter++)
	{
		(*iter)->Release();
		delete (*iter);
	}
	m_effectObjListByHero.clear();

	//m_effectObjListByNPC
	for (iter = m_effectObjListByNPC.begin(); iter != m_effectObjListByNPC.end(); iter++)
	{
		(*iter)->Release();
		delete (*iter);
	}
	m_effectObjListByNPC.clear();
	return true;
}

bool TEffectMgr::SpriteDataLoad(const TCHAR* pszFileName)
{
	m_rtSpriteList.clear();

	TCHAR pBuffer[256] = { 0, };
	TCHAR pTemp[256] = { 0, };

	int iNumSprite = 0;
	FILE* fp_src;

	_wfopen_s(&fp_src, pszFileName, _T("rt"));

	if (fp_src == NULL) return false;

	_fgetts(pBuffer, _countof(pBuffer), fp_src);
	_stscanf_s(pBuffer, _T("%s%d%s"), pTemp, _countof(pTemp), &iNumSprite);

	m_rtSpriteList.resize(iNumSprite);

	for (int iCnt = 0; iCnt < iNumSprite; iCnt++)
	{
		int iNumFrame = 0;
		_fgetts(pBuffer, _countof(pBuffer), fp_src);
		_stscanf_s(pBuffer, _T("%s%d"), pTemp, _countof(pTemp), &iNumFrame);

		m_rtSpriteList[iCnt].clear();

		RECT rt;
		for (int iFrame = 0; iFrame < iNumFrame; iFrame++)
		{
			_fgetts(pBuffer, _countof(pBuffer), fp_src);
			_stscanf_s(pBuffer, _T("%s %d %d %d %d"), pTemp, _countof(pTemp), &rt.left, &rt.top, &rt.right, &rt.bottom);

			m_rtSpriteList[iCnt].push_back(rt);
		}
	}
	fclose(fp_src);

	return true;
}

void TEffectMgr::AddEffectByHero()
{
	TEffectObject* pObj = new TEffectObject(&(I_HeroMgr.m_Hero));
	
	//pObj->m_iIndexSprite = rand() % m_rtSpriteList.size();
	pObj->m_iIndexSprite = 1;
	if ( (pObj->m_iIndexSprite) >= m_rtSpriteList.size() )
		return;

	if (m_rtSpriteList[pObj->m_iIndexSprite].empty())
		return;

	pObj->Create(g_pd3dDevice, 400, 300, g_pHeroPos.x, g_pHeroPos.y, 
		m_rtSpriteList[pObj->m_iIndexSprite][0].left, m_rtSpriteList[pObj->m_iIndexSprite][0].top, 
		m_rtSpriteList[pObj->m_iIndexSprite][0].right, m_rtSpriteList[pObj->m_iIndexSprite][0].bottom, L"vertexshader.txt", L"../data/Resource/effect.bmp");
	pObj->SetDirectionSpeed(0.0f, -1.0f, 500.0f);
	m_effectObjListByHero.push_back(pObj);
}

void TEffectMgr::AddEffectByNPC(TNPCObject* pOwner)
{
	TEffectObject* pObj = new TEffectObject(pOwner);
	pObj->Create(g_pd3dDevice, 400, 300, pOwner->m_pos.x, pOwner->m_pos.y, 0, 142, 42, 183, L"vertexshader.txt", L"../data/Resource/effect.bmp");
	pObj->m_iIndexSprite = 1;
	pObj->SetDirectionSpeed(0.0f, -1.0f, 500.0f);
	m_effectObjListByNPC.push_back(pObj);
}

bool TEffectMgr::IsCollisionAndDeleteList(RECT rt, bool isHeroEffect)
{
	list<TEffectObject*>::iterator iter;

	if (isHeroEffect)
	{
		for (iter = m_effectObjListByHero.begin(); iter != m_effectObjListByHero.end(); iter++)
		{
			if (TCollision::RectInRect(rt, (*iter)->m_rtCollision))
			{
				//충돌되었으면 m_effectObjListByHero에서 삭제한다.
				m_effectObjListByHero.erase(iter);
				return true;
			}
		}
	}
	else
	{
		for (iter = m_effectObjListByNPC.begin(); iter != m_effectObjListByNPC.end(); iter++)
		{
			if (TCollision::RectInRect(rt, (*iter)->m_rtCollision))
			{
				return true;
			}
		}
	}
	return false;
}


void TEffectMgr::DeleteEffectList()
{
	list<TEffectObject*>::iterator iter;
	for (iter = m_effectObjListByHero.begin(); iter != m_effectObjListByHero.end();)
	{
		if ((*iter)->IsDead())
		{
			(*iter)->Release();
			delete (*iter);
			iter = m_effectObjListByHero.erase(iter);
		}
		else
			iter++;
	}

	for (iter = m_effectObjListByNPC.begin(); iter != m_effectObjListByNPC.end();)
	{
		if ((*iter)->IsDead())
		{
			(*iter)->Release();
			delete (*iter);
			iter = m_effectObjListByNPC.erase(iter);
		}
		else
			iter++;
	}
}

TEffectMgr::TEffectMgr()
{
}


TEffectMgr::~TEffectMgr()
{
}