#pragma once
#include "TUser.h"

class TServer_1 : public TSingleton<TServer_1>
{
private:
	friend class TSingleton<TServer_1>;

public:
	HANDLE				m_Mutex;
	TAcceptor_2			m_Acceptor;
	TNpcManager			m_NPCMgr;
	TStreamPacket_3		m_StreamPacket;

	std::map<tGUID, TUser*, GUIDComparer> m_UserList;
	typedef std::map<tGUID, TUser*, GUIDComparer>::iterator m_ListItor;
	
	typedef void(TServer_1::*CallFunction)(T_PACKET& t);
	std::map<int, CallFunction>					m_fnExecutePacket;
public:
	virtual bool	Init();
	virtual bool	Frame();
	virtual bool	Render();
	virtual bool	Release();

public:
	virtual void	ProcessPacket();
	virtual void	AddPacket(T_PACKET& pack);
	virtual int		SendPacket(TUser* pUser, UPACKET& PacketMsg);
	void			SendPacket(TUser* pUser, int iSendByte);
	virtual void	Broadcast(T_PACKET& pSendUser);
	virtual void	Broadcast(UPACKET& packet);
	void			Broadcast(stringstream& SendStream);
public:
	virtual TUser*	GetUser(int iIndex);
	virtual bool	DelUser(int iIndex);
public:
	virtual void		MoveAction(T_PACKET& pSendUser);
	virtual void		Msg(T_PACKET& pSendUser);
	virtual void		AckChatName(T_PACKET& pSendUser);
	virtual void		ReqChatName(T_PACKET& pSendUser);
	virtual void		CreateAccount(T_PACKET& pSendUser);
	virtual void		DeleteAccount(T_PACKET& pSendUser);
	virtual void		Login(T_PACKET& pSendUser);
	virtual void		Logout(T_PACKET& pSendUser);
	virtual void		CreateCharacter(T_PACKET& pSendUser);
	virtual void		DeleteCharacter(T_PACKET& pSendUser);
	virtual void		HitCharacter(T_PACKET& pSendUser);
	virtual void		HitMonster(T_PACKET& pSendUser);

	virtual void		AttackCharacter(T_PACKET& pSendUser);
	virtual void		AttackMonster(T_PACKET& pSendUser);
	virtual void		DeadCharacters(T_PACKET& pSendUser);
	virtual void		DeadMonster(T_PACKET& pSendUser);
	virtual void		DamageCharacter(T_PACKET& pSendUser);
	virtual void		DamageMonster(T_PACKET& pSendUser);
	virtual void		SpawnCharacters(T_PACKET& pSendUser);
	virtual void		SpawnMonster(T_PACKET& pSendUser);
	virtual void		SyncCharacters(T_PACKET& pSendUser);
	virtual void		SyncMonster(T_PACKET& pSendUser);
public:
	friend ostream& operator<<(ostream& stream, TServer_1& info)
	{
		stream << info.m_UserList.size() << endl;
		for (auto& user : info.m_UserList)
		{
			stream << user.first << endl;
			stream << *user.second << endl;
		}
		return stream;
	}
	friend istream& operator>>(istream& stream, TServer_1& info)
	{
		int iNumUsers = 0;
		tGUID guid;
		TUser* pUser = NULL;
		SAFE_NEW(pUser, TUser);
		info.m_UserList.clear();

		stream >> iNumUsers;
		for (int i = 0; i < iNumUsers; i++)
		{
			stream >> guid;
			stream >> *pUser;
			info.m_UserList[guid] = pUser;
		}
		return stream;
	}
public:

	TServer_1();
	~TServer_1();
};

#define I_Server TServer_1::GetInstance()