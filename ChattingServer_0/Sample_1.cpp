#include "Sample_1.h"


bool Sample_1::Init()
{
	xCore::Init();
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);

	m_Acceptor.Set(10000, "192.168.0.2");
	return true;
}
bool Sample_1::PreRender()
{
	return true;
}
bool Sample_1::Render()
{
	xCore::Render();
	return PostRender();
}
bool Sample_1::PostRender()
{
	return true;
}
bool Sample_1::Release()
{
	xCore::Release();
	m_Acceptor.Release();
	return true;
}
Sample_1::Sample_1()
{
}


Sample_1::~Sample_1()
{
}

INT WINAPI wWinMain(HINSTANCE hInst,
	HINSTANCE hPreInst,
	LPWSTR strCmdLine,
	INT nShow)
{
	Sample_1 sWin;
	sWin.SetWindow(hInst, L"Chatting Server", 800, 600);
	sWin.Run();
	return 0;
}
