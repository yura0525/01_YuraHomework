#pragma once
#include "TWindow.h"
#include "TTimer.h"

class TCore : public TWindow
{
public:	
		//TInput				m_Input;
		TTimer				m_Time;
public:
		bool				TCoreInit();
		bool				TCoreFrame();
		bool				TCoreRender();
		bool				TCoreRelease();
		LRESULT				WndProc(	HWND hWnd,
										UINT msg,
										WPARAM wParam,
										LPARAM lParam );

		virtual bool		Init();
		virtual bool		Frame();
		virtual bool		Render();
		virtual bool		Release();
		virtual bool		Run();
		virtual bool		SetWindow(HINSTANCE hInst,
									int iWidth = 800,
									int iHeight = 600,
									const TCHAR* strTitleName = L"TBasis2D",
									DWORD dwStyle=WS_OVERLAPPEDWINDOW);
		virtual LRESULT			MsgProc(	HWND hWnd,
										UINT msg,
										WPARAM wParam,
										LPARAM lParam );
public:
	TCore(void);
	~TCore(void);
};
