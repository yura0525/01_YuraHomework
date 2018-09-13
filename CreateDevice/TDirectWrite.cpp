#include "TDirectWrite.h"


bool TDirectWrite::Set(HWND hWnd, int iWidth, int iHeight, IDXGISurface1* pSurface)
{
	CreateDeviceResource(pSurface);
	return true;
}

HRESULT TDirectWrite::CreateDeviceIndependentResource()
{
	HRESULT hr = S_OK;
	if (FAILED(hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pd2dFactory)))
	{
		return hr;
	}
	if (FAILED(hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**)&m_pWriteFactory)))
	{
		return hr;
	}
	if (FAILED(hr = m_pWriteFactory->CreateTextFormat(L"고딕", NULL, DWRITE_FONT_WEIGHT_NORMAL,	//WEIGHT
		DWRITE_FONT_STYLE_NORMAL,	//STYLE
		DWRITE_FONT_STRETCH_NORMAL, //STRETCH
		20, //fontSize
		L"ko-kr", //L"un-us"	영문일때, L"ko-kr" 한국어
		&m_pTextFormat)))
	{
		return hr;
	}
	return hr;
}
HRESULT TDirectWrite::CreateDeviceResource(IDXGISurface1* pSurface)
{
	HRESULT hr = S_OK;
	D2D1_RENDER_TARGET_PROPERTIES	rtp;
	rtp.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;

	D2D1_PIXEL_FORMAT pf;
	pf.format = DXGI_FORMAT_UNKNOWN;
	pf.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;

	rtp.pixelFormat = pf;

	m_pd2dFactory->GetDesktopDpi(&m_fdpiX, &m_fdpiY);

	rtp.dpiX = static_cast<float>(m_fdpiX);
	rtp.dpiY = static_cast<float>(m_fdpiY);

	rtp.usage = D2D1_RENDER_TARGET_USAGE_NONE;
	rtp.minLevel = D2D1_FEATURE_LEVEL_DEFAULT;

	if (FAILED(hr = m_pd2dFactory->CreateDxgiSurfaceRenderTarget(pSurface, 
		(const D2D1_RENDER_TARGET_PROPERTIES*)&rtp, &m_pRT)))				//랜더타겟 생성
	{
		return hr;
	}
	D2D1::ColorF color(D2D1::ColorF::Black);
	if (FAILED(hr = m_pRT->CreateSolidColorBrush(color, &m_pColorBrush)))
	{
		return hr;
	}
	return hr;
}

void TDirectWrite::DiscardDeviceResource()
{
	if (m_pColorBrush != NULL)	m_pColorBrush->Release();
	if( m_pRT != NULL )			m_pRT->Release();

	m_pColorBrush = NULL;
	m_pRT = NULL;
}

HRESULT TDirectWrite::DrawText(RECT rt, const TCHAR* pText, D2D1::ColorF color)
{
	HRESULT hr = S_OK;
	m_pRT->BeginDraw();

	//회전하고 스케일링 하는 부분.
	D2D1::Matrix3x2F matWorld;
	D2D1_POINT_2F	center;
	center.x = 400;
	center.y = 300;

	//float fScale = (cosf(g_fGameTimer) * 0.5f) + 0.5f;
	D2D1::Matrix3x2F rot = matWorld.Rotation(g_fGameTimer * 50.0f, center);
	//D2D1::Matrix3x2F scale = matWorld.Scale(fScale, fScale, center);
	//rot = scale * rot;
	m_pRT->SetTransform(rot);

	m_pColorBrush->SetColor(color);
	
	D2D1_RECT_F	rf = D2D1::RectF(rt.left, rt.top, rt.right, rt.bottom);
	m_pRT->DrawText(pText, wcslen(pText), m_pTextFormat, rf, m_pColorBrush);

	m_pRT->EndDraw();

	return hr;
}
void TDirectWrite::OnResize(int iWidth, int iHeight, IDXGISurface1* pSurface)
{
	DiscardDeviceResource();
	CreateDeviceResource(pSurface);
}


bool TDirectWrite::Init()
{
	CreateDeviceIndependentResource();
	return true;
}
bool TDirectWrite::Frame()
{
	return true;
}
bool TDirectWrite::Render()
{
	m_pRT->BeginDraw();

	//회전하고 스케일링 하는 부분.
	D2D1::Matrix3x2F matWorld;
	D2D1_POINT_2F	center;
	center.x = 400;
	center.y = 300;

	//float fScale = (cosf(g_fGameTimer) * 0.5f) + 0.5f;
	D2D1::Matrix3x2F rot = matWorld.Rotation(g_fGameTimer * 50.0f, center);
	//D2D1::Matrix3x2F scale = matWorld.Scale(fScale, fScale, center);
	//rot = scale * rot;

	for (int iText = 0; iText < m_TextList.size(); iText++)
	{
		m_pRT->SetTransform(rot);
		m_pColorBrush->SetColor(m_TextList[iText].m_color);
		D2D1_RECT_F rf = m_TextList[iText].m_rf;

		rf.top = rf.top + (iText * 30);
		m_pRT->DrawText(m_TextList[iText].m_pText.c_str(), m_TextList[iText].m_pText.size(),
			m_pTextFormat, rf, m_pColorBrush);
	}

	m_pRT->EndDraw();
	return true;
}
bool TDirectWrite::Release()
{
	DiscardDeviceResource();

	if (m_pTextFormat != NULL)		m_pTextFormat->Release();
	if (m_pWriteFactory != NULL)	m_pWriteFactory->Release();
	if (m_pd2dFactory != NULL)		m_pd2dFactory->Release();

	m_pTextFormat = NULL;
	m_pWriteFactory = NULL;
	m_pd2dFactory = NULL;

	return true;
}

TDirectWrite::TDirectWrite()
{
	m_fdpiX = 0.0f;
	m_fdpiY = 0.0f;
}


TDirectWrite::~TDirectWrite()
{
}
