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

BOOL CStream::ReadInt32(INT* data)
{
	//값을 받아올 data 변수에 값을 복사합니다.
	CopyMemory(data, m_BufferPointer + m_Length, sizeof(INT));

	//복사된 값만큼 길이를 더합니다. 여기서는 BYTE형이기때문에 BYTE형만큼의 길이를 더합니다.
	m_Length += sizeof(INT);

	return TRUE;
}

BOOL CStream::WriteInt32(INT* data)
{
	//값을 받아올 data 변수에 값을 복사합니다.
	CopyMemory(m_BufferPointer + m_Length, &data, sizeof(INT));

	//복사된 값만큼 길이를 더합니다. 여기서는 BYTE형이기때문에 BYTE형만큼의 길이를 더합니다.
	m_Length += sizeof(INT);

	return TRUE;
}

CStream::CStream()
{
	m_BufferPointer = NULL;
	m_Length = 0;
}


CStream::~CStream()
{
}
