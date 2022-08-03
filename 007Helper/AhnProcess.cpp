#include "AhnProcess.h"
namespace AhnProcess {


	BOOL(WINAPI* _CreateProcessA)(LPCSTR lpApplicationName, LPSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes, BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCSTR lpCurrentDirectory, LPSTARTUPINFOA lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation) = NULL;

	BOOL WINAPI extCreateProcessA(LPCSTR lpApplicationName, LPSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes, BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCSTR lpCurrentDirectory, LPSTARTUPINFOA lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation)
	{

		DWORD	nFlags = dwCreationFlags | CREATE_SUSPENDED;
#if ANTI_DEBUG
		if (!(dwCreationFlags & DETACHED_PROCESS)) nFlags |= CREATE_NEW_CONSOLE;
#endif
		if (!_CreateProcessA) _CreateProcessA = CreateProcessA;

		BOOL nRet = _CreateProcessA(lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes, bInheritHandles, nFlags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, lpProcessInformation);
		if (nRet)
		{

			char	szFileName[MAX_PATH];

			auto Hook = std::make_unique<CACEHook>();
			auto Process = std::make_unique<CACEProcess>();

			GetModuleFileNameA(Hook->GetSelfModuleHandle(), szFileName, MAX_PATH);

			Process->ProcessCreateRmoterThreadEx(lpProcessInformation->hProcess, lpProcessInformation->hThread, szFileName);

		}

		if (!(dwCreationFlags & CREATE_SUSPENDED)) ResumeThread(lpProcessInformation->hThread);

		return nRet;
	}



	NTSTATUS(WINAPI* _ZwCreateUserProcess)(PHANDLE ProcessHandle, PHANDLE ThreadHandle, ACCESS_MASK ProcessDesiredAccess, ACCESS_MASK ThreadDesiredAccess, POBJECT_ATTRIBUTES ProcessObjectAttributes, POBJECT_ATTRIBUTES ThreadObjectAttributes, ULONG CreateProcessFlags, ULONG CreateThreadFlags, PRTL_USER_PROCESS_PARAMETERS ProcessParameters, PVOID Parameter9, void* AttributeList) = NULL;
	NTSTATUS WINAPI extZwCreateUserProcess(PHANDLE ProcessHandle, PHANDLE ThreadHandle, ACCESS_MASK ProcessDesiredAccess, ACCESS_MASK ThreadDesiredAccess, POBJECT_ATTRIBUTES ProcessObjectAttributes, POBJECT_ATTRIBUTES ThreadObjectAttributes, ULONG CreateProcessFlags, ULONG CreateThreadFlags, PRTL_USER_PROCESS_PARAMETERS ProcessParameters, PVOID Parameter9, void* AttributeList)
	{
		NTSTATUS	Status = _ZwCreateUserProcess(ProcessHandle, ThreadHandle, ProcessDesiredAccess, ThreadDesiredAccess, ProcessObjectAttributes, ThreadObjectAttributes, CreateProcessFlags, CreateThreadFlags, ProcessParameters, Parameter9, AttributeList);
		if (NT_SUCCESS(Status))
		{
			char	szFileName[MAX_PATH];

			auto Hook = std::make_unique<CACEHook>();
			auto Process = std::make_unique<CACEProcess>();

			GetModuleFileNameA(Hook->GetSelfModuleHandle(), szFileName, MAX_PATH);

			Process->ProcessCreateRmoterThreadEx(*ProcessHandle, *ThreadHandle, szFileName);
		}

		return Status;
	}


}