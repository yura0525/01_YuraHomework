#include "TTimer.h"
#include "xCore.h"

float g_fSecPerFrame = 0.0f;
float g_fGameTimer = 0.0f;
bool TTimer::Init()
{
	//이전 시간 저장.
	QueryPerformanceCounter(&m_BeforeTick);
	QueryPerformanceCounter(&m_Start);

	// 고해상도 타이머 지원여부 판단
	if (m_BeforeTick.QuadPart == 0)
	{
		return false;
	}

	return true;
}
bool TTimer::Frame()
{
	// 1프레임의 시간 체크 
	// 현재 시간을 찾는다. 
	QueryPerformanceCounter(&m_CurrentTime);

	m_fSecPerFrame = static_cast<float>(m_CurrentTime.QuadPart - m_BeforeTick.QuadPart) / static_cast<float>(m_Frequency.QuadPart);
	g_fSecPerFrame = m_fSecPerFrame;

	m_fGameTime += m_fSecPerFrame;
	g_fGameTimer = m_fGameTime;

	if (((m_CurrentTime.LowPart - m_FPS.LowPart) / m_Frequency.LowPart) >= 1)
	{
		m_iFramePerSecond = m_iFrameCount;
		m_iFrameCount = 0;
		m_FPS = m_CurrentTime;

		//출력창에 출력하는 함수 
		//유니코드일때 버퍼의 내용을 채우는 함수(_stprintf_s)
		_stprintf_s(m_csBuffer, L"FPS: %d, TIME : %10.4f SPF : %10.4f\n", m_iFramePerSecond, m_fGameTime, m_fSecPerFrame);
		OutputDebugString(m_csBuffer);
	}

	if ((0.0f < m_fNPCRegenTimeGap) && ((m_fNPCRegenStartTime + m_fNPCRegenTimeGap) < m_fGameTime))
	{
		m_pOwner->NPCRegenAlarm();
		m_fNPCRegenStartTime += m_fNPCRegenTimeGap;
	}

	if ((0.0f < m_fGameTimeGap) && ((m_fGameStartTime + m_fGameTimeGap) < m_fGameTime))
	{
		//이벤트를 알려주는 함수.
		m_pOwner->GameEndTimeAlarm();
		m_fGameStartTime += m_fGameTimeGap;
	}

	m_iFrameCount++;
	m_BeforeTick = m_CurrentTime;
	
	return true;
}
bool TTimer::Render()
{
	////HDC hdc = GetDC(g_hWnd);

	//assert(g_hOffScreenDC != NULL);

	//SetBkColor(g_hOffScreenDC, RGB(255, 0, 0));
	//SetTextColor(g_hOffScreenDC, RGB(0, 0, 255));
	////SetBkMode(hdc, TRANSPARENT);			//배경색을 투명하게 한다.


	////유니코드의 wcslen 글자수를 구하는 함수 strlen이랑 같음
	//TextOut(g_hOffScreenDC, 0,0, m_csBuffer, wcslen(m_csBuffer));
	////ReleaseDC(g_hWnd, hdc);

	return true;
}
bool TTimer::Release()
{
	return true;
}

void TTimer::NPCRegenTime(float fNPCRegenTimeGap)
{
	m_fNPCRegenStartTime = 0.0f;
	m_fNPCRegenTimeGap = fNPCRegenTimeGap;
}

void TTimer::GameStartTime(float fGameTimeGap)
{
	m_fGameStartTime = 0.0f;
	m_fGameTimeGap = fGameTimeGap;
}

TTimer::TTimer(xCore * pOwner)
{
	m_pOwner = pOwner;
	m_iFramePerSecond = 0;
	m_iFrameCount = 0;
	m_fSecPerFrame = 0.0f;
	m_fGameTime = 0.0f;		//1) 1프레임의 경과 시간

	// 시스템의 주파수 변동폭을 얻어 온다. 이는 시간의 표준을 잡기 위해서 사용한다.
	QueryPerformanceFrequency((LARGE_INTEGER*)&m_Frequency);
}


TTimer::~TTimer()
{
}
