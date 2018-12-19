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
	BOOL ReadDWORD(DWORD* data);
	BOOL ReadDWORD_PTR(DWORD_PTR* data);
	BOOL ReadBytes(BYTE* data, DWORD length);
	BOOL ReadFloat(FLOAT* data);
	BOOL ReadInt64(INT64* data);
	BOOL ReadSHORT(SHORT* data);
	BOOL ReadUSHORT(USHORT* data);
	BOOL ReadBOOL(BOOL* data);
public:
	BOOL WriteInt32(INT data);
	BOOL WriteDWORD(DWORD data);
	BOOL WriteDWORD_PTR(DWORD_PTR data);
	BOOL WriteByte(BYTE data);
	BOOL WriteBytes(BYTE* data, DWORD length);
	BOOL WriteFloat(FLOAT data);
	BOOL WriteInt64(INT64 data);
	BOOL WriteSHORT(SHORT data);
	BOOL WriteUSHORT(USHORT data);
	BOOL WriteBOOL(BOOL data);
public:
	DWORD GetLength(VOID);
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

