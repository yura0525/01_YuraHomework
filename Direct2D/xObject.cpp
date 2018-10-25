#include "xObject.h"
#include "xInput.h"

const int g_NPCDamageTimeGap = 0.5f;

void xObject::SetPosition(float xPos, float yPos)
{
	m_pos.x = xPos;
	m_pos.y = yPos;

	m_posDraw.x = m_pos.x - (m_rtDraw.right / 2);
	m_posDraw.y = m_pos.y - (m_rtDraw.bottom / 2);

	m_rtCollision.left = m_posDraw.x;
	m_rtCollision.top = m_posDraw.y;
	m_rtCollision.right = m_rtCollision.left + m_rtDraw.right;
	m_rtCollision.bottom = m_rtCollision.top + m_rtDraw.bottom;

	m_iMaxDistance = sqrt((m_rtDraw.right * m_rtDraw.right)
		+ (m_rtDraw.bottom * m_rtDraw.bottom));
}

void xObject::SetPosition(float xPos, float yPos, DWORD texLeft, DWORD texTop, DWORD texRight, DWORD texBottom)
{
	m_pos.x = xPos;
	m_pos.y = yPos;

	m_rtDraw.left = texLeft;
	m_rtDraw.top = texTop;
	m_rtDraw.right = (texRight - texLeft);
	m_rtDraw.bottom = (texBottom - texTop);

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

void xObject::SetTexureUV(DWORD texLeft, DWORD texTop, DWORD texRight, DWORD texBottom)
{
	m_rtDraw.left = texLeft;
	m_rtDraw.top = texTop;
	m_rtDraw.right = (texRight - texLeft);
	m_rtDraw.bottom = (texBottom - texTop);

	m_posDraw.x = m_pos.x - (m_rtDraw.right / 2);
	m_posDraw.y = m_pos.y - (m_rtDraw.bottom / 2);

	m_rtCollision.left = m_posDraw.x;
	m_rtCollision.top = m_posDraw.y;
	m_rtCollision.right = m_rtCollision.left + m_rtDraw.right;
	m_rtCollision.bottom = m_rtCollision.top + m_rtDraw.bottom;

	m_iMaxDistance = sqrt((m_rtDraw.right * m_rtDraw.right)
		+ (m_rtDraw.bottom * m_rtDraw.bottom));
}

void xObject::SetMaxTexureUV(float texMaxU, float texMaxV)
{
	m_MaxTexUV.u = texMaxU;
	m_MaxTexUV.v = texMaxV;
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
	// 0 ~ 800 - > 0 ~1
	D3DXVECTOR2 uvRet;
	uvRet.u = _u / m_MaxTexUV.u;
	uvRet.v = _v / m_MaxTexUV.v;

	return uvRet;
}

void xObject::CreateVertexData()
{
	m_verList.resize(4);

	UpdateVertexDataPosition();
	UpdateTextureData();

	m_verList[0].c = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	m_verList[1].c = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	m_verList[2].c = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	m_verList[3].c = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
}

void xObject::UpdateVertexDataPosition()
{
	D3DXVECTOR3 pos = Generate(m_posDraw.x, m_posDraw.y);
	m_verList[0].p = D3DXVECTOR3(pos.x, pos.y, 0.0f);

	pos = Generate(m_posDraw.x + m_rtDraw.right, m_posDraw.y);
	m_verList[1].p = D3DXVECTOR3(pos.x, pos.y, 0.0f);

	pos = Generate(m_posDraw.x, m_posDraw.y + m_rtDraw.bottom);
	m_verList[2].p = D3DXVECTOR3(pos.x, pos.y, 0.0f);

	pos = Generate(m_posDraw.x + m_rtDraw.right, m_posDraw.y + m_rtDraw.bottom);
	m_verList[3].p = D3DXVECTOR3(pos.x, pos.y, 0.0f);
}

void xObject::UpdateTextureData()
{
	D3DXVECTOR2 uv = UVGenerate(m_rtDraw.left, m_rtDraw.top);
	m_verList[0].t = D3DXVECTOR2(uv.u, uv.v);

	uv = UVGenerate(m_rtDraw.left + m_rtDraw.right, m_rtDraw.top);
	m_verList[1].t = D3DXVECTOR2(uv.u, uv.v);

	uv = UVGenerate(m_rtDraw.left, m_rtDraw.top + m_rtDraw.bottom);
	m_verList[2].t = D3DXVECTOR2(uv.u, uv.v);

	uv = UVGenerate(m_rtDraw.left + m_rtDraw.right, m_rtDraw.top + m_rtDraw.bottom);
	m_verList[3].t = D3DXVECTOR2(uv.u, uv.v);
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
	m_posDraw.x = m_pos.x - (m_rtDraw.right / 2);
	m_posDraw.y = m_pos.y - (m_rtDraw.bottom / 2);

	m_rtCollision.left = m_posDraw.x;
	m_rtCollision.top = m_posDraw.y;
	m_rtCollision.right = m_rtCollision.left + m_rtDraw.right;
	m_rtCollision.bottom = m_rtCollision.top + m_rtDraw.bottom;

	UpdateVertexDataPosition();
	UpdateTextureData();
	return xObject_2D::Frame();
}
bool xObject::Render()
{
	if (IsDead())
		return true;

	return xObject_2D::Render();
}

bool xObject::Create(ID3D11Device* pd3dDevice, float texMaxU, float texMaxV, float xPos, float yPos, DWORD left, DWORD top, DWORD width, DWORD height,
	T_STR szShaderName, T_STR szTexName, T_STR VSFunc, T_STR PSFunc)
{
	Init();
	SetMaxTexureUV(texMaxU, texMaxV);
	SetPosition(xPos, yPos, left, top, width, height);
	CreateVertexData();

	CreateVertexBuffer(pd3dDevice);
	CreateIndexBuffer(pd3dDevice);
	CreateConstantBuffer(pd3dDevice);
	CreateShader(pd3dDevice, szShaderName);
	CreateInputLayout(pd3dDevice);
	CreateTexture(pd3dDevice, szTexName);
	SetRasterizerState(pd3dDevice);
	SetBlendState(pd3dDevice);
	return true;
}

bool xObject::IsDead()
{
	return ((0 < m_iHP) ? false : true);
}

void xObject::ProcessDamage(int damage)
{
	//ProcessDamage가 여러번 처리되는걸 막는다.
	if ((m_fLastDamageTime + m_fDamageTimeGap) < g_fGameTimer)
	{
		m_fLastDamageTime += m_fDamageTimeGap;

		m_iHP = m_iHP + damage;
		m_iHP = max(0, m_iHP);
	}
}

xObject::xObject()
{
	m_pos.x = 0;
	m_pos.y = 0;
	m_posDraw.x = 0;
	m_posDraw.y = 0;
	m_MaxTexUV.u = 0.0f;
	m_MaxTexUV.v = 0.0f;

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
