#pragma once
#include "xCore.h"
#include "TAsyncSelect.h"

class Sample : public xCore
{
public:
	TAsyncSelect		m_Network;
	HWND				m_hEdit;
	HWND				m_hButton;
	HWND				m_hList;
public:
	bool	PostRender()	{	return true;	}
	bool	Init();
	bool	Frame();
	bool	Release();
	LRESULT	MsgProc(HWND hWnd, UINT msg, WPARAM	wParam, LPARAM	lParam);
	void	MsgEvent(MSG msg);
public:
	Sample();
	~Sample();
};
