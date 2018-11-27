#include <windows.h>
#include <iostream>
#include <tchar.h>
#include <string.h>
#include <conio.h>

#define MAX_READ_SIZE 4096 * 100
#define MAX_WORK_THREAD 4

class TIOCP
{
public:
	HANDLE		m_hIOCP;
	HANDLE		m_hFileRead;
	HANDLE		m_hFileWrite;

	OVERLAPPED	m_hReadOV;
	OVERLAPPED	m_hWriteOV;
	HANDLE		m_hEventKillThread;
	HANDLE		m_hWorkThread[MAX_WORK_THREAD];
	char		m_szReadBuffer[MAX_READ_SIZE];

	bool m_bEndRead;
public:
	void Init();
	void Frame();
	static DWORD WINAPI WorkThread(LPVOID param);
	bool WaitForRead();
	bool DispatchRead(DWORD dwTranfer);
	bool DispatchWrite(DWORD dwTranfer);
public:
	TIOCP()
	{
		m_bEndRead = false;
	}
	void Release()
	{
		CloseHandle(m_hFileRead);
		CloseHandle(m_hFileWrite);
		for (int i = 0; i < MAX_WORK_THREAD; i++)
		{
			CloseHandle(m_hWorkThread[i]);
		}
		CloseHandle(m_hIOCP);
	}
};

void TIOCP::Init()
{
	m_bEndRead = false;
	m_hEventKillThread = ::CreateEvent(0, TRUE, FALSE, NULL);
	::ResetEvent(m_hEventKillThread);	//수동리셋 이벤트 논시그널 상태로 만든다.

	//OPEN_EXISTING : 존재하지 않으면 인발리드 핸들을 리턴한다.
	m_hFileRead = CreateFile(L"../../data_1.zip", GENERIC_READ, 0, 0, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 0);

	//CREATE_ALWAYS 매번 새로 생성
	m_hFileWrite = CreateFile(L"copy.zip", GENERIC_WRITE, 0, 0, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 0);

	//iocp를 고용
	m_hIOCP = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);

	//스레드 4개를 고용
	DWORD id;
	for (int i = 0; i < MAX_WORK_THREAD; i++)
	{
		m_hWorkThread[i] = ::CreateThread(0, 0, WorkThread, this, 0, &id);
	}

	//스레드 (작업자)에게 Read파일에 대한 읽는 결과가 완료되면, 그 결과를 완료키 (ex)1111)의 내용으로 알려줘라.
	//완료키는 유일해야한다. 유일해야하므로 핸들(포인터)값을 넣는다.
	::CreateIoCompletionPort(m_hFileRead, m_hIOCP, (ULONG_PTR)m_hFileRead, 0);
	//스레드 (작업자)에게 Write파일에 대한 쓰는 결과가 완료되면 , 그 결과를 완료키 (ex)2222)의 내용으로 알려줘라.
	::CreateIoCompletionPort(m_hFileWrite, m_hIOCP, (ULONG_PTR)m_hFileWrite, 0);
}

void TIOCP::Frame()
{
	WaitForRead();

	//모든 스레드가 다 끝날때까지 반환하지 않는다.
	//스레드가 완료된걸 알려주는 함수. 대기함수.
	while (WaitForMultipleObjects(MAX_WORK_THREAD, m_hWorkThread, TRUE, 1));

	Release();
}

bool TIOCP::WaitForRead()
{
	DWORD dwReadBytes = 0;
	if (::ReadFile(m_hFileRead, m_szReadBuffer, MAX_READ_SIZE, &dwReadBytes, &m_hReadOV) == FALSE)
	{
		//중요!!!!:정상적인 경우. 비동기 입출력이 진행되고 있음.
		if (GetLastError() == ERROR_IO_PENDING)
		{
			return true;
		}
		return false;
	}
	else
	{
		//읽은 바이트수가 0바이트이거나 파일의 끝(ERROR_HANDLE_EOF)일 경우
		if (dwReadBytes == 0 || GetLastError() == ERROR_HANDLE_EOF)
		{
			return false;
		}
	}
	return true;
}

DWORD WINAPI TIOCP::WorkThread(LPVOID param)
{
	TIOCP* iocp = (TIOCP*)param;

	OVERLAPPED ov;
	DWORD bytesTransfer;
	ULONG_PTR keyValue;

	while (1)
	{
		if (WAIT_OBJECT_0 == ::WaitForSingleObject(iocp->m_hEventKillThread, 1))
		{
			break;
		}

		//::범위 지정 연산자.
		//사용자 정의 함수와 전역 함수가 이름이 같은경우 이함수는 여기 클래스 내의 함수가 아니라 
		//상위(전역)에 있다고 알려주는 연산자.

		//비동기 입출력의 작업 결과를 확인하는 함수.//IOCP QUEUE
		bool bRet = ::GetQueuedCompletionStatus(iocp->m_hIOCP, &bytesTransfer, &keyValue, (LPOVERLAPPED*)&ov, 1);

		if (bRet == TRUE)
		{
			//전송할게 없는 경우 => 이벤트를 시그널 상태로 바꿔서 main의 while()문을 빠져나간ㄷ.
			if (bytesTransfer == 0)
			{
				::SetEvent(iocp->m_hEventKillThread);
				return true;
			}

			//WaitForRead()함수내에서 ReadFile이 다끝난 경우 => iocp->DispatchRead()
			if (keyValue == (ULONG_PTR)iocp->m_hFileRead)
			{
				iocp->DispatchRead(bytesTransfer);
			}
			else
			{
				iocp->DispatchWrite(bytesTransfer);
			}
		}
		else
		{
			if (GetLastError() == ERROR_HANDLE_EOF)
			{
				iocp->m_bEndRead = true;
				::SetEvent(iocp->m_hEventKillThread);
				return true;
			}

			if (GetLastError() == WAIT_TIMEOUT)
			{
				continue;
			}
		}
		
	}
}

bool TIOCP::DispatchRead(DWORD dwTransfer)
{
	DWORD dwWrite;

	if (::WriteFile(m_hFileWrite, m_szReadBuffer, dwTransfer, &dwWrite, (LPOVERLAPPED)&m_hWriteOV) == FALSE)
	{
		//중요:!! GetLastError() != ERROR_IO_PENDING이면 비정상적인 경우이다.
		if (GetLastError() != ERROR_IO_PENDING)
		{
			return false;
		}
	}
	//블럭 단위로 읽어서 그 다음 블럭부터 읽으라고 알려주는 부분.(m_hReadOV.offset을 사용)
	LARGE_INTEGER data;
	data.QuadPart = dwTransfer;

	m_hReadOV.Offset += data.LowPart;
	m_hReadOV.OffsetHigh += data.HighPart;

	WaitForRead();
	return true;
}

bool TIOCP::DispatchWrite(DWORD dwTransfer)
{
	//블럭 단위로 써서 그 다음부터 쓰라는 부분.
	LARGE_INTEGER data;
	data.QuadPart = dwTransfer;

	m_hWriteOV.Offset += data.LowPart;
	m_hWriteOV.OffsetHigh += data.HighPart;

	if (m_bEndRead)
	{
		::SetEvent(m_hEventKillThread);
		return true;
	}

	return true;
}
void main()
{
	TIOCP iocp;
	iocp.Init();
	iocp.Frame();
	_getch();
}