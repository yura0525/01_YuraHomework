#include "TInput.h"

bool TInput::Init()
{
	memset(&m_dwKeyState, 0, sizeof(DWORD) * 256 );
	return true;
}
int TInput::KeyCheck( DWORD dwKey)
{
	return m_dwKeyState[dwKey];
}
bool TInput::Frame()
{
	for (DWORD dwKey = 0; dwKey < 256; dwKey++)
	{
		SHORT sKey = GetAsyncKeyState(dwKey);
		// 0x8000 =  10000000 00000000
		if (sKey & 0x8000)
		{
			if (m_dwKeyState[dwKey] == KEY_FREE ||
				m_dwKeyState[dwKey] == KEY_UP)
				m_dwKeyState[dwKey] = KEY_PUSH;
			else
				m_dwKeyState[dwKey] = KEY_HOLD;
		}
		else
		{
			if (m_dwKeyState[dwKey] == KEY_PUSH ||
				m_dwKeyState[dwKey] == KEY_HOLD)
				m_dwKeyState[dwKey] = KEY_UP;
			else
				m_dwKeyState[dwKey] = KEY_FREE;
		}
	}
	return true;
}

bool TInput::Render()
{
	return true;
}
bool TInput::Release()
{
	return true;
}
TInput::TInput(void)
{
}

TInput::~TInput(void)
{
}
