#include <iostream>
#include <conio.h>
#include "Sample.h"

void err_quit(const char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, (LPCWSTR)msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(-1);
}

void main()
{
	Sample iocp;
	if (iocp.Init() == false)
	{
		std::cout << _T("\nFailed to Init program...\n") << std::endl;

		return;
	}

	DWORD startTick = ::GetTickCount();
	iocp.Frame();

	std::cout << _T("\n\nUsing ") << WORKERTHREAD_COUNT << _T(" Worker thread") << std::endl;
	std::cout << ::GetTickCount() - startTick << _T(" millisecond passed") << std::endl;;

	_getch();
}

/////////////////////////////////////////
//
//  Class Sample

#define SAFE_CLOSEHANDLE(h)		{ if( h != INVALID_HANDLE_VALUE) ::CloseHandle(h);	}

bool Sample::Init()
{
	{
		//  IOCP를 사용하기 위해서는 FILE_FLAG_OVERLAPPED를 지정해 주어야 한다.
		m_hFileRead = ::CreateFile(_T("../../data/004.flac"), GENERIC_READ, 0, 0, 
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 0);
		if (m_hFileRead == INVALID_HANDLE_VALUE)
			return false;

		m_hFileWrite = ::CreateFile(_T("004Copy.flac"), GENERIC_WRITE, 0, 0, 
			CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 0);
		if (m_hFileWrite == INVALID_HANDLE_VALUE)
			return false;
	}

	{
		//  I/O Completion Port 생성
		m_hIOCP = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
		if (m_hIOCP == INVALID_HANDLE_VALUE)
			return false;

		//Worker Thread 생성
		DWORD threadID;
		for (int i = 0; i < WORKERTHREAD_COUNT; i++)
		{
			m_hWorkerThread[i] = ::CreateThread(0, 0, WorkerThread, this, 0, &threadID);
		}

		//Thread Kill Event 생성
		m_hEventKillThread = ::CreateEvent(0, TRUE, FALSE, 0);
		::ResetEvent(m_hEventKillThread);
	}

	//  IOCP에 핸들 등록하기
	{
		::CreateIoCompletionPort(m_hFileRead, m_hIOCP, (DWORD)m_hFileRead, 0);
		::CreateIoCompletionPort(m_hFileWrite, m_hIOCP, (DWORD)m_hFileWrite, 0);
	}
	return true;
}
void Sample::Frame()
{
	//  구조체를 초기화 한다.
	::ZeroMemory(&m_hWriteOV, sizeof(m_hWriteOV));
	::ZeroMemory(&m_hReadOV, sizeof(m_hReadOV));

	m_hReadOV.flag = ASYNCFLAG_READ;
	m_hWriteOV.flag = ASYNCFLAG_WRITE;

	WaitForRead();

	//  모든 Worker thread가 종료될 때 까지 기다린다.
	while (::WaitForMultipleObjects(WORKERTHREAD_COUNT, m_hWorkerThread, TRUE, 1));

	Release();
}

void Sample::Release()
{
	SAFE_CLOSEHANDLE(m_hFileRead);
	SAFE_CLOSEHANDLE(m_hFileWrite);
	SAFE_CLOSEHANDLE(m_hIOCP);
	SAFE_CLOSEHANDLE(m_hEventKillThread);
}
DWORD WINAPI Sample::WorkerThread(LPVOID parameter)
{
	Sample*			iocp = (Sample*)parameter;
	HANDLE			m_hIOCP = iocp->m_hIOCP;
	DWORD			bytesTransfer;
	ULONG_PTR		keyValue;
	OVERLAPPEDEX*	overlapped;
	BOOL			retVal;

	while (1)
	{
		if (::WaitForSingleObject(iocp->m_hEventKillThread, 1) == WAIT_OBJECT_0)
			break;

		keyValue = 0;
		overlapped = 0;
		retVal = ::GetQueuedCompletionStatus(m_hIOCP, &bytesTransfer, &keyValue, (LPOVERLAPPED*)&overlapped, 1);

		PULONG_PTR A;
		if (retVal == TRUE)
		{
			if (bytesTransfer != 0)			// 완료 패킷을 성공적으로 Dequeue했다.
			{
				if (keyValue != 0 && overlapped != 0)
				{
					//  완료된 작업이 Read인지 Write인지 판단하는 기준은 OverlappedEx의 Flag이다.
					if (overlapped->flag == ASYNCFLAG_READ)
					{
						iocp->DispatchRead(bytesTransfer);
					}
					else if (overlapped->flag == ASYNCFLAG_WRITE)
					{
						iocp->DispatchWrite(bytesTransfer);
					}
				}
			}
			else
			{
				err_quit("ERROR");
				break;
			}
		}
		else
		{
			DWORD dwError = GetLastError();
			if (dwError == WAIT_TIMEOUT)	// 시간이 경과되었다.
			{
				continue;
			}
			else
			{
				if (overlapped != NULL)		// 완료 패킷을 Dequeue 했지만 실패한 IO이다.
				{
					// 파일의끝이다.
					if (ERROR_HANDLE_EOF == dwError)
					{
						::SetEvent(iocp->m_hEventKillThread);
					}
				}
				else// 완료 패킷을 Dequeue 못했다. 
				{
					// 실패한 io
					err_quit("ERROR");
					break;
				}
			}
		}
	}
	return 0;
}
bool	Sample::WaitForRead()
{
	DWORD readBytes = 0;
	DWORD bufSize = READBUFFER_SIZE;

	if (::ReadFile(m_hFileRead, m_szReadBuffer, bufSize, &readBytes, (LPOVERLAPPED)&m_hReadOV) == FALSE)
	{
		DWORD lastError = ::GetLastError();
		if (lastError == ERROR_IO_PENDING)
			return true;

		return false;
	}
	else
	{
		return false;
	}

	return true;
}
bool Sample::DispatchRead(DWORD transferred)
{
	LARGE_INTEGER lr;
	lr.QuadPart = transferred;
	m_hReadOV.Offset += lr.LowPart;
	m_hReadOV.OffsetHigh += lr.HighPart;

	std::cout << m_hReadOV.Offset << _T(" bytes read.") << std::endl;

	//  읽어들인 내용을 저장한다.
	{
		DWORD writtenBytes = 0;
		if (::WriteFile(m_hFileWrite, m_szReadBuffer, transferred, &writtenBytes, (LPOVERLAPPED)&m_hWriteOV) == FALSE)
		{
			DWORD lastError = ::GetLastError();
			if (lastError != ERROR_IO_PENDING && lastError != ERROR_SUCCESS)
			{
				std::cout << _T("Error occured in write file!!") << std::endl;
				return false;
			}
		}
	}
	//  읽기 작업을 새로 시작한다.
	if (WaitForRead() == false)
	{
		err_quit("ERROR");
		return false;
	}
	return true;

}
bool Sample::DispatchWrite(DWORD transferred)
{
	LARGE_INTEGER lr;
	lr.QuadPart = transferred;
	m_hWriteOV.Offset += lr.LowPart;
	m_hWriteOV.OffsetHigh += lr.HighPart;

	std::cout << m_hWriteOV.Offset << _T(" bytes write.") << std::endl;
	return true;
}

Sample::Sample() : m_hFileRead(INVALID_HANDLE_VALUE), m_hFileWrite(INVALID_HANDLE_VALUE),
					m_hIOCP(INVALID_HANDLE_VALUE), m_hEventKillThread(INVALID_HANDLE_VALUE)
{
	for (int i = 0; i < WORKERTHREAD_COUNT; i++)
	{
		m_hWorkerThread[i] = INVALID_HANDLE_VALUE;
	}
}


Sample::~Sample()
{
}
