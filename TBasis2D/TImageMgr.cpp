#include "TImageMgr.h"
#include "TBasisSys.h"

bool TImageMgr::Init()
{
	return true;
}
bool TImageMgr::Frame()
{
	TImage* pImage;
	for(TItor itor = m_Map.begin();
		itor != m_Map.end();
		itor++ )
	{
		pImage = (TImage*)(*itor).second;
		pImage->Frame();
	}
	return true;
}
bool TImageMgr::Render()
{
	TImage* pImage;
	for(TItor itor = m_Map.begin();
		itor != m_Map.end();
		itor++ )
	{
		pImage = (TImage*)(*itor).second;
		pImage->Render();
	}
	return true;
};
bool TImageMgr::Release()
{
	TImage* pImage;
	for(TItor itor = m_Map.begin();
		itor != m_Map.end();
		itor++ )
	{
		pImage = (TImage*)(*itor).second;
		pImage->Release();
		delete pImage;
	}
	return true;
}
int TImageMgr::Load( HWND hWnd, HDC hdc, T_STR pFileName  )
{
	// 중복제거
	TCHAR szFileName[MAX_PATH] = {0,};
	TCHAR Dirve[MAX_PATH] = {0,};
	TCHAR Dir[MAX_PATH] = {0,};
	TCHAR FileName[MAX_PATH] = {0,};
	TCHAR FileExt[MAX_PATH] = {0,};
	if( !pFileName.empty() )
	{
		_tsplitpath( pFileName.c_str(), Dirve, Dir, FileName, FileExt );
		_stprintf_s( szFileName, _T("%s%s"),FileName, FileExt );
		TImage* pImage;
		for(TItor itor = m_Map.begin();
			itor != m_Map.end();
			itor++ )
		{
			pImage = (TImage*)(*itor).second;
			if( !_tcsicmp( pImage->m_strName.c_str(), szFileName ) )
			{
				return (*itor).first;
			}
		}
	}

	TImage* pImage = new TImage;
	pImage->Init();
	pImage->Load(hWnd, hdc, pFileName );
	pImage->m_iIndex = ++m_iCurIndex;
	pImage->m_strName = szFileName;
	// std::map에 추가 1
	m_Map.insert( make_pair(m_iCurIndex, pImage) );
	return m_iCurIndex;
}
TImage* TImageMgr::GetPtr( int iIndex )
{
	TItor itor;
	itor	= m_Map.find( iIndex );
	if( itor == m_Map.end() ) return NULL;
	TImage* pImage = (*itor).second;
	return pImage;
}

bool TImageMgr::InitImage( TImage* pImage, HWND hWnd, HDC hdc, UINT id )
{
	pImage->hDC = CreateCompatibleDC( hdc );
	pImage->hBitmap = LoadBitmap(  g_hInstance,  MAKEINTRESOURCE(id) );
	pImage->hOldBitmap = (HBITMAP)SelectObject( pImage->hDC, pImage->hBitmap );
	return true;
}
bool TImageMgr::InitImage( TImage* pImage, HWND hWnd, HDC hdc, 
								int iX, int iY )
{
	pImage->hDC = CreateCompatibleDC( hdc );
	pImage->hBitmap = CreateCompatibleBitmap( hdc, iX, iY );
	pImage->hOldBitmap = (HBITMAP)SelectObject( pImage->hDC, pImage->hBitmap );
	return true;
}
TImageMgr::TImageMgr()
{
	m_iCurIndex = 0;
}
TImageMgr::~TImageMgr()
{
	Release();
}
