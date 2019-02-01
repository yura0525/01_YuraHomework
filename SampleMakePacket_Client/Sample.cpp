#define  _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include <stdio.h>
#include <windows.h>
#include <tchar.h>
#include "TProtocol.h"

#pragma comment(lib, "ws2_32.lib")
using namespace std;

HANDLE g_hEvent;
SOCKET sock;

int SendMsg(SOCKET sock, char* msg, WORD type)
{
	UPACKET sendmsg;
	ZeroMemory(&sendmsg, sizeof(sendmsg));
	sendmsg.ph.len = strlen(msg) + PACKET_HEADER_SIZE;
	sendmsg.ph.type = type;
	memcpy(sendmsg.msg, msg, strlen(msg));
	int sendbytes = 0;
	int iTotalsize = strlen(msg) + PACKET_HEADER_SIZE;
	char* pMsg = (char*)&sendmsg;
	int iSend = 0;
	do {
		iSend = send(sock,
			&pMsg[sendbytes],
			iTotalsize - sendbytes, 0);
		if (iSend <= 0)
		{
			printf("���� ����\n");
			return iSend;
		}
		sendbytes += iSend;
	} while (sendbytes < iTotalsize);
	return iTotalsize;
}

DWORD WINAPI ConnectThread(LPVOID arg)
{
	sock = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN sa;//sockaddr name;
	ZeroMemory(&sa, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(10000);// host byte ���� to network byte ���� short
	sa.sin_addr.s_addr = inet_addr("192.168.0.2");
	int iRet = connect(sock, (SOCKADDR*)&sa, sizeof(sa));
	if (iRet == SOCKET_ERROR)
		return 1;
	linger optval;
	int iLength = sizeof(optval);
	getsockopt(sock, SOL_SOCKET, SO_LINGER, (char*)&optval, &iLength);
	optval.l_onoff = 1;
	optval.l_linger = 1000;
	setsockopt(sock, SOL_SOCKET, SO_LINGER, (char*)&optval, iLength);

	SetEvent(g_hEvent);
	return 0;
}

DWORD WINAPI SendThread(LPVOID arg)
{
	SOCKET sock = (SOCKET)arg;
	char buf[256] = { 0, };

	while (1)
	{
		ZeroMemory(buf, sizeof(char) * 256);
		fgets(buf, 256, stdin);
		if (buf[strlen(buf) - 1] == '\n')
			buf[strlen(buf) - 1] = 0;
		if (strlen(buf) == 0 || _stricmp(buf, "quit") == 0)
		{
			SendMsg(sock, buf, PACKET_GAME_END);
			break; // ���ʹ� ����!
		}

		int iSendByte = SendMsg(sock, buf, PACKET_CHAT_MSG);
		if (iSendByte == SOCKET_ERROR || iSendByte == 0)
		{
			break;
		}
		printf("\t%d ����Ʈ�� �����Ͽ����ϴ�.", iSendByte);
		//SetEvent(g_hReceiveEvent);
	}
	shutdown(sock, SD_BOTH);
	closesocket(sock);
	return 0;
}

DWORD WINAPI RecvThread(LPVOID arg)
{
	SOCKET sock = (SOCKET)arg;
	char buf[2048] = { 0, };
	printf("\n���� ������ �Է��Ͻÿ�?");
	int iRecvByte = 0;
	int iStartByte = 0;
	ZeroMemory(buf, sizeof(char) * 2048);
	bool bConnect = true;
	while (bConnect)
	{
		//WaitForSingleObject(g_hReceiveEvent, INFINITE);

		ZeroMemory(buf, sizeof(char) * 2048);
		int iLen = 0;
		iLen = recv(sock, &buf[iStartByte], PACKET_HEADER_SIZE - iRecvByte, 0);
		iRecvByte += iLen;

		if (iLen == 0 || iLen == SOCKET_ERROR)
		{
			printf("[��������]: %s", "���� ������ ���Ͽ� ����Ǿ���.");
			break;
		}

		if (iRecvByte == PACKET_HEADER_SIZE)
		{
			UPACKET* pPacket = (UPACKET*)&buf;
			int iRecvMsg = 0;
			while (iRecvMsg < (pPacket->ph.len))
			{
				iRecvMsg = recv(sock, &buf[iRecvByte], pPacket->ph.len, 0);
				iRecvByte += iRecvMsg;
				if (iRecvMsg == 0 || iRecvMsg == SOCKET_ERROR)
				{
					printf("[��������]: %s", "���� ������ ���Ͽ� ����Ǿ���.");
					bConnect = false;
					break;
				}
			}
			UPACKET recvmsg;
			memset(&recvmsg, 0, sizeof(recvmsg));
			memcpy(&recvmsg, pPacket, pPacket->ph.len + PACKET_HEADER_SIZE);
			switch (recvmsg.ph.type)
			{
				case PACKET_CHAT_MSG:
				{
					printf("\n[�����޼���]: %s", recvmsg.msg);
					printf("\n���� ������ �Է��Ͻÿ�?");
				}break;
			}
			iStartByte = iRecvByte = 0;
		}
		else
		{
			iStartByte += iRecvByte;
		}
		//ResetEvent(g_hReceiveEvent);
	}
	shutdown(sock, SD_BOTH);
	closesocket(sock);
	return 0;
}

void main()
{
	HANDLE hExecuteMutex = 0;
	/*HANDLE hExecuteMutex = CreateMutex(NULL, FALSE, L"OnceMutex");
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CloseHandle(hExecuteMutex);
		MessageBox(NULL, L"�̹� �ٸ� �ν��Ͻ��� ���� ���Դϴ�.", L"�˸�", MB_OK);
		return;
	}*/

	// 2 : FALSE(�ڵ����� �̺�Ʈ ),TRUE( �������� �̺�Ʈ )
	// 3 : �ʱⰪ(FALSE: ��ñ׳�, TRUE:�ñ׳�)
	g_hEvent = CreateEvent(NULL, FALSE, FALSE, L"ConnectEvent");
	//g_hReceiveEvent = CreateEvent(NULL, FALSE, FALSE, L"ReceiveEvent");

	HANDLE hSendThread, hRecvThread, hConnectThread;
	WSADATA wsa;
	int iRet;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return;
	{
		DWORD dwConnectThreadID;
		hConnectThread = CreateThread(0, 0, ConnectThread, (LPVOID)0, 0, &dwConnectThreadID);

		WaitForSingleObject(g_hEvent, INFINITE);

		{
			DWORD dwSendThreadID;
			hSendThread = CreateThread(0, 0, SendThread, (LPVOID)sock, 0, &dwSendThreadID);

			//ResetEvent(g_hReceiveEvent);

			DWORD dwRecvThreadID;
			hRecvThread = CreateThread(0, 0, RecvThread, (LPVOID)sock, 0, &dwRecvThreadID);

		}

	}
	// �����尡 ����Ǿ����� Ȯ��
	WaitForSingleObject(hSendThread, INFINITE);
	WaitForSingleObject(hRecvThread, INFINITE);

	CloseHandle(hSendThread);
	CloseHandle(hRecvThread);

	// ������ ���� �Ҹ�
	iRet = WSACleanup();
	CloseHandle(hExecuteMutex);
	CloseHandle(g_hEvent);
	getchar();
	return;
}
