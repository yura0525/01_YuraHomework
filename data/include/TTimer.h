#pragma once
#include "TStd.h"

/*
1) 1프레임의 경과 시간 = m_fSecPerFrame
2) 1초의 프레임 카운터	= m_dwFPS
3) 게임이 실행된 누적시간 = m_fGameTime
*/
class TTimer
{
private:
	
	DWORD	m_dwFrameCount;
	DWORD	m_dwFPS;			//2) 1초의 프레임 카운터
	DWORD	m_BeforeTick;
	
	float	m_fSecPerFrame;		//1) 1프레임의 경과 시간
	float	m_fGameTime;		//3) 게임이 실행된 누적시간
	float	m_fTimer;

	TCHAR	m_csBuffer[256];

public:
	bool Init();			//초기화
	bool Frame();			//계산
	bool Render();			//드로우
	bool Release();			//소멸, 삭제

	TTimer();
	virtual ~TTimer();
};

