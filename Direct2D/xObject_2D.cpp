#include "xObject_2D.h"
#include "xInput.h"

const int g_NPCDamageTimeGap = 0.5f;

void xObject_2D::SetPosition(TPoint pos)
{
	m_pos = pos;
}

void xObject_2D::SetPosition(float xPos, float yPos, DWORD left, DWORD top, DWORD right, DWORD bottom)
{
	m_pos.x = xPos;
	m_pos.y = yPos;

	m_rtDraw.left = left;
	m_rtDraw.top = top;
	m_rtDraw.right = right;
	m_rtDraw.bottom = bottom;

	m_posDraw.x = m_pos.x - (m_rtDraw.right / 2);
	m_posDraw.y = m_pos.y - (m_rtDraw.bottom / 2);

	m_fDir[0] = 0.0f;								//NPC는 위에서 아래로만 움직인다.
	m_fDir[1] = 1.0f;

	m_rtCollision.left = m_posDraw.x;
	m_rtCollision.top = m_posDraw.y;
	m_rtCollision.right = m_rtCollision.left + m_rtDraw.right;
	m_rtCollision.bottom = m_rtCollision.top + m_rtDraw.bottom;

	m_iMaxDistance = sqrt((m_rtDraw.right * m_rtDraw.right)
		+ (m_rtDraw.bottom * m_rtDraw.bottom));
}

void xObject_2D::SetDirectionSpeed(int dirX, int dirY, float speed)
{
	m_fDir[0] = dirX;
	m_fDir[1] = dirY;
	m_fSpeed = speed;
}

bool xObject_2D::Init()
{
	xPlaneObj::Init();
	m_fAngle = 0.0f;

	m_fLastDamageTime = g_fGameTimer;
	m_iHP = 1;
	
	return true;
}
bool xObject_2D::Frame()
{
	xPlaneObj::Frame();

	if (I_Input.m_KeyState[DIK_W])
	{
		m_pos.y += -1 * g_fSecPerFrame * 300.0f;
		//m_bitmap.SetOffeSet(0, 1 * g_fSecPerFrame * m_fSpeed);
	}
	if (I_Input.m_KeyState[DIK_S])
	{
		m_pos.y += 1 * g_fSecPerFrame * 300.0f;
		//m_bitmap.SetOffeSet(0, -1 * g_fSecPerFrame * m_fSpeed);
	}
	if (I_Input.m_KeyState[DIK_A])
	{
		m_pos.x += -1 * g_fSecPerFrame * 300.0f;
		//m_bitmap.SetOffeSet(-1 * g_fSecPerFrame * m_fSpeed, 0);
	}
	if (I_Input.m_KeyState[DIK_D])
	{
		m_pos.x += 1 * g_fSecPerFrame * 300.0f;
		//m_bitmap.SetOffeSet(1 * g_fSecPerFrame * m_fSpeed, 0);
	}

	m_posDraw.x = m_pos.x - (m_rtDraw.right / 2);
	m_posDraw.y = m_pos.y - (m_rtDraw.bottom / 2);

	m_rtCollision.left = m_posDraw.x;
	m_rtCollision.top = m_posDraw.y;
	m_rtCollision.right = m_rtCollision.left + m_rtDraw.right;
	m_rtCollision.bottom = m_rtCollision.top + m_rtDraw.bottom;
	return true;
}
bool xObject_2D::Render()
{
	if (IsDead())
		return true;

	return xPlaneObj::Render();
}
bool xObject_2D::Release()
{
	return xPlaneObj::Release();
}

bool xObject_2D::IsDead()
{
	return ((0 < m_iHP) ? false : true);
}
void xObject_2D::ProcessDamage(int damage)
{
	//ProcessDamage가 여러번 처리되는걸 막는다.
	TCHAR	m_csBuffer[256];
	if ((m_fLastDamageTime + m_fDamageTimeGap) < g_fGameTimer)
	{
		m_fLastDamageTime += m_fDamageTimeGap;

		m_iHP = m_iHP + damage;
		m_iHP = max(0, m_iHP);
		_stprintf_s(m_csBuffer, L"ProcessDamage()!!!!!!!! m_iHP: %d damage : %d\n", m_iHP, damage);
		OutputDebugString(m_csBuffer);
	}
}

xObject_2D::xObject_2D()
{
	m_pos.x = 0;
	m_pos.y = 0;
	m_posDraw.x = 0;
	m_posDraw.y = 0;

	m_fDir[0] = 1.0f;
	m_fDir[1] = 1.0f;

	m_fSpeed = 100.0f;
	m_fAttackRadius = 200.0f;

	m_bDebugRect = false;
	m_iHP = 1;

	m_fAngle = 0.0f;
	m_fAlpha = 255.0f;

	m_fLastDamageTime = g_fGameTimer;
	m_fDamageTimeGap = g_NPCDamageTimeGap;
}


xObject_2D::~xObject_2D()
{
}
