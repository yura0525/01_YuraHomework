#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

#pragma comment(lib, "ws2_32")

using namespace std;

#define BUF_SIZE 100


void T_ERROR()
{
	char* pMsg = 0;
	FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL,
		WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (char*)&pMsg, 0, NULL);

	printf("\n%s\n", pMsg);
	LocalFree(pMsg);
}


void CompressSocket(SOCKET hSockarr[], int idx, int total)
{
	for (int i = idx; i < total; i++)
		hSockarr[i] = hSockarr[i + 1];
}
void CompressEvents(WSAEVENT hEventArr[], int idx, int total)
{
	for (int i = idx; i < total; i++)
		hEventArr[i] = hEventArr[i + 1];
}

void main()
{
	WSADATA wsaData;
	SOCKET listenSock, hClientSock;

	SOCKADDR_IN servAddr, clientAddr;
	SOCKET hSockArr[WSA_MAXIMUM_WAIT_EVENTS];
	WSAEVENT hEventArr[WSA_MAXIMUM_WAIT_EVENTS];

	WSAEVENT newEvent;
	WSANETWORKEVENTS netEvents;

	int numOfClientSock = 0;
	int strLen, i;
	int posInfo, startIdx;
	int clientAddrLen;
	char msg[BUF_SIZE];

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		T_ERROR();
		return;
	}
		

	//listen socket
	listenSock = socket(AF_INET, SOCK_STREAM, 0);

	//ip + port -> bind -> 운영 체제 보고.
	//port를 사용하고 있는 프로세스(실행파일을 구분하는 용도로 한다.)에게 알려준다.
	ZeroMemory(&servAddr, sizeof(servAddr));

	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(10000);
	//ip가 여러개인 경우에(랜선, 무선랜) 여러개중 아무거로 다 받는다는 뜻.
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(listenSock, (sockaddr*)&servAddr, sizeof(servAddr));

	//listen -> 듣다.
	listen(listenSock, SOMAXCONN);	//->개통

	newEvent = WSACreateEvent();
	if (WSAEventSelect(listenSock, newEvent, FD_ACCEPT) == SOCKET_ERROR)
	{
		printf("WSAEventSelect Error!!!!");
		return;
	}
	hSockArr[numOfClientSock] = listenSock;
	hEventArr[numOfClientSock] = newEvent;

	numOfClientSock++;

	while (1)
	{
		posInfo = WSAWaitForMultipleEvents(numOfClientSock, hEventArr, FALSE, WSA_INFINITE, FALSE);
		startIdx = posInfo - WSA_WAIT_EVENT_0;

		for (int i = startIdx; i < numOfClientSock; i++)
		{
			int sigEventIdx = WSAWaitForMultipleEvents(1, &hEventArr[i], TRUE, 0, FALSE);

			if (sigEventIdx == WSA_WAIT_FAILED || sigEventIdx == WSA_WAIT_TIMEOUT)
			{
				continue;
			}
			else
			{
				sigEventIdx = i;
				WSAEnumNetworkEvents(hSockArr[sigEventIdx],  hEventArr[sigEventIdx], &netEvents);

				if (netEvents.lNetworkEvents & FD_ACCEPT)	//연결 요청시.
				{
					if (netEvents.iErrorCode[FD_ACCEPT_BIT] != 0)
					{
						printf("Accept Error!!!!");
						break;
					}

					clientAddrLen = sizeof(clientAddr);
					hClientSock = accept(hSockArr[sigEventIdx], (SOCKADDR*)&clientAddr, &clientAddrLen);
					newEvent = WSACreateEvent();

					WSAEventSelect(hClientSock, newEvent, FD_READ | FD_CLOSE);

					hEventArr[numOfClientSock] = newEvent;
					hSockArr[numOfClientSock] = hClientSock;
					numOfClientSock++;
					printf("connected new client...");
				}
				if( netEvents.lNetworkEvents & FD_READ)	//데이터 수신시
				{
					if (netEvents.iErrorCode[FD_READ_BIT] != 0)
					{
						printf("Read Error!!!!!");
						break;
					}

					//에코서버이다.
					strLen = recv(hSockArr[sigEventIdx], msg, sizeof(msg), 0);
					send(hSockArr[sigEventIdx], msg, strLen, 0);
				}

				if (netEvents.lNetworkEvents &  FD_CLOSE)
				{
					if (netEvents.iErrorCode[FD_CLOSE_BIT] != 0)
					{
						printf("Close Error");
						break;
					}

					WSACloseEvent(hEventArr[sigEventIdx]);
					closesocket(hSockArr[sigEventIdx]);

					numOfClientSock--;
					CompressSocket(hSockArr, sigEventIdx, numOfClientSock);
					CompressEvents(hEventArr, sigEventIdx, numOfClientSock);
				}
			}
		}
	}

}
