#include "CRegistry.h"
#include <tchar.h>

//ó�� Ű�� ���� ����մϴ�.
BOOL CRegistry::Open(HKEY rootKey, LPCTSTR subKey)
{
	//�׻� �������� �Ķ���ʹ� NULL���� üũ�ϴ� ���� �����ϴ�.
	if (!subKey)
		return FALSE;
	//�̹� ����������� �����մϴ�.
	if (m_IsOpened)
		return FALSE;

	//������ Ű�� ��� m_RootKey������ �����մϴ�.
	if (RegOpenKey(rootKey, subKey, &m_RootKey) != ERROR_SUCCESS)
		return FALSE;

	//������ ����� Ű�� �������� �Ǵ��ϴ� ���� ���� TRUE�� �������ݴϴ�.
	m_IsOpened = TRUE;

	return TRUE;
}
//Ű�� ������ ����մϴ�.
BOOL CRegistry::Close(VOID)
{
	if (RegCloseKey(m_RootKey) != ERROR_SUCCESS)
		return FALSE;

	//Ű�� �����鼭 FALSE�� �������ݴϴ�.
	m_IsOpened = FALSE;
	return TRUE;
}
//Ű�� �����ϴ� ����
BOOL CRegistry::CreateKey(HKEY rootKey, LPCTSTR subKey)
{
	//rootkey�� �������� ������ subkey�� �Է¹޾� Ű�� �����մϴ�.
	if (!subKey)
		return FALSE;

	//�̹� �������� ���� ����
	if (m_IsOpened)
		return FALSE;

	//������Ʈ�� Ű�� �����ϴ� API�Լ��� ȣ���մϴ�.
	if (RegCreateKey(rootKey, subKey, &m_RootKey) != ERROR_SUCCESS)
		return FALSE;

	//OpenKey�ܿ� �����ϸ鼭 �ڵ����� ���� ���·� �����մϴ�.
	m_IsOpened = TRUE;

	return TRUE;
}
//Ű�� �����ϴ� ����
BOOL CRegistry::DeleteKey(HKEY rootKey, LPCTSTR subKey)
{
	if (!subKey)
		return FALSE;

	//������Ʈ�� Ű�� �����ϴ� API�Լ��� ȣ���մϴ�.
	if (RegDeleteKey(rootKey, subKey) != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}
//Ű�� �޸� Create�� �ʿ� ���� SetValue�� �Ҷ� ���� ���� ��� �����˴ϴ�.
BOOL CRegistry::SetValue(LPCTSTR valueName, LPCTSTR value)
{
	//TCHAR���� ���ڿ� �����͸� ������Ʈ���� �Է��Ҷ� ����մϴ�.
	if (!valueName || !value)
		return FALSE;

	//�̹� �������� ���� ����
	if (m_IsOpened)
		return FALSE;

	//REG_SZ������ �����͸� �Է��մϴ�.
	if (RegSetValueEx(m_RootKey, valueName, 0, REG_SZ, (BYTE*)value, 
		(DWORD)_tcslen(value) * sizeof(TCHAR)) != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}
//DWORD�� ���� �����ϴ� �Լ�
BOOL CRegistry::SetValue(LPCTSTR valueName, DWORD value)
{
	if (!valueName)
		return FALSE;

	//�̹� �������� ���� ����
	if (m_IsOpened)
		return FALSE;

	//REG_DWORD������ �����͸� �Է��մϴ�.
	if (RegSetValueEx(m_RootKey, valueName, 0, REG_DWORD, (BYTE*)value, sizeof(DWORD)) != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}
BOOL CRegistry::SetValueForMultiSz(LPCTSTR valueName, DWORD value, DWORD byteLength)
{
	//TCHAR���� ���ڿ� �����͸� ������Ʈ���� �Է��Ҷ� ����մϴ�.
	if (!valueName || !value)
		return FALSE;

	//�̹� �������� ���� ����
	if (m_IsOpened)
		return FALSE;

	//REG_MULTI_SZ������ �����͸� �Է��մϴ�.
	if (RegSetValueEx(m_RootKey, valueName, 0, REG_MULTI_SZ, (BYTE*)value, byteLength) != ERROR_SUCCESS)
		return FALSE;

	return TRUE;

}
//TCHAR���� ���ڿ� �����͸� �о���� �Լ�
BOOL CRegistry::GetValue(LPCTSTR valueName, LPCTSTR value, LPDWORD bufferLength)
{
	DWORD ValueType = 0;
	
	if (!valueName || !value || !bufferLength)
		return FALSE;

	if (!m_IsOpened)
		return FALSE;

	//���ڿ� �����͸� �о�ɴϴ�.
	//bufferLenth�� ����� ���̸�ŭ �о�ɴϴ�.
	if (RegQueryValueEx(m_RootKey, valueName, 0, &ValueType, (BYTE*)value, bufferLength) != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}
//DWORD���� �����͸� �о���� �Լ�
BOOL CRegistry::GetValue(LPCTSTR valueName, LPDWORD value)
{
	DWORD ValueType = 0;

	//������ ũ�⸦ sizeof�� �̿��ؼ� DWORD�� ũ�⸦ �����ɴϴ�.
	DWORD BufferLength = sizeof(DWORD);

	if (!valueName || !value)
		return FALSE;

	if (!m_IsOpened)
		return FALSE;

	//TCHAR �� ���ڿ��� �����°Ͱ� ���� ���·� ���� �����ɴϴ�.
	if (RegQueryValueEx(m_RootKey, valueName, 0, &ValueType, (BYTE*)value, &BufferLength) != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}
BOOL CRegistry::DeleteValue(LPCTSTR valueName)
{
	if (!valueName)
		return FALSE;

	if (!m_IsOpened)
		return FALSE;

	//������Ʈ���� ���� �����ϴ� API�Լ��� ȣ���մϴ�.
	if (RegDeleteValue(m_RootKey, valueName) != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}

CRegistry::CRegistry()
{
	m_RootKey = NULL;		//����� ������Ʈ�� Ű�� �����ϴ� ����
	m_IsOpened = FALSE;		//�ش� ������Ʈ���� �������� üũ�ϴ� ����
}


CRegistry::~CRegistry()
{
}
