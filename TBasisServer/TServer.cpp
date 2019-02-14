#include "TServer.h"

bool TServer::Init()
{
	//������ ����
	CreateThread();
	return true;
}
bool TServer::Release()
{
	return true;
}
bool TServer::Run()
{
	return true;
}
bool TServer::AddUser(SOCKET socket, SOCKADDR_IN address)
{
	return true;
}
bool TServer::SendMsg(SOCKET socket, char* msg, WORD code)
{
	UPACKET sendmsg;
	ZeroMemory(&sendmsg, sizeof(sendmsg));
	sprintf_s(sendmsg.msg, "%s", msg);
	sendmsg.ph.type = code;
	sendmsg.ph.len = sizeof(PACKET_HEADER) + strlen(sendmsg.msg);

	int iRet = send(socket, (char*)&sendmsg, sendmsg.ph.len, 0);
	if (iRet <= 0)
	{
		return false;
	}

	return true;
}


TServer::TServer()
{
}


TServer::~TServer()
{
}
