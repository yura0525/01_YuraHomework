#pragma once
#include "TServerObj.h"
class TUser_1 : public TServerObj
{
public:
	SOCKET	m_Socket;
public:
	virtual void DeleteUser();
	virtual void DeleteUser(SOCKET socket);
	virtual void Dispatch(DWORD dwByteSize, LPOVERLAPPED ov);
public:
	TUser_1();
	virtual ~TUser_1();
};

