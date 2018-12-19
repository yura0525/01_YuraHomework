#include "CRegistry.h"
#include <tchar.h>

//처음 키를 열때 사용합니다.
BOOL CRegistry::Open(HKEY rootKey, LPCTSTR subKey)
{
	//항상 포인터형 파라미터는 NULL인지 체크하는 것이 좋습니다.
	if (!subKey)
		return FALSE;
	//이미 열려있을경우 실패합니다.
	if (m_IsOpened)
		return FALSE;

	//지정된 키를 열어서 m_RootKey변수에 저장합니다.
	if (RegOpenKey(rootKey, subKey, &m_RootKey) != ERROR_SUCCESS)
		return FALSE;

	//앞으로 사용할 키를 열었는지 판단하는 변수 값을 TRUE로 셋팅해줍니다.
	m_IsOpened = TRUE;

	return TRUE;
}
//키를 닫을때 사용합니다.
BOOL CRegistry::Close(VOID)
{
	if (RegCloseKey(m_RootKey) != ERROR_SUCCESS)
		return FALSE;

	//키를 닫으면서 FALSE로 셋팅해줍니다.
	m_IsOpened = FALSE;
	return TRUE;
}
//키를 생성하는 변수
BOOL CRegistry::CreateKey(HKEY rootKey, LPCTSTR subKey)
{
	//rootkey를 기준으로 생성할 subkey를 입력받아 키를 생성합니다.
	if (!subKey)
		return FALSE;

	//이미 열려있을 경우는 실패
	if (m_IsOpened)
		return FALSE;

	//레지스트리 키를 생성하는 API함수를 호출합니다.
	if (RegCreateKey(rootKey, subKey, &m_RootKey) != ERROR_SUCCESS)
		return FALSE;

	//OpenKey외에 생성하면서 자동으로 열린 형태로 셋팅합니다.
	m_IsOpened = TRUE;

	return TRUE;
}
//키를 삭제하는 변수
BOOL CRegistry::DeleteKey(HKEY rootKey, LPCTSTR subKey)
{
	if (!subKey)
		return FALSE;

	//레지스트리 키를 삭제하는 API함수를 호출합니다.
	if (RegDeleteKey(rootKey, subKey) != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}
//키와 달리 Create할 필요 없이 SetValue를 할때 값이 없을 경우 설정됩니다.
BOOL CRegistry::SetValue(LPCTSTR valueName, LPCTSTR value)
{
	//TCHAR형의 문자열 데이터를 레지스트리에 입력할때 사용합니다.
	if (!valueName || !value)
		return FALSE;

	//이미 열려있을 경우는 실패
	if (m_IsOpened)
		return FALSE;

	//REG_SZ형으로 데이터를 입력합니다.
	if (RegSetValueEx(m_RootKey, valueName, 0, REG_SZ, (BYTE*)value, 
		(DWORD)_tcslen(value) * sizeof(TCHAR)) != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}
//DWORD형 값을 지정하는 함수
BOOL CRegistry::SetValue(LPCTSTR valueName, DWORD value)
{
	if (!valueName)
		return FALSE;

	//이미 열려있을 경우는 실패
	if (m_IsOpened)
		return FALSE;

	//REG_DWORD형으로 데이터를 입력합니다.
	if (RegSetValueEx(m_RootKey, valueName, 0, REG_DWORD, (BYTE*)value, sizeof(DWORD)) != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}
BOOL CRegistry::SetValueForMultiSz(LPCTSTR valueName, DWORD value, DWORD byteLength)
{
	//TCHAR형의 문자열 데이터를 레지스트리에 입력할때 사용합니다.
	if (!valueName || !value)
		return FALSE;

	//이미 열려있을 경우는 실패
	if (m_IsOpened)
		return FALSE;

	//REG_MULTI_SZ형으로 데이터를 입력합니다.
	if (RegSetValueEx(m_RootKey, valueName, 0, REG_MULTI_SZ, (BYTE*)value, byteLength) != ERROR_SUCCESS)
		return FALSE;

	return TRUE;

}
//TCHAR형의 문자열 데이터를 읽어오는 함수
BOOL CRegistry::GetValue(LPCTSTR valueName, LPCTSTR value, LPDWORD bufferLength)
{
	DWORD ValueType = 0;
	
	if (!valueName || !value || !bufferLength)
		return FALSE;

	if (!m_IsOpened)
		return FALSE;

	//문자열 데이터를 읽어옵니다.
	//bufferLenth에 저장된 길이만큼 읽어옵니다.
	if (RegQueryValueEx(m_RootKey, valueName, 0, &ValueType, (BYTE*)value, bufferLength) != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}
//DWORD형의 데이터를 읽어오는 함수
BOOL CRegistry::GetValue(LPCTSTR valueName, LPDWORD value)
{
	DWORD ValueType = 0;

	//버퍼의 크기를 sizeof를 이용해서 DWORD의 크기를 가져옵니다.
	DWORD BufferLength = sizeof(DWORD);

	if (!valueName || !value)
		return FALSE;

	if (!m_IsOpened)
		return FALSE;

	//TCHAR 형 문자열을 가져온것과 같은 형태로 값을 가져옵니다.
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

	//레지스트리의 값을 삭제하는 API함수를 호출합니다.
	if (RegDeleteValue(m_RootKey, valueName) != ERROR_SUCCESS)
		return FALSE;

	return TRUE;
}

CRegistry::CRegistry()
{
	m_RootKey = NULL;		//사용할 레지스트리 키를 저장하는 변수
	m_IsOpened = FALSE;		//해당 레지스트리를 열었는지 체크하는 변수
}


CRegistry::~CRegistry()
{
}
