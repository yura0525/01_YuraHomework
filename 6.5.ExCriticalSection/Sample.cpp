#include <windows.h>
#include <stdio.h>
#define MAXCNT 100000000

CRITICAL_SECTION cs;
int g_count = 0;

DWORD WINAPI MyThread1(LPVOID arg)
{
	for (int i = 0; i < MAXCNT; i++)
	{
		EnterCriticalSection(&cs);
		g_count += 2;
		LeaveCriticalSection(&cs);
	}
	return 0;
}

DWORD WINAPI MyThread2(LPVOID arg)
{
	for (int i = 0; i < MAXCNT; i++)
	{
		EnterCriticalSection(&cs);
		g_count -= 2;
		LeaveCriticalSection(&cs);
	}
	return 0;
}

int main()
{
	//�Ӱ� ���� �ʱ�ȭ
	InitializeCriticalSection(&cs);

	//������ �ΰ� ����
	HANDLE hThread[2];
	hThread[0] = CreateThread(NULL, 0, MyThread1, NULL, 0, NULL);
	hThread[1] = CreateThread(NULL, 0, MyThread2, NULL, 0, NULL);

	//������ �ΰ� ���� ���
	WaitForMultipleObjects(2, hThread, TRUE, INFINITE);

	DeleteCriticalSection(&cs);

	//��� ���
	printf("g_count = %d\n", g_count);
	return 0;
}