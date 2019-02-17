#define _CRT_SECURE_NO_WARNINGS
#include "TDebugString.h"
#include "Sample.h"


#define PORT		10000
#define BUFFERSIZE	100
#define NAMESIZE	30

unsigned sendThread, receiveThread;
unsigned long hSendThread, hReceiveThread;

int			mySocket;
char		myName[NAMESIZE];
TCHAR		m_SendMsg[1024] = { 0, };
bool		m_bBeginChat = false;
bool		g_bEnd = false;

unsigned __stdcall receiveMessage(void* arg)
{
	int mySocket = (int)arg;
	char fromServer[2048] = { 0, };

	int strlen = 0;
	while (!g_bEnd)
	{
		strlen = recv(mySocket, fromServer, sizeof(fromServer), 0);
		if (strlen <= 0)
		{
			break;
		}
		fromServer[strlen] = 0;
		I_DebugStr.Print(const_cast<char*>("%s"), fromServer);
	}
	I_DebugStr.Print("%s", "������ ����Ǿ����ϴ�.");
	return 0;
}
bool	Sample::Init()
{
	xCore::Init();
	I_DebugStr.Init();
	I_DebugStr.Print("=============== ä��==============");
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);

	/*������ �����ؼ� �޽����� �ְ� �޴� Ŭ���̾�Ʈ ���ϻ���*/
	mySocket = socket(PF_INET, SOCK_STREAM, 0);
	/*Ŭ���̾�Ʈ ���ϻ����� �����ϸ� �����޽����� ����ϰ� �Լ� ����*/
	if (mySocket == -1)
	{
		I_DebugStr.Print("Ŭ���̾�Ʈ ������ �����ϴµ� ���� �߽��ϴ�.");
		return 0;
	}
	I_DebugStr.Print("Ŭ���̾�Ʈ ������ ���� �߽��ϴ�.");

	/*������ �ּ� ������ ������ ����ü serverAddress ����*/
	sockaddr_in serverAddress;
	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_addr.s_addr = inet_addr("192.168.0.2");
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(PORT);

	if (connect(mySocket, (sockaddr *)&serverAddress, sizeof(serverAddress)) == -1)
	{
		I_DebugStr.Print("������ �����ϴµ� �����߽��ϴ�");
		return 0;
	}

	I_DebugStr.Print("ä���� �����մϴ�.");
	hReceiveThread = _beginthreadex(NULL, 0, receiveMessage, (void*)mySocket, 0, &receiveThread);
	return true;
}
bool	Sample::PreRender()
{
	return true;
}
bool	Sample::Render()
{
	xCore::Render();
	I_DebugStr.Frame();
	return PostRender();
}
bool	Sample::PostRender()
{
	return true;
}
bool	Sample::Release()
{
	xCore::Release();
	if (!g_bEnd)
	{
		closesocket(mySocket);
		g_bEnd = true;
		I_DebugStr.Release();
		CloseHandle((HANDLE)hReceiveThread);
		WaitForSingleObject((HANDLE)hReceiveThread, INFINITE);
		hReceiveThread = 0;
	}
	return true;
}
LRESULT	Sample::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	switch (msg)
	{
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case 1:
		{
			memset(m_SendMsg, 0, sizeof(TCHAR) * 1024);
			SendMessage(I_DebugStr.m_hEdit, WM_GETTEXT, 1024, (LPARAM)m_SendMsg);
			char toServer[2014] = { 0, };
			char* strMsg = I_DebugStr.GetWcstoMbs(m_SendMsg);
			sprintf(toServer, "%s ���� �����ϼ̽��ϴ�.", strMsg);

			int iLength = strlen(myName);
			if (iLength > 0)
			{
				ZeroMemory(toServer, sizeof(toServer));
				sprintf(toServer, "[%s] %s", myName, strMsg);
			}
			else
			{
				strcpy(myName, strMsg);
			}
			iLength = strlen(toServer);
			toServer[iLength] = 0;
			iLength = send(mySocket, toServer, iLength, 0);

			SendMessage(I_DebugStr.m_hEdit, WM_SETTEXT, 0, 0);
			InvalidateRect(hWnd, NULL, NULL);
			m_bBeginChat = true;
		}
		break;
		}
	}
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		return 0;
		break;
	}
	return 0;
}
Sample::Sample()
{
	m_bBeginChat = false;
}


Sample::~Sample()
{
}

INT WINAPI wWinMain(HINSTANCE hInst,
	HINSTANCE hPreInst,
	LPWSTR strCmdLine,
	INT nShow)
{
	Sample sWin;
	sWin.SetWindow(hInst, L"Chatting Client", 500, 450);
	sWin.Run();
	return 0;
}