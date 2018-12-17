#include "stdafx.h"
#include "TNetwork.h"

bool TNetwork::Set(int iPort, const char* address)
{
	WSADATA wd;
	WSAStartup(MAKEWORD(2, 2), &wd);

	m_ListenSock = socket(AF_INET, SOCK_STREAM, 0);
	if (m_ListenSock == INVALID_SOCKET)
	{
		return false;
	}

	SOCKADDR_IN addr;
	ZeroMemory(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	if (address == NULL)
	{
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
	}
	else
	{
		addr.sin_addr.s_addr = inet_addr(address);
	}
	addr.sin_port = htons(10000);

	//¹ÙÀÎµù
	int ret = ::bind(m_ListenSock, (sockaddr*)&addr, sizeof(addr));
	if (ret == SOCKET_ERROR)
		return false;

	ret = ::listen(m_ListenSock, SOMAXCONN);
	if (ret == SOCKET_ERROR)
		return false;

	return true;
}

bool TNetwork::Release()
{
	return true;
}

TNetwork::TNetwork()
{
}


TNetwork::~TNetwork()
{
}
