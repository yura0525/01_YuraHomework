#pragma once
#include "TNetwork.h"
#include "TUser.h"
class TSelect : public TNetwork
{
public:
	TUser			     m_SessionList[64];
	std::list<TUser*>	 m_pUserList;

	ULONG           lastprint;
public:
	bool	InitSelectServer();
	void	PrintStatistics();
	bool	SendPacketList(TUser* pSendUser, TUser* pRecvUser = NULL);
	bool	Run();
	TUser*	GetSession();
	bool	RemoveSession(TUser* pUser);
	bool	Broadcastting(TUser* pSendUser);
public:
	TSelect();
	virtual ~TSelect();
};
