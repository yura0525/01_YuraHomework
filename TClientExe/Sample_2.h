#pragma once
#include "TUdpSocket_2.h"
#include "xCore.h"

class Sample_2 : public xCore
{
public:
	TUdpSocket_2		m_Udp;
	bool				m_bLogin;
	int					m_iSerIndex;
	POINT				m_CursorPos;

public:
	bool				Init();
	bool				PreRender();
	bool				Render();
	bool				PostRender();
	bool				Frame();
	bool				Release();
public:
	Sample_2();
	~Sample_2();
};

