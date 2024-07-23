#include "includes.h"
#include "Init.h"
#include "MinHook.h"
#include "Chat.h"
typedef LRESULT(__fastcall* WarcraftRealWNDProc)(HWND hWnd, unsigned int Msg, WPARAM wParam, LPARAM lParam);
WarcraftRealWNDProc WarcraftRealWNDProc_ptr = NULL;
WarcraftRealWNDProc WarcraftRealWNDProc_org = NULL;
unsigned long Key = 0;
unsigned long Click = 0;
unsigned long tick = 0;
char KeySave;
unsigned long Count_Click = 0;
unsigned long Count_Click2 = 0;
unsigned long Count_15 = 0;
unsigned long Count_16 = 0;
unsigned long Count_0 = 0;
unsigned long Count_All = 0;
BOOL INGAME = FALSE;
unsigned long SendChatDelay2 = 0;
HWND WarcraftReal_HWND = NULL;
LPARAM WarcraftReal_LPARAM = NULL;
bool SendQc = false;
bool msclick = true;
bool msclick2 = false;
unsigned long GameTickCount = 0;
unsigned long SendMacro = 0;
bool skipms = false;
unsigned long ClickCount = 0;
int CheckChat(HWND hWnd, unsigned int& Msg, WPARAM& wParam, LPARAM& lParam)
{
	bool Chat = false;
	unsigned char* ChatOffset = GetChatOffset();
	if (!ChatOffset)
	{
		return 0;
	}
	char* pChatString = GetChatString();
	if (!pChatString)
	{
		return 0;
	}
	if (VK_RETURN == (int)wParam && !(lParam & 0x40000000))
	{
		if (IsChat())
		{
			if (strcmp(pChatString, "@ms") == 0)
			{
				msclick = !msclick;
				Chat = true;
				if (msclick == true)
				{
					msclick2 = false;
					PrintText("Ms Click Enable!");
				}
					
				else
					PrintText("Ms Clicik Disable!");
			}
			else if (strcmp(pChatString, "@ms2") == 0)
			{
				msclick2 = !msclick2;
				Chat = true;
				if (msclick2 == true)
				{
					msclick = false;
					PrintText("Ms Click 2 Enable!");
				}
				else
				{
					PrintText("Ms Clicik 2 Disable!");
				}
			}
			if (Chat == true)
			{
				GameChatSetState(ChatOffset, 0, 0);
				return 1;
			}
		}
	}

	return 0;
}
LRESULT __fastcall WarcraftRealWNDProc_my(HWND hWnd, unsigned int _Msg, WPARAM _wParam, LPARAM lParam)
{
	WarcraftReal_HWND = hWnd;
	WarcraftReal_LPARAM = lParam;
	if (IsInGame())
	{
		if (_wParam == VK_RETURN && _Msg == WM_KEYDOWN)
		{
			if (CheckChat(hWnd, _Msg, _wParam, lParam)==1)
				return DefWindowProc(hWnd, _Msg, _wParam, lParam);
		}
	}
	
		
	if (IsInGame())
	{
		if (INGAME == FALSE)
		{
			INGAME = TRUE;
			SendChat(0, "Windy Extension v1.5 Initialize!");
			Key = 0;
			Click = 0;
			Count_Click = 0;
			Count_15 = 0;
			Count_16 = 0;
			Count_0 = 0;
			Count_All = 0;
			ClickCount = 0;
			GameTickCount = GetTickCount();
		}
	}
	else
	{
		if (INGAME == TRUE)
		{
			INGAME = FALSE;
			GameTickCount = 0;
		}
			
	}
	if (IsInGame() && IsWindowActive() && !IsChat())
	{
		if (( _Msg == WM_KEYDOWN) || (_Msg == WM_LBUTTONDOWN)) {
			tick = GetTickCount();
			if (_Msg == WM_KEYDOWN)
			{
				Key = tick;
				KeySave = (int)_wParam;
			}
			else if (_Msg == WM_LBUTTONDOWN)
			{
				ClickCount++;
				Click = tick;
			}
				unsigned long ms = (Click - Key);
			if (Click >= Key && ms < 100)
			{
				Count_Click++;
				Count_All = Count_All + ms;
				if (ms == 0)
					Count_0++;
				if (ms == 15)
					Count_15++;
				if (ms == 16)
					Count_16++;
				if (Count_Click > 20)
				{
					double Count = Count_0 + Count_15 + Count_16;
					double ms2 = (double)Count_All / Count_Click;
					double ms3 = (double)Count / Count_Click;
					const char* Type = "Old Man";
					if (ms2 < 25)
						Type = "Ferrari";
					else if (ms2 < 35)
						Type = "Fast Man";
					else if (ms2 < 50)
						Type = "Human";
					else if (ms2 < 60)
						Type = "Slow Man";
					else
						Type = "Bone";
					if (ms2 < 25)
					{

						if (Count / Count_Click * 100 > 50)
						{
							Type = "Technology";
						}
					}
					if (GetTickCount() - GameTickCount > 120000)//300000)
					{
						GameTickCount = GetTickCount();
						skipms = true;
						SendChat(0, "Avg : %.2lfms | Click : %lu | %s", ms2, ClickCount, Type);
						skipms = false;
					}
					else
					{
						//PrintTextFormat("%lu", GetTickCount() - GameTickCount);
					}
					if (msclick)
						PrintTextFormat("%c : %lu ms", KeySave, ms);
					else if (msclick2)
						//PrintTextFormat("%c : %lu ms | Avg : %.2lfms | Sum : %lu | Count : %lu | %s", KeySave, ms, ms2, Count_All, Count_Click, Type);
						PrintTextFormat("%c : %lu ms | Avg : %.2lfms | Click : %lu | %s", KeySave, ms, ms2, ClickCount, Type);
				}
				else
				{
					if (msclick || msclick2)
						PrintTextFormat("%c : %lu ms", KeySave, ms);
				}
			}
		}
	}
	if (IsInGame())
	{
		if (SendQc)
		{
			SendChat(0, "am using Qc! ( %d )", GetLocalPlayerNumber());
			SendQc = false;
		}
	}
	return WarcraftRealWNDProc_ptr(hWnd, _Msg, _wParam, lParam);
}
void CheckMacro() {
	//Patch(dwGameDll + 0x0BAD40, "\x55\x8B\xEC\x83\xEC\x08",6);
	WarcraftRealWNDProc_org = (WarcraftRealWNDProc)(dwGameDll + 0x0BAD40);
	MH_CreateHook(WarcraftRealWNDProc_org, &WarcraftRealWNDProc_my, reinterpret_cast<void**>(&WarcraftRealWNDProc_ptr));
	MH_EnableHook(WarcraftRealWNDProc_org);
}