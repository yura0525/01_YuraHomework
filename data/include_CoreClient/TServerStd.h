#pragma once
#define  _WINSOCK_DEPRECATED_NO_WARNINGS
#include <windows.h>
#include <string>
#include <list>
#include "xStd.h"
using namespace std;
typedef std::basic_string<TCHAR> T_STR;
typedef std::basic_string<CHAR> C_STR;

#pragma comment(lib, "ws2_32.lib")

//template<class T> class TSingleton
//{
//public:
//	static T& GetInstance()
//	{
//		static T theSingleInstance;
//		return theSingleInstance;
//	}
//};
