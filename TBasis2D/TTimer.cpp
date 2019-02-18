#include "TTimer.h"
#include "TBasisSys.h"
float    g_fTimer = 0.0f;
float    g_fSecPerFrame = 0.0f;

bool TTimer::Init()
{
	QueryPerformanceFrequency( &m_Frequency );//ÁÖÆÄ¼ö
	memset(& m_End, 0,sizeof( LARGE_INTEGER ));
	memset(& m_Start, 0,sizeof( LARGE_INTEGER ));
	QueryPerformanceCounter(&m_Start);
	return true;
}
bool TTimer::Frame()
{
	QueryPerformanceCounter(&m_End );

	if( m_Start.QuadPart != 0 )
	{
		m_fSecPerFrame = ( m_End.QuadPart - m_Start.QuadPart)/(float)( m_Frequency.QuadPart) ;
	}
	else
	{
		m_fSecPerFrame = 0.0f;
	}
	g_fSecPerFrame = m_fSecPerFrame;

	m_fAccumulation += m_fSecPerFrame;
	m_fFrameTime += m_fSecPerFrame;
	g_fTimer = m_fAccumulation;

	if( m_fFrameTime >= 1.0f  )
	{
		m_iFPSCnt = m_iTmpTime;
		m_iTmpTime = 0;
		m_fFrameTime = m_fFrameTime - 1.0f;
	}
	m_iTmpTime++;

	m_Start = m_End;
	return true;
}
bool TTimer::Render()
{
	memset( m_strBuffer, 0, sizeof( TCHAR) * MAX_PATH );
	_stprintf( m_strBuffer, _T("FPS:%d, %10.6f %10.6f"), 
		m_iFPSCnt, m_fSecPerFrame,m_fAccumulation );	
	return true;
}
bool TTimer::Release()
{
	return true;
}
TTimer::TTimer(void)
{
	m_iFPSCnt		= 0 ;
	m_iTmpTime	= 0;
	m_fSecPerFrame = 0.0f;
	m_fAccumulation = 0.0f;
	m_fFrameTime =0.0f;
}

TTimer::~TTimer(void)
{
}
