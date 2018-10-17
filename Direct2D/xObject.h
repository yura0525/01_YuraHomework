#pragma once
#include "xObject_2D.h"
#include "TCollision.h"

struct TPoint
{
	float x;
	float y;
};

class xObject : public xObject_2D
{
public:
	TPoint	m_pos;
	TPoint	m_posDraw;

	RECT	m_rtDraw;
	RECT	m_rtCollision;

	bool	m_bDebugRect;

	float	m_fAlpha;

	D3DXVECTOR2		m_MaxTexUV;		//�ؽ��� ������
	//�̵��� ���� ������.
public:
	float	m_fDir[2];
	float	m_fSpeed;
	float	m_fAttackRadius;
	RECT	m_rtAttackRect;
public:
	float				m_iMaxDistance;
	float				m_fAngle;

	//HP�� ���� ������
public:
	int		m_iHP;
	float	m_fLastDamageTime;
	float	m_fDamageTimeGap;		//����Ÿ��	
public:
	void SetPosition(float xPos, float yPos);
	void SetPosition(float xPos, float yPos, DWORD texLeft, DWORD texTop, DWORD texRight, DWORD texBottom);

	//�ؽ�����ǥ�� ���ؽ� ��ǥ ���� �Լ�.
	void SetTexUV(float texMaxU, float texMaxV);

	D3DXVECTOR3 Generate(float x, float y);
	D3DXVECTOR2 UVGenerate(float _u, float _v);
	
	void UpdateVertexData();

	void SetDirectionSpeed(int dirX, int dirY, float speed);


	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();

	virtual bool Create(ID3D11Device* pd3dDevice, float _texMaxU, float _texMaxV, 
						float xPos, float yPos, DWORD left, DWORD top, DWORD width, DWORD height,
						T_STR szShaderName, T_STR szTexName, T_STR VSFunc = L"VS", T_STR PSFunc = L"PS");

	//FadeIn & FadeOut
	virtual bool FadeOut() { return true; }
	virtual bool FadeIn() { return true; }


	//������ ���� ó��
	bool IsDead();
	void SetDead() { m_iHP = 0; }
	void SetMAXHP(int hp) { m_iHP = hp; }
	void ProcessDamage(int damage);

public:
	xObject();
	virtual ~xObject();
};

