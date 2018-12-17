#include "xInput.h"

TGameInput	g_Input;
POINT		g_pMousePos;

bool xInput::Init()
{
	memset(&m_KeyState, 0, sizeof(BYTE) * KEYSTATECOUNT);
	memset(&m_CurrentMouseState, 0, sizeof(DIMOUSESTATE));
	memset(m_BeforeMouseState, 0, sizeof(DWORD) * 3);

	HRESULT hr = S_OK;
	if (FAILED(hr = DirectInput8Create(g_hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&m_pDI, NULL)))
	{
		return false;
	}
	//keyboard
	if (FAILED(hr = m_pDI->CreateDevice(GUID_SysKeyboard, &m_pKey, NULL)) )
	{
		return false;
	}
	if (FAILED(hr = m_pKey->SetDataFormat(&c_dfDIKeyboard)))
	{
		return false;
	}

	//������ DISCL_EXCLUSIVE���� �����Ѵ�.
	if (FAILED(hr = m_pKey->SetCooperativeLevel(g_hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND | DISCL_NOWINKEY)))
	//if (FAILED(hr = m_pKey->SetCooperativeLevel(g_hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND | DISCL_NOWINKEY)))
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

	//������ DISCL_EXCLUSIVE���� �����Ѵ�. â�� ������ ��� DISCL_NONEXCLUSIVE���� �����Ͽ���.
	//if (FAILED(hr = m_pMouse->SetCooperativeLevel(g_hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND)))
	if (FAILED(hr = m_pMouse->SetCooperativeLevel(g_hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)))
	{
		return false;
	}
	while (m_pMouse->Acquire() == DIERR_INPUTLOST);

	return true;
}


bool xInput::Frame()
{
	HRESULT hr = S_OK;
	if (FAILED(hr = m_pKey->GetDeviceState(KEYSTATECOUNT, &m_KeyState)))
	{
		while (m_pKey->Acquire() == DIERR_INPUTLOST);
	}
	if (FAILED(hr = m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_CurrentMouseState)))
	{
		while (m_pMouse->Acquire() == DIERR_INPUTLOST);
	}

	g_Input.bFront = m_KeyState[DIK_W];
	g_Input.bBack = m_KeyState[DIK_S];
	g_Input.bLeft = m_KeyState[DIK_A];
	g_Input.bRight = m_KeyState[DIK_D];
	g_Input.bAttack = m_KeyState[DIK_RETURN] | m_CurrentMouseState.rgbButtons[0];
	g_Input.bJump = m_KeyState[DIK_SPACE] | m_CurrentMouseState.rgbButtons[1];

	GetCursorPos(&g_pMousePos);				//ȭ�� ��ǥ
	ScreenToClient(g_hWnd, &g_pMousePos);	//������ ��ǥ�踦 Ŭ���̾�Ʈ ��ǥ��� ��ȯ

	for (int iKey = 0; iKey < 255; iKey++)
	{
		m_KeyState[iKey] = KeyCheck(iKey);
	}

	for (int iButton = 0; iButton < 3; iButton++)
	{
		if (m_BeforeMouseState[iButton] == KEY_PUSH)
		{
			if (m_CurrentMouseState.rgbButtons[iButton] == KEY_PUSH)
			{
				m_CurrentMouseState.rgbButtons[iButton] = KEY_HOLD;
			}
		}
		else if (m_BeforeMouseState[iButton] == KEY_UP)
		{
			if (m_CurrentMouseState.rgbButtons[iButton] == KEY_UP)
			{
				m_CurrentMouseState.rgbButtons[iButton] = KEY_FREE;
			}
		}
		m_BeforeMouseState[iButton] = m_CurrentMouseState.rgbButtons[iButton];
	}

	return true;
}

DWORD xInput::KeyCheck(DWORD dwKey)
{
	//GetKeyState()			//������
	//GetAsyncKeyState		//�񵿱���

	SHORT sKey = GetAsyncKeyState(dwKey);

	/*if (sKey & 0x8000)
	{
	m_dwKeyState[dwKey] = KEY_PUSH;
	}
	if (sKey & 0x8001)
	{
	m_dwKeyState[dwKey] = KEY_HOLD;
	}
	if (sKey & 0x0001)
	{
	m_dwKeyState[dwKey] = KEY_UP;
	}
	if (sKey & 0x0000)
	{
	m_dwKeyState[dwKey] = KEY_FREE;
	}*/

	//1000 0000 0000 0000	=> 0x8000(���� X ~ ����  0) ���� ������ ���� ������ ��������.
	//1000 0000 0000 0001	=> 0x8001(���� 0 ~ ����  0) ���� �������ְ� ���ݵ� �������ִ�.
	//0000 0000 0000 0000	=> 0x0000(���� X ~ ����  X) ������ ���������� �ʰ� ���ݵ� ���������� �ʴ�. 
	//0000 0000 0000 0001	=> 0x0001(���� 0 ~ ����  X) ���� �������ְ� ������ ���������� �ʴ�.

	if (sKey & 0x8000)
	{
		if (m_KeyState[dwKey] == KEY_FREE
			|| m_KeyState[dwKey] == KEY_UP)
		{
			m_KeyState[dwKey] = KEY_PUSH;
		}
		else
		{
			m_KeyState[dwKey] = KEY_HOLD;
		}
	}
	else
	{
		if (m_KeyState[dwKey] == KEY_PUSH
			|| m_KeyState[dwKey] == KEY_HOLD)
		{
			m_KeyState[dwKey] = KEY_UP;
		}
		else
		{
			m_KeyState[dwKey] = KEY_FREE;
		}
	}

	return m_KeyState[dwKey];
}

bool xInput::Render()
{
	return true;
}
bool xInput::Release()
{
	if( m_pKey )	m_pKey->Unacquire();
	if (m_pMouse)	m_pMouse->Unacquire();

	if (m_pKey)		m_pKey->Release();
	if (m_pMouse)	m_pMouse->Release();
	if (m_pDI)		m_pDI->Release();

	m_pKey = NULL;
	m_pMouse = NULL;
	m_pDI = NULL;
	return true;
}

void xInput::MsgEvent(MSG msg)
{
	switch (msg.message)
	{
	case WM_LBUTTONDOWN:
		m_CurrentMouseState.rgbButtons[0] = KEY_PUSH;
		break;
	case WM_LBUTTONUP:
		m_CurrentMouseState.rgbButtons[0] = KEY_UP;
		break;
	case WM_RBUTTONDOWN:
		m_CurrentMouseState.rgbButtons[1] = KEY_PUSH;
		break;
	case WM_RBUTTONUP:
		m_CurrentMouseState.rgbButtons[1] = KEY_UP;
		break;
	case WM_MBUTTONDOWN:
		m_CurrentMouseState.rgbButtons[2] = KEY_PUSH;
		break;
	case WM_MBUTTONUP:
		m_CurrentMouseState.rgbButtons[2] = KEY_UP;
		break;

	}
}

DWORD xInput::Key(DWORD dwKey)
{
	return m_KeyState[dwKey];
}

xInput::xInput()
{
}


xInput::~xInput()
{
}
