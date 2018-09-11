#include "TTimer.h"

float g_fSecPerFrame = 0.0f;
float g_fGameTime = 0.0f;

bool TTimer::Init()
{
	m_iFramePerSecond = 0.0f;
	m_iFrameCount = 0;
	m_fSecPerFrame = 0.0f;		//1) 1프레임의 경과 시간
	m_fGameTime = 0.0f;

	//이전 시간 저장.
	QueryPerformanceCounter(&m_BeforeTick);
	QueryPerformanceCounter(&m_Start);

	//고해상도 타이머 지원여부 판단.
	if (m_BeforeTick.QuadPart == 0)
		return false;
	return true;
}
bool TTimer::Frame()
{
	QueryPerformanceCounter(&m_CurrentTime);

	m_fSecPerFrame = static_cast<float>(m_CurrentTime.QuadPart - m_BeforeTick.QuadPart) / static_cast<float>(m_Frequency.QuadPart);
	g_fSecPerFrame = m_fSecPerFrame;

	m_fGameTime += m_fSecPerFrame;
	g_fGameTime = m_fGameTime;

	if( ((m_CurrentTime.LowPart - m_FPS.LowPart) / m_Frequency.LowPart) >= 1 )
	{
		m_iFramePerSecond = m_iFrameCount;
		m_iFrameCount = 0;
		m_FPS = m_CurrentTime;

		//출력창에 출력하는 함수 
		//유니코드일때 버퍼의 내용을 채우는 함수(_stprintf_s)
		TCHAR	csBuffer[256];
		_stprintf_s(csBuffer, L"FPS: %d, TIME : %10.4f SPF : %10.4f\n", m_iFramePerSecond, m_fGameTime, m_fSecPerFrame);
		OutputDebugString(csBuffer);
	}

	m_iFrameCount++;
	m_BeforeTick = m_CurrentTime;

	return true;
}
bool TTimer::Render()
{
	//HDC hdc = GetDC(g_hWnd);

	//assert(g_hOffScreenDC != NULL);

	//SetBkColor(g_hOffScreenDC, RGB(255, 0, 0));
	//SetTextColor(g_hOffScreenDC, RGB(0, 0, 255));
	////SetBkMode(hdc, TRANSPARENT);			//배경색을 투명하게 한다.


	////유니코드의 wcslen 글자수를 구하는 함수 strlen이랑 같음
	//TextOut(g_hOffScreenDC, 0,0, m_csBuffer, wcslen(m_csBuffer));
	//ReleaseDC(g_hWnd, hdc);

	return true;
}
bool TTimer::Release()
{
	return true;
}


TTimer::TTimer()
{
	m_iFramePerSecond	= 0.0f;
	m_iFrameCount		= 0;
	m_fSecPerFrame		=0.0f;		//1) 1프레임의 경과 시간
	m_fGameTime			= 0.0f;

	QueryPerformanceFrequency((LARGE_INTEGER*)&m_Frequency);
}


TTimer::~TTimer()
{
}
