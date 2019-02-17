#include "TDebugString_1.h"

void TDebugString_1::DisplayText(char* fmt, ...)
{
	EnterCriticalSection(&m_CS);
	va_list arg;
	va_start(arg, fmt);
	char buf[1024 + 256] = { 0, };
	vsprintf(buf, fmt, arg);
	if (m_Msg.size() > 4)
	{
		m_Msg.erase(m_Msg.begin());
	}
	m_Msg.push_back(GetMbtoWcs(buf));
	va_end(arg);
	LeaveCriticalSection(&m_CS);
}
TCHAR* TDebugString_1::GetMbtoWcs(const char* srcmsg)
{
	// ��Ƽ����Ʈ => �����ڵ� ��ȯ.
	TCHAR msg[2048] = { 0, };
	ConvertAnsiStringToWideCh(msg, srcmsg, 2048);
	return msg;
}
HRESULT TDebugString_1::ConvertWideStringToAnsiCh(CHAR* strDestination, const WCHAR* wstrSource, int cchDestChar)
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
HRESULT TDebugString_1::ConvertAnsiStringToWideCh(WCHAR* wstrDestination, const CHAR* strSource, int cchDestChar)
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
bool TDebugString_1::Init()
{
	InitializeCriticalSection(&m_CS);
	return true;
}
bool TDebugString_1::Frame()
{
	return true;
}
bool TDebugString_1::Release()
{
	m_Msg.clear();
	DeleteCriticalSection(&m_CS);
	return true;
}
void TDebugString_1::T_ERROR(bool bPrint)
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
			I_DebugStr.DisplayText(const_cast<char*>("ERROR WSASend:%s\r\n"), (char*)lpMsgBuf);
		else
			OutputDebugStringA((char*)lpMsgBuf);
		LocalFree(lpMsgBuf);
	}
}

TDebugString_1::TDebugString_1()
{
}


TDebugString_1::~TDebugString_1()
{
}
