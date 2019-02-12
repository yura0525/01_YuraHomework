#pragma once
#include <Windows.h>
#include <tchar.h>

#define ASYNCFLAG_READ 0x00
#define ASYNCFLAG_WRITE 0x01

#define READBUFFER_SIZE 2014 * 30 //  대규모 파일의 경우는 2014*30 정도 예)2.4기가
#define WORKERTHREAD_COUNT 5

class Sample
{
public:
	Sample();
	virtual ~Sample();
};

