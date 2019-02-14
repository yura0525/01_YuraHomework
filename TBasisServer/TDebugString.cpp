#include "TDebugString.h"
#include "TSynchronize.h"

void TDebugString::Print(const char* fmt, ...)
{
	if (m_hList == NULL)
		return;

	{
		TSynchronize sync(this);
		va_list arg;
		va_start(arg, fmt);
		char buf[256] = { 0, };
		vsprintf_s(buf, fmt, arg);
		SendMessage(m_hList, LB_ADDSTRING, 0,
			(LPARAM)I_Debug.GetMbToWcs(buf));
		int index = SendMessage(m_hList, LB_GETCOUNT, 0, 0);
		if (index > 30)
		{
			SendMessage(m_hList, LB_RESETCONTENT, 0, 0);
		}
		va_end(arg);
	}
}

bool TDebugString::Init()
{
	m_hList = NULL;
	return true;
}
bool TDebugString::Release()
{
	return true;
}
WCHAR* TDebugString::GetMbToWcs(const char* strMsg)
{
	TCHAR msg[4096] = { 0, };
	{
		TSynchronize sync(this);
		int iRen = MultiByteToWideChar(CP_ACP, 0, strMsg, -1, 0, 0);
		ConvertAnsiStringToWideCh(msg, strMsg, iRen);
	}

	return msg;
}
char* TDebugString::GetwcsToMbs(const WCHAR* strMsg)
{
	char msg[4096] = { 0, };
	{
		TSynchronize sync(this);
		int iRen = WideCharToMultiByte(CP_ACP, 0, strMsg, -1, 0, 0, NULL, NULL);
		ConvertWideStringToAnsiCh(msg, strMsg, iRen);
	}

	return msg;
}
HRESULT	TDebugString::ConvertWideStringToAnsiCh(char* msg, const WCHAR* strMsg, int iRen)
{
	{
		TSynchronize sync(this);
		if (msg == NULL || strMsg == NULL || iRen < 1)
		{
			return E_INVALIDARG;
		}
		int iReturn = WideCharToMultiByte(CP_ACP, 0, strMsg, -1, msg, iRen, NULL, NULL);
		if (iReturn == 0)
			return E_FAIL;
	}
	return S_OK;
}
HRESULT	TDebugString::ConvertAnsiStringToWideCh(WCHAR* msg, const char* strMsg, int iRen)
{
	{
		TSynchronize sync(this);
		if (msg == NULL || strMsg == NULL || iRen < 1)
		{
			return E_INVALIDARG;
		}
		int iReturn = MultiByteToWideChar(CP_ACP, 0, strMsg, -1, msg, iRen);
		if (iReturn == 0)
			return E_FAIL;
		msg[iRen - 1] = 0;
	}
	return S_OK;
}
void TDebugString::T_ERROR(bool bPrint)
{
	{
		TSynchronize sync(this);
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			char* lpMsgBuf;
			FormatMessageA(
				FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_FROM_SYSTEM, NULL,
				WSAGetLastError(),
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(char*)&lpMsgBuf, 0, NULL);
			if (bPrint)
			{
				MessageBoxA(NULL, lpMsgBuf, "ERROR", MB_OK);
			}
			else
			{
				OutputDebugStringA((char*)lpMsgBuf);
			}
			LocalFree(lpMsgBuf);
		}
	}
}

TDebugString::TDebugString()
{
}


TDebugString::~TDebugString()
{
}
