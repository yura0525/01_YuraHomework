#include "CStream.h"

//�켱 ���۸� ����ϴ� �Լ��� ��� ������ mBufferPointer�� ������ �����͸� �����մϴ�.
//�̶� ��ü ������ ���̸� �����ϴ� mLength ������ �ʱ�ȭ�մϴ�.
BOOL CStream::SetBuffer(BYTE* buffer)
{
	if (!buffer)
		return FALSE;
	m_BufferPointer = buffer;
	m_Length = 0;

	return TRUE;
}
//BYTE�� �����͸� Call by reference�� �̿��ؼ� �޾ƿɴϴ�.
BOOL CStream::ReadByte(BYTE* data)
{
	//���� �޾ƿ� data ������ ���� �����մϴ�.
	CopyMemory(data, m_BufferPointer + m_Length, sizeof(BYTE));
	
	//����� ����ŭ ���̸� ���մϴ�. ���⼭�� BYTE���̱⶧���� BYTE����ŭ�� ���̸� ���մϴ�.
	m_Length += sizeof(BYTE);

	return TRUE;
}
//Int32�� �����͸� ������ ����մϴ�.
BOOL CStream::ReadInt32(INT* data)
{
	//���� �޾ƿ� data ������ ���� �����մϴ�.
	CopyMemory(data, m_BufferPointer + m_Length, sizeof(INT));

	//����� ����ŭ ���̸� ���մϴ�. ���⼭�� BYTE���̱⶧���� BYTE����ŭ�� ���̸� ���մϴ�.
	m_Length += sizeof(INT);

	return TRUE;
}
//DWORD�� �����͸� ������ ����մϴ�.
BOOL CStream::ReadDWORD(DWORD* data)
{
	//���� �޾ƿ� data ������ ���� �����մϴ�.
	CopyMemory(data, m_BufferPointer + m_Length, sizeof(DWORD));

	//����� ����ŭ ���̸� ���մϴ�. ���⼭�� BYTE���̱⶧���� BYTE����ŭ�� ���̸� ���մϴ�.
	m_Length += sizeof(DWORD);

	return TRUE;
}
//DWORD_PTR�� �����͸� ������ ����մϴ�.
BOOL CStream::ReadDWORD_PTR(DWORD_PTR* data)
{
	//���� �޾ƿ� data ������ ���� �����մϴ�.
	CopyMemory(data, m_BufferPointer + m_Length, sizeof(DWORD_PTR));

	//����� ����ŭ ���̸� ���մϴ�. ���⼭�� BYTE���̱⶧���� BYTE����ŭ�� ���̸� ���մϴ�.
	m_Length += sizeof(DWORD_PTR);

	return TRUE;
}

//BYTE �迭�� �����͸� ������ ����մϴ�.
BOOL CStream::ReadBytes(BYTE* data, DWORD length)
{
	//���� �޾ƿ� data ������ ���� �����մϴ�.
	CopyMemory(data, m_BufferPointer + m_Length, sizeof(BYTE) * length);

	//����� ����ŭ ���̸� ���մϴ�. ���⼭�� BYTE���̱⶧���� BYTE����ŭ�� ���̸� ���մϴ�.
	m_Length += sizeof(BYTE) * length;

	return TRUE;
}

//Float�� �����͸� ������ ����մϴ�.
BOOL CStream::ReadFloat(FLOAT* data)
{
	//���� �޾ƿ� data ������ ���� �����մϴ�.
	CopyMemory(data, m_BufferPointer + m_Length, sizeof(FLOAT));

	//����� ����ŭ ���̸� ���մϴ�. ���⼭�� BYTE���̱⶧���� BYTE����ŭ�� ���̸� ���մϴ�.
	m_Length += sizeof(FLOAT);

	return TRUE;
}
//INT64�� �����͸� ������ ����մϴ�.
BOOL CStream::ReadInt64(INT64* data)
{
	//���� �޾ƿ� data ������ ���� �����մϴ�.
	CopyMemory(data, m_BufferPointer + m_Length, sizeof(INT64));

	//����� ����ŭ ���̸� ���մϴ�. ���⼭�� BYTE���̱⶧���� BYTE����ŭ�� ���̸� ���մϴ�.
	m_Length += sizeof(INT64);

	return TRUE;
}
//SHORT�� �����͸� ������ ����մϴ�.
BOOL CStream::ReadSHORT(SHORT* data)
{
	//���� �޾ƿ� data ������ ���� �����մϴ�.
	CopyMemory(data, m_BufferPointer + m_Length, sizeof(SHORT));

	//����� ����ŭ ���̸� ���մϴ�. ���⼭�� BYTE���̱⶧���� BYTE����ŭ�� ���̸� ���մϴ�.
	m_Length += sizeof(SHORT);

	return TRUE;
}
//USHORT�� �����͸� ������ ����մϴ�.
BOOL CStream::ReadUSHORT(USHORT* data)
{
	//���� �޾ƿ� data ������ ���� �����մϴ�.
	CopyMemory(data, m_BufferPointer + m_Length, sizeof(USHORT));

	//����� ����ŭ ���̸� ���մϴ�. ���⼭�� BYTE���̱⶧���� BYTE����ŭ�� ���̸� ���մϴ�.
	m_Length += sizeof(USHORT);

	return TRUE;
}
//BOOL�� �����͸� ������ ����մϴ�.
BOOL CStream::ReadBOOL(BOOL* data)
{
	//���� �޾ƿ� data ������ ���� �����մϴ�.
	CopyMemory(data, m_BufferPointer + m_Length, sizeof(BOOL));

	//����� ����ŭ ���̸� ���մϴ�. ���⼭�� BYTE���̱⶧���� BYTE����ŭ�� ���̸� ���մϴ�.
	m_Length += sizeof(BOOL);

	return TRUE;
}
//INT32�� �����͸� ���� ����մϴ�.
BOOL CStream::WriteInt32(INT data)
{
	//���� �޾ƿ� data ������ ���� �����մϴ�.
	CopyMemory(m_BufferPointer + m_Length, &data, sizeof(INT));

	//����� ����ŭ ���̸� ���մϴ�. ���⼭�� BYTE���̱⶧���� BYTE����ŭ�� ���̸� ���մϴ�.
	m_Length += sizeof(INT);

	return TRUE;
}

//DWORD�� �����͸� ���� ����մϴ�.
BOOL CStream::WriteDWORD(DWORD data)
{
	//���� �޾ƿ� data ������ ���� �����մϴ�.
	CopyMemory(m_BufferPointer + m_Length, &data, sizeof(DWORD));

	//����� ����ŭ ���̸� ���մϴ�. ���⼭�� BYTE���̱⶧���� BYTE����ŭ�� ���̸� ���մϴ�.
	m_Length += sizeof(DWORD);

	return TRUE;
}

//DWORD_PTR�� �����͸� ���� ����մϴ�.
BOOL CStream::WriteDWORD_PTR(DWORD_PTR data)
{
	//���� �޾ƿ� data ������ ���� �����մϴ�.
	CopyMemory(m_BufferPointer + m_Length, &data, sizeof(DWORD_PTR));

	//����� ����ŭ ���̸� ���մϴ�. ���⼭�� BYTE���̱⶧���� BYTE����ŭ�� ���̸� ���մϴ�.
	m_Length += sizeof(DWORD_PTR);

	return TRUE;
}

//BYTE�� �����͸� ���� ����մϴ�.
BOOL CStream::WriteByte(BYTE data)
{
	//���� �޾ƿ� data ������ ���� �����մϴ�.
	CopyMemory(m_BufferPointer + m_Length, &data, sizeof(BYTE));

	//����� ����ŭ ���̸� ���մϴ�. ���⼭�� BYTE���̱⶧���� BYTE����ŭ�� ���̸� ���մϴ�.
	m_Length += sizeof(BYTE);

	return TRUE;
}

//BYTE�迭�� �����͸� ���� ����մϴ�.
BOOL CStream::WriteBytes(BYTE* data, DWORD length)
{
	//���� �޾ƿ� data ������ ���� �����մϴ�.
	CopyMemory(m_BufferPointer + m_Length, &data, sizeof(BYTE) * length);

	//����� ����ŭ ���̸� ���մϴ�. ���⼭�� BYTE���̱⶧���� BYTE����ŭ�� ���̸� ���մϴ�.
	m_Length += sizeof(BYTE) * length;

	return TRUE;
}

//FLOAT�� �����͸� ���� ����մϴ�.
BOOL CStream::WriteFloat(FLOAT data)
{
	//���� �޾ƿ� data ������ ���� �����մϴ�.
	CopyMemory(m_BufferPointer + m_Length, &data, sizeof(FLOAT));

	//����� ����ŭ ���̸� ���մϴ�. ���⼭�� BYTE���̱⶧���� BYTE����ŭ�� ���̸� ���մϴ�.
	m_Length += sizeof(FLOAT);

	return TRUE;
}

//INT64�� �����͸� ���� ����մϴ�.
BOOL CStream::WriteInt64(INT64 data)
{
	//���� �޾ƿ� data ������ ���� �����մϴ�.
	CopyMemory(m_BufferPointer + m_Length, &data, sizeof(INT64));

	//����� ����ŭ ���̸� ���մϴ�. ���⼭�� BYTE���̱⶧���� BYTE����ŭ�� ���̸� ���մϴ�.
	m_Length += sizeof(INT64);

	return TRUE;
}

//SHORT�� �����͸� ���� ����մϴ�.
BOOL CStream::WriteSHORT(SHORT data)
{
	//���� �޾ƿ� data ������ ���� �����մϴ�.
	CopyMemory(m_BufferPointer + m_Length, &data, sizeof(SHORT));

	//����� ����ŭ ���̸� ���մϴ�. ���⼭�� BYTE���̱⶧���� BYTE����ŭ�� ���̸� ���մϴ�.
	m_Length += sizeof(SHORT);

	return TRUE;
}
//USHORT�� �����͸� ���� ����մϴ�.
BOOL CStream::WriteUSHORT(USHORT data)
{
	//���� �޾ƿ� data ������ ���� �����մϴ�.
	CopyMemory(m_BufferPointer + m_Length, &data, sizeof(USHORT));

	//����� ����ŭ ���̸� ���մϴ�. ���⼭�� BYTE���̱⶧���� BYTE����ŭ�� ���̸� ���մϴ�.
	m_Length += sizeof(USHORT);

	return TRUE;
}
//BOOL�� �����͸� ���� ����մϴ�.
BOOL CStream::WriteBOOL(BOOL data)
{
	//���� �޾ƿ� data ������ ���� �����մϴ�.
	CopyMemory(m_BufferPointer + m_Length, &data, sizeof(BOOL));

	//����� ����ŭ ���̸� ���մϴ�. ���⼭�� BYTE���̱⶧���� BYTE����ŭ�� ���̸� ���մϴ�.
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
