#include "TBulletObject.h"

void TBulletObject::SetSpriteList(vector<RECT_ARRAY>& rtSpriteList)
{
	m_rtSpriteList = rtSpriteList;
}

void TBulletObject::SetBulletDirectionSpeed(int dirX, int dirY, float speed)
{
	m_fDir[0] = dirX;
	m_fDir[1] = dirY;
	m_fSpeed = speed;
}

bool TBulletObject::Init()
{
	if( !m_rtSpriteList.empty())
		m_fOffSet = 1.0f / m_rtSpriteList[0].size();

	return true;
}

bool TBulletObject::Frame()
{
	/*static float fSpriteTime = 0.0f;
	fSpriteTime += g_fSecPerFrame;

	if (fSpriteTime >= m_fOffSet)
	{
		m_iCurrentSprite++;
		if (!m_rtSpriteList.empty())
		{
			if (m_iCurrentSprite >= m_rtSpriteList[0].size())
			{
				m_iCurrentSprite = 0;
			}
		}
		
		fSpriteTime -= m_fOffSet;
	}

	if (!m_rtSpriteList.empty())
	{
		RECT rt = m_rtSpriteList[0][m_iCurrentSprite];
		m_rtDraw = rt;
	}*/

	int iHalfX = m_rtDraw.right / 2;
	int iHalfY = m_rtDraw.bottom / 2;

	if ((m_pos.y + iHalfY) > g_rtClient.bottom)
	{
		m_bDead = true;
	}
	if ((m_pos.y - iHalfY) < g_rtClient.top)
	{
		m_bDead = true;
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

TBulletObject::TBulletObject()
{
	m_iCurrentSprite = 0;
	m_fOffSet = 1.0f;

	m_fDir[0] = 0.0f;
	m_fDir[1] = 1.0f;
	m_fSpeed = 100.0f;
}


TBulletObject::~TBulletObject()
{
}
