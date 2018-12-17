#include "Sample.h"

HANDLE g_hEvent;
DWORD WINAPI SendThread(LPVOID arg)
{
	SOCKET sock = (SOCKET)arg;

	char buffer[256] = { 0, };
	while (1)
	{
		fgets(buffer, 256, stdin);
		if (strlen(buffer) <= 1)
		{
			break;
		}

		//enter지움
		if (buffer[strlen(buffer) - 1] == '\n')
			buffer[strlen(buffer) - 1] = 0;

		int iSendByte = SendMsg(sock, buffer, PACKET_CHAT_MSG);
		if (iSendByte == SOCKET_ERROR) break;
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
	printf("\n보낼 데이터 입력하시오?\n");
	int iRecvByte = 0;
	int iStartByte = 0;
	ZeroMemory(buffer, sizeof(char) * 2048);

	bool bConnect = true;

	while (bConnect)
	{
		SOCKADDR_IN sendAddr;
		int iAddrLen = sizeof(sendAddr);

		int iLen = 0;
		iLen = recvfrom(sock, buffer, sizeof(char) * 2048, 0, (sockaddr*)&sendAddr, &iAddrLen);
		if (iLen == 0)
		{
			printf("\n------->정상퇴장\n");
			break;
		}
		if (iLen <= SOCKET_ERROR)
		{
			printf("\n----->비정상 퇴장\n");
			break;
		}
		UPACKET* pPacket = (UPACKET*)&buffer;
		switch (pPacket->ph.type)
		{
		case PACKET_CHAT_MSG:
		{
			printf("\n[받은메세지] : %s\n", pPacket->msg);
			printf("\n보낼 데이터 입력하시오?\n");
		}
		break;
		}
		Sleep(1);
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

	int ret = connect(sock, (sockaddr*)&addr, sizeof(addr));
	if (ret == SOCKET_ERROR)
	{
		return 1;
	}
	SetEvent(g_hEvent);			//이벤트 끝나면 해줘야한다.
	return 1;
}

int main()
{
	if (BeginWinSock() == false)
	{
		return 1;
	}

	// 이벤트는 스레드간 중계, 스레드간 흐름을 제어하려고 사용한다.
	// 2 : FALSE(자동리셋 이벤트 ), TRUE( 수동리셋 이벤트 )
	// 3 : 초기값(FALSE: 논시그널, TRUE:시그널)
	g_hEvent = CreateEvent(NULL, TRUE, FALSE, L"ConnectEvent");

	//UDP소켓으로 생성.
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET)
		return 1;

	DWORD id_0;
	HANDLE hConnectThread = CreateThread(0, 0, ConnectThread, (LPVOID)sock, 0, &id_0);

	//event
	//커넥트가 되야 샌드 리시브 되니까 기다리려고 이벤트를 사용한다.
	WaitForSingleObject(g_hEvent, INFINITE);
	ResetEvent(g_hEvent);					//자동리셋 이벤트는 사용 안 해도 되고, 수동리셋 이벤트는 필요하다.

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
	std::cout << "\nClient Hello World\n";
	getchar();
}