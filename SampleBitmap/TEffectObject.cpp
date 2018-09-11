#include "TEffectObject.h"
#include "TCollision.h"
#include "TInput.h"
#include "TLogManager.h"

const float g_EffectTimeGap = 0.1f;
bool TEffectObject::Frame()
{
	if (IsDead())	return true;

	m_fLifeTime -= g_fSecPerFrame;

	if (m_fLifeTime < 0.0f)
	{
		m_fLifeTime = 10.0f;
		SetDead();
	}

	int iHalfX = m_rtDraw.right / 2;
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

	m_posDraw.x = m_pos.x - iHalfX;
	m_posDraw.y = m_pos.y - iHalfY;

	m_rtCollision.left = m_posDraw.x;
	m_rtCollision.top = m_posDraw.y;
	m_rtCollision.right = m_rtCollision.left + m_rtDraw.right;
	m_rtCollision.bottom = m_rtCollision.top + m_rtDraw.bottom;

	return true;
}

TEffectObject::TEffectObject()
{
	m_iCurrentSprite = 0;
	m_iIndexSprite = 0;
	m_fSpriteTime = 1.0f;
	m_fLifeTime = 10.0f;
	m_fOffSet = 1.0f;

	m_fDir[0] = 0.0f;
	m_fDir[1] = 1.0f;
	m_fSpeed = 100.0f;
}


TEffectObject::~TEffectObject()
{
}


bool TEffectMgr::GameDataLoad(const TCHAR* pszFileName)
{
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

void TEffectMgr::AddEffect(POINT pos)
{
	TEffectObject* pObj = new TEffectObject();
	pObj->Init();
	pObj->SetPosition(pos.x, pos.y, 1, 142, 41, 42);
	pObj->Load(L"../../data/bitmap1.bmp", L"../../data/bitmap2.bmp");
	pObj->m_iIndexSprite = rand() % m_rtSpriteList.size();
	pObj->SetDirectionSpeed(0.0f, -1.0f, 500.0f);
	m_effectObjList.push_back(pObj);
}
bool TEffectMgr::IsCollision(RECT rt)
{
	list<TEffectObject*>::iterator iter;
	TCHAR	m_csBuffer[256];
	for (iter = m_effectObjList.begin(); iter != m_effectObjList.end(); iter++)
	{
		if (TCollision::RectInRect(rt, (*iter)->m_rtCollision))
		{
			I_LOGMANAGER.OutputLog("TEffectMgr::IsCollision!!!!!");
			return true;
		}
	}
	return false;
}


bool TEffectMgr::Frame()
{
	if (I_Input.Key(VK_LBUTTON))
	{
		static float fAddTime = 0.0f;
		fAddTime += g_fSecPerFrame;

		if (fAddTime >= g_EffectTimeGap)
		{
			AddEffect(g_pHeroPos);
			fAddTime -= g_EffectTimeGap;
		}
	}

	DeleteEffectList();

	list<TEffectObject*>::iterator iter;
	for (iter = m_effectObjList.begin(); iter != m_effectObjList.end(); iter++)
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
			pEffectObject->m_rtDraw = rt;
			pEffectObject->Frame();
		}
	}
	return true;
}
bool TEffectMgr::Render()
{
	m_fAngle += g_fSecPerFrame * 100.0f;

	list<TEffectObject*>::iterator iter;
	for (iter = m_effectObjList.begin(); iter != m_effectObjList.end(); iter++)
	{
		(*iter)->RotationBlt(m_fAngle);
	}
	return true;
}
bool TEffectMgr::Release()
{
	list<TEffectObject*>::iterator iter;
	for (iter = m_effectObjList.begin(); iter != m_effectObjList.end(); iter++)
	{
		(*iter)->Release();
		delete (*iter);
	}

	m_effectObjList.clear();
	return true;
}

void TEffectMgr::DeleteEffectList()
{
	list<TEffectObject*>::iterator iter;
	for (iter = m_effectObjList.begin(); iter != m_effectObjList.end();)
	{
		if ((*iter)->IsDead())
		{
			(*iter)->Release();
			delete (*iter);
			iter = m_effectObjList.erase(iter);
		}
		else
			iter++;
	}
}

TEffectMgr::TEffectMgr()
{
	m_fAngle = 0.0f;
}


TEffectMgr::~TEffectMgr()
{
}