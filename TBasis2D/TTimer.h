#pragma once
#include "TBasisStd.h"

class TTimer
{
public:
	// 64비트 정수( 1844경 )
	LARGE_INTEGER	m_Start;
	LARGE_INTEGER	m_End;
	LARGE_INTEGER	m_Frequency;

	int						m_iFPSCnt;
	float					m_fSecPerFrame;
	float					m_fAccumulation;
	int						m_iTmpTime;
	float					m_fFrameTime;

	TCHAR					m_strBuffer[MAX_PATH];
public:
	bool	Init();			// 초기화 작업
	bool	Frame();		// 계산( 프레임 단위 )
	bool	Render();	// 화면에 그린다. 뿌린다.
	bool	Release();	// 삭제, 소멸
public:
	TTimer(void);
	~TTimer(void);
};
