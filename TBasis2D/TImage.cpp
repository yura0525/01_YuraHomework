#include "TImage.h"
#include "TBasisSys.h"

HBITMAP TImage::GetRotatedBitmap(float fAngle, int iSizeWidth, int iSizeHeight, RECT rt)
{	
	// 제 2의 DC 결과물이 hbmResult;
	HDC m_hRotationDC  = CreateCompatibleDC(hDC);
	HBITMAP hbmResult = CreateCompatibleBitmap( hDC, iSizeWidth, iSizeHeight);
	HBITMAP hbmOldDest = (HBITMAP)SelectObject(	m_hRotationDC, hbmResult ); 
	// m_hRotationDC 클리어
	COLORREF bkColor = RGB( 255,255,255);
	HBRUSH hbrBack = CreateSolidBrush( bkColor );
	HBRUSH hbrOld = ( HBRUSH) SelectObject( m_hRotationDC, hbrBack );	
	PatBlt( m_hRotationDC, 0, 0, iSizeWidth, iSizeHeight, PATCOPY );
	DeleteObject( SelectObject( m_hRotationDC, hbrOld ) );
	// 월드 좌표계 변환
	SetGraphicsMode( m_hRotationDC, GM_ADVANCED );
	float fRadian = fAngle * 3.141592f / 180.0f;
	float fCosine = cos(fRadian);
	float fSine   = sin(fRadian);
	XFORM xform;
	xform.eM11 = fCosine;
	xform.eM12 = -fSine;
	xform.eM21 = fSine;
	xform.eM22 = fCosine;
	xform.eDx = (float)iSizeWidth/ 2.0f;
	xform.eDy = (float)iSizeHeight/ 2.0f;
	SetWorldTransform( m_hRotationDC, &xform );	
	BitBlt( m_hRotationDC, 
			-(rt.right/2.0f), -(rt.bottom/2.0f), rt.right, rt.bottom,		
			 hDC, rt.left, rt.top, SRCCOPY );
	SelectObject( m_hRotationDC, hbmOldDest );
	DeleteObject( m_hRotationDC );
	return hbmResult;
}
bool TImage::Load( HWND hWnd, HDC hdc, T_STR name )
{
	hDC = CreateCompatibleDC( hdc );
	hBitmap = (HBITMAP)LoadImage(
		g_hInstance,  name.c_str(),
		IMAGE_BITMAP,0,0,LR_DEFAULTSIZE | LR_LOADFROMFILE );

	hOldBitmap = (HBITMAP)SelectObject( hDC, hBitmap );
	return true;
}
bool TImage::Init() 
{
	return true;
}
bool TImage::Frame() 
{
	return true;
}

void TImage::Draw( TImage* pScreen, int x, int y, int iWidth, int iHeight,
							int iSrcX, int iSrcY , DWORD dwMode)
{
	BitBlt( pScreen->hDC, x,y, iWidth, iHeight,
			  hDC, iSrcX,iSrcY, dwMode );
}
bool TImage::Render() 
{
	return true;
}
bool TImage::Release() 
{
	SelectObject( hDC, hOldBitmap );
	DeleteObject( hBitmap );
	DeleteDC(  hDC );
	return true;
}

void	TImage::DrawBitMap( HDC hdc, int x, int y, HBITMAP hBit )
{
	HDC			MemDC;
	HBITMAP	OldBitmap;
	BITMAP		bit;

	MemDC = CreateCompatibleDC( hdc );// 메모리 DC
	OldBitmap = (HBITMAP)SelectObject( MemDC, hBit );

	   GetObject( hBit, sizeof( BITMAP), &bit );

	   //BitBlt( hdc, 0, 0, bit.bmWidth,bit.bmHeight, MemDC, 
		//		      0, 0, SRCINVERT );
	   DWORD dwWidth = GetSystemMetrics(0);
	   DWORD dwHeight = GetSystemMetrics(1);

	   StretchBlt( hdc, 0, 0, dwWidth, dwHeight, 
					MemDC, x,y, bit.bmWidth,bit.bmHeight, 
					SRCCOPY );
		
		SelectObject( MemDC, OldBitmap );
	DeleteDC( MemDC );
}
bool TImage::InitImage( HWND hWnd, HDC hdc, UINT id )
{
	hDC = CreateCompatibleDC( hdc );
	hBitmap = LoadBitmap(  g_hInstance,  MAKEINTRESOURCE(id) );
	hOldBitmap = (HBITMAP)SelectObject( hDC, hBitmap );
	return true;
}
bool TImage::InitImage(  HWND hWnd, HDC hdc, 
								int iX, int iY )
{
	hDC = CreateCompatibleDC( hdc );
	hBitmap = CreateCompatibleBitmap( hdc, iX, iY );
	hOldBitmap = (HBITMAP)SelectObject( hDC, hBitmap );
	return true;
}
//clrMask색을 투명하게 처리한다.
//TransBlt(m_hdc,0,0,m_hBitmap,RGB(255,255,255));
void TImage::TransBlt(HDC hdc, int x, int y, HBITMAP hbitmap, COLORREF clrMask)
{
	BITMAP bm;
	COLORREF cColor;
	HBITMAP bmAndBack, bmAndObject, bmAndMem, bmSave;
	HBITMAP bmBackOld, bmObjectOld, bmMemOld, bmSaveOld;
	HDC		hdcMem, hdcBack, hdcObject, hdcTemp, hdcSave;
	POINT	ptSize;

	hdcTemp = CreateCompatibleDC(hdc);
	SelectObject(hdcTemp, hbitmap);
	GetObject(hbitmap, sizeof(BITMAP), (LPSTR)&bm);
	ptSize.x = bm.bmWidth;
	ptSize.y = bm.bmHeight;
	DPtoLP(hdcTemp, &ptSize,1);

	hdcBack   = CreateCompatibleDC(hdc);
	hdcObject = CreateCompatibleDC(hdc);
	hdcMem    = CreateCompatibleDC(hdc);
	hdcSave   = CreateCompatibleDC(hdc);

	bmAndBack   = CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);
	bmAndObject = CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);
	bmAndMem    = CreateCompatibleBitmap(hdc, ptSize.x, ptSize.y);
	bmSave      = CreateCompatibleBitmap(hdc, ptSize.x, ptSize.y);

	bmBackOld   = (HBITMAP) SelectObject(hdcBack, bmAndBack);
	bmObjectOld = (HBITMAP) SelectObject(hdcObject, bmAndObject);
	bmMemOld    = (HBITMAP) SelectObject(hdcMem, bmAndMem);
	bmSaveOld   = (HBITMAP) SelectObject(hdcSave, bmSave);

	SetMapMode(hdcTemp, GetMapMode(hdc));

	BitBlt(hdcSave, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCCOPY);

	// 마스크 이미지 생성
	cColor = SetBkColor(hdcTemp, clrMask);
	BitBlt(hdcObject, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCCOPY);

	SetBkColor(hdcTemp, cColor);

	BitBlt(hdcBack, 0, 0, ptSize.x, ptSize.y, hdcObject, 0, 0, NOTSRCCOPY);
	BitBlt(hdcMem , 0, 0, ptSize.x, ptSize.y, hdc      , x, y, SRCCOPY);
	BitBlt(hdcMem , 0, 0, ptSize.x, ptSize.y, hdcObject, 0, 0, SRCAND);
	// 바탕색만 검정색으로 변환, 
	BitBlt(hdcTemp, 0, 0, ptSize.x, ptSize.y, hdcBack  , 0, 0, SRCAND);
	// 검정색 바탕색을 버린다. 배경색이 나온다.
	BitBlt(hdcMem , 0, 0, ptSize.x, ptSize.y, hdcTemp  , 0, 0, SRCPAINT);
	BitBlt(hdc    , x, y, ptSize.x, ptSize.y, hdcMem   , 0, 0, SRCCOPY);	
	//오프스크린에 원본 출력(확인용 )
	///BitBlt(hdc, x, y, ptSize.x, ptSize.y, hdcObject, 0, 0, SRCCOPY);
	// 원본 이미지 복구
	BitBlt(hdcTemp, 0, 0, ptSize.x, ptSize.y, hdcSave  , 0, 0, SRCCOPY);

	DeleteObject(SelectObject(hdcBack, bmBackOld));
	DeleteObject(SelectObject(hdcObject, bmObjectOld));
	DeleteObject(SelectObject(hdcMem, bmMemOld));
	DeleteObject(SelectObject(hdcSave, bmSaveOld));
	
	DeleteDC(hdcMem);
	DeleteDC(hdcBack);
	DeleteDC(hdcObject);
	DeleteDC(hdcSave);
	DeleteDC(hdcTemp);
}
TImage::TImage(void)
{

}

TImage::~TImage(void)
{
}
