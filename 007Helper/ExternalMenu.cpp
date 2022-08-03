#include "ExternalMenu.h"
#include "Util.h"
#include "GamePack.h"
#include "CLuaScript.h"
#include "GameCFG.h"
#include "GameOffset.h"

ExternalMenu::ExternalMenu()
{
	m_Lib = nullptr;

}

ExternalMenu* ExternalMenu::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new ExternalMenu();
	}
	return m_pInstance;
}

int ExternalMenu::vInitialize(cl_enginefunc_t* pEnginefuncs, int iVersion)
{


	return 1;
}

bool ExternalMenu::vInit()
{
	m_Hook = g_pUtil->GetAceHook();

	//载入菜单链接库
	LoadExtMenu();

	//加载导出函数
	LoadArgs();

	//初始化
	SendAsyncCallMsg(CALLBACK_TYPE::TYPE_VAR_INIT);
	//设置接口
	SendAsyncCallMsg(CALLBACK_TYPE::TYPE_CLIENT_INTERFACE);

	//显示菜单
	SendAsyncCallMsg(CALLBACK_TYPE::TYPE_MENUDLG_INIT);

	//窗口句柄
	SendAsyncCallMsg(CALLBACK_TYPE::TYPE_GAME_WINDOWS_HWND);


	return true;
}

int ExternalMenu::vKeyBoardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	m_MenuKeyBoardProc(nCode, (int)wParam);


	return 1;
}



void ExternalMenu::Interface(PVOID pBuf, ULONG Size)
{
	auto lPackRead = std::make_shared<PackReader>((PBYTE)pBuf, Size);


	FACE_CLIENT Type = (FACE_CLIENT)lPackRead->ReadChar();

	auto lPackWrite = std::make_shared<PackWriter>(Size, "", (PBYTE)pBuf);


	lPackWrite->SetOffset(lPackRead->GetIndex());

	switch (Type)
	{
	case CLIENT_GET_VARLIST:
	{
		lPackWrite->WriteByte(1);
		lPackWrite->WriteInt((ULONG)g_Vars.GetInitCvar());
	}
		break;
	case MAP_GET_ITEM:
	{
		ULONG Team = lPackRead->ReadChar();

		lPackWrite->SetOffset(lPackRead->GetIndex());


		switch (Team)
		{
		case 1:
			lPackWrite->WriteByte(1);
			lPackWrite->WriteInt((ULONG)g_pUtil->GetItemMap());
			break;
		case 2:
			lPackWrite->WriteByte(1);
			lPackWrite->WriteInt((ULONG)g_pUtil->GetGameMap());
			break;
		case 3:
			lPackWrite->WriteByte(1);
			lPackWrite->WriteInt((ULONG)g_pUtil->GetGameMode());
			break;
		default:
			lPackWrite->WriteByte(0);
			break;
		}

	}
	break;
	case COMMADN_FUN:
	{
		lPackWrite->WriteByte(1);
		lPackWrite->WriteInt((ULONG)GameCommand);
	}
	break;
	case PACK_FUN_ALTER_TEAM:
	{
		ULONG Team = lPackRead->ReadChar();

		CGamePack::GetInstance()->Room_AlterTeam(static_cast<TEAM>(Team));

		//重连
		Engine::g_Engine.pfnClientCmd(xorstr_("retry"));

		lPackWrite->SetOffset(lPackRead->GetIndex());
		lPackWrite->WriteByte(1);
	}
	break;
	case LOAD_LUA_SCRIPT:
	{
		ULONG Type = lPackRead->ReadChar();

		auto pInstance = CLuaScriptManage::GetInstance();

		if (Type == 1)
		{
			//加载全部lua

			lPackWrite->SetOffset(lPackRead->GetIndex());

			BOOL bResult = pInstance->LocalLoadLuaScript();

			lPackWrite->WriteByte(bResult);

			if (bResult)
			{
				lPackWrite->WriteInt((ULONG)(pInstance->GetAllLuaScriptInfo()));
			}
			
		}
		else if (Type == 2)
		{
			//加载指定lua

			char* LuaScriptPath = lPackRead->ReadStr();

			BOOL bResult = pInstance->CreateLua(LuaScriptPath);

			lPackWrite->SetOffset(lPackRead->GetIndex());
			lPackWrite->WriteByte(bResult);

		}
		else if (Type == 3)
		{
			// 卸载指定lua
			char* LuaScriptPath = lPackRead->ReadStr();

			BOOL bResult = pInstance->UnLoadLuaScript(LuaScriptPath);

			lPackWrite->SetOffset(lPackRead->GetIndex());
			lPackWrite->WriteByte(bResult);

		}
		else if (Type == 4)
		{
			// 卸载全部lua
			pInstance->UnLoadAllLuaScript();

			lPackWrite->SetOffset(lPackRead->GetIndex());

			lPackWrite->WriteByte(1);
		}

	}
	break;
	case SET_NET_STATUS:
	{
		ULONG Status = lPackRead->ReadChar();

		// 1 = 断开  0 = 恢复
		auto pACEProcess = std::make_shared<CACEProcess>();
		auto pACEHook = std::make_shared<CACEHook>();

		pACEProcess->EnumProcessThread(GetCurrentProcessId(), [&](PSYSTEM_THREAD_INFORMATION pThread) {
			DWORD dwThreadId = (DWORD)pThread->ClientId.UniqueThread;
			DWORD dwThreadStartAddress = pACEProcess->QueryWin32StartAddress(dwThreadId);

			auto Module = pACEHook->GetModuleNameByAddress(dwThreadStartAddress);

			if (lstrcmpi(Module.data(),xorstr_("mswsock.dll")) == 0)
			{
				pACEProcess->SetThreadStatus(dwThreadId, Status);
			}

			ACEDebugLog("线程ID:0x{:X} 线程起始地址:0x{:X} 模块名：{}", dwThreadId, dwThreadStartAddress, Module);
		
			});




		lPackWrite->SetOffset(lPackRead->GetIndex());
		lPackWrite->WriteByte(1);
	}
	break;
	case LOAD_CFG:
	{
		ULONG Status = lPackRead->ReadChar();

		// 0 = 恢复默认  1 = 保存配置  2 = 加载配置
		BYTE bRet = FALSE;
		switch (Status)
		{
		case 0:
			bRet = g_Vars.DefaultVariable();
			break;
		case 1:
			bRet = g_GameCFG.SavaCVarToJsonFile();
			break;
		case 2:
			bRet = g_GameCFG.LoadJsonFile();
			break;
		default:
			break;
		}

		
		lPackWrite->SetOffset(lPackRead->GetIndex());
		lPackWrite->WriteByte(bRet);
	}
	break;
	case GAME_CVAR:
	{
		ULONG Status     = lPackRead->ReadChar();
		std::string Name = lPackRead->ReadStr();
		auto pCvar       = Engine::g_Engine.pfnGetCvarPointer(Name.data());

		lPackWrite->SetOffset(lPackRead->GetIndex());

		lPackWrite->WriteByte(pCvar ? true :false);

		if (pCvar)
		{
			lPackWrite->WriteInt((ULONG(pCvar)));
		}
		
	}
	break;
	case WEAPON_MP:
	{
		lPackWrite->SetOffset(lPackRead->GetIndex());

		lPackWrite->WriteByte(1);

		lPackWrite->WriteInt((ULONG(CommandByGiveWeapon)));
	}
	break;
	default:
		break;
	}


}

void ExternalMenu::GameCommand(char* Text)
{
	Engine::g_Engine.pfnClientCmd(Text);
}

void ExternalMenu::CommandByGiveWeapon(std::string Text)
{
	Engine::g_Engine.pfnClientCmd(Text.data());
}

void ExternalMenu::LoadExtMenu()
{
	m_Path = fmt::format(xorstr_("{}\\GameMenu.dll"), m_Hook->GetModulePath(m_Hook->GetSelfModuleHandle()));

	m_Lib = LoadLibraryA(m_Path.data());

	ACEDebugLog("[{}] Menu  Path:{} lib:{:X}", __FUNCTION__, m_Path, (ULONG32)m_Lib);
}

void ExternalMenu::LoadArgs()
{
	if (m_Lib)
	{
		//键盘钩子
		void (WINAPI * fn1)(int nCode, int keyNum) = nullptr;	
		(FARPROC &)fn1  = 	m_Hook->GetProcAddress(m_Lib, xorstr_("Menu_KeyBoardProc"));
		m_MenuKeyBoardProc = fn1;

		//通讯回调
		void (WINAPI * fn2)(PVOID pBuf, ULONG Size) = nullptr;
		(FARPROC&)fn2 = m_Hook->GetProcAddress(m_Lib, xorstr_("Menu_AsyncCallBack"));
		m_MenuAsyncCallBack = fn2;
	}
}

void ExternalMenu::SendAsyncCallMsg(CALLBACK_TYPE Type)
{

	auto lPackWriter = std::make_shared<PackWriter>(1024);

	lPackWriter->Clear();
	lPackWriter->WriteByte(Type);

	switch (Type)
	{
	case TYPE_MENUDLG_INIT:
		break;
	case TYPE_VAR_INIT:
	{
		//语言 国服->简体中文 台服->繁体中文 其他服->英文
		lPackWriter->WriteByte(g_Vars.m_Menu.sk_menu_language->value);

	}
		break;
	case TYPE_CLIENT_INTERFACE:
	{
		lPackWriter->WriteInt((ULONG)Interface);
		break;
	}
	case TYPE_GAME_WINDOWS_HWND:
	{
		lPackWriter->WriteInt((int)g_Vars.m_GameHwnd);
		break;
	}
	case TYPE_MENUDLG_DRAW:
	{
		lPackWriter->WriteInt(1);
		break;
	}
	default:
		break;
	}

	m_MenuAsyncCallBack(lPackWriter->GetBytes(), lPackWriter->GetBytesLength());
}
