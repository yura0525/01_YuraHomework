#pragma once
#include "xWindow.h"
#include "TTimer.h"
#include "TDirectWrite.h"
#include "xInput.h"

struct TOutput								//모니터의 정보와 갯수를 알수있다.
{
	DXGI_OUTPUT_DESC				desc;
	std::vector<DXGI_MODE_DESC>		m_pModeList;
	std::vector<IDXGIOutput*>		m_pOutputList;
};

struct TAdapter									//그래픽카드
{
	IDXGIAdapter*				m_pAdapter;
	DXGI_ADAPTER_DESC			desc;
	std::vector<TOutput>		m_OutputList;	//그래픽카드에 연결된 모니터
};


class xCore : public xWindow
{
public:
	TTimer			m_Timer;
	TDirectWrite	m_Font;
	xInput			m_Input;
public:
	void DeleteDeviceResources(UINT iWidth, UINT iHeight);
	HRESULT CreateDeviceResources(UINT iWidth, UINT iHeight);
public:
	bool GamePreInit();
	bool GameInit();
	bool GameRun();
	bool GameRelease();

	bool GameFrame();
	bool GameRender();
	bool GamePreRender();
	bool GamePostRender();

public:
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();

	virtual void NPCRegenAlarm() { }
	virtual void GameEndTimeAlarm() { }
public:
	xCore();
	virtual ~xCore();
};

