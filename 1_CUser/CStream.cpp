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

BOOL CStream::ReadInt32(INT* data)
{
	//���� �޾ƿ� data ������ ���� �����մϴ�.
	CopyMemory(data, m_BufferPointer + m_Length, sizeof(INT));

	//����� ����ŭ ���̸� ���մϴ�. ���⼭�� BYTE���̱⶧���� BYTE����ŭ�� ���̸� ���մϴ�.
	m_Length += sizeof(INT);

	return TRUE;
}

BOOL CStream::WriteInt32(INT* data)
{
	//���� �޾ƿ� data ������ ���� �����մϴ�.
	CopyMemory(m_BufferPointer + m_Length, &data, sizeof(INT));

	//����� ����ŭ ���̸� ���մϴ�. ���⼭�� BYTE���̱⶧���� BYTE����ŭ�� ���̸� ���մϴ�.
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
