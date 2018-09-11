#include "xCore.h"

bool xCore::GameInit()
{
	GamePreInit();
	Init();

	return true;
}

bool xCore::GameFrame()
{
	Frame();

	return true;
}

bool xCore::GameRender()
{
	GamePreRender();
	{
		PreRender();
		Render();
		PostRender();
	}
	GamePostRender();
	return true;
}

bool xCore::GamePreRender()
{
	return true;
}
bool xCore::GamePostRender()
{
	return true;
}

bool xCore::GameRun()
{
	GameFrame();
	GameRender();

	return true;
}
bool xCore::GameRelease()
{
	Release();
	return true;
}

bool xCore::Init()
{
	TDevice::Init();
	m_Timer.Init();
	return true;
}
bool xCore::Frame()
{
	TDevice::Frame();
	m_Timer.Frame();
	return true;
}
bool xCore::Render()
{
	TDevice::Render();
	m_Timer.Render();
	return true;
}
bool xCore::Release()
{
	TDevice::Release();
	m_Timer.Release();
	return true;
}
xCore::xCore()
{
}


xCore::~xCore()
{
}
