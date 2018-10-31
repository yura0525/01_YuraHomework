#include <winsock2.h>
#include <iostream>
#include <vector>

#pragma comment(lib, "ws2_32.lib")
using namespace std;

vector<SOCKET> g_userList;
int main()
{
	WSADATA wd;
	//���� �ʱ�ȭ. 0�̸� ����.
	if (WSAStartup(MAKEWORD(2, 2), &wd) != 0)
	{
		return 1;
	}

	//���� ���� �Ϸ�
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

	if (sock == INVALID_SOCKET)
		return 1;

	SOCKADDR_IN addr;
	ZeroMemory(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(10000);
	addr.sin_addr.s_addr = inet_addr(INADDR_ANY);
	//������ ��� Ư�� IP�ּҸ� �����ϱ⺸�ٴ� INADDR_ANY�� ����ϴ°��� �ٶ����ϴ�.
	//�� ������ ������ �ΰ� �̻��� IP �ּҸ� ������� 
	//Ŭ���̾�Ʈ�� ��� �ּҷ� ������ �ϵ��� ó���Ҽ� �ֱ� �����̴�.

	//���ε�
	int ret = bind(sock, (sockaddr*), sizeof(addr));
	if (ret == SOCKET_ERROR)
	{
		return 1;
	}

	//����. SOMAXCONN ���������� �ִ밪�� ���.
	ret = listen(sock, SOMAXCONN);
	if (ret == SOCKET_ERROR)
	{
		return 1;
	}

	SOCKET clientSock;
	SOCKADDR_IN clientInfo;
	int addlen = sizeof(clientInfo);

	while (1)
	{
		//3��° �������ڰ� sizeof(clientInfo)�� �ƴ϶� �׺����� �ּҰ�(&addlen)�� ���Ѵ�.
		clientSock = accept(sock, (sockaddr*)&clientInfo, &addlen);

		if (clientSock == INVALID_SOCKET)
			break;

		printf("\n %s, %d", inet_ntoa(clientInfo.sin_addr), ntohs(clientInfo.sin_port));
		g_userList.push_back(clientSock);

		char buffer2[256] = { 0, };

		while (1)
		{
			ZeroMemory(buffer2, sizeof(char) * 256);
			ret = recv(clientSock, buffer2, sizeof(buffer2), 0);
			
			if (ret == 0)	//��������� recv�� 0�� �����Ѵ�.
			{
				printf("\n���� ���� %s, %d �������ϴ�.", 
					inet_ntoa(clientInfo.sin_addr), ntohs(clientInfo.sin_port));
				break;
			}
		}
	}
}