#include "TWindow.h"
#include "TInput.h"

TWindow*	g_pWindow	= 0;
HWND		g_hWnd		= NULL;
HINSTANCE	g_hInstance = NULL;
RECT		g_rtClient;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//DEBUG�ÿ��� �۵��Ѵ�. ���ǹ��� false�̸� ���ܹ߻�.
	assert( g_pWindow != NULL );

	LRESULT ret;
	
	if (ret = g_pWindow->MsgProc(hWnd, msg, wParam, lParam))
	{
		return ret;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT TWindow::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
	{
		//MessageBox(NULL, L"�����찡 ����.", L"������ ����", MB_OK);
	}
	break;
	case WM_DESTROY:
	{
		PostQuitMessage(0);		// WM_QUIT �޼����� �޽���ť�� Ǫ���ȴ�.
								//DestroyWindow(hWnd);	// WM_DESTROY �޼����� �޼���ť�� Ǫ���ȴ�.(�ٸ���ư���� �����Ҷ�)
	}
	return 0;
	//default:
	//	return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

void TWindow::MsgEvent(MSG msg)
{
	I_Input.MsgEvent(msg);
}

bool TWindow::SetWindow(HINSTANCE hInstance, const TCHAR *pTitleName,
						UINT iWidth, UINT iHeight)
{
	
	m_hInstance = hInstance;
	g_hInstance = hInstance;

	//������ Ŭ���� ���
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = m_hInstance;
	wc.lpszClassName = L"MyWindow";
	wc.hIcon = LoadIcon(m_hInstance, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	//wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.hIconSm = LoadIcon(NULL, IDI_HAND);
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;

	

	if (RegisterClassEx(&wc) == FALSE)
	{
		assert(RegisterClassEx(&wc) != FALSE);
		return false;
	}

	//������ ����(��ϵ� Ŭ������ ����ؼ�)
	m_hWnd = CreateWindowEx(WS_EX_APPWINDOW,
		L"MyWindow",pTitleName, m_dwStyle,
		0, 0, iWidth, iHeight, NULL, NULL, m_hInstance, NULL);

	if (m_hWnd == NULL)
	{
		assert(m_hWnd != NULL);
		return false;
	}

	g_hWnd = m_hWnd;

	GetWindowRect(m_hWnd, &m_rtWindow);
	GetClientRect(m_hWnd, &m_rtClient);

	g_rtClient = m_rtClient;

	CenterWindow();

	ShowWindow(m_hWnd, SW_SHOW);

	return true;
}
void TWindow::Set(DWORD dwStyle)
{
	m_dwStyle = dwStyle;
}

void TWindow::CenterWindow()
{
	assert(m_hWnd != NULL);

	int iScreenWidth = GetSystemMetrics(SM_CXFULLSCREEN);
	int iScreenHeigth = GetSystemMetrics(SM_CYFULLSCREEN);

	int x = (iScreenWidth - (m_rtWindow.right - m_rtWindow.left)) / 2;
	int y = (iScreenHeigth - (m_rtWindow.bottom - m_rtWindow.top)) / 2;

 	MoveWindow(m_hWnd, x, y, m_rtWindow.right, m_rtWindow.bottom, true);
}
bool TWindow::Run()
{
	if ( !GameInit() )
		return false;
	
	//GetMessage(&msg, NULL, 0, 0)
	//�޼���ť���� �޼����� 1���̻� ���� ��� 1���� ������.
	//PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)
	//�޼���ť���� �޼��� ������ �޼��� ��ȯ, ������ false�� ��ȯ.
	MSG msg;		// WM_QUIT
	ZeroMemory(&msg, sizeof(MSG));

	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if (msg.message == WM_QUIT)
			{
				break;
			}
			MsgEvent(msg);
		}
		else
		{
			GameRun();
			Sleep(1);
		}
	}

	return GameRelease();
}

bool TWindow::GameInit()
{
	return true;
}

bool TWindow::GameRun()
{
	return true;
}

bool TWindow::GameRelease()
{
	return true;
}

TWindow::TWindow()
{
	g_pWindow = this;
	m_dwStyle = WS_OVERLAPPEDWINDOW;
}


TWindow::~TWindow()
{
}

