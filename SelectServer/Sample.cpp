#include "TSelect.h"
TSelect g_Select;

int main()
{
	if (g_Select.InitSelectServer() == false)
	{
		printf("\n���� �ʱ�ȭ ����");
		return 0;
	}
	if (g_Select.NetworkProcess() == false)
	{
		printf("\n������ �� ���� ���� �Ǿ����ϴ�.");
	}
	printf("\n������ ���� ���� �Ǿ����ϴ�.");
	g_Select.Release();
	getchar();
	return 0;
}