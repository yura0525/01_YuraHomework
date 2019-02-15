#include "TChatSvr.h"
#include "TServerIOCP.h"
#include "TDebugString.h"
#include "Sample.h"

bool Sample::Init()
{
	xCore::Init();
	I_ServerIOCP.Init();
	I_Server.Init();
	m_Udp.Init();
	return true;
}
bool Sample::Release()
{
	xCore::Release();
	I_ServerIOCP.Release();
	I_Server.Release();
	m_Udp.Release();
	return true;
}
LRESULT Sample::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case WM_CREATE:
		{
			int iWidth = m_rtClient.right - m_rtClient.left;
			int iHeight = m_rtClient.bottom - m_rtClient.top;
			I_Debug.m_hList = CreateWindow(L"listbox", NULL, WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL,
				0, 0, iWidth - 100, iHeight, hWnd, (HMENU)0, m_hInstance, NULL);
		}
		break;
	}
	return 0;
}

Sample::Sample()
{
}


Sample::~Sample()
{
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	Sample server;
	if (!server.SetWindow(hInstance, _T("ChatSvr_1"), 800, 600))
	{
		return 0;
	}
	SetCursor(NULL);
	//������ ���ν��� ���
	if (server.Run() == false)
	{
		return 0;
	}
	return 1;
}