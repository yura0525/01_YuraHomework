#pragma once
#include <tchar.h>
#include <vector>
#include <list>
#include <map>
#include <algorithm>

using namespace std;

template<class T>
class TSingleton
{
public:
	static T& GetInstance()
	{
		static T the;
		return the;
	}
};
