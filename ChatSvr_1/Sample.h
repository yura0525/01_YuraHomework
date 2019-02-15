#pragma once
#include "xCore.h"
#include "TUdpSocket_1.h"

class Sample : public xCore
{
public:
	TUdpSocket_1		m_Udp;
public:
	bool Init();
	bool Release();
	bool PreRender()	{	return true;	}
	bool PostRender()	{	return true;	}

	LRESULT MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
public:
	Sample();
	~Sample();
};

