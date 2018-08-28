#include "TTimer.h"

float g_fSecPerFrame = 0.0f;

bool TTimer::Init()
{
	//__imp_ timeGetTime() 링커의 입력 추가종속성에 추가 : winmm.lib
	m_dwBeforeTick = timeGetTime();				//1 / 1000초의 정밀도

	return true;
}
bool TTimer::Frame()
{
	DWORD dwCurrentTick = timeGetTime();

	DWORD dwElapseTick = dwCurrentTick - m_dwBeforeTick;
	m_fSecPerFrame = dwElapseTick / 1000.0f;
	g_fSecPerFrame = m_fSecPerFrame;

	m_fGameTime += m_fSecPerFrame;

	m_fTimer += m_fSecPerFrame;
	if (m_fTimer >= 1.0f)
	{
		m_dwFPS = m_dwFrameCount;
		m_dwFrameCount = 0;
		m_fTimer -= 1.0f;

		//출력창에 출력하는 함수 
		//유니코드일때 버퍼의 내용을 채우는 함수(_stprintf_s)
		//_stprintf_s(m_csBuffer, L"FPS: %d, TIME : %10.4f SPF : %10.4f", m_dwFPS, m_fGameTime, m_fSecPerFrame);
		//OutputDebugString(m_csBuffer);
	}

	m_dwFrameCount++;

	m_dwBeforeTick = dwCurrentTick;
	return true;
}
bool TTimer::Render()
{
	//HDC hdc = GetDC(g_hWnd);

	assert(g_hOffScreenDC != NULL);

	SetBkColor(g_hOffScreenDC, RGB(255, 0, 0));
	SetTextColor(g_hOffScreenDC, RGB(0, 0, 255));
	//SetBkMode(hdc, TRANSPARENT);			//배경색을 투명하게 한다.


	//유니코드의 wcslen 글자수를 구하는 함수 strlen이랑 같음
	TextOut(g_hOffScreenDC, 0,0, m_csBuffer, wcslen(m_csBuffer));
	//ReleaseDC(g_hWnd, hdc);

	return true;
}
bool TTimer::Release()
{
	return true;
}


TTimer::TTimer()
{
	m_fGameTime = 0.0f;
	m_dwFrameCount = 0;
	m_dwFPS = 0;
	m_dwBeforeTick = 0;
	m_fSecPerFrame =0.0f;		//1) 1프레임의 경과 시간
	m_fTimer = 0.0f;
}


TTimer::~TTimer()
{
}
