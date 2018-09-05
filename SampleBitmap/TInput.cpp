#include "TInput.h"

DWORD TInput::KeyCheck(DWORD dwKey)
{
	//GetKeyState()			//동기적
	//GetAsyncKeyState		//비동기적

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

	//1000 0000 0000 0000	=> 0x8000(이전 X ~ 지금  0) 이전 누른적 없고 지금은 눌러졌다.
	//1000 0000 0000 0001	=> 0x8001(이전 0 ~ 지금  0) 이전 눌러져있고 지금도 눌러져있다.
	//0000 0000 0000 0000	=> 0x0000(이전 X ~ 지금  X) 이전도 눌러져있지 않고 지금도 눌러져있지 않다. 
	//0000 0000 0000 0001	=> 0x0001(이전 0 ~ 지금  X) 이전 눌러져있고 지금은 눌러져있지 않다.

	if (sKey & 0x8000)
	{
		if (m_dwKeyState[dwKey] == KEY_FREE 
			|| m_dwKeyState[dwKey] == KEY_UP)
		{
			m_dwKeyState[dwKey] = KEY_PUSH;
		}
		else
		{
			m_dwKeyState[dwKey] = KEY_HOLD;
		}
	}
	else
	{
		if (m_dwKeyState[dwKey] == KEY_PUSH 
			|| m_dwKeyState[dwKey] == KEY_HOLD)
		{
			m_dwKeyState[dwKey] = KEY_UP;
		}
		else
		{
			m_dwKeyState[dwKey] = KEY_FREE;
		}
	}

	return m_dwKeyState[dwKey];
}

bool TInput::Init()				//초기화
{
	ZeroMemory(m_dwKeyState, sizeof(DWORD) * 256);
	ZeroMemory(m_dwMouseState, sizeof(DWORD) * 3);
	ZeroMemory(m_dwBeforeMouseState, sizeof(DWORD) * 3);
	return true;
}
bool TInput::Frame()			//계산
{
	GetCursorPos(&m_MousePos);				//화면 좌표
	ScreenToClient(g_hWnd, &m_MousePos);	//윈도우 좌표계를 클라이언트 좌표계로 변환

	for (int iKey = 0; iKey < 255; iKey++)
	{
		m_dwKeyState[iKey] = KeyCheck(iKey);
	}
	for (int iButton = 0; iButton < 3; iButton++)
	{
		if (m_dwBeforeMouseState[iButton] == KEY_PUSH)
		{
			if (m_dwMouseState[iButton] == KEY_PUSH)
			{
				m_dwMouseState[iButton] = KEY_HOLD;
			}
		}
		else if(m_dwBeforeMouseState[iButton] == KEY_UP)
		{
			if (m_dwMouseState[iButton] == KEY_UP)
			{
				m_dwMouseState[iButton] = KEY_FREE;
			}
		}
		m_dwBeforeMouseState[iButton] = m_dwMouseState[iButton];
	}
	return true;
}
bool TInput::Render()			//드로우
{
	//HDC hdc = GetDC(g_hWnd);

	//SetBkColor(hdc, RGB(255, 0, 0));
	//SetTextColor(hdc, RGB(0, 0, 255));
	////SetBkMode(hdc, TRANSPARENT);			//배경색을 투명하게 한다.


	////유니코드의 wcslen 글자수를 구하는 함수 strlen이랑 같음
	//TextOut(hdc, 0, 0, m_csBuffer, wcslen(m_csBuffer));
	//ReleaseDC(g_hWnd, hdc);

	return true;
}

DWORD TInput::Key(DWORD dwKey)
{
	return m_dwKeyState[dwKey];
}

bool TInput::Release()			//소멸, 삭제
{
	return true;
}
void TInput::MsgEvent(MSG msg)
{
	switch (msg.message)
	{
	case WM_LBUTTONDOWN:
		m_dwMouseState[0] = KEY_PUSH;
		break;
	case WM_LBUTTONUP:
		m_dwMouseState[0] = KEY_UP;
		break;
	case WM_RBUTTONDOWN:
		m_dwMouseState[1] = KEY_PUSH;
		break;
	case WM_RBUTTONUP:
		m_dwMouseState[1] = KEY_UP;
		break;
	case WM_MBUTTONDOWN:
		m_dwMouseState[2] = KEY_PUSH;
		break;
	case WM_MBUTTONUP:
		m_dwMouseState[2] = KEY_UP;
		break;

	}
}
TInput::TInput()
{
}


TInput::~TInput()
{
}
