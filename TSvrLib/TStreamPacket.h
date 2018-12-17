#pragma once
#include "TServerStd.h"
#include "TProtocol.h"

//��Ŷ�ϳ��� 2000�̸� 48����Ʈ�� �߷��� ���� ������
//�ִ���Ŷ ����� ���� ������� ��ƾ��Ѵ�.
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

