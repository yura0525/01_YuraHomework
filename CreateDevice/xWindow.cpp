#include "xWindow.h"

xWindow * g_pWindow = 0;
HWND g_hWnd = NULL;
HINSTANCE g_hInstance = NULL;
RECT g_rtClient;

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//DEBUG�ÿ��� �۵��Ѵ�. ���ǹ��� false�̸� ���ܹ߻�.
	assert(g_pWindow != NULL);

	LRESULT ret;

	if (ret = g_pWindow->MsgProc(hWnd, msg, wParam, lParam))
	{
		return ret;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT xWindow::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_SIZE:
	{
		UINT width = LOWORD(lParam);
		UINT height = HIWORD(lParam);
		ResizeDevice(width, height);
	}
	break;
	case WM_KEYDOWN:
		switch (wParam)
		{
			// ��üȭ��� ������ ��� ��� ��ȯ(�� ������ ũ��� ��ȭ�� ����.)
			case '0':
			{
				BOOL isScreenMode = false;
				m_pSwapChain->GetFullscreenState(&isScreenMode, NULL);
				m_pSwapChain->SetFullscreenState(!isScreenMode, NULL);

				if (isScreenMode)
				{
					ShowWindow(m_hWnd, SW_SHOW);
				}
			}
			break;
			case '1':		// ������ ũ��(�ػ�) �� ���� ����
			{
				if (m_pSwapChain)
				{
					DXGI_MODE_DESC desc;
					ZeroMemory(&desc, sizeof(DXGI_MODE_DESC));

					desc.Width = 1024;
					desc.Height = 768;
					desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
					desc.RefreshRate.Denominator = 0;
					desc.RefreshRate.Numerator = 0;
					desc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
					desc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
					// ������ ũ�� �� �ػ�( ȭ���� ) ����
					m_pSwapChain->ResizeTarget(&desc);
				}
			}
			break;
			case '2':		// ��üȭ�� ���� ��ȯ �� �ػ� ����
			{
				if (m_pSwapChain)
				{
					DXGI_MODE_DESC desc;
					ZeroMemory(&desc, sizeof(DXGI_MODE_DESC));

					desc.Width = 800;
					desc.Height = 600;
					desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
					desc.RefreshRate.Denominator = 0;
					desc.RefreshRate.Numerator = 0;
					desc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
					desc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

					// ������ ũ�� �� �ػ�( ȭ���� ) ����
					m_pSwapChain->ResizeTarget(&desc);
					m_pSwapChain->SetFullscreenState(true, NULL);
				}
			}
			break;
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

void xWindow::MsgEvent(MSG msg)
{
}

bool xWindow::SetWindow(HINSTANCE hInstance, const TCHAR *pTitleName,
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

	//Ŭ���̾�Ʈ ���� ����(Ŭ���̾�Ʈ ������ ��Ȯ�ϰ�, (800, 600)���� �����Ѵ�)
	RECT rt{ 0,0, iWidth, iHeight };
	AdjustWindowRect(&rt, m_dwStyle, FALSE);

	//������ ����(��ϵ� Ŭ������ ����ؼ�)
	m_hWnd = CreateWindowEx(WS_EX_APPWINDOW,
		L"MyWindow", pTitleName, m_dwStyle,
		0, 0, rt.right - rt.left, rt.bottom - rt.top, NULL, NULL, m_hInstance, NULL);

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
void xWindow::Set(DWORD dwStyle)
{
	m_dwStyle = dwStyle;
}

void xWindow::CenterWindow()
{
	assert(m_hWnd != NULL);

	int iScreenWidth = GetSystemMetrics(SM_CXFULLSCREEN);
	int iScreenHeigth = GetSystemMetrics(SM_CYFULLSCREEN);

	int x = (iScreenWidth - (m_rtWindow.right - m_rtWindow.left)) / 2;
	int y = (iScreenHeigth - (m_rtWindow.bottom - m_rtWindow.top)) / 2;

	TCHAR	csBuffer[256];
	_stprintf_s(csBuffer, L"m_hWnd = %p, iScreenWidth  = %d, iScreenHeigth = %d, x = %d, y = %d, windowWidth = %d, windowHeight = %d\n",
		m_hWnd, iScreenWidth, iScreenHeigth, x, y, (m_rtWindow.right - m_rtWindow.left), (m_rtWindow.bottom - m_rtWindow.top));

	OutputDebugString(csBuffer);
	MoveWindow(m_hWnd, x, y, m_rtWindow.right, m_rtWindow.bottom, true);
}
bool xWindow::Run()
{
	if (!GameInit())
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

bool xWindow::GameInit()
{
	return true;
}

bool xWindow::GameRun()
{
	return true;
}

bool xWindow::GameRelease()
{
	return true;
}

xWindow::xWindow()
{
	g_pWindow = this;
	m_dwStyle = WS_OVERLAPPEDWINDOW;
}


xWindow::~xWindow()
{
}

