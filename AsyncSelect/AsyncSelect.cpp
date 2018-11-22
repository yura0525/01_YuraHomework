#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "sample.h"
#include <algorithm>

#define WM_KGCA (WM_USER + 0)
#define WM_RWC (WM_USER + 1)

int RecvData(TUser* pUser)
{
	char buffer[256] = { 0, };
	int recvByte = 0;
	SOCKET sock = pUser->sock;
	int bConnect = true;

	int iRet = recv(sock, &buffer[recvByte], sizeof(char) * PACKET_HEADER_SIZE - recvByte, 0);
	if (CheckReturn(iRet) <= 0)
	{
		printf("\n---->정상 퇴장\n");
		return -1;
	}

	recvByte += iRet;

	if (recvByte == PACKET_HEADER_SIZE)
	{
		UPACKET packet;
		ZeroMemory(&packet, sizeof(UPACKET));
		memcpy(&packet.ph, buffer, sizeof(char)*PACKET_HEADER_SIZE);

		int rByte = 0;
		while (rByte < packet.ph.len)
		{
			int iRecvByte = recv(sock, &(packet.msg[rByte]), sizeof(char) * (packet.ph.len - rByte), 0);

			if (iRecvByte == 0 || iRecvByte == SOCKET_ERROR)
			{
				bConnect = false;
				T_ERROR();
				break;
			}
			rByte += iRecvByte;
		}

		recvByte = 0;
		if (bConnect)
		{
			switch (packet.ph.type)
			{
			case PACKET_CHAT_MSG:
			{
				printf("\n패킷 완성 => %s\n", packet.msg);
				Broadcastting(packet.msg);
				break;
			}
			case PACKET_CREATE_CHARACTER:
			{
				CHARACTER_INFO cInfo;
				memcpy(&cInfo, packet.msg, sizeof(CHARACTER_INFO));
				printf("\n패킷 완성 => %s\n", packet.msg);
				break;
			}
			}
		}
	}

	if (bConnect == false)
		return -1;
	return 1;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_KGCA)
	{
		if (WSAGETSELECTERROR(lParam))
		{
			closesocket(wParam);
		}
		else
		{
			switch (WSAGETSELECTEVENT(lParam))
			{
			case FD_ACCEPT:
			{
				TUser addUser;
				addUser.sock = accept(wParam, (SOCKADDR*)&addUser.clientAddr, &addUser.addrlen);
				if (addUser.sock == INVALID_SOCKET)
				{
					break;
				}

				AddUser(addUser);

				//WSAASyncSelect같은 함수를 여러번 호출하면 마지막만 적용된다.
				//WM_DROP만 된다.
				int iRet = WSAAsyncSelect(addUser.sock, hWnd, WM_RWC, FD_READ | FD_WRITE | FD_CLOSE);

				if (iRet == SOCKET_ERROR)
				{
					return -1;
				}
			}
			break;
			}
		}
	}
	else if (uMsg == WM_RWC)
	{
		switch (WSAGETSELECTEVENT(lParam))
		{
		case FD_READ:
		{
			TUser::g_socket = wParam;
			ITOR itor = find_if(g_allUser.begin(), g_allUser.end(), TUser());
			TUser* pUser = (TUser*)&(*itor);
			//클라이언트 소켓일경우 recv()
			if (RecvData(pUser) <= 0)
			{
				closesocket(pUser->sock);
				itor = g_allUser.erase(itor);
			}
		}
		break;
		case FD_WRITE:
		{
			//받은 패킷을 넣어두는 장소가 없음.
			//패킷풀에 있는걸 Send한다.
		}
		break;
		case FD_CLOSE:
		{
			//받은 데이터가 0일경우에 나가는 처리를 하였다.
			PostMessage(hWnd, WM_KGCA, wParam, FD_READ);
		}
		break;
		}

	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

HWND MakeWindow()
{
	//윈도우 클래스 등록
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;

	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = NULL;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = L"MyWindow";
	wc.hIconSm = LoadIcon(NULL, IDI_HAND);

	if (RegisterClassEx(&wc) == FALSE)
	{
		return NULL;
	}

	//윈도우 생성(등록된 클래스를 사용해서)
	HWND m_hWnd = CreateWindowEx(WS_EX_APPWINDOW, L"MyWindow", NULL, NULL,
		0, 0, 800, 600, NULL, NULL, NULL, NULL);

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);
	return m_hWnd;
}


int main()
{
	g_hMutex = CreateMutex(NULL, FALSE, L"KGCA");

	//ERROR_ALREADY_EXISTS : 프로세스가 2개 중복 생성되는 걸 막는다.
	//프로세스가 실행된지 모르고 2개 실행될때, 이미 실행 되어 있다고 막는다.
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CloseHandle(g_hMutex);
		return 1;
	}

	HWND hWnd = MakeWindow();
	if (BeginWinSock() == false)
	{
		return -1;
	}

	//listen socket
	SOCKET listenSock;
	listenSock = socket(AF_INET, SOCK_STREAM, 0);

	//ip + port -> bind -> 운영 체제 보고.
	//port를 사용하고 있는 프로세스(실행파일을 구분하는 용도로 한다.)에게 알려준다.
	SOCKADDR_IN sa;
	ZeroMemory(&sa, sizeof(sa));

	sa.sin_family = AF_INET;
	sa.sin_port = htons(10000);
	//ip가 여러개인 경우에(랜선, 무선랜) 여러개중 아무거로 다 받는다는 뜻.
	sa.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(listenSock, (sockaddr*)&sa, sizeof(sa));

	//listen -> 듣다.
	listen(listenSock, SOMAXCONN);	//->개통

	g_listenSocket = listenSock;

	int iRet = WSAAsyncSelect(listenSock, hWnd, WM_KGCA, FD_ACCEPT | FD_CLOSE);
	if (iRet == SOCKET_ERROR)
	{
		return -1;
	}

	MSG msg;
	while (iRet = GetMessage(&msg, NULL, 0, 0))
	{
		if (iRet == -1)
			break;
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	closesocket(listenSock);
	EndWinSock();

	CloseHandle(g_hMutex);
	return 0;
}
