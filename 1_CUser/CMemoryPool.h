#pragma once
#include <windows.h>

template<class T, int ALLOC_BLOCK_SIZE = 50>
class CMemoryPool
{
public:
	//new�� ���� ��� ȣ��˴ϴ�.
	static VOID* operator new(std::size_t allocLength)
	{
		assert(sizeof(T) == allocLength);
		assert(sizeof(T) >= sizeof(UCHAR*));

		//���� ���̻� �Ҵ� �� �� �ִ� ������ ���� ��� ���� �Ҵ��մϴ�.
		if (!m_FreePointer)
			allocBlock();

		UCHAR* ReturnPointer = m_FreePointer;

		//m_FreePointer���� �����ϴ� ��� �տ� 4����Ʈ�� �ִ� �ּҰ� ���ϴ�.
		m_FreePointer = *reinterpret_cast<UCHAR**>(ReturnPointer);

		return ReturnPointer;
	}

	//delete�� ������
	static VOID operator delete(VOID* deletePointer)
	{
		//delete�� ����� Next�� ���� m_FreePointer�� �ּҸ� �־��ݴϴ�.
		*reinterpret_cast<UCHAR**>(deletePointer) = m_FreePointer;

		//m_FreePointer�� ������Ʈ �մϴ�.
		m_FreePointer = static_cast<UCHAR*>(deletePointer);
	}

private:
	static VOID allocBlock()
	{
		//m_FreePointer�� ����� ũ���� �޸𸮸� �Ҵ��մϴ�.
		m_FreePointer = new UCHAR[sizeof(T) * ALLOC_BLOCK_SIZE];

		//�Ҵ��� �޸��� ù����� �����͸� Current�� �ֽ��ϴ�.
		UCHAR** Current = reinterpret_cast<UCHAR**>(m_FreePointer);

		UCHAR* Next = m_FreePointer;		//�Ҵ�� �޸��� ù����Դϴ�.

		for (INT i = 0; i < ALLOC_BLOCK_SIZE - 1; i++)
		{
			Next += sizeof(T);		//���� ����� ����մϴ�.
			*Current = Next;		//�Ҵ�� �޸� �� 4����Ʈ�� ���� ����� �ּҸ� �ֽ��ϴ�.
			Current = reinterpret_cast<UCHAR**>(Next);
		}
		*Current = 0;				//�������� ��� �տ� 4����Ʈ�� NULL�Դϴ�.
		
	}
private:
	static UCHAR* m_FreePointer;

public:
	CMemoryPool()
	{

	}
	virtual ~CMemoryPool()
	{

	}
};

template<class T, int ALLOC_BLOCK_SIZE>
UCHAR* CMemoryPool<T, ALLOC_BLOCK_SIZE>::m_FreePointer;


