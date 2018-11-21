#include "sample.h"
#include <algorithm>

#define WM_KGCA (WM_USER + 0)
#define WM_RWC (WM_USER + 1)

int RecvData(TUser* pUser)
{
	char buffer[256] = { 0, };
	int recvByte = 0;
	SOCKET sock = pUser->sock;
	int bConnect = true;

	int iRet = recv(sock, &buffer[recvByte], sizeof(char) * PACKET_HEADER_SIZE - recvByte, 0);
	if (CheckReturn(iRet) <= 0)
	{
		printf("\n---->���� ����\n");
		return -1;
	}

	recvByte += iRet;

	if (recvByte == PACKET_HEADER_SIZE)
	{
		UPACKET packet;
		ZeroMemory(&packet, sizeof(UPACKET));
		memcpy(&packet.ph, buffer, sizeof(char)*PACKET_HEADER_SIZE);

		int rByte = 0;
		do
		{
			int iRecvByte = recv(sock, &(packet.msg[rByte]), sizeof(char) * (packet.ph.len - rByte), 0);

			if (iRecvByte == 0 || iRecvByte == SOCKET_ERROR)
			{
				bConnect = false;
				T_ERROR();
				break;
			}
			rByte += iRecvByte;
		} while (rByte < packet.ph.len);

		recvByte = 0;
		if (bConnect)
		{
			switch (packet.ph.type)
			{
			case PACKET_CHAT_MSG:
			{
				printf("\n��Ŷ �ϼ� => %s\n", packet.msg);
				Broadcastting(packet.msg);
				break;
			}
			case PACKET_CREATE_CHARACTER:
			{
				CHARACTER_INFO cInfo;
				memcpy(&cInfo, packet.msg, sizeof(CHARACTER_INFO));
				printf("\n��Ŷ �ϼ� => %s\n", packet.msg);
				break;
			}
			}
		}
	}

	if (bConnect == false)
		return -1;
	return 1;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_KGCA)
	{
		if (WSAGETSELECTERROR(lParam))
		{
			closesocket(wParam);
		}
		else
		{
			switch (WSAGETSELECTEVENT(lParam))
			{
			case FD_ACCEPT:
			{
				TUser addUser;
				addUser.sock = accept(wParam, (SOCKADDR*)&addUser.clientAddr, &addUser.addrlen);
				if (addUser.sock == INVALID_SOCKET)
				{
					break;
				}

				AddUser(addUser);

				//WSAASyncSelect���� �Լ��� ������ ȣ���ϸ� �������� ����ȴ�.
				//WM_DROP�� �ȴ�.
				int iRet = WSAAsyncSelect(addUser.sock, hWnd, WM_RWC, FD_READ | FD_WRITE | FD_CLOSE);

				if (iRet == SOCKET_ERROR)
				{
					return -1;
				}
			}
			break;
			case FD_READ:
			{
				TUser::g_socket = wParam;
				ITOR itor = find_if(g_allUser.begin(), g_allUser.end(), TUser());
				TUser* pUser = (TUser*)&(*itor);
				//Ŭ���̾�Ʈ �����ϰ�� recv()
				if (RecvData(pUser) <= 0)
				{
					closesocket(pUser->sock);
					itor = g_allUser.erase(itor);
				}
			}
			break;
			case FD_WRITE:
			{
				//���� ��Ŷ�� �־�δ� ��Ұ� ����.
				//��ŶǮ�� �ִ°� Send�Ѵ�.
			}
			break;
			case FD_CLOSE:
			{
				//���� �����Ͱ� 0�ϰ�쿡 ������ ó���� �Ͽ���.
				PostMessage(hWnd, WM_KGCA, wParam, FD_READ);
			}
			break;
			}
		}
	}
	else if (uMsg == WM_RWC)
	{
		printf("uMsg == WM_RWC\n");
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

HWND MakeWindow()
{
	//������ Ŭ���� ���
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;

	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = NULL;
	wc.hIcon;
	wc.hCursor;
	wc.hbrBackground;
	wc.lpszMenuName;
	wc.lpszClassName;
	wc.hIconSm;


int main()
{

}
