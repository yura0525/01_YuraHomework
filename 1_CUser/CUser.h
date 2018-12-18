#pragma once
#include <tchar.h>
#include <Windows.h>

class CUser
{
private:
	TCHAR m_szUserID[64];	//���̵�
	TCHAR m_szPassword[64];	//��ȣ

	TCHAR m_szName[64];		//�Ǹ�
	TCHAR m_szAddress[64];	//�ּ�

	USHORT m_usAge;			//����
public:
	CUser();
	virtual ~CUser();
	//�����Լ��� protected�� �����մϴ�.
protected:
	virtual VOID OnUpdateUserID(VOID) = 0;
public:
	VOID SetUserID(LPCTSTR szUserID, LPCTSTR szPassword);
	VOID SetUserInformation(LPCTSTR szName, LPCTSTR szAddress, USHORT usAge);
};

//���ӿ��� ����ϴ� ����� Ŭ����
class CGameUser : public CUser
{
private:
	TCHAR m_szCharacterName[64];	//����ĳ���� �̸�
	USHORT m_usLevel;				//ĳ���� ����
public:
	VOID SetGameInformation(LPCTSTR szCharacterName, USHORT usLevel);
	VOID OnUpdateUserID(VOID);
};

//������ ����ϴ� ����� Ŭ����
class CWebUser : public CUser
{
private:
	TCHAR m_szBoardName[64];			//�Խ��� �ʸ�
public:
	VOID SetWebInformation(LPCTSTR szBoardName);
	VOID OnUpdateUserID(VOID);
};