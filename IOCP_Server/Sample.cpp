#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <list>
#include <map>

#pragma comment(lib, "ws2_32")

#define MAX_WORK_THREAD 4
#define RECV 1000
#define SEND 2000
#define BUFSIZE 2047
struct OVERLAPPED_EX : OVERLAPPED
{
	DWORD flag;
};

struct TUser
{
	OVERLAPPED_EX ov;
	SOCKET sock;
	char buf[BUFSIZE+1];
	int recvbytes;
	int sendbytes;
	WSABUF buffer;
};

//HANDLE m_hIOCP;
HANDLE m_hWorkThread[MAX_WORK_THREAD];

bool BeginWinSock()
{
	WSADATA wsa;
	int iRet = WSAStartup(MAKEWORD(2, 2), &wsa);
	if (iRet != 0)	return false;
	
	return true;
}

bool EndWinSock()
{
	int iRet = WSACleanup();
	if (iRet != 0) return false;

	return true;
}
DWORD WINAPI WorkerThread(LPVOID param)
{
	LPOVERLAPPED ov;
	DWORD bytesTransfer;
	HANDLE hcp = (HANDLE)param;

	while (1)
	{
		ULONG_PTR keyValue;
		int retval = ::GetQueuedCompletionStatus(hcp, &bytesTransfer, (PULONG_PTR)&keyValue, &ov, INFINITE);

		TUser* pUser = (TUser*)keyValue;
		OVERLAPPED_EX* ovex = (OVERLAPPED_EX*)&ov;
		DWORD trans = 0;

		//비동기 입출력 결과 확인
		if (retval == 0 || bytesTransfer == 0)
		{
			if (retval == 0)
			{
				DWORD temp1, temp2;
				WSAGetOverlappedResult(pUser->sock, &pUser->ov, &temp1, FALSE, &temp2);
				printf("Error!!!WSAGetOverlappedResult()");
			}

			closesocket(pUser->sock);
			delete pUser;
			continue;
		}

		//접속한 클라이언트 정보 출력
		SOCKADDR_IN clientaddr;
		int addrlen = sizeof(clientaddr);
		getpeername(pUser->sock, (SOCKADDR*)&clientaddr, &addrlen);

		//데이터 전송량 갱신
		if (pUser->recvbytes == 0)
		{
			pUser->recvbytes = bytesTransfer;
			pUser->sendbytes = 0;
			//받은 데이터 출력
			pUser->buf[pUser->recvbytes] = '\0';
			printf("\n[TCP %s: %d] %s\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), pUser->buf);
		}
		else
		{
			pUser->sendbytes += bytesTransfer;
		}

		if (pUser->recvbytes > pUser->sendbytes)
		{
			//데이터 보내기
			ZeroMemory(&pUser->ov, sizeof(pUser->ov));
			pUser->buffer.buf = pUser->buf + pUser->sendbytes;
			pUser->buffer.len = pUser->recvbytes - pUser->sendbytes;

			DWORD sendbytes;
			retval = WSASend(pUser->sock, &pUser->buffer, 1, &sendbytes, 0, (OVERLAPPED*)&pUser->ov, NULL);
			if (retval == SOCKET_ERROR)
			{
				if (WSAGetLastError() != WSA_IO_PENDING)
				{
					printf("ERROR!!WSASend()");
				}
				continue;
			}
		}
		else
		{
			pUser->recvbytes = 0;

			//데이터 받기
			ZeroMemory(&pUser->ov, sizeof(pUser->ov));
			pUser->buffer.buf = pUser->buf;
			pUser->buffer.len = BUFSIZE;

			DWORD recvbytes;
			DWORD flags = 0;
			retval = WSARecv(pUser->sock, &pUser->buffer, 1, &recvbytes, &flags, &pUser->ov, NULL);
			if (retval == SOCKET_ERROR)
			{
				if (WSAGetLastError() != WSA_IO_PENDING)
				{
					printf("Error!!!WSARecv()");
				}
				continue;
			}
		}
	}
	return 0;
}
int main()
{
	if (BeginWinSock() == false)
		return -1;

	//작업자를 고용.
	HANDLE hcp = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	//스레드 4개를 고용.
	DWORD id;
	for (int i = 0; i < MAX_WORK_THREAD; i++)
	{
		m_hWorkThread[i] = ::CreateThread(NULL, 0, WorkerThread, hcp, 0, NULL);
		if (m_hWorkThread[i] == NULL)
			return 1;
		CloseHandle(m_hWorkThread[i]);
	}

	//listen socket
	SOCKET listenSock;
	listenSock = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN sa;
	ZeroMemory(&sa, sizeof(sa));

	sa.sin_family = AF_INET;
	sa.sin_port = htons(10000);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(listenSock, (sockaddr*)&sa, sizeof(sa));

	listen(listenSock, SOMAXCONN);

	SOCKADDR_IN addr;

	while (1)
	{
		int addrlen = sizeof(addr);
		SOCKET sock = accept(listenSock, (SOCKADDR*)&addr, &addrlen);
		if (sock == INVALID_SOCKET)
		{
			printf("ERROR!!! accept()");
			break;
		}

		printf("\n[TCP 서버] 클라이언트 접속 : IP 주소 = %s, 포트 번호 = %d\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

		TUser* pUser = new TUser;
		if (pUser == NULL)
			break;

		DWORD trans, flag = 0;
		pUser->sock = sock;
		pUser->buffer.buf = pUser->buf;
		pUser->buffer.len = BUFSIZE;
		ZeroMemory(&pUser->ov, sizeof(pUser->ov));
		pUser->ov.flag = RECV;
		pUser->recvbytes = pUser->sendbytes = 0;
		::CreateIoCompletionPort((HANDLE)sock, hcp, (ULONG_PTR)&pUser, 0);
		

		int iRet = WSARecv(sock, &pUser->buffer, 1, &trans, &flag, (LPOVERLAPPED)&pUser->ov, NULL);
		if (iRet == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSA_IO_PENDING)
			{
				return -1;
			}
			continue;
		}
	}
	EndWinSock();
}