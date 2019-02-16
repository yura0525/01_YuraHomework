#pragma once
#include "xCore.h"
#include "TSender.h"
#include "TReceiver.h"

class Sample : public xCore
{
public:
	TSender		m_Sender;
	TReceiver	m_Receiver;
public:
	bool		Init();
	bool		PreRender();
	bool		Render();
	bool		PostRender();
	bool		Release();
	LRESULT		MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

public:
	Sample();
	~Sample();
};

