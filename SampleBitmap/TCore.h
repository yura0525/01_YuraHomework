#pragma once
#include "TWindow.h"
#include "TTimer.h"

class TCore : public TWindow
{
public:
	//TBitmap m_bitmap;
	//float m_fSpeed;
	TTimer m_Timer;

	HDC		m_hScreenDC;
	HDC		m_hOffScreenDC;			//������۸��� ���� DC 2��
	HBITMAP m_hOffScreenBitmap;
	HBITMAP m_hOldOffBitmap;
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
	virtual void NPCRegenAlarm() { }
	virtual void GameEndTimeAlarm() { }

public:
	TCore();
	virtual ~TCore();
};

