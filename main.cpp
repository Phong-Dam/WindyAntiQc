#define CINTERFACE

#include "includes.h"
#include "Init.h"
#include "Chat.h"
#include "MinHook.h"
HHOOK hMouseHook;
using namespace std;
void Patch(DWORD dwBaseAddress, const char* szData, int iSize)
{
	DWORD dwOldProtection = NULL;
	VirtualProtect((LPVOID)dwBaseAddress, iSize, PAGE_EXECUTE_READWRITE, &dwOldProtection);
	CopyMemory((LPVOID)dwBaseAddress, szData, iSize);
	VirtualProtect((LPVOID)dwBaseAddress, iSize, dwOldProtection, NULL);
}
bool HotKeyPressed(int iKey)
{
	if (GetForegroundWindow() != FindWindow("Warcraft III", NULL))
		return false;

	if (GetAsyncKeyState(iKey))
		return true;

	return false;
}
void WINAPI HotKeys() {
	while (true) {
		Sleep(10000);
	}
}
MSLLHOOKSTRUCT mouseStruct;
int idx = 0;
LRESULT CALLBACK mouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{

	if (nCode >= 0)
	{	
		if (IsInGame())
		{
			mouseStruct = *((MSLLHOOKSTRUCT*)lParam);
			if (mouseStruct.flags & 0x01)
			{
				switch (wParam)
				{
				case WM_LBUTTONDOWN: idx = 1; break;
				case WM_LBUTTONUP: idx = 2; break;
				case WM_MOUSEMOVE: idx = 3; break;
				case WM_MOUSEWHEEL: idx = 4; break;
				case WM_MOUSEHWHEEL: idx = 5; break;
				case WM_RBUTTONDOWN: idx = 6; break;
				case WM_RBUTTONUP: idx = 7;
				}
				if (idx == 1)
				{
					SendQc = true;
				}

			}
		}
		
	}

	return CallNextHookEx(hMouseHook, nCode, wParam, lParam);
}

DWORD WINAPI MyMouseLogger(LPVOID lpParm)
{
	while (true)
	{
		if (INGAME==TRUE)
		{
			HINSTANCE hInstance = GetModuleHandle(NULL);

			hMouseHook = SetWindowsHookEx(WH_MOUSE_LL, mouseProc, hInstance, NULL);
			MSG message;
			while (GetMessage(&message, NULL, 0, 0) || INGAME==FALSE) {
				TranslateMessage(&message);
				DispatchMessage(&message);
			}
		}
		else
		{
			UnhookWindowsHookEx(hMouseHook);
		}
		Patch(dwGameDll + 0x210391, "\x75", 1);
		Patch(dwGameDll + 0x6A2F03, "\x75", 1);
		Patch(dwGameDll + 0x210341, "\x66\x0B\x0C\x50\x0F\xB7\xC1", 7);
		Patch(dwGameDll + 0x791D20, "\x74\x04", 2);
		Sleep(1000);
	}
	
	return 0;
}
HRESULT __stdcall DllMain(HMODULE _hModule, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{	
		DWORD dwThread;
		MH_Initialize();
		Init();
		InitChat();
		CheckMacro();
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)MyMouseLogger, NULL, NULL, &dwThread);
		HideDll(_hModule);
	}
		
	return 1;
}


