#pragma once
#include "xStd.h"
class xCore;

/*
1) 1�������� ��� �ð� = m_fSecPerFrame
2) 1���� ������ ī����	= m_dwFPS
3) ������ ����� �����ð� = m_fGameTime
*/
class TTimer
{
public:
	xCore * m_pOwner;					//Owner
	LARGE_INTEGER	m_Frequency;		// �ʴ� ���ļ� 
	LARGE_INTEGER	m_CurrentTime;		// ���� �ð� 
	LARGE_INTEGER	m_FPS;				//(FPS üũ Ÿ�̸�)
	LARGE_INTEGER	m_BeforeTick;		// ���� �������� �ð� üũ 
	LARGE_INTEGER	m_Start;			// �̺�Ʈ Ÿ�̸� ���� �ð�

	int				m_iFramePerSecond;			// 2) 1���� ������ ī����
	int				m_iFrameCount;				// �ʴ� ����� ������ 

	float			m_fSecPerFrame;				//1) 1�������� ��� �ð�
	float			m_fGameTime;				//3) ������ ����� �����ð�

	TCHAR	m_csBuffer[256];

	float	m_fNPCRegenStartTime;	//4)�̺�Ʈ ���� �ð�.(NPC����)
	float	m_fNPCRegenTimeGap;		//5)�̺�Ʈ ���� �ð�.
	float	m_fNPCRegenTimer;

	float	m_fGameStartTime;		//6)���� ���� �ð�.
	float	m_fGameTimeGap;			//7)���� ���� �ð�.

public:
	bool Init();			//�ʱ�ȭ
	bool Frame();			//���
	bool Render();			//��ο�
	bool Release();			//�Ҹ�, ����
public:
	void NPCRegenTime(float fNPCRegenTimeGap);
	void GameStartTime(float fGameTimeGap);
public:
	TTimer(xCore * m_pOwner);
	virtual ~TTimer();
};

