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
	int SendData(UPACKET* pPacket);
	void Dispatch(DWORD bytes, LPOVERLAPPED_EX ovex);
	int CheckReturn(int iRet);
public:
	XUser();
	XUser(SOCKET _Sock, SOCKADDR_IN _Addr);
	XUser(const XUser& user);
	virtual ~XUser();
};

//실제로는 서버의 역활이다.(Sample 에서는 TServer)
class XUserManager : public TSingleton<XUserManager>, public TThread
{
	friend class TSingleton<XUserManager>;
public:
	TStreamPacket	m_PacketList;
	list<XUser*>	m_UserList;
public:
	void Run();
	void AddPacket(T_PACKET& packet);

	XUser* GetMemory(SOCKET clientSocket, SOCKADDR_IN clientAddr);
	XUser* AddUser(SOCKET clientSocket, SOCKADDR_IN clientAddr);
	bool DelUser(SOCKET clientSocket);
	int Broadcastting(UPACKET* pPacket);
};

#define I_User XUserManager::GetInstance()