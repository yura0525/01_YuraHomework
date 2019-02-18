#ifndef TBASIS_WINDOW
#define TBASIS_WINDOW
#include "TBasisStd.h"
//class TBasis
//{
//	bool	Init() = 0;			// 초기화 작업
//	bool	Frame() = 0;		// 계산( 프레임 단위 )
//	bool	Render() = 0;	// 화면에 그린다. 뿌린다.
//	bool	Release() = 0;	// 삭제, 소멸
//};
namespace TBASIS2D_CORE 	{
	class TWindow 
	{
	public:
		HINSTANCE			m_hInst;
		WNDCLASSEX			m_wc; 
		RECT				m_rtWindow;
		RECT				m_rtClient;
		INT					m_BackGrougdColor;
		int						m_iWidth;
		int						m_iHeight;
		virtual void			SetBackground( INT iColor )
		{
			m_BackGrougdColor = iColor;
			m_wc.hbrBackground	= (HBRUSH)GetStockObject(m_BackGrougdColor);		
		}
		void		CenterWindow();
	public:
		virtual LRESULT WndProc(	HWND hWnd,
							 	UINT msg,
								WPARAM wParam,
								LPARAM lParam);
		bool		Init();
		bool		Frame();
		bool		Render();
		bool		Release();
		//virtual bool		SetWindow(HINSTANCE hInst,
		//							int iWidth = 800,
		//							int iHeight = 600,
		//							TCHAR* strTitleName = L"AAA" );
	public:
		TWindow(void);
		~TWindow(void);
	};
}

using namespace TBASIS2D_CORE;
#endif