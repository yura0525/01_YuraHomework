#pragma once
#include <Windows.h>
#define MAX_BUFFER_LENGTH 2048

enum IO_TYPE
{
	IO_ACCEPT,
	IO_READ,
	IO_WRITE
};

typedef struct _OVERLAPPED_EX
{
	OVERLAPPED overlapped;	//원래 구조체
	IO_TYPE IoType;			//이 구조체가 하는일
							//IO_TYPE의 종류
							//IO_READ : 읽기 작업을 할때 사용
							//IO_WRITE : 쓰기 작업을 할때 사용
							//IO_ACCEPT : Accept를 받았을때 사용
	VOID *Object;			//부모개체 주소
}OVERLAPPED_EX;


class CNetworkSession
{
public:
	OVERLAPPED_EX m_AcceptOverlapped;			//Accept에 관련된 Overlapped구조체
	OVERLAPPED_EX m_ReadOverlapped;				//Read에 관련된 Overlapped구조체
	OVERLAPPED_EX m_WriteOverlapped;			//Write에 관련된 Overlapped구조체
	char m_ReadBuffer[MAX_BUFFER_LENGTH];		//Read에서 사용하는 버퍼
	HANDLE m_Socket;							//소켓 핸들 값을 저장

	SOCKADDR_IN m_UDPRemoteInfo;				//UDP를 사용할때 상대 주소 정보
	HANDLE m_ReliableUDPThreadHandle;			//Reliable UDP 관련 변수
	HANDLE m_ReliableUDPThreadStartupEvent;		//Reliable UDP 관련 변수
	HANDLE m_ReliableUDPThreadDestroyEvent;		//Reliable UDP 관련 변수
	HANDLE m_ReliableUDPThreadWakeUpEvent;		//Reliable UDP 관련 변수
	HANDLE m_ReliableUDPWriteCompleteEvent;		//Reliable UDP 관련 변수
	BOOL m_IsReliableUDPSending;				//Reliable UDP 관련 변수

public:
	BOOL Begin(VOID);
	BOOL End(VOID);
public:
	CNetworkSession();
	virtual ~CNetworkSession();
};

