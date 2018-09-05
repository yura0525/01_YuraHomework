#include "TBitmap.h"

int TBitmap::Load(T_STR szLoadFile)
{
	assert(g_hWnd != NULL);

	HDC hdc = GetDC(g_hWnd);

	assert(hdc != NULL);

	m_hBitmap = (HBITMAP)LoadImage(g_hInstance,
								szLoadFile.c_str(), 
								IMAGE_BITMAP, 0,0, 
								LR_DEFAULTSIZE | LR_LOADFROMFILE);
	if (m_hBitmap != NULL)
	{
		GetObject(m_hBitmap, sizeof(BITMAP), &m_BmpInfo);
		m_hMemDC = CreateCompatibleDC(hdc);
		SelectObject(m_hMemDC, m_hBitmap);
		
		m_szName = szLoadFile;
		ReleaseDC(g_hWnd, hdc);
		return true;
	}

	ReleaseDC(g_hWnd, hdc);
	return false;
}

bool TBitmap::Init()			//초기화
{
	return true;
}
bool TBitmap::Frame()			//계산
{
	return true;
}
bool TBitmap::Render()			//드로우
{
	HDC hdc = GetDC(g_hWnd);

	//원본 SrcPos, SrcPos(46, 62)좌표에서 
	//width, height(68,79)만큼 비트맵을 가져와서 
	//DesXPos, DesYPos(100, 100)에 뿌려준다.
	//BitBlt(hdc, m_Position.x, m_Position.y, 68, 79, m_hMemDC, 46, 62, SRCCOPY);
	
	//비트맵을 확대 축소하는 함수(좌우 상하 반전되는 함수)
	//StretchBlt(hdc, m_Position.x, m_Position.y + 79, 68, -79, m_hMemDC, 46, 62, 68, 79, SRCCOPY);
	ReleaseDC(g_hWnd, hdc);
	return true;
}
bool TBitmap::Release()			//소멸, 삭제
{
	DeleteObject(m_hBitmap);
	ReleaseDC(g_hWnd, m_hMemDC);

	return true;
}
//void TBitmap::SetPosition(float xPos, float yPos)
//{
//	m_Position.x = xPos;
//	m_Position.y = yPos;
//}
//
//void TBitmap::SetOffeSet(float xPos, float yPos)
//{
//	m_Position.x += xPos;
//	m_Position.y += yPos;
//}

TBitmap::TBitmap()
{
}


TBitmap::~TBitmap()
{
}
