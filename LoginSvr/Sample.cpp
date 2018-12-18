#include "stdafx.h"
#include "TAccept.h"
#include "TSynchronize.h"

#pragma comment(lib, "TSvrLib.lib")

void main()
{
	TAccept svr;
	svr.Set(10000);
	svr.CreateThread();

	while (1);
}