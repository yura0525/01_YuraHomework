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
	while (1)
	{
		char buffer[256] = { 0, };

		printf("\n보낼 데이터 입력하시오?\n");
		fgets(buffer, 256, stdin);

		if (strlen(buffer) <= 1)
		{
			break;
		}
		//enter지움.
		buffer[strlen(buffer) - 1] = '\0';

		send(sock, buffer, strlen(buffer), 0);
		printf("[%s] : %zd 바이트를 전송하였습니다.\n", buffer, strlen(buffer));
	}

	closesocket(sock);
	return 1;
}

DWORD WINAPI RecvThread(LPVOID arg)
{
	SOCKET sock = (SOCKET)arg;

	char buffer[2048] = { 0, };

	int iRecvByte = 0;
	int iStartByte = 0;

	ZeroMemory(buffer, sizeof(char) * 2048);
	
	bool bConnect = true;

	while(bConnect)
	{
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
			while (iRecvByte < pPacket->ph.len)
			{
				iLen = recv(sock, (char*)&buffer[iRecvByte], ((sizeof(char) * pPacket->ph.len) - iRecvByte), 0);
				iRecvByte += iLen;

				if (iLen == 0 || iLen <= SOCKET_ERROR)
				{
					printf("\niLen == 0 || iLen <= SOCKET_ERROR [연결 종료] : %s", "서버 문제로 인하여 종료되었다.\n");
					bConnect = false;
					break;
				}
			}

			if (iRecvByte == pPacket->ph.len)
			{
				UPACKET recvMsg;
				ZeroMemory(&recvMsg, sizeof(recvMsg));
				memcpy(&recvMsg, pPacket, pPacket->ph.len);

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
	addr.sin_addr.s_addr = inet_addr("192.168.0.101");
	addr.sin_port = htons(10000);

	int iRet = connect(sock, (sockaddr*)&addr, sizeof(addr));
	if (iRet == SOCKET_ERROR)
	{
		return 1;
	}

	SetEvent(g_hEvent);			//이벤트 끝나면 해줘야한다.
	return 1;
}
int main()
{
	WSADATA wd;

	//윈속초기화. 0이면 성공
	if (BeginWinSock == false)
	{
		return 1;
	}

	//소켓 생성 완료.
	//SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);	//UDP
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);				//TCP

	if (sock == INVALID_SOCKET)
	{
		return 1;
	}

	SOCKADDR_IN addr;
	ZeroMemory(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("192.168.0.27");
	addr.sin_port = htons(10000);

	int ret = connect(sock, (sockaddr*)&addr, sizeof(addr));
	if (ret == SOCKET_ERROR)
	{
		return 1;
	}

	char buffer2[256] = { 0, };
	int iLen = 0;
	DWORD id;
	HANDLE hThread = CreateThread(NULL, 0, SendThread, (LPVOID)sock, 0, &id);

	while (1)
	{
		char buffer[256] = { 0, };
		int iRet = recv(sock, buffer, sizeof(buffer), 0);

		if (iRet == 0)
			break;
		
		if (iRet == SOCKET_ERROR)
			break;

		printf("[%s] : %zd 바이트를 받았습니다.\n", buffer2, strlen(buffer2));
	}

	closesocket(sock);
	EndWinSock();

	std::cout << "Client Hello World\n";
	getchar();
}
