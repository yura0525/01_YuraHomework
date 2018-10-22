#pragma once
#include "TStd.h"

/*
1) 1�������� ��� �ð� = m_fSecPerFrame
2) 1���� ������ ī����	= m_dwFPS
3) ������ ����� �����ð� = m_fGameTime
*/
class TTimer
{
private:
	
	DWORD	m_dwFrameCount;
	DWORD	m_dwFPS;			//2) 1���� ������ ī����
	DWORD	m_BeforeTick;
	
	float	m_fSecPerFrame;		//1) 1�������� ��� �ð�
	float	m_fGameTime;		//3) ������ ����� �����ð�
	float	m_fTimer;

	TCHAR	m_csBuffer[256];

public:
	bool Init();			//�ʱ�ȭ
	bool Frame();			//���
	bool Render();			//��ο�
	bool Release();			//�Ҹ�, ����

	TTimer();
	virtual ~TTimer();
};

