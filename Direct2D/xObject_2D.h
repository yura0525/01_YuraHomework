#pragma once
#include "xPlaneObj.h"
#include "TCollision.h"

struct TPoint
{
	float x;
	float y;
};

class xObject_2D : public xPlaneObj
{
public:
	TPoint	m_pos;
	TPoint	m_posDraw;

	RECT	m_rtDraw;
	RECT	m_rtCollision;

	bool	m_bDebugRect;

	float	m_fAlpha;

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

	void SetPosition(TPoint pos);
	void SetPosition(float xPos, float yPos, DWORD left, DWORD top, DWORD right, DWORD bottom);
	void SetDirectionSpeed(int dirX, int dirY, float speed);


	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();


	//FadeIn & FadeOut
	virtual bool FadeOut() { return true; }
	virtual bool FadeIn() { return true; }


	//������ ���� ó��
	bool IsDead();
	void SetDead() { m_iHP = 0; }
	void SetMAXHP(int hp) { m_iHP = hp; }
	void ProcessDamage(int damage);

public:
	xObject_2D();
	virtual ~xObject_2D();
};

