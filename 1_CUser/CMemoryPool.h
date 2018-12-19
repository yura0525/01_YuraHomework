#pragma once
#include <windows.h>

template<class T, int ALLOC_BLOCK_SIZE = 50>
class CMemoryPool
{
public:
	//new를 했을 경우 호출됩니다.
	static VOID* operator new(std::size_t allocLength)
	{
		assert(sizeof(T) == allocLength);
		assert(sizeof(T) >= sizeof(UCHAR*));

		//만약 더이상 할당 할 수 있는 공간이 없을 경우 새로 할당합니다.
		if (!m_FreePointer)
			allocBlock();

		UCHAR* ReturnPointer = m_FreePointer;

		//m_FreePointer에는 리턴하는 블록 앞에 4바이트로 있던 주소가 들어갑니다.
		m_FreePointer = *reinterpret_cast<UCHAR**>(ReturnPointer);

		return ReturnPointer;
	}

	//delete을 했을때
	static VOID operator delete(VOID* deletePointer)
	{
		//delete된 블록의 Next에 현재 m_FreePointer의 주소를 넣어줍니다.
		*reinterpret_cast<UCHAR**>(deletePointer) = m_FreePointer;

		//m_FreePointer를 업데이트 합니다.
		m_FreePointer = static_cast<UCHAR*>(deletePointer);
	}

private:
	static VOID allocBlock()
	{
		//m_FreePointer에 사용할 크기의 메모리를 할당합니다.
		m_FreePointer = new UCHAR[sizeof(T) * ALLOC_BLOCK_SIZE];

		//할당한 메모리의 첫블록의 포인터를 Current에 넣습니다.
		UCHAR** Current = reinterpret_cast<UCHAR**>(m_FreePointer);

		UCHAR* Next = m_FreePointer;		//할당된 메모리의 첫블록입니다.

		for (INT i = 0; i < ALLOC_BLOCK_SIZE - 1; i++)
		{
			Next += sizeof(T);		//다음 블록을 계산합니다.
			*Current = Next;		//할당된 메모리 앞 4바이트에 다음 블록의 주소를 넣습니다.
			Current = reinterpret_cast<UCHAR**>(Next);
		}
		*Current = 0;				//마지막일 경우 앞에 4바이트는 NULL입니다.
		
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


