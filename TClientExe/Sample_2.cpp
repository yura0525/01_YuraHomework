#include "Sample_2.h"

bool Sample_2::Init()
{
	xCore::Init();
	return true;
}
bool Sample_2::PreRender()
{
	return true;
}
bool Sample_2::Render()
{
	return true;
}
bool Sample_2::PostRender()
{
	return true;
}
bool Sample_2::Frame()
{
	xCore::Frame();
	return true;
}
bool Sample_2::Release()
{
	xCore::Release();
	return true;
}

Sample_2::Sample_2()
{
	m_bLogin = false;
	m_iSerIndex = 0;
}


Sample_2::~Sample_2()
{
}

INT WINAPI wWinMain(HINSTANCE hInst,
	HINSTANCE hPreInst,
	LPWSTR strCmdLine,
	INT nShow)
{
	Sample_2 sWin;
	sWin.SetWindow(hInst, L"SampleObject_0", 800, 600);
	return 0;
}