#define  _CRT_SECURE_NO_WARNINGS
#include "CUser.h"

CUser::CUser()
{
	ZeroMemory(m_szUserID, sizeof(TCHAR) * 64);
	ZeroMemory(m_szPassword, sizeof(TCHAR) * 64);
	ZeroMemory(m_szName, sizeof(TCHAR) * 64);
	ZeroMemory(m_szAddress, sizeof(TCHAR) * 64);
	m_usAge = 0;
}


CUser::~CUser()
{
}

//LPCTSTR은 CONST TCHAR*을 의미합니다.
VOID CUser::SetUserID(LPCTSTR szUserID, LPCTSTR szPassword)
{
	//넘어오는 파라미터의 NULL체크는 필수
	if (!szUserID || !szPassword)
		return;

	//일반 _tcscpy을 사용하지 않고 _tcsncpy을 사용했습니다.
	_tcsncpy(m_szUserID, szUserID, 64);
	_tcsncpy(m_szPassword, szPassword, 64);
}

VOID CUser::SetUserInformation(LPCTSTR szName, LPCTSTR szAddress, USHORT usAge)
{
	if (!szName || !szAddress)
		return;

	_tcsncpy(m_szName, szName, 64);
	_tcsncpy(m_szAddress, szAddress, 64);

	m_usAge = usAge;
}

VOID CGameUser::SetGameInformation(LPCTSTR szCharacterName, USHORT usLevel)
{
	if (!szCharacterName)
		return;

	_tcsncpy(m_szCharacterName, szCharacterName, 64);
	m_usLevel = usLevel;
}

VOID CGameUser::OnUpdateUserID(VOID)
{
	//여기서는 B는 자신이 하고싶은 일을 하면 됩니다.
	_tprintf(_T("Updated UserID"));
}

VOID CWebUser::SetWebInformation(LPCTSTR szBoardName)
{
	if (!szBoardName)
		return;

	_tcsncpy(m_szBoardName, szBoardName, 64);
}

VOID CWebUser::OnUpdateUserID(VOID)
{
	//여기서는 B는 자신이 하고싶은 일을 하면 됩니다.
	_tprintf(_T("Updated UserID"));
}