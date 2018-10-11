#pragma once
#include "xStd.h"
#define KEYSTATECOUNT		256

class xInput : public TSingleton<xInput>
{
private:
	friend class TSingleton<xInput>;

public:
	LPDIRECTINPUT8			m_pDI;
	LPDIRECTINPUTDEVICE8	m_pKey;
	LPDIRECTINPUTDEVICE8	m_pMouse;

	BYTE					m_KeyState[KEYSTATECOUNT];
	DIMOUSESTATE			m_MouseState;

public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();

public:
	xInput();
	virtual ~xInput();
};

#define I_Input xInput::GetInstance()
