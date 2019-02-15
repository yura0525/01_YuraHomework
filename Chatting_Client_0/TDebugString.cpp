#include "TDebugString.h"

void	TDebugString::DisplayText(char* fmt, ...)
{

}

void	TDebugString::Print(char* fmt, ...)
{

}
TCHAR*	TDebugString::GetMbtoWcs(const char* srcmsg)
{
	return NULL;
}
char*	TDebugString::GetWcstoMbs(WCHAR* srcmsg)
{
	return NULL;
}
HRESULT	TDebugString::ConvertAnsiStringToWideCh(WCHAR* wstrDestination, const CHAR* strSource, int cchDestChar)
{
	return S_OK;
}
HRESULT	TDebugString::ConvertWideStringToAnsiCh(CHAR* strDestination, const WCHAR* wstrSource, int cchDestChar)
{
	return S_OK;
}

bool TDebugString::Init()
{
	return true;
}
bool TDebugString::Frame()
{
	return true;
}
bool TDebugString::Release()
{
	return true;
}
void TDebugString::T_ERROR(bool bPrint)
{

}

TDebugString::TDebugString()
{
}


TDebugString::~TDebugString()
{
}
