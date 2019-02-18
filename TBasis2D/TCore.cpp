#include "TCore.h"
#include "TBasisSys.h"

HWND g_hWnd;
HINSTANCE  g_hInstance;
bool		TCore::Init()
{
	return true;
}
bool		TCore::Render()
{
	return true;
}
bool		TCore::Frame()
{
	return true;
}
bool		TCore::Release()
{
	return true;
}

bool		TCore::SetWindow(HINSTANCE hInst,
							 int iWidth,
							 int iHeight,
							 const TCHAR* strTitleName,
							 DWORD dwStyle)
	{		
		m_hInst = hInst,
		g_hInstance = hInst;
		TWindow::Init();
		RegisterClassEx(&m_wc);

		RECT rt = { 0,0,iWidth, iHeight};
		AdjustWindowRect(&rt, dwStyle, FALSE );		
		
		m_iWidth  = iWidth;
		m_iHeight = iHeight;

		// 윈도우 생성
		HWND hWnd = CreateWindow( L"TBasis2DCore",
			strTitleName,
			dwStyle,
			0, 0, rt.right - rt.left, rt.bottom-rt.top,
			NULL, NULL, g_hInstance, NULL );
		
		g_hWnd = hWnd;

		// 
		//RECT rt;
		GetWindowRect( g_hWnd, &m_rtWindow );
		GetClientRect( g_hWnd, &m_rtClient );
		

		if( m_iWidth  != m_rtClient.right ||
			m_iHeight != m_rtClient.bottom )
		{
			return false;
		}

		// 윈도우 중앙에 위치
		CenterWindow();

		ShowWindow( hWnd, SW_SHOWDEFAULT );
		UpdateWindow( hWnd );

		TCoreInit();

		Run();

		TCoreRelease();
		UnregisterClass(L"TBasis2DCore", m_wc.hInstance);
		return true;
	}
bool	TCore::Run()
{
	// 메세지 루프 구축
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	DWORD dwTime = timeGetTime();
	while( msg.message != WM_QUIT )
	{
		//BOOL bMsg  = PeekMessage(&msg, NULL, 0L, 0L, PM_REMOVE);
		if( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) )
		{
			TranslateMessage( &msg );
			DispatchMessage(&msg);
		}
		else
		{		
			if ((timeGetTime() - dwTime) >= 20)
			{	
				dwTime = timeGetTime();
				TCoreFrame();			
				TCoreRender();									
			}					
		}
	}
	return true;
}
TCore::TCore(void)
{
}

TCore::~TCore(void)
{
}
