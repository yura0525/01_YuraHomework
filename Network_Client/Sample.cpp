#include <winsock2.h>
#include <iostream>
#include "TPacket.h"
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

DWORD WINAPI SendThread(LPVOID param)
{
	SOCKET sock = (SOCKET)param;
	while (1)
	{
		char buffer[256] = { 0, };
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


bool BeginWinSock()
{
	WSADATA wsa;
	//���� �ʱ�ȭ. 0�̸� ����.
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return false;

	return true;
}
bool EndWinSock()
{
	//��ü �Ҹ�
	if (WSACleanup() == 0)
		return false;

	return true;
}

int main()
{
	WSADATA wd;

	//�����ʱ�ȭ. 0�̸� ����
	if (BeginWinSock == false)
	{
		return 1;
	}

	//���� ���� �Ϸ�.
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

		printf("[%s] : %zd ����Ʈ�� �޾ҽ��ϴ�.\n", buffer2, strlen(buffer2));
	}

	closesocket(sock);
	EndWinSock();

	std::cout << "Client Hello World\n";
	getchar();
}
