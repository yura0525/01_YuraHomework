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
				printf("\nReadFile returned FALSE and EOF condition, async EOF not triggered.\n");
				break;
			case ERROR_IO_PENDING:
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
					// 비동기 Read시에는 GetOverlappedResult=FALSE이며 ERROR_HANDLE_EOF을 
					// 반환한다.
					// 참고로 동기 Read=TRUE을 반환하며  pNumberOfBytesRead=0이 된다.

				}
			}
		}

	}
}