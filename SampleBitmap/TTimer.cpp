#include "TTimer.h"
#include "TCore.h"

float g_fSecPerFrame = 0.0f;
float g_fGameTime = 0.0f;
bool TTimer::Init()
{
	//__imp_ timeGetTime() ��Ŀ�� �Է� �߰����Ӽ��� �߰� : winmm.lib
	m_dwBeforeTick = timeGetTime();				//1 / 1000���� ���е�

	return true;
}
bool TTimer::Frame()
{
	DWORD dwCurrentTick = timeGetTime();

	DWORD dwElapseTick = dwCurrentTick - m_dwBeforeTick;
	m_fSecPerFrame = dwElapseTick / 1000.0f;
	g_fSecPerFrame = m_fSecPerFrame;

	m_fGameTime += m_fSecPerFrame;
	g_fGameTime = m_fGameTime;

	m_fTimer += m_fSecPerFrame;
	if (m_fTimer >= 1.0f)
	{
		m_dwFPS = m_dwFrameCount;
		m_dwFrameCount = 0;
		m_fTimer -= 1.0f;

		//���â�� ����ϴ� �Լ� 
		//�����ڵ��϶� ������ ������ ä��� �Լ�(_stprintf_s)
		/*TCHAR	m_csBuffer[256];
		_stprintf_s(m_csBuffer, L"FPS: %d, TIME : %10.4f SPF : %10.4f", m_dwFPS, m_fGameTime, m_fSecPerFrame);
		OutputDebugString(m_csBuffer);*/
	}

	if ((0.0f < m_fNPCRegenTimeGap) && ((m_fNPCRegenStartTime + m_fNPCRegenTimeGap) < m_fGameTime) )
	{
		m_pOwner->NPCRegenAlarm();
		m_fNPCRegenStartTime += m_fNPCRegenTimeGap;
	}

	if ((0.0f < m_fGameTimeGap) && ((m_fGameStartTime + m_fGameTimeGap) < m_fGameTime))
	{
		//�̺�Ʈ�� �˷��ִ� �Լ�.
		m_pOwner->GameEndTimeAlarm();
		m_fGameStartTime += m_fGameTimeGap;
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
	//SetBkMode(hdc, TRANSPARENT);			//������ �����ϰ� �Ѵ�.


	//�����ڵ��� wcslen ���ڼ��� ���ϴ� �Լ� strlen�̶� ����
	//TextOut(g_hOffScreenDC, 0,0, m_csBuffer, wcslen(m_csBuffer));
	//ReleaseDC(g_hWnd, hdc);

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

TTimer::TTimer(TCore* pOwner)
{
	m_pOwner = pOwner;

	m_fGameTime = 0.0f;
	m_dwFrameCount = 0;
	m_dwFPS = 0;
	m_dwBeforeTick = 0;
	m_fSecPerFrame =0.0f;		//1) 1�������� ��� �ð�
	m_fTimer = 0.0f;

	m_fNPCRegenStartTime = 0.0f;
	m_fNPCRegenTimeGap = 0.0f;
	m_fGameStartTime = 0.0f;
	m_fGameTimeGap = 0.0f;
}


TTimer::~TTimer()
{
}
