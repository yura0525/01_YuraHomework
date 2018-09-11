#pragma once
#include "xWindow.h"
#include "TTimer.h"
class xCore : public xWindow
{
public:
	TTimer		m_Timer;

public:
	bool GameInit();
	bool GameRun();
	bool GameRelease();

	bool GameFrame();
	bool GameRender();
	bool GamePreRender();
	bool GamePostRender();

	virtual bool GamePreInit()
	{
		return true;
	}
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();

public:
	xCore();
	virtual ~xCore();
};

