#pragma once
#include "xStd.h"

/*
1) 1�������� ��� �ð� = m_fSecPerFrame
2) 1���� ������ ī����	= m_dwFPS
3) ������ ����� �����ð� = m_fGameTime
*/
class TTimer
{
private:
	LARGE_INTEGER	m_Frequency;			//�ʴ� ���ļ�
	LARGE_INTEGER	m_CurrentTime;			//���� �ð�
	LARGE_INTEGER	m_FPS;					//FPSüũ Ÿ�̸�
	LARGE_INTEGER	m_BeforeTick;			//���� �������� �ð� üũ
	LARGE_INTEGER	m_Start;				//�̺�Ʈ Ÿ�̸� ���۽ð�

	DWORD			m_iFramePerSecond;			//2) 1���� ������ ī����
	DWORD			m_iFrameCount;
	
	float	m_fSecPerFrame;		//1) 1�������� ��� �ð�
	float	m_fGameTime;		//3) ������ ����� �����ð�

public:
	bool Init();			//�ʱ�ȭ
	bool Frame();			//���
	bool Render();			//��ο�
	bool Release();			//�Ҹ�, ����

	TTimer();
	virtual ~TTimer();
};

