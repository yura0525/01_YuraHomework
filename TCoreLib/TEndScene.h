#pragma once
#include "TScene.h"
#include "TButton.h"

class TEndScene : public TScene
{
public:
	TButton		m_BtnStart;

public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();

	bool IsDead()	{	return m_BtnStart.m_bDead;	}

public:
	TEndScene();
	virtual ~TEndScene();
};

