#pragma once
#include <Windows.h>
const INT C1 = 52845;
const INT C2 = 22719;
const INT KEY = 72957;

class CCrypt
{
public:
	//��ȣȭ�� �ϴ� �Լ��Դϴ�.
	static BOOL Encrypt(BYTE* source, BYTE* destination, DWORD length)
	{
		DWORD i;
		INT Key = KEY;

		if (!source || !destination || length <= 0)
			return FALSE;
		for (i = 0; i < length; i++)			//����Ʈ ������ ��ȣȭ�� �ϰ� �˴ϴ�.
		{
			//1����Ʈ�� Key����ŭ ������ ���ְ� 8��Ʈ�� �о��ݴϴ�.
			destination[i] = source[i] ^ Key >> 8;

			//���� ����Ʈ������ ����� Ű�� C1, C2�� �̿��ؼ� �������ݴϴ�.
			Key = (destination[i] + Key) * C1 + C2;
		}
		return TRUE;
	}
	//��ȣȭ�� �ϴ� �Լ��Դϴ�.
	static BOOL Decrypt(BYTE* source, BYTE* destination, DWORD length)
	{
		DWORD i;
		BYTE PreviousBlock;
		INT Key = KEY;

		if (!source || !destination || length <= 0)
			return FALSE;

		//����Ʈ ������ ��ȣȭ�� �ϰ� �˴ϴ�.
		for (i = 0; i < length; i++)
		{
			PreviousBlock = source[i];
			destination[i] = source[i] ^ Key >> 8;
			//���� ����� �����صξ��� ��ȣȭ�� ���Ǵ� Ű�� �˾Ƴ��ϴ�.
			Key = (PreviousBlock + Key) * C1 + C2;
		}
	}
public:
	CCrypt()
	{

	}
	virtual ~CCrypt()
	{

	}
};

