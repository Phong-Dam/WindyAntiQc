#include "includes.h"
#include "Init.h"
#include "chat.h"
#include "MinHook.h"
#include "vector"
sub_10061900 WfeCall;
pSetChatTargetUsers pSetChatTargetUsers_org;
pSetChatTargetUsers pSetChatTargetUsers_ptr;
int UsingCustomChatTarget = false;
int CustomChatTarget = 0;
bool WFEEnable = true;
unsigned char* GetChatOffset()
{
	unsigned char* pclass = *(unsigned char**)(GameDll + 0xD326F0);
	if (pclass)
	{
		return *(unsigned char**)(pclass + 0x3FC);
	}

	return 0;
}
char* GetChatString()
{
	unsigned char* pChatOffset = GetChatOffset();
	if (pChatOffset)
	{
		pChatOffset = *(unsigned char**)(pChatOffset + 0x1E0);
		if (pChatOffset)
		{
			pChatOffset = *(unsigned char**)(pChatOffset + 0x1E4);
			return (char*)pChatOffset;
		}
	}
	return 0;
}
int __fastcall SetChatTargetUsers_my(int chataddr, int ecx, int valtype)
{
	if (!UsingCustomChatTarget)
		return pSetChatTargetUsers_ptr(chataddr, ecx, valtype);
	else
		return pSetChatTargetUsers_ptr(chataddr, ecx, CustomChatTarget);
}
unsigned long SendChatDelay = 0;
pGameChatSetState GameChatSetState;
BOOL IsChat()
{
	return*(BOOL*)(GameDll + 0xD04FEC);
}
void SendChat(int type, const char* format, ...) //0 all //1 allies //3 obs //4private
{
	if (IsInGame() && !IsChat())
	{
		if (WFEEnable==false)
		{
			if ((GetTickCount() - SendChatDelay > 5000) || skipms==true)
			{
				unsigned char* ChatOffset = GetChatOffset();
				char* pChatString = GetChatString();
				SendChatDelay = GetTickCount();
				UsingCustomChatTarget = true;
				CustomChatTarget = type;
				GameChatSetState(ChatOffset, 0, 1);
				va_list args;
				va_start(args, format);
				vsprintf_s(pChatString, 128, format, args);
				va_end(args);
				WarcraftRealWNDProc_ptr(WarcraftReal_HWND, WM_KEYDOWN, VK_RETURN, WarcraftReal_LPARAM);
				WarcraftRealWNDProc_ptr(WarcraftReal_HWND, WM_KEYUP, VK_RETURN, WarcraftReal_LPARAM);
				UsingCustomChatTarget = false;
			}
		}
		else
		{
			if (GetTickCount() - SendChatDelay > 1000)
			{
				SendChatDelay = GetTickCount();
				char str[8192] = { 0 };
				va_list args;
				va_start(args, format);
				vsprintf_s(str, sizeof(str), format, args);
				va_end(args);
				UsingCustomChatTarget = true;
				CustomChatTarget = type;
				WfeCall(0, str, format);
				UsingCustomChatTarget = false;
			}
		}
	}
}
void InitChat()
{
	GameChatSetState = (pGameChatSetState)(dwGameDll + 0x3E3890);
	pSetChatTargetUsers_org = (pSetChatTargetUsers)(dwGameDll + 0x404600);
	MH_CreateHook(pSetChatTargetUsers_org, &SetChatTargetUsers_my, reinterpret_cast<void**>(&pSetChatTargetUsers_ptr));
	MH_EnableHook(pSetChatTargetUsers_org);
	unsigned char* WFE = (unsigned char*)GetModuleHandle("WAR3WFE.dll");
	unsigned char* WFE2 = (unsigned char*)GetModuleHandle("WfeDll.dll");
	if (WFE == NULL && WFE2 == NULL)
	{
		WFEEnable = false;
	}
	else
	{
		if (WFE != NULL)
		{
			int* pValue = reinterpret_cast<int*>(reinterpret_cast<char*>(WFE) + 0x61900);
			if (*pValue == 1828086784)
			{
				WfeCall = (sub_10061900)(WFE + 0x61900);
			}
			else
				WFEEnable = false;
			
		}
		else if (WFE2 != NULL)
		{
			int* pValue = reinterpret_cast<int*>(reinterpret_cast<char*>(WFE2) + 0x61900);
			if (*pValue == 1828086784)
			{
				WfeCall = (sub_10061900)(WFE2 + 0x61900);
			}
			else
				WFEEnable = false;
		}
	}
}