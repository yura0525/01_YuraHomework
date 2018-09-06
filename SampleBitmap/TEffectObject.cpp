#include "TEffectObject.h"
#include "TCollision.h"
#include "TInput.h"

const int g_iMaxBulletCount = 50;

bool TEffectObject::Frame()
{
	if (IsDead())	return true;

	m_fLifeTime -= g_fSecPerFrame;

	if (m_fLifeTime < 0.0f)
	{
		m_fLifeTime = 1.0f;
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

	if ((m_pos.x - iHalfX) < g_rtClient.left)
	{
		SetDead();
	}

	if ((m_pos.x + iHalfX) > g_rtClient.right)
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

	TCHAR m_csBuffer[256];
	_stprintf_s(m_csBuffer, L"pos.x : %f, pos.y : %f, dirX : %f, dirY : %f, speed : %f",
		m_pos.x, m_pos.y, m_fDir[0], m_fDir[1], m_fSpeed);
	OutputDebugString(m_csBuffer);
	return true;
}

TEffectObject::TEffectObject()
{
	m_iCurrentSprite = 0;
	m_iIndexSprite = 0;
	m_fSpriteTime = 1.0f;
	m_fLifeTime = 1.0f;
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
	//for (int iObj = 0; iObj < m_effectObjList.size(); iObj++)
	//{
	//	if (m_effectObjList[iObj].IsDead())
	//	{
	//		m_effectObjList[iObj].Init();
	//		m_effectObjList[iObj].SetPosition(pos.x, pos.y, 1, 142, 41, 42);
	//		m_effectObjList[iObj].Load(L"../../data/bitmap1.bmp", L"../../data/bitmap2.bmp");
	//		m_effectObjList[iObj].m_iIndexSprite = rand() % m_rtSpriteList.size();
	//		m_effectObjList[iObj].SetDirectionSpeed(0.0f, -1.0f, 1000.0f);
	//		return;
	//	}
	//}

	TEffectObject obj;
	obj.Init();
	obj.SetPosition(pos.x, pos.y, 1, 142, 41, 42);
	obj.Load(L"../../data/bitmap1.bmp", L"../../data/bitmap2.bmp");
	obj.m_iIndexSprite = rand() % m_rtSpriteList.size();
	obj.SetDirectionSpeed(0.0f, -1.0f, 1000.0f);
	m_effectObjList.push_back(obj);

}
bool TEffectMgr::IsCollision(RECT rt)
{
	for (int iObj = 0; iObj < m_effectObjList.size(); iObj++)
	{
		if (TCollision::SphereInSphere(rt, m_effectObjList[iObj].m_rtCollision))
			return true;
	}
	return false;
}
bool TEffectMgr::Init()
{
	/*m_effectObjList.resize(g_iMaxBulletCount);
	for (int iObj = 0; iObj < m_effectObjList.size(); iObj++)
	{
		m_effectObjList[iObj].Init();
		m_effectObjList[iObj].Load(L"../../data/bitmap1.bmp", L"../../data/bitmap2.bmp");
	}*/
	return true;
}
bool TEffectMgr::Frame()
{
	if (I_Input.Key(VK_LBUTTON))
	{
		static float fAddTime = 0.0f;
		fAddTime += g_fSecPerFrame;

		if (fAddTime >= 0.1f)
		{
			AddEffect(g_pHeroPos);
			fAddTime -= 0.1f;
		}
	}

	for (int iObj = 0; iObj < m_effectObjList.size(); iObj++)
	{
		if (m_effectObjList[iObj].IsDead())
			continue;

		m_effectObjList[iObj].m_fOffSet = 1.0f / m_rtSpriteList[m_effectObjList[iObj].m_iIndexSprite].size();
		m_effectObjList[iObj].m_fSpriteTime += g_fSecPerFrame;

		if (m_effectObjList[iObj].m_fSpriteTime >= m_effectObjList[iObj].m_fOffSet)
		{
			m_effectObjList[iObj].m_iCurrentSprite++;
			if (!m_rtSpriteList.empty())
			{
				if (m_effectObjList[iObj].m_iCurrentSprite >= m_rtSpriteList[ m_effectObjList[iObj].m_iIndexSprite ].size())
				{
					m_effectObjList[iObj].m_iCurrentSprite = 0;
				}
			}

			m_effectObjList[iObj].m_fSpriteTime -= m_effectObjList[iObj].m_fOffSet;
		}

		if (!m_rtSpriteList.empty())
		{
			RECT rt = m_rtSpriteList[m_effectObjList[iObj].m_iIndexSprite][m_effectObjList[iObj].m_iCurrentSprite];
			m_effectObjList[iObj].m_rtDraw = rt;
			m_effectObjList[iObj].Frame();
		}
	}
	return true;
}
bool TEffectMgr::Render()
{
	m_fAngle += g_fSecPerFrame * 100.0f;

	for (int iObj = 0; iObj < m_effectObjList.size(); iObj++)
	{
		if (m_effectObjList[iObj].IsDead())
			continue;
		m_effectObjList[iObj].RotationBlt(m_fAngle);
	}
	return true;
}
bool TEffectMgr::Release()
{
	for (int iObj = 0; iObj < m_effectObjList.size(); iObj++)
	{
		m_effectObjList[iObj].Release();
	}
	return true;
}

TEffectMgr::TEffectMgr()
{
	m_fAngle = 0.0f;
}


TEffectMgr::~TEffectMgr()
{
}