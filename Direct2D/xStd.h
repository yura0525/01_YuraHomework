#pragma once

#include <windows.h>
#include <assert.h>
#include <tchar.h>

#include <iostream>
#include <map>
#include <string>
#include <list>
#include <vector>
#include <d3dx11.h>
#include <dxgi.h>
#include <dinput.h>
#include "D3Dcompiler.h"		//D3DCOMPILE_DEBUG
#include <math.h>

//프로젝트 속성에서 추가 종속성에 넣을걸 코드로 넣을수 있다.
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment( lib, "dinput8.lib")
#pragma comment( lib, "dxguid.lib")
#pragma comment( lib, "d3dx11.lib")
#pragma comment(lib, "fmod64_vc.lib")

#pragma warning (disable : 4005)

typedef std::basic_string <char>	C_STR;	//multi-byte
typedef std::basic_string <wchar_t>	W_STR;	//unicode byte
typedef std::basic_string <TCHAR>	T_STR;	//프로젝트속성이 멀티바이트이면 멀티바이트, 유니코드이면 유니코드로 바뀜.

											//string a;		//multi-byte
											//wstring b;	//unicode byte

#define GAMESTART int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPWSTR lpCmdLine, int nCmdShow) {
											//
											////L#s ==> L"" s는 문자열이므로 #을 붙인다. 유니코드라서 L을 붙인다.
											//#define GAMERUN(s) xWindow win; win.SetWindow(hInstance, L#s, 600, 800); win.Run();
											//
#define GAME_END }


											//L#s ==> L"" s는 문자열이므로 #을 붙인다. 유니코드라서 L을 붙인다.
#define GAMERUN(s, w, h) int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPWSTR lpCmdLine, int nCmdShow) { Sample win; win.SetWindow(hInst, L#s, w, h); win.Run(); }

extern HWND			g_hWnd;
extern HINSTANCE	g_hInstance;
extern float		g_fSecPerFrame;
extern float		g_fGameTimer;
extern RECT			g_rtClient;
//DirectX2D
extern ID3D11Device*		g_pd3dDevice;
extern ID3D11DeviceContext*	g_pContext;

//Game
extern POINT		g_pHeroPos;
extern POINT		g_pMousePos;

struct TGameInput
{
	BOOL bFront;
	BOOL bBack;
	BOOL bLeft;
	BOOL bRight;
	BOOL bAttack;
	BOOL bJump;
};

extern TGameInput g_Input;
//추가 포함 디렉토리에 해당파일을 또 추가하면
//../../include; 경로를 추가해서 또 include하면
//컴파일러 오류 C2953
//'TSingleton': 클래스 템플릿이 이미 정의되었습니다. 에러 발생.
// 라이브러리는 추가포함 디렉토리를 추가하지 않는다.
template<class T> class TSingleton
{
public:
	static T& GetInstance()
	{
		static T theSingleInstance;
		return theSingleInstance;
	}
};

#ifndef V_RETURN
#define V_RETURN(x) { hr = (x); if( FAILED(hr) ) {return hr;} }
#endif

#ifndef V
#define V(x) { hr = (x);}
#endif

//////////////////////////////////////////////
// 객체 및 배열 할당과 삭제 및 소멸 매크로
//////////////////////////////////////////////
#ifndef SAFE_ZERO
#define SAFE_ZERO(A)				{ A = 0; }
#endif

#ifndef SAFE_NEW
#define SAFE_NEW(A, B)				{ if (!A) A = new B; }
#endif 

#ifndef SAFE_DEL
#define SAFE_DEL(A)					{ if (A) delete A; (A)=NULL; }
#endif 

#ifndef SAFE_NEW_ARRAY
#define SAFE_NEW_ARRAY(A, B, C)		{ if (!A && C) A = new B[C]; }
#endif 

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(A)		{ if (A) delete [] A; (A)=NULL; }
#endif 

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(A)				{ if(A) { (A)->Release(); (A)=NULL; } }
#endif 

#ifndef SAFE_NEW_CLEAR
#define SAFE_NEW_CLEAR( A, B )			{ if (!A) A = new B; if(A) memset( A, 0, sizeof(B) ); };
#endif 

#ifndef SAFE_NEW_ARRAY_CLEAR
#define NEW_ARRAY_CLEAR( A, B, C )	{ if (!A && C) A = new B[C]; if(A) memset( A, 0, sizeof(B)*C ); };
#endif
