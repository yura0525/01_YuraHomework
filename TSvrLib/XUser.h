#pragma once
#include "TStreamPacket.h"
#include "TThread.h"

typedef struct _OVERLAPPED_EX : public OVERLAPPED
{
	enum
	{
		MODE_RECV = 0,
		MODE_SEND = 1,
	};
	int m_iFlags;
}OVERLAPPED_EX, *LPOVERLAPPED_EX;

class XUser : public TSvrObject
{
public:
	SOCKET			m_clientSocket;
	SOCKADDR_IN		m_clientAddr;
	WSABUF			m_wsaBuffer;
	char			m_strBuffer[2048];
	OVERLAPPED_EX	m_ov;
	TStreamPacket	m_StreamPacket;

public:
	void RecvData();
	void SendData();
	void Dispatch(DWORD bytes, LPOVERLAPPED_EX ovex);
public:
	XUser();
	XUser(SOCKET _Sock, SOCKADDR_IN _Addr);
	XUser(const XUser& user);
	virtual ~XUser();
};

