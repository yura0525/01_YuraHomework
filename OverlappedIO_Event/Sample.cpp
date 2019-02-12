#include <Windows.h>
#include <tchar.h>
#include <stdio.h>
#include <assert.h>

#define BUF_SIZE 1024

int main()
{
	HANDLE			h = INVALID_HANDLE_VALUE;
	char			buf[BUF_SIZE] = { 0, };
	char			buf2[BUF_SIZE] = { 0, };
	char			buf3[BUF_SIZE] = { 0, };

	int				i = 0;
	LARGE_INTEGER	nLarge = { 0, };
	char			temppath[MAX_PATH + 1] = { 0, };
	OVERLAPPED		ov = { 0, };
	DWORD			dwValue = 0;

	// abc...xy�� �ݺ�
	for (i = 0; i < 1024; i++)
	{
		buf[i] = 'a' + i % ('z' - 'a');
	}

	// ABC...XY�� �ݺ�
	for (i = 0; i < 1024; i++)
	{
		buf2[i] = 'A' + i % ('Z' - 'A');
	}

	// �ӽ� ��� ����
	//::GetTempPathA(MAX_PATH, temppath);
	//strncat_s(temppath, "\\_test_.txt", MAX_PATH);

	// %temp%_test_.txt ����
	// FILE_FLAG_NO_BUFFERING�� OR�Ǹ� ������ �������� �ȴٰ� �Ѵ�.
	h = ::CreateFileA("test.txt", GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 
		FILE_FLAG_OVERLAPPED,	// | FILE_FLAG_NO_BUFFERING �� OR�Ǿ� ������, ReadFile���� sector ũ����� ����, File Pointer ��ġ�� ������ ������.
		NULL);
	
	if (INVALID_HANDLE_VALUE == h)
	{
		assert(FALSE);
		exit(1);
	}

	// i/o completion �߻��� event�� signal ��
	// auto-reset mode�̱� ������, WaitForSingleObject���� �ѹ� ����ϸ�,
	// �ڵ����� non-signaled ���·� ��ȯ��
	ov.hEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);

	//////////////////////////////////////////////////////////////////////////
	// Overlapped i/o�� ���� ����

	// Overlapped i/o�� ����. ��� ���ϵȴ�.
	// FILE_FLAG_NO_BUFFERING ���� sector ũ���� ��� (512 x 2 = 1024)�� Write�ؾ� �Ѵ�.
	if (FALSE == ::WriteFile(h, buf, 1024, NULL, &ov))
	{
		dwValue = ::GetLastError();
		if (dwValue == ERROR_IO_PENDING)
		{
			// i/o�� �Ϸ���� �ʾ���
			// ����
		}
		else
		{
			// ���� ��Ȳ
			assert(FALSE);
			exit(1);
		}
	}
	else
	{
		// �������� ���� ���� ��Ȳ
		assert(FALSE);
		exit(1);
	}

	// �̰����� CPU ���Ǵ� �ڵ带 �����ϸ� �ȴ�.
	{

	}

	// I/O�� �Ϸ�� �� ���� ��ٸ���.
	::WaitForSingleObject(ov.hEvent, INFINITE);

	// I/O�� ����� ����޴´�.
	if (FALSE == ::GetOverlappedResult(h, &ov, &dwValue, TRUE))
	{
		// ���� ��Ȳ
		assert(FALSE);
		exit(1);
	}

	// ���� File Pointer�� ����Ѵ�.
	// LARGE_INTER�� QuadPart�� �̿��ϸ� ���ϴ�.
	nLarge.QuadPart += 1024;

	// LARGE_INTEGER ���� ������� File Pointer ��ġ�� �����Ѵ�.
	ov.Offset = nLarge.LowPart;
	ov.OffsetHigh = nLarge.HighPart;

	// Overlapped i/o�� ����. ��� ���ϵȴ�.
	// FILE_FLAG_NO_BUFFERING ���� sector ũ���� ��� (512 x 2 = 1024)�� Write�ؾ� �Ѵ�.
	if (FALSE == ::WriteFile(h, buf2, 1024, NULL, &ov))
	{
		dwValue = ::GetLastError();
		if (dwValue == ERROR_IO_PENDING)
		{
			// i/o�� �Ϸ���� �ʾ���
			// ����
		}
		else
		{
			// ���� ��Ȳ
			assert(FALSE);
			exit(1);
		}
	}
	else
	{
		// �������� ���� ���� ��Ȳ
		assert(FALSE);
		exit(1);
	}

	// �̰����� CPU ���Ǵ� �ڵ带 �����ϸ� �ȴ�.
	{

	}

	// I/O�� �Ϸ�� �� ���� ��ٸ���.
	// ����, MsgWaitForMultipleObjects(...)�� ����ϸ�,
	// UI�� block�� ���� �� �־� �����Ѵ�.
	// ���� ��Ҹ� ���Ѵٸ�, ::CancelIo(...)�� ����� �� �ִ�.
	::WaitForSingleObject(ov.hEvent, INFINITE);

	// I/O�� ����� ����޴´�.
	if (FALSE == ::GetOverlappedResult(h, &ov, &dwValue, TRUE))
	{
		// ���� ��Ȳ
		assert(FALSE);
		exit(1);
	}

	// �������,
	// abc....xyabc...xy... �� 1024 byte
	// ABC....XYABC...XY... �� 1024 byte
	// �� 2048 byte�� ����Ǿ���.
	printf("2048 bytes written.\r\n");

	//////////////////////////////////////////////////////////////////////////
	// Overlapped i/o�� ���� �б�

	// 1byte ��ġ ���� �о��.
	// �б�� WriteFile ó��, sector ����� ������ �ʿ� ����.
	// ���⼭�� File Pointer�� ���� 1byte, ���� ũ��� 10byte�� �Ѵ�.
	// Overlapped i/o�� �ϱ� ������, �ٷ� ���ϵȴ�.
	// ����, FILE_FLAG_NO_BUFFERING�� OR�Ǿ��ٸ�, �Ʒ��� 87�� ����(=ERROR_INVALID_PARAMETER)
	// �� ���´�.

	nLarge.QuadPart = 0;
	ov.Offset = nLarge.LowPart;
	ov.OffsetHigh = nLarge.HighPart;
	if (FALSE == ::ReadFile(h, buf3, 10, NULL, &ov))
	{
		dwValue == ::GetLastError();
		if (dwValue == ERROR_IO_PENDING)
		{
			// i/o�� �Ϸ���� �ʾ���
			// ����
		}
		else
		{
			// ���� ��Ȳ
			/*assert(FALSE);
			exit(1);*/
		}
	}
	else
	{
		// �������� ���� ���� ��Ȳ
		assert(FALSE);
		exit(1);
	}

	// �̰����� CPU ���Ǵ� �ڵ带 �����ϸ� �ȴ�.
	{

	}

	// I/O�� �Ϸ�� �� ���� ��ٸ���.
	::WaitForSingleObject(ov.hEvent, INFINITE);

	// I/O�� ����� ����޴´�.
	if (FALSE == ::GetOverlappedResult(h, &ov, &dwValue, TRUE))
	{
		// ���� ��Ȳ
		assert(FALSE);
		exit(1);
	}

	printf("Readed : %s\r\n", buf3);

	ZeroMemory(buf3, sizeof(buf3));

	// EOF ��ó���� �ѹ� �о� ����.
	// ��� ���ϵȴ�.
	nLarge.QuadPart = 2048 - 5;
	ov.Offset = nLarge.LowPart;
	ov.OffsetHigh = nLarge.HighPart;
	
	if (FALSE == ::ReadFile(h, buf3, 1024, NULL, &ov))
	{
		dwValue = ::GetLastError();
		if (dwValue == ERROR_IO_PENDING)
		{
			// i/o�� �Ϸ���� �ʾ���
			// ����
		}
		else
		{
			// ���� ��Ȳ
			assert(FALSE);
			exit(1);
		}
	}
	else
	{
		// �������� ���� ���� ��Ȳ
		assert(FALSE);
		exit(1);
	}

	// I/O�� �Ϸ�� �� ���� ��ٸ���.
	::WaitForSingleObject(ov.hEvent, INFINITE);

	// I/O�� ����� ����޴´�.
	if (FALSE == ::GetOverlappedResult(h, &ov, &dwValue, TRUE))
	{
		// ���� ��Ȳ
		assert(FALSE);
		exit(1);
	}

	printf("Readed : %s, Byte Transfered : %d\r\n", buf3, dwValue);

	// EOF ��ġ���� �о� ����.
	nLarge.QuadPart = 2048;
	ov.Offset = nLarge.LowPart;
	ov.OffsetHigh = nLarge.HighPart;
	if (FALSE == ::ReadFile(h, buf3, 1024, NULL, &ov))
	{
		dwValue = ::GetLastError();
		if (dwValue == ERROR_IO_PENDING)
		{
			// EOF �������� �о, ������ ���� ���� ��������,
			// I/O Pending�� ���´�.
			// ����
		}
		else
		{
			// ���� ��Ȳ
			assert(FALSE);
			exit(1);
		}
	}
	else
	{
		// �������� ���� ���� ��Ȳ
		assert(FALSE);
		exit(1);
	}
	// I/O�� �Ϸ�� �� ���� ��ٸ���.
	::WaitForSingleObject(ov.hEvent, INFINITE);

	// I/O�� ����� ����޴´�.
	if (FALSE == ::GetOverlappedResult(h, &ov, &dwValue, TRUE))
	{
		dwValue = ::GetLastError();
		if (dwValue == ERROR_HANDLE_EOF)
		{
			// EOF�� ���� ������
			// ����
		}
		else
		{
			// �������� ���� ���� ��Ȳ
			assert(FALSE);
			exit(1);
		}
	}
	else
	{
		// ������ �� ����
		// �������� ���� ���� ��Ȳ
		assert(FALSE);
		exit(1);
	}

	printf("Read Seek at %d, ERROR_HANDLE_EOF returned, Byte Transfered : %d\r\n", nLarge.QuadPart, dwValue);

	if (NULL != ov.hEvent)
	{
		::CloseHandle(ov.hEvent);
		ov.hEvent = NULL;
	}

	if (INVALID_HANDLE_VALUE != h)
	{
		::CloseHandle(h);
		h = INVALID_HANDLE_VALUE;
	}

	return 0;
}