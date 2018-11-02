#include <winsock2.h>
#include <iostream>
#include "TPacket.h"
#include "Sample.h"
#pragma comment(lib, "ws2_32.lib")

/*
//#include<winsock2.h> ��������� #include <windows.h>���� ���� ��Ŭ��� �ؾ� �Ѵ�.
//�ȱ׷��� ���� ������ ����.
int SendMsg(SOCKET sock, const char* msg, WORD type)
{
	string address = "����Ư����";
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

		printf("\n���� ������ �Է��Ͻÿ�?\n");
		fgets(buffer, 256, stdin);

		if (strlen(buffer) <= 1)
		{
			break;
		}
		//enter����.
		buffer[strlen(buffer) - 1] = '\0';

		send(sock, buffer, strlen(buffer), 0);
		printf("[%s] : %zd ����Ʈ�� �����Ͽ����ϴ�.\n", buffer, strlen(buffer));
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
			printf("\n--------->���� ����\n");
			break;
		}
		if (iLen <= SOCKET_ERROR)
		{
			printf("\n--------->������ ����\n");
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
					printf("\niLen == 0 || iLen <= SOCKET_ERROR [���� ����] : %s", "���� ������ ���Ͽ� ����Ǿ���.\n");
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
						printf("\n[���� �޼���] : %s\n", recvMsg.msg);
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

	SetEvent(g_hEvent);			//�̺�Ʈ ������ ������Ѵ�.
	return 1;
}
int main()
{
	WSADATA wd;

	//�����ʱ�ȭ. 0�̸� ����
	if (BeginWinSock() == false)
	{
		return 1;
	}

	//�̺�Ʈ�� �����尣 �߰�, �����尣 �帧�� �����Ϸ��� ����Ѵ�.
	//���� ���� 2��° : FALSE(�ڵ� ���� �̺�Ʈ), TRUE(���� ���� �̺�Ʈ)
	//���� ���� 3��° : �ʱⰪ(FALSE : ��ñ׳�, TRUE: �ñ׳�)
	g_hEvent = CreateEvent(NULL, FALSE, FALSE, L"ConnectEvent");

	//���� ���� �Ϸ�.
	//SOCKET sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);	//UDP
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);				//TCP

	if (sock == INVALID_SOCKET)
	{
		return 1;
	}

	DWORD id_0;
	HANDLE hConnectThread = CreateThread(0, 0, ConnectThread, (LPVOID)sock, 0, &id_0);

	//event
	//Ŀ��Ʈ�� �Ǿ� ����, ���ú갡 �Ǵϱ� ��ٸ����� �̺�Ʈ�� ����Ѵ�.
	WaitForSingleObject(hConnectThread, INFINITE);
	//WaitForSingleObject(g_hEvent, INFINITE);
	ResetEvent(g_hEvent);		//�ڵ����� �̺�Ʈ�� ��� ���ص� �ǰ�, ���� ���� �̺�Ʈ�� �ʿ��ϴ�.

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

	EndWinSock();

	std::cout << "Client Hello World\n";
	getchar();
}
