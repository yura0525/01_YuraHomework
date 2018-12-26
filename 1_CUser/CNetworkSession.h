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
	OVERLAPPED overlapped;	//���� ����ü
	IO_TYPE IoType;			//�� ����ü�� �ϴ���
							//IO_TYPE�� ����
							//IO_READ : �б� �۾��� �Ҷ� ���
							//IO_WRITE : ���� �۾��� �Ҷ� ���
							//IO_ACCEPT : Accept�� �޾����� ���
	VOID *Object;			//�θ�ü �ּ�
}OVERLAPPED_EX;


class CNetworkSession
{
public:
	OVERLAPPED_EX m_AcceptOverlapped;			//Accept�� ���õ� Overlapped����ü
	OVERLAPPED_EX m_ReadOverlapped;				//Read�� ���õ� Overlapped����ü
	OVERLAPPED_EX m_WriteOverlapped;			//Write�� ���õ� Overlapped����ü
	char m_ReadBuffer[MAX_BUFFER_LENGTH];		//Read���� ����ϴ� ����
	HANDLE m_Socket;							//���� �ڵ� ���� ����

	SOCKADDR_IN m_UDPRemoteInfo;				//UDP�� ����Ҷ� ��� �ּ� ����
	HANDLE m_ReliableUDPThreadHandle;			//Reliable UDP ���� ����
	HANDLE m_ReliableUDPThreadStartupEvent;		//Reliable UDP ���� ����
	HANDLE m_ReliableUDPThreadDestroyEvent;		//Reliable UDP ���� ����
	HANDLE m_ReliableUDPThreadWakeUpEvent;		//Reliable UDP ���� ����
	HANDLE m_ReliableUDPWriteCompleteEvent;		//Reliable UDP ���� ����
	BOOL m_IsReliableUDPSending;				//Reliable UDP ���� ����

public:
	BOOL Begin(VOID);
	BOOL End(VOID);
public:
	CNetworkSession();
	virtual ~CNetworkSession();
};

