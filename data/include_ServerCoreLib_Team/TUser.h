#pragma once
#include "TNPC.h"

#define MAX_BUFFER 2048

// 어떤 메세지인지 구분해야 한다. ( 필요성 )
// 프로토콜 생성
typedef struct _OVERLAPPED2 : OVERLAPPED
{
	enum
	{
		MODE_RECV = 0,
		MODE_SEND = 1
	};
	int m_iFlags;
	_OVERLAPPED2()
	{
		m_iFlags = MODE_RECV;
	}
}OVERLAPPED2;

typedef std::map<tGUID, TCharacter, GUIDComparer> TCharacterList;

class TActiveCharacter : public TServerObj
{
private:
	TCharacterList m_CharList;
public:
	TCharacterList& Get();
public:
	TActiveCharacter()
	{
	}
	virtual ~TActiveCharacter()
	{
	}
	friend ostream& operator<<(ostream& stream, TActiveCharacter& info);
	friend istream& operator>>(istream& stream, TActiveCharacter& info);
};

class TUser
{
private:
	TCharacter*			m_pCharacter;
	TCharacterList		m_CharacterList;
public:
	TCharacterList&		GetCharacterList();
	void				SetActiveCharacter(TCharacter* pChar, bool bAlive);
public:
	OVERLAPPED2			m_OV;
	int					m_iType;
	tGUID				m_LoginGuid;
	tGUID				m_Guid;
	tGUID				m_GuidActiveCharacter;
	CONNECT_TYPE		m_ConnectType;
	string				m_Name;
	SOCKET				m_Socket;
	WSABUF				m_wsaRecvBuffer;
	WSABUF				m_wsaSendBuffer;
	char				m_recvBuffer[MAX_BUFFER];
	char				m_sendBuffer[MAX_BUFFER];
	TStreamPacket		m_StreamPacket;
	bool				m_bUsed;

public:
	void				Dispatch(DWORD dwByteSize, LPOVERLAPPED ov);
	void				WaitForPacketReceive();
	void				Release();
public:
	friend ostream& operator<<(ostream& stream, TUser& info);
	friend istream& operator>>(istream& stream, TUser& info);

public:
	TUser();
	virtual ~TUser();
};

