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

int main()
{
	int retval;

	//윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		return 1;
	}

	//socket()
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET)
		err_quit("socket()");

	//SO_REUSEADDR 소켓 옵션 설정
	BOOL optval = TRUE;
	retval = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&optval, sizeof(optval));
	if (retval == SOCKET_ERROR) err_quit("setsockopt()");

	//bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	//데이터 통신에 사용할 변수
	SOCKADDR_IN clientaddr;
	int addrlen;
	char buf[BUFSIZE + 1];

	while (1)
	{
		//데이터 받기
		addrlen = sizeof(clientaddr);
		retval = recvfrom(sock, buf, BUFSIZE, 0, (SOCKADDR*)&clientaddr, &addrlen);
		if (retval == SOCKET_ERROR)
		{
			err_display("recvfrom()");
			continue;
		}

		//받은 데이터 출력
		buf[retval] = '\0';
		printf("\n[UDP %s: %d] %s\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), buf);

		//데이터 보내기
		retval = sendto(sock, buf, retval, 0, (SOCKADDR*)&clientaddr, sizeof(clientaddr));
		if (retval == SOCKET_ERROR)
		{
			err_display("send()");
			continue;
		}
	}

	//closesocket()
	closesocket(sock);

	//윈속 종료
	WSACleanup();
	return 0;
}
