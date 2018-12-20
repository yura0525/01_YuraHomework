#define _CRT_SECURE_NO_WARNINGS
#include "CMiniDump.h"
#include <excpt.h>
#include <stdio.h>
#include <Dbghelp.h>
#include <minidumpapiset.h>
#include <tchar.h>

#pragma comment( lib, "Dbghelp.lib")

//Callback �Լ��� ����
typedef BOOL(WINAPI* MINIDUMPWRITEDUMP)(
	HANDLE hProcess,
	DWORD dwPid,
	HANDLE hFile,
	MINIDUMP_TYPE DumpType,
	CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
	CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
	CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam);

//Unhandled Exception �� �߻������� �Ѿ���� �ݹ�
LONG WINAPI UnHandledExceptionFilter(struct _EXCEPTION_POINTERS * exceptionInfo)
{
	HMODULE DllHandle = NULL;
	HMODULE DllHandle2 = NULL;

	//windows 2000�������� ���� DBGHELP�� �����ؼ� ������ �־�� �մϴ�.
	//���⼭ ����ϴ� ���� DBGHELP.DLL�ȿ� ���Ե� �Լ����Դϴ�. �� �ε����־�� �մϴ�.
	DllHandle = LoadLibrary(_T("Dbghelp.dll"));
	DllHandle2 = LoadLibrary(_T("Dbgcore.dll"));

	if (DllHandle)
	{
		//������ �޾� ���Ϸ� ����� ����
		//DBGHELP.DLL���� MiniDumpWriteDump�� �ҷ��ͼ� Dump��� �����մϴ�.
		//�̰��� �̿��ؼ� ���� ������ ����� �˴ϴ�.
		MINIDUMPWRITEDUMP Dump = (MINIDUMPWRITEDUMP)GetProcAddress(DllHandle, "MiniDumpWriteDump");

		if (Dump)
		{
			TCHAR DumpPath[MAX_PATH] = { 0, };
			SYSTEMTIME SystemTime;

			//���� �ð��� �����ɴϴ�.
			GetLocalTime(&SystemTime);

			//���� �ð��� �������� �α������� ����ϴ�.
			_sntprintf(DumpPath, MAX_PATH, _T("%d-%d-%d %d_%d_%d.dmp"), SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay,
				SystemTime.wHour, SystemTime.wMinute, SystemTime.wSecond);

			//������ ����ϴ�.
			HANDLE FileHandle = CreateFile(DumpPath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

			//���� ����⿡ ����������
			if (FileHandle != INVALID_HANDLE_VALUE)
			{
				_MINIDUMP_EXCEPTION_INFORMATION MiniDumpExceptionInfo;

				//MiniDump �������� ���� ����ü�Դϴ�.
				MiniDumpExceptionInfo.ThreadId = GetCurrentThreadId();
				MiniDumpExceptionInfo.ExceptionPointers = exceptionInfo;
				MiniDumpExceptionInfo.ClientPointers = NULL;

				//���� ���μ����� ���� ������ �����մϴ�.
				//���⼭ Dump�� ���� DBGHELP.DLL���� �ҷ��� ���� �̿��մϴ�.
				//�̰��� ���ؼ� ���� ������ �����˴ϴ�.
				BOOL Success = Dump(GetCurrentProcess(), GetCurrentProcessId(), FileHandle,		//�̸� ������ ���� ������ �ڵ� ���Դϴ�.
					MiniDumpNormal, &MiniDumpExceptionInfo,										//���� �����Դϴ�.
					NULL, NULL);

				if (Success)
				{
					//����������
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
	//�߸��� ����â�� ��Ÿ���� �ʰ� �մϴ�.
	SetErrorMode(SEM_FAILCRITICALERRORS);

	//Unhandled Exception�� �߻������� ����� �ݹ��� �����մϴ�.
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
