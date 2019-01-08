#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <tchar.h>
#include <string.h>

#define SERVERPORT 9000
#define BUFSIZE 512
#define WM_SOCKET (WM_USER+1)

//유니코드에서 멀티	바이트로 바꾸는 함수
char* GetW2M(const WCHAR* data)
{
	char cData[3333] = { 0, };

	//변환된 크기를 찾는다.
	int iLength = WideCharToMultiByte(CP_ACP, 0, data, -1, 0, 0, NULL, NULL);
	int iRet = WideCharToMultiByte(CP_ACP, 0, data, -1, cData, iLength, NULL, NULL);

	return cData;
}

//멀티바이트에서 유니코드로 바꾸는 함수
WCHAR* GetM2W(const char* data)
{
	WCHAR cData[3333] = { 0, };

	//변환된 크기를 찾는다.
	int iLength = MultiByteToWideChar(CP_ACP, 0, data, -1, 0, 0);
	int iRet = MultiByteToWideChar(CP_ACP, 0, data, -1, cData, iLength);

	return cData;
}

std::string w2m(std::wstring data)
{
	std::string ret = std::string(data.begin(), data.end());
	return ret;
}

std::wstring m2w(std::string data)
{
	std::wstring ret = std::wstring(data.begin(), data.end());
	return ret;
}


//소켓 함수 오류 출력후 종료
void err_quit(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);

	MessageBox(NULL, (LPCTSTR)lpMsgBuf, GetM2W(msg), MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

//소켓 함수 오류 출력
void err_display(const char* msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);

	printf("[%s] %s", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

void err_display(int errcode)
{
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, errcode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);

	printf("[오류] %s", (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}
//소켓 정보 저장을 위한 구조체와 변수
struct SOCKETINFO
{
	SOCKET sock;
	char buf[BUFSIZE + 1];
	int recvbytes;
	int sendbytes;
	BOOL recvdelayed;
	SOCKETINFO* next;
};

SOCKETINFO* SocketInfoList;
//윈도우 메시지 처리 함수
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void ProcessSocketMessage(HWND, UINT, WPARAM, LPARAM);

//소켓 관리 함수
BOOL AddSocketInfo(SOCKET sock);
SOCKETINFO* GetSocketInfo(SOCKET sock);
void RemoveSocketInfo(SOCKET sock);


int main()
{
	int retval;

	//윈도우 클래스 등록
	WNDCLASS wndclass;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = NULL;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = L"MyWndClass";
	if (!RegisterClass(&wndclass)) return 1;

	//윈도우 생성
	HWND hWnd = CreateWindow(L"MyWndClass", L"TCP서버", WS_OVERLAPPEDWINDOW, 0, 0, 600, 200, NULL, NULL, NULL, NULL);
	if (hWnd == NULL) return 1;
	ShowWindow(hWnd, SW_SHOWNORMAL);
	UpdateWindow(hWnd);

	//윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return 1;
	}

	//socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET)
		err_quit("socket()");

	//bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	//listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	//WSAAsyncSelect()
	retval = WSAAsyncSelect(listen_sock, hWnd, WM_SOCKET, FD_ACCEPT | FD_CLOSE);
	if (retval == SOCKET_ERROR) err_quit("WSAAsyncSelect()");

	//메세지 루프
	MSG msg;
	while (GetMessage(&msg, 0, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	//윈속 종료
	WSACleanup();
	return msg.wParam;
}

//윈도우 메세지 처리
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SOCKET:	//소켓 관련 윈도우 메세지
		ProcessSocketMessage(hWnd, uMsg, wParam, lParam);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//소켓 관련 윈도우 메세지 처리
void ProcessSocketMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//데이터 통신에 사용할 변수
	SOCKETINFO* ptr;
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen, retval;

	//오류 발생 여부 확인
	if (WSAGETSELECTERROR(lParam))
	{
		err_display(WSAGETSELECTERROR(lParam));
		RemoveSocketInfo(wParam);
		return;
	}

	//메세지 처리
	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_ACCEPT:
		addrlen = sizeof(clientaddr);
		client_sock = accept(wParam, (SOCKADDR*)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET)
		{
			err_display("accept()");
			return;
		}

		//접속한 클라이언트 정보 출력
		printf("\n[TCP 서버] 클라이언트 접속 : IP 주소 = %s, 포트 번호 = %d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
		AddSocketInfo(client_sock);
		retval = WSAAsyncSelect(client_sock, hWnd, WM_SOCKET, FD_READ | FD_WRITE | FD_CLOSE);
		if (retval == SOCKET_ERROR)
		{
			err_display("WSAAsyncSelect()");
			RemoveSocketInfo(client_sock);
		}
		break;
	case FD_READ:
		ptr = GetSocketInfo(wParam);
		if (ptr->recvbytes > 0)
		{
			ptr->recvdelayed = TRUE;
			return;
		}

		//데이터 받기
		retval = recv(ptr->sock, ptr->buf, BUFSIZE, 0);
		if (retval == SOCKET_ERROR)
		{
			err_display("recv()");
			RemoveSocketInfo(wParam);
			return;
		}
		ptr->recvbytes = retval;

		//받은 데이터 출력
		ptr->buf[retval] = '\0';
		addrlen = sizeof(clientaddr);
		getpeername(wParam, (SOCKADDR*)&clientaddr, &addrlen);
		printf("\n[TCP %s: %d] %s\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), ptr->buf);
		//break;없다.
	case FD_WRITE:
		ptr = GetSocketInfo(wParam);
		if (ptr->recvbytes <= ptr->sendbytes)
			return;
		//데이터 보내기
		retval = send(ptr->sock, ptr->buf + ptr->sendbytes, ptr->recvbytes - ptr->sendbytes, 0);
		if (retval == SOCKET_ERROR)
		{
			err_display("send()");
			RemoveSocketInfo(wParam);
			return;
		}

		ptr->sendbytes += retval;

		//받은 데이터를 모두 보냈는지 체크
		if (ptr->recvbytes == ptr->sendbytes)
		{
			ptr->recvbytes = ptr->sendbytes = 0;
			if (ptr->recvdelayed)
			{
				ptr->recvdelayed = FALSE;
				PostMessage(hWnd, WM_SOCKET, wParam, FD_READ);
			}
		}
		break;
	case FD_CLOSE:
		RemoveSocketInfo(wParam);
		break;
	}
}

//소켓 정보 추가
BOOL AddSocketInfo(SOCKET sock)
{
	SOCKETINFO* ptr = new SOCKETINFO;
	if (ptr == NULL)
	{
		printf("[오류] 메모리가 부족합니다.\n");
		return FALSE;
	}

	ptr->sock = sock;
	ptr->recvbytes = 0;
	ptr->sendbytes = 0;
	ptr->recvdelayed = FALSE;
	ptr->next = SocketInfoList;
	SocketInfoList = ptr;

	return TRUE;
}

//소켓 정보 얻기
SOCKETINFO * GetSocketInfo(SOCKET sock)
{
	SOCKETINFO* ptr = SocketInfoList;

	while (ptr)
	{
		if (ptr->sock == sock)
		{
			return ptr;
		}
		ptr = ptr->next;
	}
	return NULL;
}

//소켓 정보 제거
void RemoveSocketInfo(SOCKET sock)
{
	SOCKADDR_IN clientaddr;
	int addrlen = sizeof(clientaddr);
	getpeername(sock, (SOCKADDR*)&clientaddr, &addrlen);

	printf("\n[TCP 서버] 클라이언트 종료 : IP 주소 = %s, 포트 번호 = %d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

	SOCKETINFO *curr = SocketInfoList;
	SOCKETINFO* prev = NULL;

	while (curr)
	{
		if (curr->sock == sock)
		{
			if (prev)
			{
				prev->next = curr->next;
			}
			else
			{
				SocketInfoList = curr->next;
			}

			closesocket(curr->sock);
			delete curr;
			return;
		}
		prev = curr;
		curr = curr->next;
	}
}