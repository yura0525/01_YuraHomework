#define _CRT_SECURE_NO_WARNINGS
#include "CMiniDump.h"
#include <excpt.h>
#include <stdio.h>
#include <Dbghelp.h>
#include <minidumpapiset.h>
#include <tchar.h>

#pragma comment( lib, "Dbghelp.lib")

//Callback 함수의 원형
typedef BOOL(WINAPI* MINIDUMPWRITEDUMP)(
	HANDLE hProcess,
	DWORD dwPid,
	HANDLE hFile,
	MINIDUMP_TYPE DumpType,
	CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
	CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
	CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam);

//Unhandled Exception 이 발생했을때 넘어오는 콜백
LONG WINAPI UnHandledExceptionFilter(struct _EXCEPTION_POINTERS * exceptionInfo)
{
	HMODULE DllHandle = NULL;
	HMODULE DllHandle2 = NULL;

	//windows 2000이전에는 따로 DBGHELP를 배포해서 설정해 주어야 합니다.
	//여기서 사용하는 것은 DBGHELP.DLL안에 포함된 함수들입니다. 꼭 로드해주어야 합니다.
	DllHandle = LoadLibrary(_T("Dbghelp.dll"));
	DllHandle2 = LoadLibrary(_T("Dbgcore.dll"));

	if (DllHandle)
	{
		//덤프를 받아 파일로 만드는 과정
		//DBGHELP.DLL에서 MiniDumpWriteDump를 불러와서 Dump라고 정의합니다.
		//이것을 이용해서 덤프 파일을 만들게 됩니다.
		MINIDUMPWRITEDUMP Dump = (MINIDUMPWRITEDUMP)GetProcAddress(DllHandle, "MiniDumpWriteDump");

		if (Dump)
		{
			TCHAR DumpPath[MAX_PATH] = { 0, };
			SYSTEMTIME SystemTime;

			//현재 시간을 가져옵니다.
			GetLocalTime(&SystemTime);

			//현재 시간을 기준으로 로그파일을 만듭니다.
			_sntprintf(DumpPath, MAX_PATH, _T("%d-%d-%d %d_%d_%d.dmp"), SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay,
				SystemTime.wHour, SystemTime.wMinute, SystemTime.wSecond);

			//파일을 만듭니다.
			HANDLE FileHandle = CreateFile(DumpPath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

			//파일 만들기에 성공했을때
			if (FileHandle != INVALID_HANDLE_VALUE)
			{
				_MINIDUMP_EXCEPTION_INFORMATION MiniDumpExceptionInfo;

				//MiniDump 예외정보 저장 구조체입니다.
				MiniDumpExceptionInfo.ThreadId = GetCurrentThreadId();
				MiniDumpExceptionInfo.ExceptionPointers = exceptionInfo;
				MiniDumpExceptionInfo.ClientPointers = NULL;

				//현재 프로세스에 대한 덤프를 실행합니다.
				//여기서 Dump는 위의 DBGHELP.DLL에서 불러온 것을 이용합니다.
				//이것을 통해서 덤프 파일이 생성됩니다.
				BOOL Success = Dump(GetCurrentProcess(), GetCurrentProcessId(), FileHandle,		//미리 생성해 놓은 파일의 핸들 값입니다.
					MiniDumpNormal, &MiniDumpExceptionInfo,										//예외 정보입니다.
					NULL, NULL);

				if (Success)
				{
					//성공했을떄
					CloseHandle(FileHandle);

					return EXCEPTION_EXECUTE_HANDLER;
				}
			}
			CloseHandle(FileHandle);
		}
	}
	return EXCEPTION_CONTINUE_SEARCH;
}

BOOL CMiniDump::Begin(VOID)
{
	//잘못된 연산창이 나타나지 않게 합니다.
	SetErrorMode(SEM_FAILCRITICALERRORS);

	//Unhandled Exception이 발생했을때 진행될 콜백을 정의합니다.
	//PreviousExceptionFilter = SetUnhandledExceptionFilter(UnHandledExceptionFilter);
	return TRUE;
}
BOOL CMiniDump::End(VOID)
{
	//SetUnhandledExceptionFilter(PreviousExceptionFilter);
	return TRUE;
}

CMiniDump::CMiniDump()
{
}


CMiniDump::~CMiniDump()
{
}
