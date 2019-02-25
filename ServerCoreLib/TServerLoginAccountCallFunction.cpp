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

}
void TServer::DeleteAccount(T_PACKET& pSendUser) {}
void TServer::LogIn(T_PACKET& pSendUser)
{

}
void TServer::LogOut(T_PACKET& pSendUser)
{

}