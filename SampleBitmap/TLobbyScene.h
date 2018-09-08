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

	bool IsDead() { return m_BtnStart.IsDead(); }
public:
	TLobbyScene();
	virtual ~TLobbyScene();
};

