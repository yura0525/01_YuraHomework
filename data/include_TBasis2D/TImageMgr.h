#pragma once
#include "TImage.h"

class TImageMgr : public TSingleton<TImageMgr>
{
private:
	friend class TSingleton<TImageMgr>;
public:
	typedef std::map<int,TImage*>::iterator TItor;
	std::map<int,TImage*>	m_Map;
	int						m_iCurIndex;
public:
	bool		Init();
	bool		Frame();
	bool		Render();
	bool		Release();
	int			Load( HWND hWnd, HDC hdc, TCHAR_STRING name  ); // 사운드 추가
	TImage*		GetPtr( int iIndex );
public:
	bool	InitImage( TImage* pImage, HWND hWnd, HDC hdc, UINT id );
	bool	InitImage( TImage* pImage, HWND hWnd, HDC hdc, int x, int y );
public:
	TImageMgr();
	~TImageMgr();
};
#define I_ImageMgr TImageMgr::GetInstance()
