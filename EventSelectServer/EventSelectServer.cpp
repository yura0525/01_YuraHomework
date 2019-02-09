#include "TEventSelect.h"

TEventSelect g_EventSelect;

int main()
{
	g_EventSelect.InitSelectServer();
	g_EventSelect.NetworkProcess();
	g_EventSelect.Release();
	return 0;
}