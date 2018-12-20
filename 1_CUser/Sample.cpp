#include "CUser.h"
#include "CStream.h"
#include "CMiniDump.h"
#include "MemoryLeak.h"
#include <iostream>

class CObject
{
public:
	BOOL m_IsOpened;
};

VOID UnhandledExceptionFunction(VOID)
{
	CObject* Object = new CObject;		//개체를 생성

	Object = NULL;						//개체의 포인터를 NULL로 변경
	//Object->m_IsOpened = FALSE;			//포인터가 NULL의 개체의 멤버 호출(에러!)

	BYTE* pData = new BYTE[1024];
	ZeroMemory(pData, 1024 * sizeof(BYTE));
}

int main()
{
	CMiniDump::Begin();					//MiniDump 사용을 선언한다.
	UnhandledExceptionFunction();		//문제가 생기는 함수
	CMiniDump::End();

	
	getchar();
	return 0;
}