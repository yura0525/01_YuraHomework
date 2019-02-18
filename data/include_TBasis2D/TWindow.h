#ifndef TBASIS_WINDOW
#define TBASIS_WINDOW
#include "TBasisStd.h"
//class TBasis
//{
//	bool	Init() = 0;			// �ʱ�ȭ �۾�
//	bool	Frame() = 0;		// ���( ������ ���� )
//	bool	Render() = 0;	// ȭ�鿡 �׸���. �Ѹ���.
//	bool	Release() = 0;	// ����, �Ҹ�
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