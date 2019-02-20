#define _CRT_SECURE_NO_WARNINGS
#include "TDebugString_2.h"
#include "TSynchronize_1.h"

void	TDebugString_2::DisplayText(char* fmt, ...)
{
	{
		TSynchronize_1 sync(this);
		va_list arg;
		va_start(arg, fmt);
		char buf[1024 + 256] = { 0, };
		vsprintf(buf, fmt, arg);
		int nLen = GetWindowTextLength(m_hEdit);
		if (nLen > 3000)
		{
			SendMessage(m_hEdit, EM_SETSEL, 0, -1);
			SendMessage(m_hEdit, WM_CLEAR, 0, 0);
		}
		SendMessage(m_hEdit, EM_REPLACESEL, FALSE, (LPARAM)GetMbtoWcs(buf));
		va_end(arg);
	}
}
TCHAR*	TDebugString_2::GetMbtoWcs(const char* srcmsg)
{
	// 멀티바이트 => 유니코드 변환.
	TCHAR msg[1024] = { 0, };
	ConvertAnsiStringToWideCh(msg, srcmsg, 1024);
	return msg;
}
char*	TDebugString_2::GetWcstoMbs(WCHAR* srcmsg)
{
	// 유니코드 => 멀티바이트 변환.
	char msg[1024] = { 0, };
	ConvertWideStringToAnsiCh(msg, srcmsg, 1024);
	return msg;
}
HRESULT	TDebugString_2::ConvertAnsiStringToWideCh(WCHAR* wstrDestination, const CHAR* strSource, int cchDestChar)
{
	if (wstrDestination == NULL || strSource == NULL || cchDestChar < 1)
		return E_INVALIDARG;

	int nResult = MultiByteToWideChar(CP_ACP, 0, strSource, -1,
		wstrDestination, cchDestChar);
	wstrDestination[cchDestChar - 1] = 0;

	if (nResult == 0)
		return E_FAIL;
	return S_OK;
}
HRESULT	TDebugString_2::ConvertWideStringToAnsiCh(CHAR* strDestination, const WCHAR* wstrSource, int cchDestChar)
{
	if (strDestination == NULL || wstrSource == NULL || cchDestChar < 1)
		return E_INVALIDARG;

	int nResult = WideCharToMultiByte(CP_ACP, 0, wstrSource, -1, strDestination,
		cchDestChar * sizeof(CHAR), NULL, NULL);
	strDestination[cchDestChar - 1] = 0;

	if (nResult == 0)
		return E_FAIL;
	return S_OK;
}
bool	TDebugString_2::Init()
{
	//InitializeCriticalSection(&m_CS);
	// 에디터 컨트롤 윈도우 생성
	m_hEdit = CreateWindow(L"edit", NULL,
		WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL
		| ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY
		| ES_AUTOHSCROLL,
		0, 0,
		500,//this->m_iWidth,
		500,//this->m_iHeight,
		m_hWnd, NULL,
		m_hInst,
		NULL);


	return true;
}
bool	TDebugString_2::Frame()
{
	return true;
}
bool	TDebugString_2::Release()
{
	return true;
}
void	TDebugString_2::T_ERROR(bool bPrint)
{
	if (WSAGetLastError() != WSA_IO_PENDING)
	{
		setlocale(LC_ALL, "KOREAN");
		char* lpMsgBuf;
		FormatMessageA(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM,
			NULL, WSAGetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(char*)&lpMsgBuf, 0, NULL);
		if (bPrint)
			I_DebugStr.DisplayText(const_cast<char*>("\nERROR WSASend:%s\r\n"), (char*)lpMsgBuf);
		else
			OutputDebugStringA((char*)lpMsgBuf);
		LocalFree(lpMsgBuf);
	}
}

TDebugString_2::TDebugString_2()
{
}


TDebugString_2::~TDebugString_2()
{
}
