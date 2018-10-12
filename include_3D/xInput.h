#pragma once
#include "xStd.h"
#define KEYSTATECOUNT		256

enum KeyState
{
	KEY_FREE = 0,
	KEY_PUSH,
	KEY_UP,
	KEY_HOLD,
};

class xInput : public TSingleton<xInput>
{
private:
	friend class TSingleton<xInput>;

public:
	LPDIRECTINPUT8			m_pDI;
	LPDIRECTINPUTDEVICE8	m_pKey;
	LPDIRECTINPUTDEVICE8	m_pMouse;

	BYTE					m_KeyState[KEYSTATECOUNT];
	DIMOUSESTATE			m_CurrentMouseState;
	DWORD					m_BeforeMouseState[3];
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();

	DWORD KeyCheck(DWORD dwKey);
	DWORD Key(DWORD dwKey);
	void MsgEvent(MSG msg);
public:
	xInput();
	virtual ~xInput();
};

#define I_Input xInput::GetInstance()
