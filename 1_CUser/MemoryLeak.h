#pragma once

#ifdef _MSC_VER
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC			//�޸� ���� Ž���� ���ؼ� ������ �־�� �մϴ�.
#include <crtdbg.h>
#ifndef _CONSOLE
#include <cstdlib>					//�ܼ� ���α׷��� ��� ���� ������ �־���մϴ�.
#endif

class CMemoryLeak
{
public:
	CMemoryLeak()
	{
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

		//�ܼ� ���α׷��� ���
#ifdef _CONSOLE
		_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
		_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
		_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_FILE);
		_CrtSetReportFile(_CRT_ERROR, _CRTDBG_FILE_STDOUT);
		_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_FILE);
		_CrtSetReportFile(_CRT_ASSERT, _CRTDBG_FILE_STDOUT);
#define DEBUG_NORMALBLOCK new(_NORMAL_BLOCK, __FILE__, __LINE__)
#ifdef new
#undef new
#endif
#define new DEBUG_NORMALBLOCK
#else
		_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
		_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
		_CrtSetReportMode(_CRT_ASSERT, _CRTDBG_MODE_DEBUG);
#endif

#ifdef malloc
#undef malloc
#endif
#define malloc(s) (_malloc_dbg(s, _NORMAL_BLOCK, __FILE__, __LINE__))
	}
};

//�ʱ�ȭ�� �����ڸ� ���� �ڵ����� ���ֱ� ���� �������� �����մϴ�.
static CMemoryLeak MemoryLeak;
#endif
#endif
