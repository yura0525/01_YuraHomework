#include "xInput.h"
POINT		g_pMousePos;
TGameInput	g_Input;

bool xInput::Init()
{
	memset(&m_KeyState, 0, sizeof(BYTE) * KEYSTATECOUNT);
	memset(&m_MouseState, 0, sizeof(DIMOUSESTATE));

	HRESULT hr = S_OK;

	if (FAILED(hr = DirectInput8Create(g_hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&m_pDI, NULL)))
	{
		return false;
	}

	//Keyboard
	if (FAILED(hr = m_pDI->CreateDevice(GUID_SysKeyboard, &m_pKey, NULL)))
	{
		return false;
	}
	if (FAILED(hr = m_pKey->SetDataFormat(&c_dfDIKeyboard)))
	{
		return false;
	}
	if (FAILED(hr = m_pKey->SetCooperativeLevel(g_hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND | DISCL_NOWINKEY)))
	{
		return false;
	}
	while (m_pKey->Acquire() == DIERR_INPUTLOST);

	//mouse
	if (FAILED(hr = m_pDI->CreateDevice(GUID_SysMouse, &m_pMouse, NULL)))
	{
		return false;
	}
	if (FAILED(hr = m_pMouse->SetDataFormat(&c_dfDIMouse)))
	{
		return false;
	}
	if (FAILED(hr = m_pMouse->SetCooperativeLevel(g_hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND)))
	{
		return false;
	}
	while (m_pMouse->Acquire() == DIERR_INPUTLOST);
	return true;
}
bool xInput::Frame()
{
	HRESULT hr = S_OK;
	if (FAILED(hr = m_pKey->GetDeviceState(256, &m_KeyState)))
	{
		while (m_pKey->Acquire() == DIERR_INPUTLOST);
	}

	if (FAILED(hr = m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_MouseState)))
	{
		while (m_pMouse->Acquire() == DIERR_INPUTLOST);
	}

	g_Input.bFront	= m_KeyState[DIK_W];
	g_Input.bBack	= m_KeyState[DIK_S];
	g_Input.bLeft	= m_KeyState[DIK_A];
	g_Input.bRight	= m_KeyState[DIK_D];
	g_Input.bAttack = m_KeyState[DIK_RETURN] | m_MouseState.rgbButtons[0];
	g_Input.bJump	= m_KeyState[DIK_SPACE] | m_MouseState.rgbButtons[1];

	g_pMousePos.x = I_Input.m_MouseState.lX;
	g_pMousePos.y = I_Input.m_MouseState.lY;
		
	return true;
}
bool xInput::Render()
{
	return true;
}
bool xInput::Release()
{
	if (m_pKey != NULL)		m_pKey->Unacquire();
	if (m_pMouse != NULL)	m_pMouse->Unacquire();

	if (m_pKey != NULL)		m_pKey->Release();
	if (m_pMouse != NULL)	m_pMouse->Release();
	if (m_pDI != NULL)		m_pDI->Release();

	m_pKey = NULL;
	m_pMouse = NULL;
	m_pDI = NULL;
	return true;
}

xInput::xInput()
{
}


xInput::~xInput()
{
}
