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

//LPCTSTR�� CONST TCHAR*�� �ǹ��մϴ�.
VOID CUser::SetUserID(LPCTSTR szUserID, LPCTSTR szPassword)
{
	//�Ѿ���� �Ķ������ NULLüũ�� �ʼ�
	if (!szUserID || !szPassword)
		return;

	//�Ϲ� _tcscpy�� ������� �ʰ� _tcsncpy�� ����߽��ϴ�.
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
	//���⼭�� B�� �ڽ��� �ϰ���� ���� �ϸ� �˴ϴ�.
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
	//���⼭�� B�� �ڽ��� �ϰ���� ���� �ϸ� �˴ϴ�.
	_tprintf(_T("Updated UserID"));
}