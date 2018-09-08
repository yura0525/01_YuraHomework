#pragma once
#include "TStd.h"
/*
1) 1�������� ��� �ð� = m_fSecPerFrame
2) 1���� ������ ī����	= m_dwFPS
3) ������ ����� �����ð� = m_fGameTime
*/
class TCore;
class TTimer
{
private:
	TCore * m_pOwner;

	DWORD	m_dwFrameCount;
	DWORD	m_dwFPS;			//2) 1���� ������ ī����
	DWORD	m_dwBeforeTick;
	
	float	m_fSecPerFrame;		//1) 1�������� ��� �ð�
	float	m_fGameTime;		//3) ������ ����� �����ð�
	float	m_fTimer;

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

	void NPCRegenTime(float fNPCRegenTimeGap);
	void GameStartTime(float fGameTimeGap);

	TTimer(TCore* pOwner);
	virtual ~TTimer();
};

