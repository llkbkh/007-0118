// 007TestStartApp.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <ACEConnect.h>
#include <functional>
#include <regex>
#include <winbase.h>
#include <io.h>
#pragma comment(lib,"RcfDll.lib")

BOOL(WINAPI* AhnCreateProcessA)(char* ulFileName, char* ulCmdLine, char* ulPath) = NULL;
extern "C" 
{
	#include "lua\lua.h"
	#include "lua\lualib.h"
	#include "lua\lauxlib.h"
}
#pragma comment(lib,"lua.lib")
lua_State* L = nullptr;

static int l_GameCommand(lua_State* L)
{
	const char* TextCommand = luaL_checkstring(L, 1);//获取参数，字符串类型

	printf("Game Command:%s\n", TextCommand);
	return 1;
}


#include "taskflow/taskflow.hpp"


int main()
{




	//L = luaL_newstate();  /* 创建lua状态机 */
	//luaL_openlibs(L);   /* 打开Lua状态机中所有Lua标准库 */

	//lua_pushcfunction(L, l_GameCommand);
	//lua_setglobal(L, "l_GameCommand");

	//int ret;
	//if (ret = luaL_loadfile(L, "E:\\Lua\\test01.lua"))//装载lua脚本
	//{
	//	return false;
	//}
	//if (ret = lua_pcall(L, 0, 0, 0))//运行lua脚本
	//{
	//	::printf(lua_tostring(L, -1));

	//	return false;
	//}



	//lua_getglobal(L, "f");//获取需要调用的Lua函数f
	//lua_pushnumber(L, 1);//入栈第一个参数
	//lua_pushnumber(L, 2);//入栈第二个参数

	////调用函数, 2个参数,1个返回值
	//if (lua_pcall(L, 2, 1, 0) != LUA_OK)
	//{
	//	
	//}
	//int isnum = 0;
	//int Value = 0;
	//Value = lua_tonumberx(L, -1, &isnum);//获取返回值
	//if (!isnum)
	//{
	//	//error(L, "function 'f' shound return a number");
	//}
	//printf("Value: %d", isnum);

	//lua_pop(L, 1);
	//

	//lua_getglobal(L, "l_Sciript");
	////压入表中的key
	//lua_pushstring(L, "Name");

	//lua_gettable(L, 1);

	//const char* name = lua_tostring(L, -1); //在栈顶取出数据

	//CACEUtil Tool;
	//auto v1 =  Tool.UTF8_To_string(name);
	//printf("name:%s\n", v1.data());





	///*清除Lua*/
	//lua_close(L);



	//

	//getchar();







	//if (!CACEModule::GetModuleDetect(&TAG_ACE_HOOK_MODULE_INFO(xorstr_("007TestStartApp.exe")), 1))
	//{
	//	return 0;
	//}

	auto hModule = LoadLibraryA("007Helper.dll");

	ACEInfoLog("[lib]{:X}", (ULONG)hModule);

	if (hModule)
	{
		(FARPROC&)AhnCreateProcessA = GetProcAddress(hModule, "AhnCreateProcessA");
		AhnCreateProcessA("D:\\Game\\Counter-Strike Online\\Counter-Strike Online\\Bin\\CSOLauncher.exe", "", "D:\\Game\\Counter-Strike Online\\Counter-Strike Online\\Bin");
		//AhnCreateProcessA("D:\\Game\\TW_CSOL\\TW_CSOL\\Bin\\CSOLauncher.exe", "", "D:\\Game\\TW_CSOL\\TW_CSOL\\Bin");

		//
	
		//AhnCreateProcessA("D:\\Game\\CSO_HF\\Counter-Strike Online\\Bin\\CSOLauncher.exe", "", "D:\\Game\\CSO_HF\\Counter-Strike Online\\Bin");
		//AhnCreateProcessA("D:\\Counter-Strike Online\\Bin\\CSOLauncher.exe", "", "D:\\Counter-Strike Online\\Bin");
		//
		//AhnCreateProcessA("C:\\Program Files (x86)\\Steam\\steamapps\\common\\CSNZ\\Bin\\cstrike-online.exe", "", "C:\\Program Files (x86)\\Steam\\steamapps\\common\\CSNZ\\Bin");
	}

	
	// 手动注入

	//auto Process = std::make_unique<CACEProcess>();

	//auto Pid =  Process->GetProcessId("cstrike-online.exe");

	//;

	//Process->ProcessCreateRmoterThread(Process->OpenProcess(Pid), "E:\\Amber\\Project\\007Manager\\Release\\007Helper.dll");


	//getchar();
   
}
#pragma data_seg(".Shared")
namespace SharedData
{
	int		Key = 0x123;
	int		Value = 0x321;
};

#pragma comment(linker,"/section:.Shared,RWS")