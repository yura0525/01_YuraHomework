#pragma once
#include "TScene.h"
#include "TButton.h"
class TLobbyScene : public TScene
{
public:
	TButton		m_BtnStart;
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();

	bool IsDead() { return m_BtnStart.m_bDead; }
public:
	TLobbyScene();
	virtual ~TLobbyScene();
};

