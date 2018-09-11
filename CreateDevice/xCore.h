#pragma once
#include "xWindow.h"

class xCore : public xWindow
{
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
	xCore();
	virtual ~xCore();
};

