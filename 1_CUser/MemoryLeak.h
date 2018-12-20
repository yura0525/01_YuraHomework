#pragma once

#ifdef _MSC_VER
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC			//메모리 누수 탐지를 위해서 선언해 주어야 합니다.
#include <crtdbg.h>
#ifndef _CONSOLE
#include <cstdlib>					//콘솔 프로그램일 경우 따로 선언해 주어야합니다.
#endif

class CMemoryLeak
{
public:
	CMemoryLeak()
	{
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

		//콘솔 프로그램일 경우
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

//초기화를 생성자를 통해 자동으로 해주기 위해 전역으로 선언합니다.
static CMemoryLeak MemoryLeak;
#endif
#endif
