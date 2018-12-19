#define _CRT_SECURE_NO_WARNINGS
#include "CIniFile.h"
#include <tchar.h>

//Ini������ ó�� �����ϴ� �Լ��Դϴ�.
//�Ķ���ʹ� �� TCHAR������ �޽��ϴ�.
BOOL CIniFile::Open(LPCTSTR fileName)
{
	//���� �̸��� �߸� ��� �� ��� �����մϴ�.
	if (!fileName)
		return FALSE;
	
	//�̸� ���ǵǾ��ִ� MAX_PATH�� �̿��ؼ� �����մϴ�.
	_tcsncpy(m_FileName, fileName, MAX_PATH);

	return TRUE;
}
BOOL CIniFile::Close(VOID)
{
	return TRUE;
}
//�Ϲ� ���ڿ� ���� ���� ����մϴ�.
BOOL CIniFile::SetValue(LPCTSTR keyName, LPCTSTR valueName, LPCTSTR value)
{
	if (!keyName || !valueName || !value)
		return FALSE;

	WritePrivateProfileString(keyName, valueName, value, m_FileName);

	return TRUE;
}
//DWORD ���� ���� ����մϴ�.
BOOL CIniFile::SetValue(LPCTSTR keyName, LPCTSTR valueName, DWORD value)
{
	if (!keyName || !valueName )
		return FALSE;

	//�� ��ȯ �Լ��� �̿��ؼ� ���� ��ȯ�մϴ�.
	TCHAR Value[16] = { 0, };
	_itot(value, Value, 10);

	WritePrivateProfileString(keyName, valueName, Value, m_FileName);

	return TRUE;
}
//FLOAT ���� ���� ����մϴ�.
BOOL CIniFile::SetValue(LPCTSTR keyName, LPCTSTR valueName, FLOAT value)
{
	if (!keyName || !valueName)
		return FALSE;

	//�� ��ȯ �Լ��� �̿��ؼ� ���� ��ȯ�մϴ�.
	TCHAR Value[16] = { 0, };
	_stprintf(Value, _T("%f"), value);

	WritePrivateProfileString(keyName, valueName, Value, m_FileName);
	return TRUE;
}
//�ϴ� ���̳ʸ� �����͸� �������� �Լ��Դϴ�.
//���ڿ��� �̰��� �̿��ؼ� ������ �� �ֽ��ϴ�.
BOOL CIniFile::GetValue(LPCTSTR keyName, LPCTSTR valueName, LPTSTR value, LPDWORD bufferLength)
{
	//���ڿ� ������ �Ķ���ʹ� �� ���� üũ���ݴϴ�.
	if (!keyName || !valueName || !value || !bufferLength)
		return FALSE;

	*bufferLength = GetPrivateProfileString(keyName, valueName, _T(""), value, *bufferLength, m_FileName);

	return TRUE;
}
//DWORD�� ���� �������� �Լ��Դϴ�.
//key, valueName�� �Է��ϸ� value�� ������ݴϴ�.
BOOL CIniFile::GetValue(LPCTSTR keyName, LPCTSTR valueName, LPDWORD value)
{
	//���ڿ� ������ �Ķ���ʹ� �� ���� üũ���ݴϴ�.
	if (!keyName || !valueName || !value)
		return FALSE;

	*value = GetPrivateProfileInt(keyName, valueName, 0, m_FileName);

	return TRUE;
}
//FLOAT�� ���� �������� �Լ��Դϴ�.
BOOL CIniFile::GetValue(LPCTSTR keyName, LPCTSTR valueName, FLOAT* value)
{
	//���ڿ� ������ �Ķ���ʹ� �� ���� üũ���ݴϴ�.
	if (!keyName || !valueName || !value)
		return FALSE;

	TCHAR Value[16] = { 0, };

	//FLOAT ���� ���� �������� �Լ��� ���� ������ �ϴ� ���ڿ� ���·� �����ͼ� ��ȯ�� ���ݴϴ�.
	GetPrivateProfileString(keyName, valueName, _T(""), Value, 16, m_FileName);
	*value = (FLOAT)_tstof(Value);

	return TRUE;
}

CIniFile::CIniFile()
{
	//�����ڿ��� ����� ������ �̸��� �ʱ�ȭ���ݴϴ�.
	ZeroMemory(m_FileName, sizeof(m_FileName));
}


CIniFile::~CIniFile()
{
}
