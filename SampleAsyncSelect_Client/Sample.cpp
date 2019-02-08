#include "Sample.h"

static char* GetWtM(WCHAR* data)
{
	char retData[4096] = { 0 };
	// �����Ǵ� ���ڿ��� ũ�Ⱑ ��ȯ�ȴ�.
	int iLength = WideCharToMultiByte(CP_ACP, 0,
		data, -1, 0, 0, NULL, NULL);
	int iRet = WideCharToMultiByte(CP_ACP, 0,
		data, -1,  //  �ҽ�
		retData, iLength, // ���
		NULL, NULL);
	return retData;
}
static bool GetWtM(WCHAR* src, char* pDest)
{
	// �����Ǵ� ���ڿ��� ũ�Ⱑ ��ȯ�ȴ�.
	int iLength = WideCharToMultiByte(CP_ACP, 0,
		src, -1, 0, 0, NULL, NULL);
	int iRet = WideCharToMultiByte(CP_ACP, 0,
		src, -1,  //  �ҽ�
		pDest, iLength, // ���
		NULL, NULL);
	if (iRet == 0) return false;
	return true;
}
static WCHAR* GetMtW(char* data)
{
	WCHAR retData[4096] = { 0 };
	// �����Ǵ� ���ڿ��� ũ�Ⱑ ��ȯ�ȴ�.
	int iLength = MultiByteToWideChar(CP_ACP, 0, data, -1, 0, 0);
	int iRet = MultiByteToWideChar(CP_ACP, 0,
		data, -1,  //  �ҽ�
		retData, iLength); // ���
	return retData;
}
static bool GetMtW(char* pSrc, WCHAR* pDest)
{
	// �����Ǵ� ���ڿ��� ũ�Ⱑ ��ȯ�ȴ�.
	int iLength = MultiByteToWideChar(CP_ACP, 0,
		pSrc, -1, 0, 0);
	int iRet = MultiByteToWideChar(CP_ACP, 0,
		pSrc, -1,  //  �ҽ�
		pDest, iLength); // ���		
	if (iRet == 0) return false;
	return true;
}

bool Sample::Init()
{
	// ������ ��Ʈ��(edit)
	DWORD sytle = WS_CHILD | WS_VISIBLE | ES_MULTILINE;
	m_hEdit = CreateWindow(L"edit", NULL,
		sytle, 420, 100, 200, 100,
		m_hWnd,
		(HMENU)100, // id
		m_hInstance, NULL);
	// ������ ��Ʈ��(button)
	sytle = WS_CHILD | WS_VISIBLE;
	m_hButton = CreateWindow(L"button", L"����",
		sytle, 630, 100, 200, 100,
		m_hWnd,
		(HMENU)200, // id
		m_hInstance, NULL);

	sytle = WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL;
	m_hList = CreateWindow(L"listbox", NULL,
		sytle, 10, 10, 400, 500,
		m_hWnd,
		(HMENU)300, // id
		m_hInstance, NULL);

	SendMessage(m_hList, LB_ADDSTRING, 0, (LPARAM)L"ä�� ����");

	m_Network.Init();
	if (!m_Network.Connect("192.168.0.2", 10000))
	{
		return false;
	}
	return true;
}
bool Sample::Frame()
{
	if (m_Network.m_RecvList.empty() == false)
	{
		for (std::list<UPACKET>::iterator itor = m_Network.m_RecvList.begin();
			itor != m_Network.m_RecvList.end(); itor++)
		{
			UPACKET pPacket = *itor;
			switch (pPacket.ph.type)
			{
			case PACKET_CHAT_MSG:
			{
				SendMessage(m_hList, LB_ADDSTRING, 0, (LPARAM)GetMtW(pPacket.msg));
			}
			break;
			}
		}
		m_Network.m_RecvList.clear();
	}
	return true;
}
bool Sample::Release()
{
	m_Network.Release();
	return true;
}
LRESULT	Sample::MsgProc(HWND hWnd, UINT msg, WPARAM	wParam, LPARAM	lParam)
{
	switch (msg)
	{
	case WM_COMMAND:
	{
		// id�� �ش��ϴ� ��Ʈ��
		switch (LOWORD(wParam))
		{
		case 200:
		{
			// ��ưŬ��
			TCHAR buffer[MAX_PATH] = { 0 };
			SendMessage(m_hEdit, WM_GETTEXT, MAX_PATH, (LPARAM)buffer);

			int iCnt = SendMessage(m_hList, LB_GETCOUNT, 0, 0);
			if (iCnt > 20)
			{
				SendMessage(m_hList, LB_RESETCONTENT, 0, 0);
			}
			//SendMessage(m_hList, LB_ADDSTRING, 0, (LPARAM)buffer);
			int iSendByte = m_Network.SendMsg(GetWtM(buffer), PACKET_CHAT_MSG);
			if (iSendByte == SOCKET_ERROR) break;
		}
		}
	}break;
	}
	return 0;
}
void Sample::MsgEvent(MSG msg)
{
	m_Network.MsgEvent(msg);
}
Sample::Sample()
{

}
Sample::~Sample()
{

}

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrevInst = NULL, LPWSTR lpCmdLine = NULL, int nCmdShow = 0)
{
	Sample win;
	win.SetWindow(hInst, L"Sample", 1024, 768);
	win.Run();
}