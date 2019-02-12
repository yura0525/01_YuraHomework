#include <Windows.h>
#include <tchar.h>
#include <stdio.h>

#define BUF_SIZE 61

// Routine Description:
//      Retrieve the system error message for the last-error code
LPCTSTR ErrorMessage(DWORD error)
{
	LPVOID lpMsgBuf;

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);

	return ((LPCTSTR)lpMsgBuf);
}

// Routine Description:
//     Placeholder to demo when async I/O might want to do
//     other processing.
void GoDoSometingElse(void)
{
	printf("Inside GoDoSomethingElse()\n");
}

// Routine Description:
//      Demonstrate async ReadFile operations that can catch
//      End-of-file conditions. Unless the operation completes
//      synchronously or the file size happens to be an exact
//      multiple of BUF_SIZE, this routine will eventually force
//      an EOF condition on any file.

// Parameters:
//      hEvent - pre-made manual-reset event.
//
//      hFile - pre-opened file handle, overlapped.
//
//      inBuffer - the buffer to read in the data to.
//
//      nBytesToRead - how much to read (usually the buffer size).

// Return Value:
//      Number of bytes read.
DWORD AsyncTestForEnd(HANDLE hEvent, HANDLE hFile)
{
	char inBuffer[BUF_SIZE];
	DWORD nBytesToRead = BUF_SIZE;
	DWORD dwBytesRead = 0;
	DWORD dwFileSize = GetFileSize(hFile, NULL);
	OVERLAPPED stOverlapped = { 0 };

	DWORD dwError = 0;
	LPCTSTR errMsg = NULL;

	BOOL bResult = FALSE;
	BOOL bContinue = TRUE;

	// Set up overlapped structure event. Other members are already 
	// initialized to zero.
	stOverlapped.hEvent = hEvent;

	// This is an intentionally brute-force loop to force the EOF trigger.
	// A properly designed loop for this simple file read would use the
	// GetFileSize API to regulate execution. However, the purpose here
	// is to demonstrate how to trigger the EOF error and handle it.

	while (bContinue)
	{
		// Default to ending the loop.
		bContinue = FALSE;

		// Attempt an asynchronous read operation.
		bResult = ReadFile(hFile, inBuffer, nBytesToRead, &dwBytesRead, &stOverlapped);

		dwError = GetLastError();

		// Check for a problem or pending operation. 
		if (!bResult)
		{
			switch (dwError)
			{
			case ERROR_HANDLE_EOF:
			{
				printf("\nReadFile returned FALSE and EOF condition, async EOF not triggered.\n");
				break;
			}

			case ERROR_IO_PENDING:
			{
				BOOL bPending = TRUE;
				// Loop until the I/O is complete, that is: the overlapped 
				// event is signaled.

				while (bPending)
				{
					bPending = FALSE;
					// Pending asynchronous I/O, do something else
					// and re-check overlapped structure.
					printf("\nReadFile operation is pending\n");

					// Do something else then come back to check. 
					GoDoSometingElse();
					//////////////  ERROR_HANDLE_EOF //////////////////
					// �񵿱� Read�ÿ��� GetOverlappedResult=FALSE�̸� ERROR_HANDLE_EOF�� 
					// ��ȯ�Ѵ�.
					// ����� ���� Read=TRUE�� ��ȯ�ϸ�  pNumberOfBytesRead=0�� �ȴ�.
					bResult = GetOverlappedResult(hFile, &stOverlapped, &dwBytesRead, FALSE);

					if (!bResult)
					{
						switch (dwError = GetLastError())
						{
						case ERROR_HANDLE_EOF:
							// Handle an end of file
							printf("GetOverlappedResult found EOF\n");
							break;
						case ERROR_IO_INCOMPLETE:
						{
							// Operation is still pending, allow while loop
							// to loop again after printing a little progress.
							printf("GetOverlappedResult I/O Incomplete\n");
							bPending = TRUE;
							bContinue = TRUE;
							break;
						}
						default:
						{
							// Decode any other errors codes.
							errMsg = ErrorMessage(dwError);
							_tprintf(TEXT("GetOverlappedResult failed (%d): %s\n"),
								dwError, errMsg);
							LocalFree((LPVOID)errMsg);
						}
						}
					}
					else
					{
						printf("ReadFile operation completed %d->%d\n", stOverlapped.Offset,
							dwBytesRead);

						// Manual-reset event should be reset since it is now signaled.
						ResetEvent(stOverlapped.hEvent);
					}
				}
				break;
			}
			default:
			{
				// Decode any other errors codes.
				errMsg = ErrorMessage(dwError);
				printf("ReadFile GLE unhandled (%d): %s\n", dwError, errMsg);
				LocalFree((LPVOID)errMsg);
				break;
			}
			}
		}
		else
		{
			printf("ReadFile completed synchronously\n");
		}
		stOverlapped.Offset += dwBytesRead;
		if (stOverlapped.Offset < dwFileSize)
			bContinue = TRUE;
	}
	return stOverlapped.Offset;
}

void main()
{
	HANDLE hEvent;
	HANDLE hFile;
	DWORD dwReturnValue;

	hFile = CreateFile(L"bigdata.zip", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		DWORD dwError = GetLastError();
		LPCTSTR errMsg = ErrorMessage(dwError);
		printf("Could not open file (%d): %s\n", dwError, errMsg);
		LocalFree((LPVOID)errMsg);
		return;
	}

	hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (hEvent == NULL)
	{
		DWORD dwError = GetLastError();
		LPCTSTR errMsg = ErrorMessage(dwError);
		printf("Could not CreateEvent: %d %s\n", dwError, errMsg);
		LocalFree((LPVOID)errMsg);
		return;
	}

	dwReturnValue = AsyncTestForEnd(hEvent, hFile);

	printf("\nRead complete. Bytes read: %d\n", dwReturnValue);
	
	CloseHandle(hFile);
	CloseHandle(hEvent);
}