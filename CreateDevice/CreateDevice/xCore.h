#pragma once
#include "xWindow.h"
#include "TTimer.h"
#include "TDirectWrite.h"
#include "xInput.h"

struct TOutput								//������� ������ ������ �˼��ִ�.
{
	DXGI_OUTPUT_DESC				desc;
	std::vector<DXGI_MODE_DESC>		m_pModeList;
	std::vector<IDXGIOutput*>		m_pOutputList;
};

struct TAdapter									//�׷���ī��
{
	IDXGIAdapter*				m_pAdapter;
	DXGI_ADAPTER_DESC			desc;
	std::vector<TOutput>		m_OutputList;	//�׷���ī�忡 ����� �����
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

public:
	xCore();
	virtual ~xCore();
};

