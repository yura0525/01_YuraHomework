#pragma once

#include "xStd.h"
#include <d2d1.h>
#include <dwrite.h>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")
struct TTextArray
{
	D2D1_MATRIX_3X2_F		m_matWorld;
	D2D1_RECT_F				m_rf;
	D3DCOLORVALUE			m_color;
	T_STR					m_pText;

	TTextArray()
	{
		m_matWorld = D2D1::IdentityMatrix();
		m_color = D2D1::ColorF(0, 0, 0, 1);
		m_rf.left = g_rtClient.left;
		m_rf.top = g_rtClient.top;
		m_rf.right = g_rtClient.right;
		m_rf.bottom = g_rtClient.bottom;
	}
};
class TDirectWrite
{
public:
	std::vector<TTextArray>		m_TextList;

public:
	ID2D1Factory *			m_pd2dFactory;
	IDWriteFactory*			m_pWriteFactory;
	ID2D1RenderTarget*		m_pRT;
	IDWriteTextFormat*		m_pTextFormat;
	ID2D1SolidColorBrush*	m_pColorBrush;

	float					m_fdpiX;
	float					m_fdpiY;
public:
	bool Set(HWND hWnd, int iWidth, int iHeight, IDXGISurface1* pSurface);
	
	HRESULT CreateDeviceIndependentResource();
	HRESULT CreateDeviceResource(IDXGISurface1* pSurface);

	void DiscardDeviceResource();
	
	HRESULT DrawText(RECT rt, const TCHAR* pText, D2D1::ColorF color);
	void OnResize(int iWidth, int iHeight, IDXGISurface1* pSurface);

public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();
public:
	TDirectWrite();
	virtual ~TDirectWrite();
};

