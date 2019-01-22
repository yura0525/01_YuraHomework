#include "Sample.h"

DWORD WINAPI ClientThread(LPVOID arg)
{
	SOCKET sock = (SOCKET)arg;
	TUser* pUser = &g_allClientSocket[sock];

	char buffer[256] = { 0, };
	int recvbyte = 0;
	int iRet = 0;
	while (1)
	{
		iRet = recv(sock, &buffer[recvbyte], PACKET_HEADER_SIZE - recvbyte, 0);
		if (CheckReturn(iRet) <= 0)
			break;

		recvbyte += iRet;
		if (recvbyte == PACKET_HEADER_SIZE)
		{
			UPACKET packet;
			ZeroMemory(&packet, sizeof(packet));
			memcpy(&packet.ph, buffer, PACKET_HEADER_SIZE);
			
			int rByte = 0;
			int iRecvByte = 0;
			do
			{
				rByte = recv(sock, (char*)&packet.msg[iRecvByte], packet.ph.len - iRecvByte, 0);
				if (CheckReturn(iRet) <= 0)
					break;

				iRecvByte += rByte;
			} while (packet.ph.len > iRecvByte);

			if (CheckReturn(iRet) <= 0)
				break;

			recvbyte = 0;
			switch (packet.ph.type)
			{
			case PACKET_CHAT_MSG:
			{
				cout << "패킷 완성:--->" << packet.msg;
				Broadcastting(&packet);
			}
			break;
			}
		}
		Sleep(1);
	}
	DeleteUser(sock);
	return 0;
}
void main()
{

}