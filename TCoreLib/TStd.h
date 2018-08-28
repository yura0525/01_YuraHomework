#pragma once

#include <windows.h>
#include <assert.h>
#include <tchar.h>

#include <iostream>
#include <map>
#include <string>
#include <list>

//������Ʈ �Ӽ����� �߰� ���Ӽ��� ������ �ڵ�� ������ �ִ�.
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "TCoreLib.lib")

typedef std::basic_string <char>	C_STR;	//multi-byte
typedef std::basic_string <wchar_t>	W_STR;	//unicode byte
typedef std::basic_string <TCHAR>	T_STR;	//������Ʈ�Ӽ��� ��Ƽ����Ʈ�̸� ��Ƽ����Ʈ, �����ڵ��̸� �����ڵ�� �ٲ�.

//string a;		//multi-byte
//wstring b;	//unicode byte

#define GAMESTART int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPWSTR lpCmdLine, int nCmdShow) {
//
////L#s ==> L"" s�� ���ڿ��̹Ƿ� #�� ���δ�. �����ڵ�� L�� ���δ�.
//#define GAMERUN(s) TWindow win; win.SetWindow(hInstance, L#s, 600, 800); win.Run();
//
#define GAME_END }


//L#s ==> L"" s�� ���ڿ��̹Ƿ� #�� ���δ�. �����ڵ�� L�� ���δ�.
#define GAMERUN(s, w, h) int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPWSTR lpCmdLine, int nCmdShow) { Sample win; win.SetWindow(hInst, L#s, w, h); win.Run(); }

extern HWND			g_hWnd;
extern HINSTANCE	g_hInstance;
extern float		g_fSecPerFrame;
extern HDC			g_hScreenDC;
extern HDC			g_hOffScreenDC;
extern RECT			g_rtClient;

template<class T> class TSingleton
{
public:
	static T& GetInstance()
	{
		static T theSingletonInstance;
		return theSingletonInstance;
	}
};