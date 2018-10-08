#include "xObject.h"
#include "xInput.h"

const int g_NPCDamageTimeGap = 0.5f;

//void xObject::SetPosition(TPoint pos)
//{
//	m_pos = pos;
//}

void xObject::SetPosition(float xPos, float yPos, DWORD left, DWORD top, DWORD right, DWORD bottom)
{
	m_pos.x = left;
	m_pos.y = top;

	m_rtDraw.left = xPos;
	m_rtDraw.top = yPos;
	m_rtDraw.right = right;
	m_rtDraw.bottom = bottom;

	//m_posDraw.x = m_pos.x - (m_rtDraw.right / 2);
	//m_posDraw.y = m_pos.y - (m_rtDraw.bottom / 2);

	m_fDir[0] = 0.0f;								//NPC는 위에서 아래로만 움직인다.
	m_fDir[1] = 1.0f;

	m_rtCollision.left = m_posDraw.x;
	m_rtCollision.top = m_posDraw.y;
	m_rtCollision.right = m_rtCollision.left + m_rtDraw.right;
	m_rtCollision.bottom = m_rtCollision.top + m_rtDraw.bottom;

	m_iMaxDistance = sqrt((m_rtDraw.right * m_rtDraw.right)
		+ (m_rtDraw.bottom * m_rtDraw.bottom));

	SetVertexData();
}

void xObject::SetTexUV(float _u, float _v)
{
	m_texUV.u = _u;
	m_texUV.v = _v;
}

D3DXVECTOR3 xObject::Generate(float x, float y)
{
	D3DXVECTOR3 vRet;
	// 0 ~ 800 - > 0 ~1 -> -1 ~ +1
	vRet.x = x / 800;
	vRet.y = y / 600;
	vRet.x = vRet.x * 2.0f - 1.0f;
	vRet.y = (vRet.y * 2.0f - 1.0f)*-1.0f;
	return vRet;
}

D3DXVECTOR2 xObject::UVGenerate(float _u, float _v)
{
	D3DXVECTOR2 uvRet;
	uvRet.u = _u / m_texUV.u;
	uvRet.v = _v / m_texUV.v;

	return uvRet;
}

void xObject::SetVertexData()
{
	m_verList.resize(4);

	D3DXVECTOR3 pos = Generate(m_rtDraw.left, m_rtDraw.top);
	D3DXVECTOR2 uv = UVGenerate(m_pos.x, m_pos.y);
	m_verList[0].p = D3DXVECTOR3(pos.x, pos.y, 0.0f);
	m_verList[0].t = D3DXVECTOR2(uv.u, uv.v);
	m_verList[0].c = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);

	pos = Generate(m_rtDraw.left + m_rtDraw.right,
		m_rtDraw.top);
	uv = UVGenerate(m_pos.x + m_rtDraw.right, m_pos.y);
	m_verList[1].p = D3DXVECTOR3(pos.x, pos.y, 0.0f);
	m_verList[1].t = D3DXVECTOR2(uv.u, uv.v);
	m_verList[1].c = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);

	pos = Generate(m_rtDraw.left,
		m_rtDraw.top + m_rtDraw.bottom);
	uv = UVGenerate(m_pos.x, m_pos.y + m_rtDraw.bottom);
	m_verList[2].p = D3DXVECTOR3(pos.x, pos.y, 0.0f);
	m_verList[2].t = D3DXVECTOR2(uv.u, uv.v);
	m_verList[2].c = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);

	pos = Generate(m_rtDraw.left + m_rtDraw.right,
		m_rtDraw.top + m_rtDraw.bottom);
	uv = UVGenerate(m_pos.x + m_rtDraw.right, m_pos.y + m_rtDraw.bottom);
	m_verList[3].p = D3DXVECTOR3(pos.x, pos.y, 0.0f);
	m_verList[3].t = D3DXVECTOR2(uv.u, uv.v);
	m_verList[3].c = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);

	GenCenter();
}
void xObject::GenCenter()
{
	m_pos.x = 0.0f;
	m_pos.y = 0.0f;
	for (int iV = 0; iV < m_verList.size(); iV++)
	{
		D3DXVECTOR3 vertex = m_verList[iV].p;
		m_pos.x += vertex.x;
		m_pos.y += vertex.y;
	}

	m_pos.x /= (float)(m_verList.size());
	m_pos.y /= (float)(m_verList.size());
}
void xObject::SetDirectionSpeed(int dirX, int dirY, float speed)
{
	m_fDir[0] = dirX;
	m_fDir[1] = dirY;
	m_fSpeed = speed;
}

bool xObject::Init()
{
	xObject_2D::Init();
	m_fAngle = 0.0f;

	m_fLastDamageTime = g_fGameTimer;
	m_iHP = 1;
	
	return true;
}
bool xObject::Frame()
{
	xObject_2D::Frame();

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
bool xObject::Render()
{
	if (IsDead())
		return true;

	return xObject_2D::Render();
}
bool xObject::Release()
{
	return xObject_2D::Release();
}

bool xObject::IsDead()
{
	return ((0 < m_iHP) ? false : true);
}
void xObject::ProcessDamage(int damage)
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

xObject::xObject()
{
	m_pos.x = 0;
	m_pos.y = 0;
	m_posDraw.x = 0;
	m_posDraw.y = 0;
	m_texUV.u = 0.0f;
	m_texUV.v = 0.0f;

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


xObject::~xObject()
{
}
