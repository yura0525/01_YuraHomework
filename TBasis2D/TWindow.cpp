#include "TWindow.h"
#include "TBasisSys.h"


namespace TBASIS2D_CORE 	{
	static TWindow* g_pWindow = NULL;
	LRESULT TWindow::WndProc(HWND hWnd,
										UINT msg,
										WPARAM wParam,
										LPARAM lParam)
	{
		return FALSE;
	}
	// 메세지 프러시져 구축
	LRESULT WINAPI TCoreWinProc( HWND hWnd, 
										UINT msg,
										WPARAM wParam,
										LPARAM lParam )
	{	
		return g_pWindow->WndProc( hWnd, msg, wParam, lParam );
	}
	void TWindow::CenterWindow()
	{
		int iScreenWidth = GetSystemMetrics( SM_CXFULLSCREEN );
		int iScreenHeight= GetSystemMetrics( SM_CYFULLSCREEN );

		int iDestX = (iScreenWidth - (m_rtWindow.right - m_rtWindow.left)) / 2;
		int iDestY = (iScreenHeight - (m_rtWindow.bottom - m_rtWindow.top)) / 2;

		MoveWindow( g_hWnd, iDestX, iDestY, 
					m_rtWindow.right - m_rtWindow.left, 
					m_rtWindow.bottom - m_rtWindow.top, 
					true );
	}
bool		TWindow::Init()
{
	m_BackGrougdColor	= GRAY_BRUSH;
	m_wc.cbSize			=sizeof(WNDCLASSEX) ;		
	m_wc.style			= CS_VREDRAW | CS_HREDRAW;
	m_wc.lpfnWndProc	= TCoreWinProc;
	m_wc.cbClsExtra		= 0;
	m_wc.cbWndExtra		= 0;
	m_wc.hInstance		= GetModuleHandle( NULL );
	m_wc.hIcon			= NULL;
	m_wc.hCursor		= NULL;
	m_wc.hbrBackground	= (HBRUSH)GetStockObject(m_BackGrougdColor);;
	m_wc.lpszMenuName	= NULL;
	m_wc.lpszClassName	= L"TBasis2DCore";		
	m_wc.hIconSm		= NULL;
	return true;
}
bool		TWindow::Frame()
{
	return true;
}
bool		TWindow::Render()
{
	return true;
};
bool		TWindow::Release()
{
	return true;
}
TWindow::TWindow(void)
{
	g_pWindow = this;
	Init();
}

TWindow::~TWindow(void)
{
}
}