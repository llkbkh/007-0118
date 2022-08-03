#include "AhnCheat.h"
#include "AhnProcess.h"
#include "CSagaClient.h"


#if 0
int KsGenerateMiniDump(PEXCEPTION_POINTERS pExceptionPointers)
{
	// 定义函数指针
	typedef BOOL(WINAPI* MiniDumpWriteDumpT)(
		HANDLE,
		DWORD,
		HANDLE,
		MINIDUMP_TYPE,
		PMINIDUMP_EXCEPTION_INFORMATION,
		PMINIDUMP_USER_STREAM_INFORMATION,
		PMINIDUMP_CALLBACK_INFORMATION
		);

	// 从 "DbgHelp.dll" 库中获取 "MiniDumpWriteDump" 函数
	MiniDumpWriteDumpT pfnMiniDumpWriteDump = NULL;
	HMODULE hDbgHelp = LoadLibrary("DbgHelp.dll");
	if (NULL == hDbgHelp)
	{
		return EXCEPTION_CONTINUE_EXECUTION;
	}
	pfnMiniDumpWriteDump = (MiniDumpWriteDumpT)GetProcAddress(hDbgHelp, "MiniDumpWriteDump");

	if (NULL == pfnMiniDumpWriteDump)
	{

		FreeLibrary(hDbgHelp);

		return EXCEPTION_CONTINUE_EXECUTION;
	}


	char	szFileName[MAX_PATH];

	lstrcpyA(szFileName + GetCurrentDirectoryA(MAX_PATH, szFileName), "\\Dump");
	CreateDirectoryA(szFileName, NULL);

	SYSTEMTIME stLocalTime;
	GetLocalTime(&stLocalTime);
	wsprintfA(szFileName + GetCurrentDirectoryA(MAX_PATH, szFileName), "\\Dump\\%s-%04d%02d%02d-%02d_%02d_%02d.dmp", "cso", stLocalTime.wYear, stLocalTime.wMonth, stLocalTime.wDay,
		stLocalTime.wHour, stLocalTime.wMinute, stLocalTime.wSecond);



	HANDLE hDumpFile = CreateFile(szFileName, GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_WRITE | FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, 0);

	if (INVALID_HANDLE_VALUE == hDumpFile)
	{
		FreeLibrary(hDbgHelp);
		return EXCEPTION_CONTINUE_EXECUTION;
	}
	// 写入 dmp 文件
	MINIDUMP_EXCEPTION_INFORMATION expParam;
	expParam.ThreadId = GetCurrentThreadId();
	expParam.ExceptionPointers = pExceptionPointers;
	expParam.ClientPointers = FALSE;
	auto Result = pfnMiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(),
		hDumpFile, MiniDumpWithDataSegs, (pExceptionPointers ? &expParam : NULL), NULL, NULL);

	// 释放文件
	CloseHandle(hDumpFile);
	FreeLibrary(hDbgHelp);
	return EXCEPTION_EXECUTE_HANDLER;
}

LONG NTAPI AhnExceptionHandler(struct _EXCEPTION_POINTERS* ExceptionInfo)
{
	ULONG				nRet = EXCEPTION_CONTINUE_SEARCH;

	PEXCEPTION_RECORD	Record = ExceptionInfo->ExceptionRecord;
	PCONTEXT			Context = ExceptionInfo->ContextRecord;

	switch (Record->ExceptionCode)
	{

	case EXCEPTION_INT_DIVIDE_BY_ZERO:
	case 0xE06D7363:	//RadioException
	case EXCEPTION_ILLEGAL_INSTRUCTION:
	case EXCEPTION_PRIV_INSTRUCTION:
	case 0x40010006:
	{
		return EXCEPTION_CONTINUE_SEARCH;
	}
	case  0xC0000374:
	{
		/*
		游戏未正常运行,关闭句柄出现的异常
		*/
		nRet = EXCEPTION_CONTINUE_EXECUTION;
		break;

	}
	case  0xC0000005:
	{
		/*
				C05 Dump下来
		*/
		KsGenerateMiniDump(ExceptionInfo);
		break;

	}

	}
	ACEDebugLog("[{}]{}", __FUNCTION__, Record->ExceptionCode);

	return nRet;
}
#endif



BOOL APIENTRY DllMain( HMODULE hModule,DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	VMProtectBegin(__FUNCTION__);

    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
		auto Module  = std::make_unique<CACEModule>();
		auto HOOK    = std::make_unique<CACEHook>();
		auto Process = std::make_unique<CACEProcess>();
		Process->ProcessSetToken();
		DisableThreadLibraryCalls(HOOK->GetSelfModuleByAddress(hModule));

		TAG_ACE_HOOK_MODULE_INFO AppList[] =
		{
		    ACE_HOOK_MODULE_INFO("007TestStartApp.exe",true),
			ACE_HOOK_MODULE_INFO("1","CSOLauncher.exe",	"NexonCSOMutex"),					//反恐精英OL 登陆器
			ACE_HOOK_MODULE_INFO("2","cstrike-online.exe",	"NexonCSOMutex"),			    //反恐精英OL 主程序
			ACE_HOOK_MODULE_INFO("3","TestApp.exe",		""),								//测试程序
			ACE_HOOK_MODULE_INFO("6","007Launcher.exe",	""),								//启动器
			ACE_HOOK_MODULE_INFO("7","steam.exe",			""),				            //CSNS
			ACE_HOOK_MODULE_INFO("8","TestApp-Start.exe",	""),				            //cn
			ACE_HOOK_MODULE_INFO("8","TestApp_HF.exe",	""),								//hf
			ACE_HOOK_MODULE_INFO("8","TestApp-TW.exe",	""),								//TW
			ACE_HOOK_MODULE_INFO("8","007TestStartApp_CSNS.exe",	""),								//TW
			ACE_HOOK_MODULE_INFO("8","LCUI_Lanuncher.exe",	""),								
		
		};

		if (!CACEModule::GetModuleDetect(AppList, sizeof(AppList) / sizeof(TAG_ACE_HOOK_MODULE_INFO)))  return false;

		auto AppName = Module->GetModuleInfo()->AppName;

#if ANTI_DEBUG
		//安装日志
		ACELog.ACELogInstal(AppName, ACE_LOG_FILE_ONCUT);
		ACELog.ACEDebugView(CONSOLE| DEBUG_VIEW);
#endif
		

		//Hook启动进程
		HOOK->HookSetDetectAll(&TAG_ACE_HOOK_API_INFO(xorstr_("kernel32.dll"), xorstr_("CreateProcessA"), AhnProcess::extCreateProcessA, &AhnProcess::_CreateProcessA, ACE_HOOK_TYPE_IAT | ACE_HOOK_TYPE_EAT));


		if (lstrcmpiA(AppName, "cstrike-online.exe") == 0)
		{
			//PVOID	VectException = AddVectoredExceptionHandler(EXCEPTION_EXECUTE_HANDLER, AhnExceptionHandler);

			AhnCheat* pCheat = new AhnCheat();
			pCheat->CheatInit();

			CSagaClient::GetInstance()->StartThreadRCFInit();


		}
		else if (lstrcmpiA(AppName, "007TestStartApp.exe") == 0)
		{


		}
		else if (lstrcmpiA(AppName, "007Launcher.exe") == 0)
		{
			HOOK->HookSetDetectAll(&TAG_ACE_HOOK_API_INFO("ntdll.dll", "NtCreateUserProcess", AhnProcess::extZwCreateUserProcess, &AhnProcess::_ZwCreateUserProcess, ACE_HOOK_TYPE_IAT | ACE_HOOK_TYPE_EAT));

		}
		else if (lstrcmpiA(AppName, "steam.exe") == 0)
		{
			HOOK->HookSetDetectAll(&TAG_ACE_HOOK_API_INFO("ntdll.dll", "NtCreateUserProcess", AhnProcess::extZwCreateUserProcess, &AhnProcess::_ZwCreateUserProcess, ACE_HOOK_TYPE_IAT | ACE_HOOK_TYPE_EAT));

		}
    }

	VMProtectEnd();
    return TRUE;
}

