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
	CObject* Object = new CObject;		//��ü�� ����

	Object = NULL;						//��ü�� �����͸� NULL�� ����
	//Object->m_IsOpened = FALSE;			//�����Ͱ� NULL�� ��ü�� ��� ȣ��(����!)

	BYTE* pData = new BYTE[1024];
	ZeroMemory(pData, 1024 * sizeof(BYTE));
}

int main()
{
	CMiniDump::Begin();					//MiniDump ����� �����Ѵ�.
	UnhandledExceptionFunction();		//������ ����� �Լ�
	CMiniDump::End();

	
	getchar();
	return 0;
}