#pragma once
#include "TBasisStd.h"
struct TInputState
{
	DWORD dwkeyW;
	DWORD dwkeyA;
	DWORD dwkeyS;
	DWORD dwkeyD;
	DWORD dwSpace;
};
enum KeyState{
	KEY_FREE = 0,
	KEY_PUSH,
	KEY_UP,
	KEY_HOLD,
};

class TInput : public TSingleton<TInput>
{
private:
	friend class TSingleton<TInput>;
public:
	TInputState		m_InputState;
	DWORD			m_dwKeyState[256];
public:
	bool	Init();			// �ʱ�ȭ �۾�
	bool	Frame();		// ���( ������ ���� )
	bool	Render();	// ȭ�鿡 �׸���. �Ѹ���.
	bool	Release();	// ����, �Ҹ�
	int		KeyCheck( DWORD dwCheckKey);
public:
	TInput(void);
	~TInput(void);
};

#define I_Input TInput::GetInstance()
