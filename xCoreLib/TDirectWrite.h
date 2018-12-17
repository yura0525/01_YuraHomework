#pragma once
#include "xStd.h"
#include <d2d1.h>
#include <dwrite.h>
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

struct TTextArray
{
	D2D1_MATRIX_3X2_F	matWorld;
	D2D1_RECT_F			rf;
	D3DCOLORVALUE		color;
	T_STR				pText;
	//IDWriteTextFormat*	pTextFormat;
	TTextArray()
	{
		matWorld = D2D1::IdentityMatrix();
		color = D2D1::ColorF(0, 0, 0, 1);
		rf.left = g_rtClient.left;
		rf.top = g_rtClient.top;
		rf.right = g_rtClient.right;
		rf.bottom = g_rtClient.bottom;
	}
};
class TDirectWrite
{
public:
	std::vector<TTextArray>		m_TextList;
	D2D1_MATRIX_3X2_F			m_matWorld;

public:
	ID2D1Factory*			m_pd2dFactory;
	IDWriteFactory*			m_pWriteFactory;
	ID2D1RenderTarget*		m_pRT;
	IDWriteTextFormat*		m_pTextFormat;
	ID2D1SolidColorBrush*	m_pColorBrush;

	float m_fdpiX;
	float m_fdpiY;
public:
	bool Set(HWND hWnd, int iWidth, int iHeight, IDXGISurface1*	pSurface);
	HRESULT CreateDeviceIndependentResources();
	HRESULT CreateDeviceResources(IDXGISurface1*	m_pSurface);

	void DiscardDeviceResources();
	HRESULT DrawText(RECT rt, const TCHAR* pText, D2D1::ColorF color);
	void OnResize(int iWidth, int iHeight, IDXGISurface1*	pSurface);
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
	TDirectWrite();
	virtual ~TDirectWrite();
};

