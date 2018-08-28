#pragma once
#include "TWindow.h"
#include "TTimer.h"
//#include "TInput.h"

class TCore : public TWindow
{
public:
	//TBitmap m_bitmap;
	//float m_fSpeed;
	TTimer m_Timer;

	HDC		m_hScreenDC;
	HDC		m_hOffScreenDC;			//더블버퍼링을 위한 DC 2개
	HBITMAP m_hOffScreenBitmap;
	HBRUSH	m_hbrBack;

public:
	bool GameInit();
	bool GameRun();
	bool GameRelease();

	bool GameFrame();
	bool GameRender();
	bool GamePreRender();
	bool GamePostRender();
public:
	virtual bool Init() { return true; }
	virtual bool Frame() { return true; }
	virtual bool Render() { return true; }
	virtual bool Release() { return true; }
	
public:
	TCore();
	virtual ~TCore();
};

