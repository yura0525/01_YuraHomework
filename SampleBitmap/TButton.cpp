#include "TButton.h"
#include "TCollision.h"
#include "TInput.h"

bool TButton::Frame()
{
	m_pCurrentBitmap = m_pColorBitmap[Btn_Normal];
	if (m_pColorBitmap[Btn_Hover] != NULL && TCollision::RectInPoint(m_rtCollision, I_Input.m_MousePos))
	{
		m_pCurrentBitmap = m_pColorBitmap[Btn_Hover];
		if (m_pColorBitmap[Btn_Click] != NULL && I_Input.Key(VK_LBUTTON))
		{
			m_pCurrentBitmap = m_pColorBitmap[Btn_Click];
		}
	}
	return true;
}
bool TButton::Render()
{
	BitBlt(g_hOffScreenDC, m_posDraw.x, m_posDraw.y,
		m_rtDraw.right, m_rtDraw.bottom,
		m_pCurrentBitmap->m_hMemDC, m_rtDraw.left, m_rtDraw.top, SRCCOPY);

	return true;
}

bool TButton::LoadUI(const TCHAR* pszColor, const TCHAR* pszMask, int iState)
{
	int iIndex = I_BitmapMgr.Load(pszColor);
	m_pColorBitmap[iState] = I_BitmapMgr.GetPtr(iIndex);
	
	if (pszMask != NULL)
	{
		iIndex = I_BitmapMgr.Load(pszMask);
		m_pMaskBitmap[iState] = I_BitmapMgr.GetPtr(iIndex);
	}

	m_pCurrentBitmap = m_pColorBitmap[0];
	return true;
}
TButton::TButton()
{
}


TButton::~TButton()
{
}