#pragma once
#include "TBitmapMgr.h"

#define LR_ROTATION 0x01
#define TB_ROTATION 0x02
#define LRTB_ROTATION 0x04

struct TPoint
{
	float x;
	float y;
};

class TObject
{
public:
	TBitmap* m_pColorBitmap;
	TBitmap* m_pMaskBitmap;
	TPoint	m_pos;
	TPoint	m_posDraw;

	RECT	m_rtDraw;
	RECT	m_rtCollision;

	float	m_fDir[2];
	float	m_fSpeed;
	
	bool	m_bDead;
	bool	m_bDebugRect;

	float	m_fAlpha;
	
public:
	//비트맵 회전과 관련된 변수들.
	HBITMAP				m_hColorRotateBitmap;
	HBITMAP				m_hMaskRotateBitmap;
	HDC					m_hRotationDC;
	HDC					m_hMemMaskDC;
	HDC					m_hMemColorDC;
	HBRUSH				m_hBrBack;

	float				m_iMaxDistance;
	float				m_fAngle;
public:

	void SetPosition(TPoint pos);
	void SetPosition(float xPos, float yPos, DWORD left, DWORD top, DWORD right, DWORD bottom);
	void SetDirectionSpeed(int dirX, int dirY, float speed);

	bool Load(const TCHAR* pszColor, const TCHAR* pszMask = NULL);
	virtual bool LoadUI(const TCHAR* pszColor, const TCHAR* pszMask, int iState)
	{
		return true;
	}

	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();

	virtual bool Draw(short sType, RECT* rt = NULL);
	virtual bool DrawColorKey(DWORD maskColor);

	//비트맵 회전과 관련된 함수들
	void GetRotateBitmap(float fAngle, HBITMAP hBitmap, TBitmap* pSrcBitmap);
	void RotationBlt(float fAngle);

	//FadeIn & FadeOut
	virtual bool FadeOut() { return true; }
	virtual bool FadeIn() { return true; }

	//알파블렌딩
	bool AlphaBlend(HDC dcDest, int x, int y, int cx, int cy,HDC dcSrc, HDC dcMaskSrc, 
		int sx, int sy, int scx, int scy, int alpha, DWORD opMode, COLORREF rgbMask);

public:
	TObject();
	virtual ~TObject();
};

