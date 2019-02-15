#include "Sample.h"
#include "TDebugString.h"
#include "xInput.h"

bool Sample::Init()
{
	xCore::Init();
	m_Client.Init();
	m_Udp.Init();
	return true;
}

bool Sample::Frame()
{
	xCore::Frame();

	// ���� Ű �Է� �� ����
	if (I_Input.KeyCheck(VK_LEFT) == KEY_PUSH)
	{
		TPACKET_USER_POSITION userdata;
		userdata.direction = VK_LEFT;
		userdata.posX = 100;
		userdata.posY = 100;
		userdata.user_idx = 999;
		char buffer[256] = { 0, };
		int iSize = sizeof(userdata);
		memcpy(buffer, &userdata, iSize);
		m_Client.SendMsg(buffer, iSize, PACKET_USER_POSITION);
	}
	m_Client.ProcessPacket();
	m_Client.Frame();
	return true;
}
bool Sample::Release()
{
	xCore::Release();
	m_Udp.Release();
	m_Client.Release();
	return true;
}
LRESULT	Sample::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CREATE:
	{
		m_hEdit = CreateWindow(_T("edit"), NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
			500, 10, 200, 25, hWnd, (HMENU)202, g_hInstance, NULL);

		CreateWindow(_T("button"), _T("Send"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			700, 10, 50, 25, hWnd, (HMENU)302, g_hInstance, NULL);

		int iWidth = this->m_rtClient.right - this->m_rtClient.left;
		int iHeight = this->m_rtClient.bottom - this->m_rtClient.top;

		I_Debug.m_hList = CreateWindow(L"listbox", NULL, WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL,
			0, 0, iWidth - 300, iHeight, hWnd, (HMENU)0, m_hInstance, NULL);
	}
	return 0;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case 302:
		{
			GetWindowText(m_hEdit, m_strBuffer, 128);
			I_Debug.ConvertWideStringToAnsiCh(m_Client.m_strBuffer, m_strBuffer, 128);
			InvalidateRect(hWnd, NULL, NULL);
			if (m_Client.m_bLoginOK == false && m_Client.SendMsg(m_Client.m_strBuffer, PACKET_CHAT_NAME_ACK))
			{
				m_Client.m_bLoginOK = true;
			}
			else
			{
				m_Client.m_bSend = true;
			}
		}
		}
	}
	break;
	
	case WM_CHAR:
		break;
	
	}
	return xCore::MsgProc(hWnd, msg, wParam, lParam);
}

Sample::Sample()
{
}


Sample::~Sample()
{
}


INT WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPreInst, LPWSTR strCmdLine, INT nShow)
{
	Sample server;
	if (!server.SetWindow(hInst, _T("ChatClient_2"), 800, 600))
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