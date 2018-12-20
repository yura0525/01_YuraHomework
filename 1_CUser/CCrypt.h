#pragma once
#include <Windows.h>
const INT C1 = 52845;
const INT C2 = 22719;
const INT KEY = 72957;

class CCrypt
{
public:
	//암호화를 하는 함수입니다.
	static BOOL Encrypt(BYTE* source, BYTE* destination, DWORD length)
	{
		DWORD i;
		INT Key = KEY;

		if (!source || !destination || length <= 0)
			return FALSE;
		for (i = 0; i < length; i++)			//바이트 단위로 암호화를 하게 됩니다.
		{
			//1바이트를 Key값만큼 제곱을 해주고 8비트를 밀어줍니다.
			destination[i] = source[i] ^ Key >> 8;

			//다음 바이트에서는 사용할 키를 C1, C2를 이용해서 변경해줍니다.
			Key = (destination[i] + Key) * C1 + C2;
		}
		return TRUE;
	}
	//복호화를 하는 함수입니다.
	static BOOL Decrypt(BYTE* source, BYTE* destination, DWORD length)
	{
		DWORD i;
		BYTE PreviousBlock;
		INT Key = KEY;

		if (!source || !destination || length <= 0)
			return FALSE;

		//바이트 단위로 복호화를 하게 됩니다.
		for (i = 0; i < length; i++)
		{
			PreviousBlock = source[i];
			destination[i] = source[i] ^ Key >> 8;
			//이전 블록을 저장해두었다 복호화에 사용되는 키를 알아냅니다.
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

