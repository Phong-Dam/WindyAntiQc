#pragma once
#include "Windows.h"
#include <stdio.h>
const DWORD dwGameDll = (DWORD)GetModuleHandle("Game.dll");
const int GameDll = (int)GetModuleHandle("Game.dll");
typedef DWORD __fastcall GAME_GetPtrList_t(VOID);
extern GAME_GetPtrList_t* GAME_GetPtrList;
typedef void __fastcall GAME_Print_t(DWORD ptrList, DWORD _EDX, const CHAR* text, DWORD* color, DWORD stayUpTime, DWORD _1);
extern GAME_Print_t* GAME_Print;
void PrintText(const char* message, float stayUpTime = 1);
void PrintTextFormat(const char* format, ...);
//Func
BOOL IsInGame();
void Init();
void HideDll(HMODULE hMod);
BOOL IsWindowActive();
void CheckMacro();
void Patch(DWORD dwBaseAddress, const char* szData, int iSize);
typedef LRESULT(__fastcall* WarcraftRealWNDProc)(HWND hWnd, unsigned int Msg, WPARAM wParam, LPARAM lParam);
extern WarcraftRealWNDProc WarcraftRealWNDProc_ptr ;
extern WarcraftRealWNDProc WarcraftRealWNDProc_org ;
extern bool WFEEnable;
extern HWND WarcraftReal_HWND;
extern LPARAM WarcraftReal_LPARAM;
extern bool SendQc;
extern BOOL INGAME;
BOOL IsChat();
extern bool skipms;
int GetLocalPlayerNumber();
BOOL IsOkayPtr(void* ptr, unsigned int size = 4);;