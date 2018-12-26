#pragma once
#include <Windows.h>

class CCriticalSection
{
private:
	CRITICAL_SECTION m_Sync;						//CRITICAL_SECTION ��ü
public:
	CCriticalSection(VOID)
	{
		InitializeCriticalSection(&m_Sync);			//CRITICAL_SECTION ��ü �ʱ�ȭ
	}

	~CCriticalSection(VOID)
	{
		DeleteCriticalSection(&m_Sync);				//CRITICAL_SECTION ��ü ����
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
class CMultiThreadSync			//����� Ŭ���� Ÿ���� Ȯ���ϱ� ���ؼ� ���ø����� ���۵Ǿ����ϴ�.
{
	friend class CThreadSync;
public:
	class CThreadSync
	{
	public:
		CThreadSync(VOID)
		{
			T::m_Sync.Enter();		//�ش� Ŭ������ ����ȭ ��ü�� Enter�մϴ�.
		}
		~CThreadSync(VOID)
		{
			T::m_Sync.Leave();		//�ش� Ŭ������ ����ȭ ��ü�� Leave�մϴ�.
		}
	};
private:
	static CCriticalSection m_Sync;			//���� CRITICAL_SECTION ��ü�Դϴ�.
};

template<class T>
CCriticalSection CMultiThreadSync<T>::m_Sync;