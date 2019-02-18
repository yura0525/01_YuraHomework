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
	bool	Init();			// 초기화 작업
	bool	Frame();		// 계산( 프레임 단위 )
	bool	Render();	// 화면에 그린다. 뿌린다.
	bool	Release();	// 삭제, 소멸
	int		KeyCheck( DWORD dwCheckKey);
public:
	TInput(void);
	~TInput(void);
};

#define I_Input TInput::GetInstance()
