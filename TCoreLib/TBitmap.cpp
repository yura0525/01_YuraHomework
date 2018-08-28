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

bool TBitmap::Init()			//�ʱ�ȭ
{
	return true;
}
bool TBitmap::Frame()			//���
{
	return true;
}
bool TBitmap::Render()			//��ο�
{
	HDC hdc = GetDC(g_hWnd);

	//���� SrcPos, SrcPos(46, 62)��ǥ���� 
	//width, height(68,79)��ŭ ��Ʈ���� �����ͼ� 
	//DesXPos, DesYPos(100, 100)�� �ѷ��ش�.
	//BitBlt(hdc, m_Position.x, m_Position.y, 68, 79, m_hMemDC, 46, 62, SRCCOPY);
	
	//��Ʈ���� Ȯ�� ����ϴ� �Լ�(�¿� ���� �����Ǵ� �Լ�)
	//StretchBlt(hdc, m_Position.x, m_Position.y + 79, 68, -79, m_hMemDC, 46, 62, 68, 79, SRCCOPY);
	ReleaseDC(g_hWnd, hdc);
	return true;
}
bool TBitmap::Release()			//�Ҹ�, ����
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
