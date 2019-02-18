#pragma once
#include "TCore.h"
#include "TObject.h"
#include "TClient_1.h"
#include "TUdpSocket_2.h"
#include "TGameUser.h"


class Sample_2 : public TCore
{
public:
	TUdpSocket_2			m_Udp;
	std::vector<TGameUser>	m_UserList;
	TClient_1				m_Client;
	bool					m_bLogin;
	int						m_iSerIndex;
	POINT					m_CursorPos;

public:
	bool					Init();
	bool					PreRender();
	bool					Render();
	bool					PostRender();
	bool					Frame();
	bool					Release();
public:
	Sample_2();
	~Sample_2();
};

