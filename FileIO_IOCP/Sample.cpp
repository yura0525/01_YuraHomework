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
	::ResetEvent(m_hEventKillThread);	//�������� �̺�Ʈ ��ñ׳� ���·� �����.

	//OPEN_EXISTING : �������� ������ �ι߸��� �ڵ��� �����Ѵ�.
	m_hFileRead = CreateFile(L"../../data_1.zip", GENERIC_READ, 0, 0, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 0);

	//CREATE_ALWAYS �Ź� ���� ����
	m_hFileWrite = CreateFile(L"copy.zip", GENERIC_WRITE, 0, 0, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 0);

	//iocp�� ���
	m_hIOCP = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);

	//������ 4���� ���
	DWORD id;
	for (int i = 0; i < MAX_WORK_THREAD; i++)
	{
		m_hWorkThread[i] = ::CreateThread(0, 0, WorkThread, this, 0, &id);
	}

	//������ (�۾���)���� Read���Ͽ� ���� �д� ����� �Ϸ�Ǹ�, �� ����� �Ϸ�Ű (ex)1111)�� �������� �˷����.
	//�Ϸ�Ű�� �����ؾ��Ѵ�. �����ؾ��ϹǷ� �ڵ�(������)���� �ִ´�.
	::CreateIoCompletionPort(m_hFileRead, m_hIOCP, (ULONG_PTR)m_hFileRead, 0);
	//������ (�۾���)���� Write���Ͽ� ���� ���� ����� �Ϸ�Ǹ� , �� ����� �Ϸ�Ű (ex)2222)�� �������� �˷����.
	::CreateIoCompletionPort(m_hFileWrite, m_hIOCP, (ULONG_PTR)m_hFileWrite, 0);
}

void TIOCP::Frame()
{
	WaitForRead();

	//��� �����尡 �� ���������� ��ȯ���� �ʴ´�.
	//�����尡 �Ϸ�Ȱ� �˷��ִ� �Լ�. ����Լ�.
	while (WaitForMultipleObjects(MAX_WORK_THREAD, m_hWorkThread, TRUE, 1));

	Release();
}

bool TIOCP::WaitForRead()
{
	DWORD dwReadBytes = 0;
	if (::ReadFile(m_hFileRead, m_szReadBuffer, MAX_READ_SIZE, &dwReadBytes, &m_hReadOV) == FALSE)
	{
		//�߿�!!!!:�������� ���. �񵿱� ������� ����ǰ� ����.
		if (GetLastError() == ERROR_IO_PENDING)
		{
			return true;
		}
		return false;
	}
	else
	{
		//���� ����Ʈ���� 0����Ʈ�̰ų� ������ ��(ERROR_HANDLE_EOF)�� ���
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

		//::���� ���� ������.
		//����� ���� �Լ��� ���� �Լ��� �̸��� ������� ���Լ��� ���� Ŭ���� ���� �Լ��� �ƴ϶� 
		//����(����)�� �ִٰ� �˷��ִ� ������.

		//�񵿱� ������� �۾� ����� Ȯ���ϴ� �Լ�.//IOCP QUEUE
		bool bRet = ::GetQueuedCompletionStatus(iocp->m_hIOCP, &bytesTransfer, &keyValue, (LPOVERLAPPED*)&ov, 1);

		if (bRet == TRUE)
		{
			//�����Ұ� ���� ��� => �̺�Ʈ�� �ñ׳� ���·� �ٲ㼭 main�� while()���� ����������.
			if (bytesTransfer == 0)
			{
				::SetEvent(iocp->m_hEventKillThread);
				return true;
			}

			//WaitForRead()�Լ������� ReadFile�� �ٳ��� ��� => iocp->DispatchRead()
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
		//�߿�:!! GetLastError() != ERROR_IO_PENDING�̸� ���������� ����̴�.
		if (GetLastError() != ERROR_IO_PENDING)
		{
			return false;
		}
	}
	//�� ������ �о �� ���� ������ ������� �˷��ִ� �κ�.(m_hReadOV.offset�� ���)
	LARGE_INTEGER data;
	data.QuadPart = dwTransfer;

	m_hReadOV.Offset += data.LowPart;
	m_hReadOV.OffsetHigh += data.HighPart;

	WaitForRead();
	return true;
}

bool TIOCP::DispatchWrite(DWORD dwTransfer)
{
	//�� ������ �Ἥ �� �������� ����� �κ�.
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