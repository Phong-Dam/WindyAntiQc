#include "includes.h"
#include "Init.h"
int GetGlobalPlayerData()
{
	return  *(int*)(0xD305E0 + dwGameDll);
}
BOOL IsOkayPtr(void* ptr, unsigned int size)
{
	BOOL returnvalue = FALSE;
	returnvalue = IsBadReadPtr(ptr, size) == 0;
	return returnvalue;
}
int GetLocalPlayerNumber()
{
	int gldata = GetGlobalPlayerData();
	if (gldata > 0)
	{
		int playerslotaddr = (int)gldata + 0x28;
		if (IsOkayPtr((void*)playerslotaddr))
			return (int)*(short*)(playerslotaddr);
		else
			return -2;
	}
	else
		return -2;
}