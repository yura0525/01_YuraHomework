#pragma once
#include <Windows.h>
#define MAX_QUEUE_LENGTH 2048

template<class T>
class CCircularQueue
{
private:
	T		m_Queue[MAX_QUEUE_LENGTH];
	DWORD	m_QueueHead;
	DWORD	m_QueueTail;
public:
	BOOL Begin(VOID)
	{
		ZeroMemory(m_Queue, sizeof(m_Queue));
		m_QueueHead = m_QueueTail = 0;
		return TRUE;
	}
	BOOL End(VOID)
	{
		return TRUE;
	}
	BOOL Push(T data)
	{
		DWORD tempTail = (m_QueueTail + 1) % MAX_QUEUE_LENGTH;
		if (tempTail == m_QueueHead)
			return FALSE;

		CopyMemory(&m_Queue[tempTail], &data, sizeof(T));
		m_QueueTail = tempTail;

		return TRUE;
	}
	BOOL Pop(T& data)
	{
		if (m_QueueHead == m_QueueTail)
			return FALSE;

		DWORD tempHead = (m_QueueHead + 1) % MAX_QUEUE_LENGTH;

		CopyMemory(&data, &m_Queue[tempHead], sizeof(T));
		m_QueueHead = tempHead;

		return TRUE;
	}
	BOOL IsEmpty(VOID)
	{
		if (m_QueueHead == m_QueueTail)
			return TRUE;

		return FALSE;
	}
public:
	CCircularQueue()
	{
		ZeroMemory(m_Queue, sizeof(m_Queue));
		m_QueueHead = m_QueueTail = 0;
	}
	virtual ~CCircularQueue()
	{
	}
};

