#pragma once
#include "TServerStd.h"
#include "TProtocol.h"

//패킷하나가 2000이면 48바이트가 잘려서 들어가기 떄문에
//최대패킷 사이즈를 더한 사이즈로 잡아야한다.
#define MAX_RECV_SIZE 2048 * 2

class XUser;

typedef struct
{
	UPACKET packet;
	XUser* pUser;
}T_PACKET, *LP_TPACKET;


class TStreamPacket
{
public:
	std::list<T_PACKET>		m_List;
	DWORD					m_dwStartPos;
	DWORD					m_dwWritePos;
	DWORD					m_dwReadPos;
	UPACKET*				m_pPacket;
	char					m_RecvBuffer[MAX_RECV_SIZE];
public:
	void Put(char* recvBuffer, int iSize, XUser* pUser);
public:
	TStreamPacket();
	virtual ~TStreamPacket();
};

