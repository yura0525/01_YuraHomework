#pragma once
#include "TBasisStd.h"
class TImage
{
public:
	HDC				hDC;
	HBITMAP			hBitmap;
	HBITMAP			hOldBitmap;
	int				m_iIndex;
	TCHAR_STRING	m_strName;
public:
	bool	Init();
	bool	Frame();
	bool	Render();
	bool	Release();	
	bool	Load( HWND hWnd, HDC hdc, TCHAR_STRING name );
	bool	InitImage( HWND hWnd, HDC hdc, UINT id );
	bool	InitImage(  HWND hWnd, HDC hdc, int x, int y );
	
	void  Draw( TImage* pScreen, 
		int x, int y, int Width, int height,
		int iSrcX=0, int iSrcY=0, DWORD dwMode = SRCCOPY);
	
	void	DrawBitMap( HDC hdc, int x, int y, HBITMAP hBit );
	HBITMAP GetRotatedBitmap(float fAngle, int iSizeWidth, int iSizeHeight, RECT rt);

	void TransBlt(HDC hdc, int x, int y, HBITMAP hbitmap, COLORREF clrMask);
public:
	TImage(void);
	~TImage(void);
};
