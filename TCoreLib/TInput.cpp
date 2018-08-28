#include "TInput.h"

DWORD TInput::KeyCheck(DWORD dwKey)
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

bool TInput::Init()				//�ʱ�ȭ
{
	ZeroMemory(m_dwKeyState, sizeof(DWORD) * 256);
	ZeroMemory(m_dwMouseState, sizeof(DWORD) * 3);
	ZeroMemory(m_dwBeforeMouseState, sizeof(DWORD) * 3);
	return true;
}
bool TInput::Frame()			//���
{
	GetCursorPos(&m_MousePos);				//ȭ�� ��ǥ
	ScreenToClient(g_hWnd, &m_MousePos);	//������ ��ǥ�踦 Ŭ���̾�Ʈ ��ǥ��� ��ȯ

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
bool TInput::Render()			//��ο�
{
	//HDC hdc = GetDC(g_hWnd);

	//SetBkColor(hdc, RGB(255, 0, 0));
	//SetTextColor(hdc, RGB(0, 0, 255));
	////SetBkMode(hdc, TRANSPARENT);			//������ �����ϰ� �Ѵ�.


	////�����ڵ��� wcslen ���ڼ��� ���ϴ� �Լ� strlen�̶� ����
	//TextOut(hdc, 0, 0, m_csBuffer, wcslen(m_csBuffer));
	//ReleaseDC(g_hWnd, hdc);

	return true;
}

DWORD TInput::Key(DWORD dwKey)
{
	return m_dwKeyState[dwKey];
}

bool TInput::Release()			//�Ҹ�, ����
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
