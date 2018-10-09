#include "TBKObject.h"

bool TBKObject::Render()
{
	/*if (FadeIn())
		RenderScroll();*/

	return xObject::Render();

	//return true;
}
bool TBKObject::RenderScroll()
{
	//스크롤 하는 부분
	static float fStep = 0.0f;
	fStep += g_fSecPerFrame * m_fScrollSpeed;

	//if (fStep >= m_pColorBitmap->m_BmpInfo.bmHeight)
	//	fStep = 0.0f;

	//int iScroll = m_pColorBitmap->m_BmpInfo.bmHeight - fStep;

	////두번에 나눠서 출력한다. 아래부분 출력
	//BitBlt(g_hOffScreenDC, m_rtDraw.left, fStep, m_rtDraw.right, m_rtDraw.bottom - fStep,
	//	m_pColorBitmap->m_hMemDC, m_rtDraw.left, m_rtDraw.top, SRCCOPY);

	////윗부분 출력
	//BitBlt(g_hOffScreenDC, m_rtDraw.left, m_rtDraw.top, m_rtDraw.right, fStep,
	//	m_pColorBitmap->m_hMemDC, m_rtDraw.left, iScroll, SRCCOPY);
	return true;
}

bool TBKObject::FadeOut()
{
	//블렌딩하는 부분
	static float fAlpha = 255;
	fAlpha -= g_fSecPerFrame * 20;

	if (fAlpha < 0)
		return true;

	/*static BLENDFUNCTION blend;
	blend.BlendOp = AC_SRC_OVER;
	blend.BlendFlags = 0;
	blend.SourceConstantAlpha = fAlpha;
	blend.AlphaFormat = AC_SRC_OVER;

	::AlphaBlend(g_hOffScreenDC, m_rtDraw.left, m_rtDraw.top,
		m_rtDraw.right, m_rtDraw.bottom,
		m_pColorBitmap->m_hMemDC, m_rtDraw.left, m_rtDraw.top, m_rtDraw.right, m_rtDraw.bottom, blend);*/

	return false;
}
bool TBKObject::FadeIn()
{
	//블렌딩하는 부분
	static float fAlpha = 0;
	fAlpha += g_fSecPerFrame * 50;

	if (fAlpha > 255)
		return true;

	/*static BLENDFUNCTION blend;
	blend.BlendOp = AC_SRC_OVER;
	blend.BlendFlags = 0;
	blend.SourceConstantAlpha = fAlpha;
	blend.AlphaFormat = AC_SRC_OVER;

	::AlphaBlend(g_hOffScreenDC, m_rtDraw.left, m_rtDraw.top,
		m_rtDraw.right, m_rtDraw.bottom,
		m_pColorBitmap->m_hMemDC, m_rtDraw.left, m_rtDraw.top, m_rtDraw.right, m_rtDraw.bottom, blend);*/

	return false;
}

TBKObject::TBKObject()
{
	m_fScrollSpeed = 100.0f;
}


TBKObject::~TBKObject()
{
}
