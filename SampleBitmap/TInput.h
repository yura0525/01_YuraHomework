#pragma once
#include "Tstd.h"

enum KeyState
{
	KEY_FREE = 0,
	KEY_PUSH,
	KEY_UP,
	KEY_HOLD,
};

class TInput : public TSingleton<TInput>
{
private:
	friend class TSingleton<TInput>;

	DWORD m_dwKeyState[256];
	DWORD m_dwBeforeMouseState[3];

public:
	DWORD m_dwMouseState[3];
	POINT m_MousePos;
	DWORD KeyCheck(DWORD dwKey);
public:
	
	DWORD Key(DWORD dwKey);
	void MsgEvent(MSG msg);

	bool Init();			//�ʱ�ȭ
	bool Frame();			//���
	bool Render();			//��ο�
	bool Release();			//�Ҹ�, ����

protected:
	TInput();
public:
	virtual ~TInput();
};

#define I_Input TInput::GetInstance()
