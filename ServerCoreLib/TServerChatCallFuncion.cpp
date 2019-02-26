#include "TServer.h"
#include "TDebugString.h"
#include "TServerIOCP.h"
#include "TSynchronize.h"

void TServer::Msg(T_PACKET& pSendUser)
{
	{
		TSynchronize sync(this);
		UPACKET* pPacket = &(pSendUser.packet);
		string msg(pSendUser.packet.msg, pSendUser.packet.ph.len - PACKET_HEADER_SIZE);
		stringstream RecvStream(msg);
		tGUID tGuid;
		string recvMsg;
		RecvStream >> tGuid;
		RecvStream >> recvMsg;
		I_DebugStr.DisplayText("\nTServer::Msg() Message:%s\r\n", recvMsg.c_str());

		TUserListItor iter;
		for (iter = m_UserList.begin(); iter != m_UserList.end(); iter++)
		{
			TUser* pUser = (TUser*)iter->second;
			if (SendPacket(pUser, pSendUser.packet) <= 0)
				break;
		}
	}
}
void TServer::ReqChatName(T_PACKET& pSendUser) {}
void TServer::AckChatName(T_PACKET& pSendUser)
{
	UPACKET* pPacket = &(pSendUser.packet);
	UPACKET PacketMsg;
	ZeroMemory(&PacketMsg, sizeof(PacketMsg));
	PacketMsg.ph.iotype = 777;
	PacketMsg.ph.len = pPacket->ph.len;
	PacketMsg.ph.type = PACKET_CHAT_MSG;
	pPacket->msg[pPacket->ph.len - PACKET_HEADER_SIZE] = 0;

	C_STR name = pPacket->msg;
	memcpy(&PacketMsg.msg, &pPacket->msg, pPacket->ph.len - PACKET_HEADER_SIZE);
	
	char buffer[64] = "님이 입장하였습니다.";
	memcpy(&PacketMsg.msg[pPacket->ph.len - PACKET_HEADER_SIZE], buffer, strlen(buffer));
	PacketMsg.ph.len += (uint16_t)strlen(buffer);

	I_DebugStr.DisplayText("\nTServer::AckChatName() Message:%s\n", PacketMsg.msg);
	Broadcast(PacketMsg);
}