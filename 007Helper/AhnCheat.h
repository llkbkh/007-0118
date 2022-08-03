#pragma once
#include "007Helper.h"
#include "Util.h"
class AhnCheat
{
public:
	AhnCheat();
	static AhnCheat* GetInstance();
	void CheatInit();





public:
	static NTSTATUS NTAPI extLdrLoadDll(PWCHAR PathToFile OPTIONAL, ULONG Flags OPTIONAL, PUNICODE_STRING ModuleFileName, PHANDLE ModuleHandle);
	static FARPROC WINAPI extGetProcAddress(HMODULE hModule, LPCSTR lpProcName);
	static BOOL WINAPI extCreateProcessA(LPCSTR lpApplicationName, LPSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes, BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCSTR lpCurrentDirectory, LPSTARTUPINFOA lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation);
	static HWND WINAPI extCreateWindowExA(
		_In_ DWORD dwExStyle,
		_In_opt_ LPCSTR lpClassName,
		_In_opt_ LPCSTR lpWindowName,
		_In_ DWORD dwStyle,
		_In_ int X,
		_In_ int Y,
		_In_ int nWidth,
		_In_ int nHeight,
		_In_opt_ HWND hWndParent,
		_In_opt_ HMENU hMenu,
		_In_opt_ HINSTANCE hInstance,
		_In_opt_ LPVOID lpParam);

	static int F(PVOID Pointer);

private:
	DWORD InternetEngineStartMode(LPVOID lpThreadParameter);
	// @作弊器线程
	ULONG CheatEntry();
	// @Load其他DLL
	bool LoadAddLibrary();
private:

	static AhnCheat*          m_Instance;
	HMODULE	                  m_EngineLib;
	
};

