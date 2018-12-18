#pragma once
#include <Windows.h>

class CStream
{
public:
	BYTE* m_BufferPointer;
	DWORD m_Length;
public:
	BOOL SetBuffer(BYTE* buffer);
	BOOL ReadByte(BYTE* data);
	BOOL ReadInt32(INT* data);
	BOOL WriteInt32(INT* data);
public:
	CStream();
	virtual ~CStream();
};

class CStreamSP
{
private:
	CStream* Stream;

public:
	CStreamSP(VOID)
	{
		Stream = new CStream();
	}
	~CStreamSP()
	{
		delete Stream;
	}
	CStream* operator->(VOID)
	{
		return Stream;
	}
	operator CStream*(VOID)
	{
		return Stream;
	}
};

