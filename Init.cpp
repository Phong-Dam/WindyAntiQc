#include "Init.h"
#include "string"
GAME_GetPtrList_t* GAME_GetPtrList = 0;
GAME_Print_t* GAME_Print = 0;
int IsGameOffset1 = 0;
int IsGameOffset2 = 0;
void Init() {
	IsGameOffset1 = 0xD328D0;
	IsGameOffset2 = 0xD30110;
	GAME_GetPtrList = (GAME_GetPtrList_t*)((DWORD)dwGameDll + (DWORD)0x11BD30);
	GAME_Print = (GAME_Print_t*)((DWORD)dwGameDll + (DWORD)0x144480);
}
//In Game
BOOL IsInGame()
{
	return *(int*)(GameDll + IsGameOffset1) > 0 || *(int*)(GameDll + IsGameOffset2) > 0;
}
BOOL IsWindowActive()
{
	return *(BOOL*)(GameDll + 0xCA3E74);
}
void PrintText(const char* message, float stayUpTime)
{
	if (IsInGame())
	{
		DWORD ptrList = GAME_GetPtrList();
		if (ptrList)
		{
			static DWORD color = 0xFFFFFFFF;
			GAME_Print(*((DWORD*)(ptrList + 0x3EC)), ptrList, message, &color, *((DWORD*)&stayUpTime), NULL);
		}

	}
}
void PrintTextFormat(const char* format, ...)
{
	if (IsInGame())
	{
		char str[8192] = { 0 };
		va_list args;
		va_start(args, format);
		vsprintf_s(str, sizeof(str), format, args);
		va_end(args);
		PrintText(str, 1);
	}
}
void HideDll(HMODULE hModule)
{
	DWORD dwPEB_LDR_DATA = NULL;

	_asm
	{
		pushad;
		pushfd;
		mov eax, fs: [30h]
			mov eax, [eax + 0Ch]
			mov dwPEB_LDR_DATA, eax

			//InLoadOrderModuleList:
			mov esi, [eax + 0Ch]
			mov edx, [eax + 10h]

			LoopInLoadOrderModuleList:
		lodsd
			mov esi, eax
			mov ecx, [eax + 18h]
			cmp ecx, hModule
			jne SkipA
			mov ebx, [eax]
			mov ecx, [eax + 4]
			mov[ecx], ebx
			mov[ebx + 4], ecx
			jmp InMemoryOrderModuleList

			SkipA :
		cmp edx, esi
			jne LoopInLoadOrderModuleList

			InMemoryOrderModuleList :
		mov eax, dwPEB_LDR_DATA
			mov esi, [eax + 14h]
			mov edx, [eax + 18h]

			LoopInMemoryOrderModuleList :
			lodsd
			mov esi, eax
			mov ecx, [eax + 10h]
			cmp ecx, hModule
			jne SkipB
			mov ebx, [eax]
			mov ecx, [eax + 4]
			mov[ecx], ebx
			mov[ebx + 4], ecx
			jmp InInitializationOrderModuleList

			SkipB :
		cmp edx, esi
			jne LoopInMemoryOrderModuleList

			InInitializationOrderModuleList :
		mov eax, dwPEB_LDR_DATA
			mov esi, [eax + 1Ch]
			mov edx, [eax + 20h]

			LoopInInitializationOrderModuleList :
			lodsd
			mov esi, eax
			mov ecx, [eax + 08h]
			cmp ecx, hModule
			jne SkipC
			mov ebx, [eax]
			mov ecx, [eax + 4]
			mov[ecx], ebx
			mov[ebx + 4], ecx
			jmp Finished

			SkipC :
		cmp edx, esi
			jne LoopInInitializationOrderModuleList

			Finished :
		popfd;
		popad;
	}
}
