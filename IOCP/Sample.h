#pragma once
#include <Windows.h>
#include <tchar.h>

#define ASYNCFLAG_READ 0x00
#define ASYNCFLAG_WRITE 0x01

#define READBUFFER_SIZE 2014 * 30 //  ��Ը� ������ ���� 2014*30 ���� ��)2.4�Ⱑ
#define WORKERTHREAD_COUNT 5

class Sample
{
public:
	Sample();
	virtual ~Sample();
};

