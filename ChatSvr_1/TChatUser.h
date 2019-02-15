#pragma once
#include "TUser_1.h"

class TChatUser : public TUser_1
{
public:
	int		m_iIndex;
	int		m_PositionX;
	int		m_PositionY;
	int		m_iDirection;
public:
	void Movement(UPACKET* pPacket);
	void AddPacket(T_PACKET& addPacket);
	void DeleteUser(SOCKET socket);
	void DeleteUser();
	void Dispatch(DWORD dwByteSize, LPOVERLAPPED ov);

public:
	TChatUser();
	~TChatUser();
};

