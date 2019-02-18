#pragma once
#include "TBasisStd.h"

class TTimer
{
public:
	// 64��Ʈ ����( 1844�� )
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
	bool	Init();			// �ʱ�ȭ �۾�
	bool	Frame();		// ���( ������ ���� )
	bool	Render();	// ȭ�鿡 �׸���. �Ѹ���.
	bool	Release();	// ����, �Ҹ�
public:
	TTimer(void);
	~TTimer(void);
};
