#include "TBKObject.h"
bool TBKObject::FadeOut()
{
	//�����ϴ� �κ�
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
	//�����ϴ� �κ�
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
}


TBKObject::~TBKObject()
{
}
