#pragma once
#include <Windows.h>

class CCriticalSection
{
private:
	CRITICAL_SECTION m_Sync;						//CRITICAL_SECTION 개체
public:
	CCriticalSection(VOID)
	{
		InitializeCriticalSection(&m_Sync);			//CRITICAL_SECTION 객체 초기화
	}

	~CCriticalSection(VOID)
	{
		DeleteCriticalSection(&m_Sync);				//CRITICAL_SECTION 객체 삭제
	}
	inline VOID Enter(VOID)
	{
		EnterCriticalSection(&m_Sync);
	}
	inline VOID Leave(VOID)
	{
		LeaveCriticalSection(&m_Sync);
	}
};


template<class T>
class CMultiThreadSync			//사용할 클래스 타입을 확인하기 위해서 템플릿으로 제작되었습니다.
{
	friend class CThreadSync;
public:
	class CThreadSync
	{
	public:
		CThreadSync(VOID)
		{
			T::m_Sync.Enter();		//해당 클래스의 동기화 개체를 Enter합니다.
		}
		~CThreadSync(VOID)
		{
			T::m_Sync.Leave();		//해당 클래스의 동기화 개체를 Leave합니다.
		}
	};
private:
	static CCriticalSection m_Sync;			//실제 CRITICAL_SECTION 개체입니다.
};

template<class T>
CCriticalSection CMultiThreadSync<T>::m_Sync;