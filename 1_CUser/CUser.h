#pragma once
#include <tchar.h>
#include <Windows.h>

class CUser
{
private:
	TCHAR m_szUserID[64];	//아이디
	TCHAR m_szPassword[64];	//암호

	TCHAR m_szName[64];		//실명
	TCHAR m_szAddress[64];	//주소

	USHORT m_usAge;			//나이
public:
	CUser();
	virtual ~CUser();
	//가상함수를 protected로 선언합니다.
protected:
	virtual VOID OnUpdateUserID(VOID) = 0;
public:
	VOID SetUserID(LPCTSTR szUserID, LPCTSTR szPassword);
	VOID SetUserInformation(LPCTSTR szName, LPCTSTR szAddress, USHORT usAge);
};

//게임에서 사용하는 사용자 클래스
class CGameUser : public CUser
{
private:
	TCHAR m_szCharacterName[64];	//게임캐릭터 이름
	USHORT m_usLevel;				//캐릭터 레벨
public:
	VOID SetGameInformation(LPCTSTR szCharacterName, USHORT usLevel);
	VOID OnUpdateUserID(VOID);
};

//웹에서 사용하는 사용자 클래스
class CWebUser : public CUser
{
private:
	TCHAR m_szBoardName[64];			//게시판 필명
public:
	VOID SetWebInformation(LPCTSTR szBoardName);
	VOID OnUpdateUserID(VOID);
};