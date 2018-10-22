#pragma once

#include "TBitmapMgr.h"
#include "TCollision.h"

#define LR_ROTATION 0x01
#define TB_ROTATION 0x02
#define LRTB_ROTATION 0x04

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
	bool	m_bDebugRect;
	bool	m_bDead;
	float	m_fAttackRadius;
	
public:
	//비트맵의 회전과 관련된 변수들.
	HBITMAP m_hColorRotateBitmap;
	HBITMAP m_hMaskRotateBitmap;

	HDC		m_hRotationDC;
	HDC		m_hMemMaskDC;
	HDC		m_hMemColorDC;
	
	HBRUSH	m_hBrBack;

	float	m_iMaxDistance;
	float	m_fAngle;

	float	m_fScrollSpeed;
	float	m_fAlpha;
public:
	virtual void Set(TPoint pos);
	virtual void Set(float x, float y,
		DWORD left, DWORD top, DWORD right, DWORD bottom);

	virtual bool FadeOut() {	return true;	}
	virtual bool FadeIn() {		return true;	}

	virtual bool Load(const TCHAR* pszColor, const TCHAR* pszMask = NULL);
	virtual bool LoadUI(const TCHAR* pszColor, const TCHAR* pszMask, int iState)
	{
		return true;
	}

	virtual bool Init();
	virtual bool Frame();
	virtual bool Draw(short sType, RECT* rt = NULL);
	virtual bool DrawColorKey(DWORD maskColor);
	virtual bool Render();
	virtual bool Release();

	void RotationBlt(float fAngle);
	void GetRotateBitmap(float fAngle, HBITMAP hBitmap, TBitmap* pSrcBitmap);

	bool AlphaBlend(HDC hdcDest, int x, int y, int cx, int cy,
		HDC hdcSrc, HDC hdcMaskSrc, int sx, int sy, int scx, int scy, int alpha,
		DWORD opMode, COLORREF rgbMask);
public:
	TObject();
	virtual ~TObject();
};