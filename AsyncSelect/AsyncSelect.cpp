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

int main()
{

}
