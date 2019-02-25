#include "TServer.h"
#include "TDebugString.h"
#include "TServerIOCP.h"
#include "TSynchronize.h"


void TServer::ReqVerification(T_PACKET& pSendUser)
{

}


void TServer::SendLogoutPlayer(tGUID tGuid)
{
	stringstream data;
	data << tGuid;
	stringstream stream = MakePacket(PACKET_LOGOUT_PLAYER, data);
	Broadcast(tGuid, stream);
}

void TServer::CreateAccount(T_PACKET& pSendUser)
{
	tGUID uuid;
	RPC_STATUS ret_val = ::UuidCreate(&uuid);
	string msg(pSendUser.packet.msg, pSendUser.packet.ph.len - PACKET_HEADER_SIZE);
	stringstream RecvStream(msg);
	string id;
	string pw;
	RecvStream >> id;
	RecvStream >> pw;
	stringstream data;
	// ret : 1(OK),2(ID-Faild),3(PS-Faild),4(etc)
	data << 1;
	data << uuid;

	stringstream stream(MakePacket(PACKET_CREATE_ACCOUNT_ACK, data));
	SendPacket(pSendUser.pUser, stream);
}
void TServer::DeleteAccount(T_PACKET& pSendUser) {}
void TServer::LogIn(T_PACKET& pSendUser)
{
	string msg(pSendUser.packet.msg, pSendUser.packet.ph.len - PACKET_HEADER_SIZE);
	stringstream RecvStream(msg);
	string id;
	string pw;
	RecvStream >> id;
	RecvStream >> pw;
	stringstream data;
	// ret : 1(OK),2(ID-Faild),3(PS-Faild),4(etc)
	int iRet = 1;
	data << iRet;
	data << pSendUser.pUser->m_LoginGuid;
	data << pSendUser.pUser->m_Guid;
	data << pSendUser.pUser->m_GuidActiveCharacter;

	stringstream stream(MakePacket(PACKET_LOGIN_ACK, data));
	SendPacket(pSendUser.pUser, stream);

	PrintGuid(pSendUser.pUser->m_LoginGuid, " TServer::LogIn() loginguid");
	PrintGuid(pSendUser.pUser->m_Guid, "TServer::LogIn() guid");
	PrintGuid(pSendUser.pUser->m_GuidActiveCharacter, "TServer::LogIn() character");
}
void TServer::LogOut(T_PACKET& pSendUser)
{
	string msg(pSendUser.packet.msg, pSendUser.packet.ph.len - PACKET_HEADER_SIZE);
	stringstream RecvStream(msg);
	tGUID tGuid;
	RecvStream >> tGuid;
	SendLogoutPlayer(tGuid);
}