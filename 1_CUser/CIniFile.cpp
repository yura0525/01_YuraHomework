#define _CRT_SECURE_NO_WARNINGS
#include "CIniFile.h"
#include <tchar.h>

//Ini파일을 처음 오픈하는 함수입니다.
//파라미터는 꼭 TCHAR형으로 받습니다.
BOOL CIniFile::Open(LPCTSTR fileName)
{
	//파일 이름이 잘못 들어 온 경우 실패합니다.
	if (!fileName)
		return FALSE;
	
	//미리 정의되어있는 MAX_PATH를 이용해서 복사합니다.
	_tcsncpy(m_FileName, fileName, MAX_PATH);

	return TRUE;
}
BOOL CIniFile::Close(VOID)
{
	return TRUE;
}
//일반 문자열 형을 쓸때 사용합니다.
BOOL CIniFile::SetValue(LPCTSTR keyName, LPCTSTR valueName, LPCTSTR value)
{
	if (!keyName || !valueName || !value)
		return FALSE;

	WritePrivateProfileString(keyName, valueName, value, m_FileName);

	return TRUE;
}
//DWORD 형을 쓸때 사용합니다.
BOOL CIniFile::SetValue(LPCTSTR keyName, LPCTSTR valueName, DWORD value)
{
	if (!keyName || !valueName )
		return FALSE;

	//형 변환 함수를 이용해서 값을 반환합니다.
	TCHAR Value[16] = { 0, };
	_itot(value, Value, 10);

	WritePrivateProfileString(keyName, valueName, Value, m_FileName);

	return TRUE;
}
//FLOAT 형을 쓸때 사용합니다.
BOOL CIniFile::SetValue(LPCTSTR keyName, LPCTSTR valueName, FLOAT value)
{
	if (!keyName || !valueName)
		return FALSE;

	//형 변환 함수를 이용해서 값을 반환합니다.
	TCHAR Value[16] = { 0, };
	_stprintf(Value, _T("%f"), value);

	WritePrivateProfileString(keyName, valueName, Value, m_FileName);
	return TRUE;
}
//일단 바이너리 데이터를 가져오는 함수입니다.
//문자열도 이것을 이용해서 가져올 수 있습니다.
BOOL CIniFile::GetValue(LPCTSTR keyName, LPCTSTR valueName, LPTSTR value, LPDWORD bufferLength)
{
	//문자열 형태의 파라미터는 꼭 값을 체크해줍니다.
	if (!keyName || !valueName || !value || !bufferLength)
		return FALSE;

	*bufferLength = GetPrivateProfileString(keyName, valueName, _T(""), value, *bufferLength, m_FileName);

	return TRUE;
}
//DWORD형 값을 가져오는 함수입니다.
//key, valueName을 입력하면 value를 출력해줍니다.
BOOL CIniFile::GetValue(LPCTSTR keyName, LPCTSTR valueName, LPDWORD value)
{
	//문자열 형태의 파라미터는 꼭 값을 체크해줍니다.
	if (!keyName || !valueName || !value)
		return FALSE;

	*value = GetPrivateProfileInt(keyName, valueName, 0, m_FileName);

	return TRUE;
}
//FLOAT형 값을 가져오는 함수입니다.
BOOL CIniFile::GetValue(LPCTSTR keyName, LPCTSTR valueName, FLOAT* value)
{
	//문자열 형태의 파라미터는 꼭 값을 체크해줍니다.
	if (!keyName || !valueName || !value)
		return FALSE;

	TCHAR Value[16] = { 0, };

	//FLOAT 형을 따로 가져오는 함수가 없기 때문에 일단 문자열 형태로 가져와서 변환을 해줍니다.
	GetPrivateProfileString(keyName, valueName, _T(""), Value, 16, m_FileName);
	*value = (FLOAT)_tstof(Value);

	return TRUE;
}

CIniFile::CIniFile()
{
	//생성자에서 사용할 파일의 이름을 초기화해줍니다.
	ZeroMemory(m_FileName, sizeof(m_FileName));
}


CIniFile::~CIniFile()
{
}
