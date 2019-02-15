#pragma once
#include "TServer.h"
#include "TChatPacketPool.h"
#include "TChatAccept.h"
#include "TChatUser.h"

class TChatSvr : public TSingleton<TChatSvr> , public TServer
{
private:
	friend class TSingleton<TChatSvr>;
public:
	TChatPacketPool	m_PacketPool;
	TChatAccept		m_Acceptor;

	//////////////  유저 리스트 ///////////////////
	std::list<TChatUser*>	m_UserList;
	typedef std::list<TChatUser*>::iterator	m_ListItor;
	int						m_iUserCount;

public:
	bool Init();
	bool Release();
	bool Run();
	bool AddUser(SOCKET socket, SOCKADDR_IN address);
	void DeleteUser(SOCKET socket);
	void DeleteUser(TChatUser* pDeleteUser);
	void Broadcastting(char* msg, WORD code, SOCKET SendSocket = NULL);
	void Broadcastting(T_PACKET* pUserData);
	void Broadcastting(UPACKET* pPacket, SOCKET SendSocket = NULL);

public:
	TChatSvr();
	~TChatSvr();
};

#define I_Server TChatSvr::GetInstance()

