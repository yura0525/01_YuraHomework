#include "CStream.h"

//우선 버퍼를 등록하는 함수로 멤버 변수인 mBufferPointer에 버퍼의 포인터를 저장합니다.
//이때 전체 데이터 길이를 관리하는 mLength 변수를 초기화합니다.
BOOL CStream::SetBuffer(BYTE* buffer)
{
	if (!buffer)
		return FALSE;
	m_BufferPointer = buffer;
	m_Length = 0;

	return TRUE;
}
//BYTE형 데이터를 Call by reference를 이용해서 받아옵니다.
BOOL CStream::ReadByte(BYTE* data)
{
	//값을 받아올 data 변수에 값을 복사합니다.
	CopyMemory(data, m_BufferPointer + m_Length, sizeof(BYTE));
	
	//복사된 값만큼 길이를 더합니다. 여기서는 BYTE형이기때문에 BYTE형만큼의 길이를 더합니다.
	m_Length += sizeof(BYTE);

	return TRUE;
}
//Int32형 데이터를 읽을때 사용합니다.
BOOL CStream::ReadInt32(INT* data)
{
	//값을 받아올 data 변수에 값을 복사합니다.
	CopyMemory(data, m_BufferPointer + m_Length, sizeof(INT));

	//복사된 값만큼 길이를 더합니다. 여기서는 BYTE형이기때문에 BYTE형만큼의 길이를 더합니다.
	m_Length += sizeof(INT);

	return TRUE;
}
//DWORD형 데이터를 읽을때 사용합니다.
BOOL CStream::ReadDWORD(DWORD* data)
{
	//값을 받아올 data 변수에 값을 복사합니다.
	CopyMemory(data, m_BufferPointer + m_Length, sizeof(DWORD));

	//복사된 값만큼 길이를 더합니다. 여기서는 BYTE형이기때문에 BYTE형만큼의 길이를 더합니다.
	m_Length += sizeof(DWORD);

	return TRUE;
}
//DWORD_PTR형 데이터를 읽을때 사용합니다.
BOOL CStream::ReadDWORD_PTR(DWORD_PTR* data)
{
	//값을 받아올 data 변수에 값을 복사합니다.
	CopyMemory(data, m_BufferPointer + m_Length, sizeof(DWORD_PTR));

	//복사된 값만큼 길이를 더합니다. 여기서는 BYTE형이기때문에 BYTE형만큼의 길이를 더합니다.
	m_Length += sizeof(DWORD_PTR);

	return TRUE;
}

//BYTE 배열형 데이터를 읽을때 사용합니다.
BOOL CStream::ReadBytes(BYTE* data, DWORD length)
{
	//값을 받아올 data 변수에 값을 복사합니다.
	CopyMemory(data, m_BufferPointer + m_Length, sizeof(BYTE) * length);

	//복사된 값만큼 길이를 더합니다. 여기서는 BYTE형이기때문에 BYTE형만큼의 길이를 더합니다.
	m_Length += sizeof(BYTE) * length;

	return TRUE;
}

//Float형 데이터를 읽을때 사용합니다.
BOOL CStream::ReadFloat(FLOAT* data)
{
	//값을 받아올 data 변수에 값을 복사합니다.
	CopyMemory(data, m_BufferPointer + m_Length, sizeof(FLOAT));

	//복사된 값만큼 길이를 더합니다. 여기서는 BYTE형이기때문에 BYTE형만큼의 길이를 더합니다.
	m_Length += sizeof(FLOAT);

	return TRUE;
}
//INT64형 데이터를 읽을때 사용합니다.
BOOL CStream::ReadInt64(INT64* data)
{
	//값을 받아올 data 변수에 값을 복사합니다.
	CopyMemory(data, m_BufferPointer + m_Length, sizeof(INT64));

	//복사된 값만큼 길이를 더합니다. 여기서는 BYTE형이기때문에 BYTE형만큼의 길이를 더합니다.
	m_Length += sizeof(INT64);

	return TRUE;
}
//SHORT형 데이터를 읽을때 사용합니다.
BOOL CStream::ReadSHORT(SHORT* data)
{
	//값을 받아올 data 변수에 값을 복사합니다.
	CopyMemory(data, m_BufferPointer + m_Length, sizeof(SHORT));

	//복사된 값만큼 길이를 더합니다. 여기서는 BYTE형이기때문에 BYTE형만큼의 길이를 더합니다.
	m_Length += sizeof(SHORT);

	return TRUE;
}
//USHORT형 데이터를 읽을때 사용합니다.
BOOL CStream::ReadUSHORT(USHORT* data)
{
	//값을 받아올 data 변수에 값을 복사합니다.
	CopyMemory(data, m_BufferPointer + m_Length, sizeof(USHORT));

	//복사된 값만큼 길이를 더합니다. 여기서는 BYTE형이기때문에 BYTE형만큼의 길이를 더합니다.
	m_Length += sizeof(USHORT);

	return TRUE;
}
//BOOL형 데이터를 읽을때 사용합니다.
BOOL CStream::ReadBOOL(BOOL* data)
{
	//값을 받아올 data 변수에 값을 복사합니다.
	CopyMemory(data, m_BufferPointer + m_Length, sizeof(BOOL));

	//복사된 값만큼 길이를 더합니다. 여기서는 BYTE형이기때문에 BYTE형만큼의 길이를 더합니다.
	m_Length += sizeof(BOOL);

	return TRUE;
}
//INT32형 데이터를 쓸때 사용합니다.
BOOL CStream::WriteInt32(INT data)
{
	//값을 받아올 data 변수에 값을 복사합니다.
	CopyMemory(m_BufferPointer + m_Length, &data, sizeof(INT));

	//복사된 값만큼 길이를 더합니다. 여기서는 BYTE형이기때문에 BYTE형만큼의 길이를 더합니다.
	m_Length += sizeof(INT);

	return TRUE;
}

//DWORD형 데이터를 쓸때 사용합니다.
BOOL CStream::WriteDWORD(DWORD data)
{
	//값을 받아올 data 변수에 값을 복사합니다.
	CopyMemory(m_BufferPointer + m_Length, &data, sizeof(DWORD));

	//복사된 값만큼 길이를 더합니다. 여기서는 BYTE형이기때문에 BYTE형만큼의 길이를 더합니다.
	m_Length += sizeof(DWORD);

	return TRUE;
}

//DWORD_PTR형 데이터를 쓸때 사용합니다.
BOOL CStream::WriteDWORD_PTR(DWORD_PTR data)
{
	//값을 받아올 data 변수에 값을 복사합니다.
	CopyMemory(m_BufferPointer + m_Length, &data, sizeof(DWORD_PTR));

	//복사된 값만큼 길이를 더합니다. 여기서는 BYTE형이기때문에 BYTE형만큼의 길이를 더합니다.
	m_Length += sizeof(DWORD_PTR);

	return TRUE;
}

//BYTE형 데이터를 쓸때 사용합니다.
BOOL CStream::WriteByte(BYTE data)
{
	//값을 받아올 data 변수에 값을 복사합니다.
	CopyMemory(m_BufferPointer + m_Length, &data, sizeof(BYTE));

	//복사된 값만큼 길이를 더합니다. 여기서는 BYTE형이기때문에 BYTE형만큼의 길이를 더합니다.
	m_Length += sizeof(BYTE);

	return TRUE;
}

//BYTE배열형 데이터를 쓸때 사용합니다.
BOOL CStream::WriteBytes(BYTE* data, DWORD length)
{
	//값을 받아올 data 변수에 값을 복사합니다.
	CopyMemory(m_BufferPointer + m_Length, &data, sizeof(BYTE) * length);

	//복사된 값만큼 길이를 더합니다. 여기서는 BYTE형이기때문에 BYTE형만큼의 길이를 더합니다.
	m_Length += sizeof(BYTE) * length;

	return TRUE;
}

//FLOAT형 데이터를 쓸때 사용합니다.
BOOL CStream::WriteFloat(FLOAT data)
{
	//값을 받아올 data 변수에 값을 복사합니다.
	CopyMemory(m_BufferPointer + m_Length, &data, sizeof(FLOAT));

	//복사된 값만큼 길이를 더합니다. 여기서는 BYTE형이기때문에 BYTE형만큼의 길이를 더합니다.
	m_Length += sizeof(FLOAT);

	return TRUE;
}

//INT64형 데이터를 쓸때 사용합니다.
BOOL CStream::WriteInt64(INT64 data)
{
	//값을 받아올 data 변수에 값을 복사합니다.
	CopyMemory(m_BufferPointer + m_Length, &data, sizeof(INT64));

	//복사된 값만큼 길이를 더합니다. 여기서는 BYTE형이기때문에 BYTE형만큼의 길이를 더합니다.
	m_Length += sizeof(INT64);

	return TRUE;
}

//SHORT형 데이터를 쓸때 사용합니다.
BOOL CStream::WriteSHORT(SHORT data)
{
	//값을 받아올 data 변수에 값을 복사합니다.
	CopyMemory(m_BufferPointer + m_Length, &data, sizeof(SHORT));

	//복사된 값만큼 길이를 더합니다. 여기서는 BYTE형이기때문에 BYTE형만큼의 길이를 더합니다.
	m_Length += sizeof(SHORT);

	return TRUE;
}
//USHORT형 데이터를 쓸때 사용합니다.
BOOL CStream::WriteUSHORT(USHORT data)
{
	//값을 받아올 data 변수에 값을 복사합니다.
	CopyMemory(m_BufferPointer + m_Length, &data, sizeof(USHORT));

	//복사된 값만큼 길이를 더합니다. 여기서는 BYTE형이기때문에 BYTE형만큼의 길이를 더합니다.
	m_Length += sizeof(USHORT);

	return TRUE;
}
//BOOL형 데이터를 쓸때 사용합니다.
BOOL CStream::WriteBOOL(BOOL data)
{
	//값을 받아올 data 변수에 값을 복사합니다.
	CopyMemory(m_BufferPointer + m_Length, &data, sizeof(BOOL));

	//복사된 값만큼 길이를 더합니다. 여기서는 BYTE형이기때문에 BYTE형만큼의 길이를 더합니다.
	m_Length += sizeof(BOOL);

	return TRUE;
}
DWORD CStream::GetLength(VOID)
{
	return m_Length;
}

CStream::CStream()
{
	m_BufferPointer = NULL;
	m_Length = 0;
}


CStream::~CStream()
{
}
