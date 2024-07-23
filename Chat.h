#pragma once
typedef int(__fastcall* pSetChatTargetUsers/*sub_6F3412F0*/)(int chataddr, int ecx, int valtype);
extern pSetChatTargetUsers pSetChatTargetUsers_org;
extern pSetChatTargetUsers pSetChatTargetUsers_ptr;
typedef int(__fastcall* sub_10061900)(const char* a1, const char* Format, ...);
typedef int(__fastcall* pGameChatSetState)(unsigned char* chat, int unused, int IsOpened);
extern pGameChatSetState GameChatSetState;
extern sub_10061900 WfeCall;
extern int UsingCustomChatTarget;
extern int CustomChatTarget;
void InitChat();
void SendChat(int type, const char* format, ...);
unsigned char* GetChatOffset();
char* GetChatString();