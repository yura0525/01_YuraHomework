#pragma once

#include "TStd.h"

class TBitmap
{
public:
	T_STR		m_szName;
	HBITMAP		m_hBitmap;

	HDC			m_hMemDC;
	BITMAP		m_BmpInfo;
	//TPoint		m_Position;

public:

	bool Init();			//�ʱ�ȭ
	bool Frame();			//���
	bool Render();			//��ο�
	bool Release();			//�Ҹ�, ����
	//void SetPosition(float xPos, float yPos);
	//void SetOffeSet(float xPos, float yPos);

	int Load(T_STR szLoadFile);

	TBitmap();
	virtual ~TBitmap();
};

