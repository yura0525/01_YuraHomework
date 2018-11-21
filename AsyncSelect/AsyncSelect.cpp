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
		printf("\n---->정상 퇴장\n");
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
				printf("\n패킷 완성 => %s\n", packet.msg);
				Broadcastting(packet.msg);
				break;
			}
			case PACKET_CREATE_CHARACTER:
			{
				CHARACTER_INFO cInfo;
				memcpy(&cInfo, packet.msg, sizeof(CHARACTER_INFO));
				printf("\n패킷 완성 => %s\n", packet.msg);
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

				//WSAASyncSelect같은 함수를 여러번 호출하면 마지막만 적용된다.
				//WM_DROP만 된다.
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
				//클라이언트 소켓일경우 recv()
				if (RecvData(pUser) <= 0)
				{
					closesocket(pUser->sock);
					itor = g_allUser.erase(itor);
				}
			}
			break;
			case FD_WRITE:
			{
				//받은 패킷을 넣어두는 장소가 없음.
				//패킷풀에 있는걸 Send한다.
			}
			break;
			case FD_CLOSE:
			{
				//받은 데이터가 0일경우에 나가는 처리를 하였다.
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
	//윈도우 클래스 등록
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
