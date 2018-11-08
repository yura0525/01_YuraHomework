
#include <iostream>
#include <Windows.h>

int number = 0;
HANDLE hMutex;

DWORD WINAPI ThreadIncrement(void* arg)
{
	for (int i = 0; i < 5; i++)
	{
		WaitForSingleObject(hMutex, INFINITE);
		Sleep(100);
		number++;
		printf("run : %s, number = %d\n", (char*)arg, number);
		ReleaseMutex(hMutex);
	}
	return 0;
}

void main()
{
	HANDLE hThread1, hThread2;
	DWORD dwThreadID1, dwThreadID2;

	hMutex = CreateMutex(NULL, FALSE, NULL);

	if (hMutex == NULL)
	{
		printf("Failed CreateMutex()\n");
		return;
	}

	hThread1 = (HANDLE)CreateThread(NULL, 0, ThreadIncrement, (void*)"A", 0, &dwThreadID1);
	hThread2 = (HANDLE)CreateThread(NULL, 0, ThreadIncrement, (void*)"B", 0, &dwThreadID2);

	if (hThread1 == NULL || hThread2 == NULL)
	{
		printf("Failed CreateThread()\n");
		return;
	}

	if (WaitForSingleObject(hThread1, INFINITE) == WAIT_FAILED)
	{
		printf("Failed WaitForSingleObject() == WAIT_FAILED\n");
	}

	if (WaitForSingleObject(hThread2, INFINITE) == WAIT_FAILED)
	{
		printf("Failed WaitForSingleObject() == WAIT_FAILED\n");
	}

	printf("Sum number = %d\n", number);
	CloseHandle(hMutex);
	getchar();
	return;
}
