// LCUI_Lanuncher.cpp : 定义应用程序的入口点。
//
#include "LCUIGui.h"
#include <windows.h>
#include "LCUI_Lanuncher.h"
#include "Util.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR    lpCmdLine,_In_ int       nCmdShow)
{
	VMProtectBegin(__FUNCTION__);

	TAG_ACE_HOOK_MODULE_INFO AppList[] =
	{
		ACE_HOOK_MODULE_INFO("0","007Launcher.exe","54ED5FB2C037A6BE7ED4467553ED4C9E"),								//启动器
		//ACE_HOOK_MODULE_INFO("0","LCUI_Lanuncher.exe","54ED5FB2C037A6BE7ED4467553ED4C9E"),							//启动器
	};
	auto Hook = CACEInterface::GetInstance()->GetHook();
	if (!CACEModule::GetModuleDetect(AppList, sizeof(AppList) / sizeof(TAG_ACE_HOOK_MODULE_INFO)))
	{
		std::string Text = fmt::format(xorstr_("{}{}"), xorstr_(u8"请勿修改进程名:"), Hook->GetModuleNameByAddress((DWORD)Hook->GetSelfModuleHandle()));
		MessageBoxW(0, CACEInterface::GetInstance()->GetUtil()->UTF8_To_UniCode(Text).data(), 0, MB_ICONSTOP);
		return FALSE;
	}
	//检查多开
	if (!g_Util.CheckMulti())
	{
		return FALSE;
	}
	//检查自身路径 判断是否在压缩包内
	if (!g_Util.CheckSelfPathIsZipTempPath())
	{
		return FALSE;
	}
	//检查自身路径
	if (!g_Util.CheckSelfPath())
	{
		return FALSE;
	}
	//创建快捷方式
	g_Util.CreateShortcut();
	//结束游戏进程
	g_Util.ExitGameProcess();
	//创建监测游戏线程
	g_Util.CreateThreadMonitorinGame();

	CLCUIGui gui;

	if (gui.Init() )
	{
		gui.ConfigInit();
		gui.ViewInit();
		gui.Loop();
	}
	VMProtectEnd();
	return 0;
}
