#pragma once
#include <Windows.h>
#include <tchar.h>

#define ASYNCFLAG_READ 0x00
#define ASYNCFLAG_WRITE 0x01

#define READBUFFER_SIZE 2014 * 30 //  대규모 파일의 경우는 2014*30 정도 예)2.4기가
#define WORKERTHREAD_COUNT 5

class Sample
{
private:
	typedef struct OVERLAPPEDEX : OVERLAPPED
	{
		DWORD flag;
	}OVERLAPPEDEX;
private:
	HANDLE			m_hIOCP;
	HANDLE			m_hWorkerThread[WORKERTHREAD_COUNT];
	HANDLE			m_hFileRead;
	HANDLE			m_hFileWrite;
	HANDLE			m_hEventKillThread;
	OVERLAPPEDEX	m_hReadOV;
	OVERLAPPEDEX	m_hWriteOV;
	char			m_szReadBuffer[READBUFFER_SIZE];

public:
	bool			Init();
	void			Frame();
	void			Release();
private:
	static	DWORD	WINAPI WorkerThread(LPVOID parameter);
	bool	WaitForRead();
	bool	DispatchRead(DWORD transferred);
	bool	DispatchWrite(DWORD transferred);

public:
	Sample();
	virtual ~Sample();
};

