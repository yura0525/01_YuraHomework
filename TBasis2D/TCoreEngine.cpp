#include "TCore.h"
#include "TInput.h"
#include "TBasisSys.h"
bool		TCore::TCoreInit()
{
	I_Input.Init();
	m_Time.Init();
	Init();
	return true;
}	
bool		TCore::TCoreFrame()
{
	I_Input.Frame();
	m_Time.Frame();
	Frame();
	return true;
}	
bool		TCore::TCoreRender()
{	
	Render();
	m_Time.Render();
	I_Input.Render();	
	return true;
}	
bool		TCore::TCoreRelease()
{
	I_Input.Release();
	m_Time.Release();
	Release();
	return true;
}	
LRESULT TCore::MsgProc( HWND hWnd,
						UINT msg,
						WPARAM wParam,
						LPARAM lParam )
{
	return -1;
}
LRESULT TCore::WndProc( HWND hWnd,
						UINT msg,
						WPARAM wParam,
						LPARAM lParam )
{
	LRESULT hr = MsgProc( hWnd, msg, wParam, lParam );
	if( hr >= 0 ) return hr;	
	switch( msg )
	{
		case WM_DESTROY:
			TCoreRelease();
			PostQuitMessage(0);
			return 0;
	}	
	return DefWindowProc( hWnd, msg, wParam, lParam );
}