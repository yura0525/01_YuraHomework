#pragma once
#include "TUser.h"

typedef std::map<tGUID, TCharacter, GUIDComparer>::iterator TCharMapItor;
typedef std::map<tGUID, TUser*, GUIDComparer>				TUserMapList;
typedef std::map<tGUID, TUser*, GUIDComparer>::iterator		TUserMapItor;


class TServer : public TSingleton<TServer>, public TServerObj
{
private:
	friend class TSingleton<TServer>;
private:
	HANDLE				m_hMutexPacket;
	TAcceptor			m_Acceptor;
	TNPCManager			m_NPCMgr;
	TStreamPacket		m_StreamPacket;
	TActiveCharacter	m_UserCharacterList;
	TTargetPointMgr		m_tpMgr;
	TUserMapList		m_UserList;

	typedef void(TServer::*CallFunction)(T_PACKET& t);
	std::map<int, CallFunction>			m_fnExecutePacket;
public:
	TAcceptor&				GetAcceptor();
	TTargetPointMgr&		GetTargetPointMgr();
	TActiveCharacter&		GetUserCharacterList();			// active
	TServer*				GetPtr();

public:
	virtual bool			Init();
	virtual bool			Frame();
	virtual bool			Render();
	virtual bool			Release();
	virtual void			ProcessPacket();
	virtual void			AddPacket(T_PACKET& pack);
	virtual int				SendPacket(TUser* pUser, UPACKET& packetMsg);
	virtual int				SendPacket(TUser* pUser, stringstream& SendStream);
	void					SendPacket(TUser* pUser, size_t iSendByte);
	virtual void			Broadcast(T_PACKET& pSendUser);
	virtual void			Broadcast(UPACKET& pSendUser);
	void					Broadcast(stringstream& SendStream);
	void					Broadcast(tGUID tGuidExcept, stringstream& SendStream);
	virtual void			BroadcastNewPlayer(TCharacter& player);
public:
	virtual TUser*			GetUser(int iIndex);
	virtual bool			DelUser(int iIndex);
	virtual bool			DelUser(tGUID tGuid);
	virtual void			AddUser(SOCKET sock);

	void					ChangeSuperUser();
	void					PrintGuid(tGUID& tguid, const char* msg);
public:
	virtual void			ReqVerification(T_PACKET& pSendUsert);
	virtual void			SyncAllCharacters(TUser* pUser);
	virtual void			SendDeadPlayer(TUser* pUSer);
	virtual void			SendLogoutPlayer(tGUID tGuid);

public:
	virtual void			MoveAction(T_PACKET& pSendUser);
	virtual void			Msg(T_PACKET& pSendUser);
	virtual void			ReqChatName(T_PACKET& pSendUser);
	virtual void			AckChatName(T_PACKET& pSendUser);
	virtual void			CreateAccount(T_PACKET& pSendUser);
	virtual void			DeleteAccount(T_PACKET& pSendUser);
	virtual void			LogIn(T_PACKET& pSendUser);
	virtual void			LogOut(T_PACKET& pSendUser);
	virtual void			CreateCharacter(T_PACKET& pSendUser);
	virtual void			DeleteCharacter(T_PACKET& pSendUser);
	virtual void			HitCharacter(T_PACKET& pSendUser);
	virtual void			HitMonster(T_PACKET& pSendUser);
	virtual void			ReqZoneEntry(T_PACKET& pSendUser);
	virtual void			AckZoneEntry(T_PACKET& pSendUser);
	virtual void			RecvZoneTargetPoint(T_PACKET& pSendUser);
	virtual void			AttackCharacter(T_PACKET& pSendUser);
	virtual void			AttackMonster(T_PACKET& pSendUser);
	virtual void			DeadCharacter(T_PACKET& pSendUser);
	virtual void			DeadMonster(T_PACKET& pSendUser);
	virtual void			DamageCharacter(T_PACKET& pSendUser);
	virtual void			DamageMonster(T_PACKET& pSendUser);
	virtual void			SpawnCharacter(T_PACKET& pSendUser);
	virtual void			SpawnMonster(T_PACKET& pSendUser);
	virtual void			SyncCharacters(T_PACKET& pSendUser);
	virtual void			SyncMonster(T_PACKET& pSendUser);

public:
	void					SpawnMonsterList();
	
public:
	friend ostream& operator<<(ostream& stream, TServer& info);
	friend istream& operator>>(istream& stream, TServer& info);
public:
	TServer();
	virtual ~TServer();
};

#define I_Server TServer::GetInstance()
