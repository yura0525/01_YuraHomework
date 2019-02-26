#include "TDebugString.h"
#include "TSynchronize.h"

void	TDebugString::DisplayText(const char* fmt, ...)
{
	{
		TSynchronize sync(this);
		va_list arg;
		va_start(arg, fmt);
			char buf[1024 + 256] = { 0, };
			vsprintf_s(buf, fmt, arg);
			int nLen = GetWindowTextLength(m_hEdit);
			if (nLen > 3000)
			{
				SendMessage(m_hEdit, EM_SETSEL, 0, -1);
				SendMessage(m_hEdit, WM_CLEAR, 0, 0);
			}
			TCHAR msg[1024] = { 0, };
			GetMbToWc(msg, buf);
			SendMessage(m_hEdit, EM_REPLACESEL, FALSE, (LPARAM)(msg));
		va_end(arg);
	}
}
void	TDebugString::GetMbToWc(TCHAR* destMsg, const char* srcMsg)
{
	// 멀티바이트 => 유니코드 변환.
	ConvertAnsiStringToWideCh(destMsg, srcMsg, 1024);
	return;
}
void	TDebugString::GetWcToMb(char* destMsg, WCHAR* srcMsg)
{
	//유니코드 => 멀티바이트 변환
	ConvertWideStringToAnsiCh(destMsg, srcMsg, 1024);
	return;
}
HRESULT	TDebugString::ConvertAnsiStringToWideCh(WCHAR* wstrDest, const CHAR* strSrc, int chDestChar)
{
	if (wstrDest == NULL || strSrc == NULL || chDestChar < 1)
		return E_INVALIDARG;

	int nResult = MultiByteToWideChar(CP_ACP, 0, strSrc, -1, wstrDest, chDestChar);
	wstrDest[chDestChar - 1] = 0;

	if (nResult == 0)
		return E_FAIL;

	return S_OK;
}
HRESULT	TDebugString::ConvertWideStringToAnsiCh(CHAR* strDest, const WCHAR* wstrSrc, int chDestChar)
{
	if (strDest == NULL || wstrSrc == NULL || chDestChar < 1)
		return E_INVALIDARG;

	int nResult = WideCharToMultiByte(CP_ACP, 0, wstrSrc, -1, strDest, chDestChar * sizeof(CHAR), NULL, NULL);
	strDest[chDestChar - 1] = 0;

	if (nResult == 0)
		return E_FAIL;

	return S_OK;
}
void	TDebugString::T_ERROR(bool bPrint)
{
	if (WSAGetLastError() != WSA_IO_PENDING)
	{
		char* lpMsgBuf;
		setlocale(LC_ALL, "KOREAN");
		
		FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL, WSAGetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (char*)&lpMsgBuf, 0, NULL);

		if (bPrint)
		{
			I_DebugStr.DisplayText("\nTDebugString::T_ERROR() WSASend : %s\n", (char*)lpMsgBuf);
		}
		else
		{
			OutputDebugStringA((char*)lpMsgBuf);
		}

		LocalFree(lpMsgBuf);
	}
}
bool	TDebugString::Init()
{
	RECT rt;
	GetClientRect(m_hWnd, &rt);

	// 에디터 컨트롤 윈도우 생성
	m_hEdit = CreateWindow(L"edit", NULL,
		WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL |
		ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL | ES_READONLY,
		rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top, m_hWnd, NULL, m_hInst, NULL);

	return true;
}
bool	TDebugString::Frame()
{
	return true;
}
bool	TDebugString::Release()
{
	return true;
}


TDebugString::TDebugString()
{
}


TDebugString::~TDebugString()
{
}
