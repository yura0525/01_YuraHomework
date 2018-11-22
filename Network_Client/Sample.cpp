#include <winsock2.h>
#include <iostream>
#include "TPacket.h"
#include "Sample.h"
#pragma comment(lib, "ws2_32.lib")

/*
//#include<winsock2.h> 헤더파일은 #include <windows.h>보다 위에 인클루드 해야 한다.
//안그러면 많은 에러가 난다.
int SendMsg(SOCKET sock, const char* msg, WORD type)
{
	string address = "서울특별시";
	TPacket sendPacket(type);
	sendPacket << msg;
	sendPacket << 1000;
	sendPacket << address;
	int iRet = send(sock, (char*)&sendPacket.m_uPacket, sendPacket.m_uPacket.ph.len + PACKET_HEADER_SIZE, 0);

	char name[5] = { 0, };
	int iValue;
	string add;
	TPacket recvPacket;
	recvPacket.m_uPacket.ph = sendPacket.m_uPacket.ph;
	memcpy(recvPacket.m_uPacket.msg, sendPacket.m_uPacket.msg, sizeof(char) * 256);

	recvPacket >> name;
	recvPacket >> iValue;
	recvPacket >> add;

	return iRet;
}
void main()
{
	SOCKET sock = 0;
	SendMsg(sock, "game", PACKET_CHAT_MSG);

}*/

HANDLE g_hEvent;

DWORD WINAPI SendThread(LPVOID param)
{
	SOCKET sock = (SOCKET)param;
	char buffer[256] = { 0, };

	while (1)
	{
		ZeroMemory(buffer, sizeof(char) * 256);

		printf("\n보낼 데이터 입력하시오?\n");
		fgets(buffer, 256, stdin);

		if (strlen(buffer) <= 1)
		{
			break;
		}
		//enter지우기
		if (buffer[strlen(buffer) - 1] == '\n')
			buffer[strlen(buffer) - 1] = 0;

		int iSendByte = SendMsg(sock, buffer, PACKET_CHAT_MSG);
		if (iSendByte == SOCKET_ERROR)
			break;
		printf("\n[%s] : %zd 바이트를 전송하였습니다.\n", buffer, strlen(buffer));
	}

	shutdown(sock, SD_BOTH);
	closesocket(sock);
	return 1;
}

DWORD WINAPI RecvThread(LPVOID arg)
{
	SOCKET sock = (SOCKET)arg;

	char buffer[2048] = { 0, };

	int iRecvByte = 0;
	int iStartByte = 0;
	bool bConnect = true;

	while(bConnect)
	{
		ZeroMemory(buffer, sizeof(char) * 2048);

		int iLen = 0;
		iLen = recv(sock, (char*)&buffer[iStartByte], sizeof(char) * PACKET_HEADER_SIZE - iRecvByte, 0);
		iRecvByte += iLen;

		if (iLen == 0)
		{
			printf("\n--------->정상 퇴장\n");
			break;
		}
		if (iLen <= SOCKET_ERROR)
		{
			printf("\n--------->비정상 퇴장\n");
			break;
		}

		if (iRecvByte == PACKET_HEADER_SIZE)
		{
			UPACKET* pPacket = (UPACKET*)&buffer;
			int iRecvMsg = 0;
			do
			{
				iRecvMsg = recv(sock, (char*)&buffer[iRecvByte], (sizeof(char) * pPacket->ph.len), 0);
				iRecvByte += iRecvMsg;

				if (iRecvMsg == 0 || iRecvMsg <= SOCKET_ERROR)
				{
					printf("\iRecvMsg == 0 || iRecvMsg <= SOCKET_ERROR [연결 종료] : %s", "서버 문제로 인하여 종료되었다.\n");
					bConnect = false;
					break;
				}
			} while (iRecvMsg < pPacket->ph.len);

			UPACKET recvMsg;
			ZeroMemory(&recvMsg, sizeof(recvMsg));
			memcpy(&recvMsg, pPacket, (pPacket->ph.len + PACKET_HEADER_SIZE));

			switch (recvMsg.ph.type)
			{
				case PACKET_CHAT_MSG:
				{
					printf("\n[받은 메세지] : %s\n", recvMsg.msg);
				}
				break;
			}
			iStartByte = iRecvByte = 0;
		}
		else
		{
			iStartByte += iRecvByte;
		}
	}

	shutdown(sock, SD_BOTH);
	closesocket(sock);
	return 1;
}

DWORD WINAPI ConnectThread(LPVOID param)
{
	SOCKET sock = (SOCKET)param;

	SOCKADDR_IN addr;
	ZeroMemory(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("192.168.0.27");
	addr.sin_port = htons(10000);

	int iRet = connect(sock, (sockaddr*)&addr, sizeof(addr));
	if (iRet == SOCKET_ERROR)
	{
		return 1;
	}

	//이벤트의 상태를 시그널 상태가 되게한다.
	SetEvent(g_hEvent);
	return 1;
}
int main()
{
	WSADATA wd;

	//윈속초기화. 0이면 성공
	if (BeginWinSock() == false)
	{
		return 1;
	}

	// 이벤트는 스레드간 중계, 스레드간 흐름을 제어하려고 사용한다.
	// 이벤트 객체는 어떤사건이 일어났음을 알리는 동기화 객체이다.
	// 스레드간의 작업 순서나 시기를 조정하고 신호를 보내기 위해 사용되며,
	// 윈도우의 메세지와 유사한 기능을 갖고있다.
	// 자동 리셋 이벤트 : 대기 상태가 종료되면 자동으로 비신호 상태가 된다.
	// 수동 리셋 이벤트 : 스레드가 비신호 상태로 만들때까지 신호 상태를 유지한다.
	// 2 : FALSE(자동리셋 이벤트 ), TRUE( 수동리셋 이벤트 )
	// 3 : 초기값(FALSE: 논시그널, TRUE:시그널)
	//자동리셋 이벤트는 사용 안 해도 되고, 수동리셋 이벤트는 필요하다.
	//이벤트의 상태를 논시그널 상태가 되게한다. => ResetEvent(g_hEvent);
	//이벤트의 상태를 시그널 상태가 되게한다.	=> SetEvent(g_hEvent);

	//초기값은 논시그널된 상태.
	g_hEvent = CreateEvent(NULL, FALSE, FALSE, L"ConnectEvent");

	//소켓 생성 완료.
	//SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);	//UDP
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);				//TCP

	if (sock == INVALID_SOCKET)
	{
		return 1;
	}

	DWORD id_0;
	HANDLE hConnectThread = CreateThread(0, 0, ConnectThread, (LPVOID)sock, 0, &id_0);

	//event
	// 커넥트가 되야 샌드 리시브 되니까 커넥트가 될때까지 대기하려고 이벤트를 사용한다
	//커넥트가 되면 이벤트가 시그널 상태가 된다.
	//WaitForSingleObject(hConnectThread, INFINITE);
	//자동리셋 이벤트는 사용 안해도 되고, 수동 리셋 이벤트는 필요하다.
	//SetEvent <------> ResetEvent(g_hEvent);
	WaitForSingleObject(g_hEvent, INFINITE);

	//이벤트의 상태를 논시그널 상태가 되게한다.
	//ResetEvent(g_hEvent);					//자동리셋이벤트라서 안해줘도 된다.

	char buffer2[256] = { 0, };
	int iLen = 0;
	DWORD id_1;
	HANDLE hSendThread = CreateThread(NULL, 0, SendThread, (LPVOID)sock, 0, &id_1);

	DWORD id_2;
	HANDLE hRecvThread = CreateThread(NULL, 0, RecvThread, (LPVOID)sock, 0, &id_2);

	WaitForSingleObject(hSendThread, INFINITE);
	WaitForSingleObject(hRecvThread, INFINITE);

	closesocket(sock);
	CloseHandle(hConnectThread);
	CloseHandle(hSendThread);
	CloseHandle(hRecvThread);
	CloseHandle(g_hEvent);

	EndWinSock();

	std::cout << "Client Hello World\n";
	getchar();
}
