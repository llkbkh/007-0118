#include "pch.h"
#include "MenuHelper.h"
#include <fstream>
#include <PackWrite.h>
#include <PackReader.h>

MenuHelper* g_pTool = nullptr;

MenuHelper::MenuHelper()
{
	m_Hook    = std::make_shared<CACEHook>();
	m_ACEUtil = std::make_shared<CACEUtil>();
	m_MenuName.insert(std::make_pair(MENU_LANGUAGE::MU_CN, "CN_Menu.json"));
	m_MenuName.insert(std::make_pair(MENU_LANGUAGE::MU_TW, "TW_Menu.json"));
	m_MenuName.insert(std::make_pair(MENU_LANGUAGE::MU_EN, "EN_Menu.json"));

}

bool MenuHelper::LoadMenuCFG(MENU_LANGUAGE Language)
{
	m_Language = Language;

	auto Path  =  m_Hook->GetModulePath(m_Hook->GetSelfModuleHandle());

	m_MenuCfgPath = fmt::format("{}\\CFG\\{}", Path, m_MenuName.at(m_Language));

	std::ifstream is(m_MenuCfgPath.data());

	try
	{
		is >> m_data;

	}
	catch (nlohmann::detail::exception& e)
	{
		ACEDebugLog("[{}]exception:{} ",__FUNCTION__,e.what());
	}

	is.close();

	return true;
}

CString MenuHelper::GetMenuItemValue(std::string Item)
{
	std::string Value;
	try
	{
		Value = m_data["MU_Item"][Item];
	}
	catch (nlohmann::detail::exception& e)
	{
		ACEDebugLog("[{}]:{} Item:{}", __FUNCTION__, e.what(), Item);

		//Value = "Unknown_item";

		Value = Item;

	}

	//utf8->uncode
	return CString(m_ACEUtil->UTF8_To_UniCode(Value).data());
}

CString MenuHelper::GetMenuItemValueW(CString Item)
{
	auto UTF8_Text =  m_ACEUtil->UnicodeToUtf8(Item.GetBuffer());

	std::string Value;
	try
	{
		Value = m_data["MU_Item"][UTF8_Text];
	}
	catch (nlohmann::detail::exception& e)
	{
		ACEDebugLog("[{}]:{}", __FUNCTION__, e.what());
		//Value = "Unknown_item";
		Value = UTF8_Text;
	}

	//utf8->uncode
	auto data = m_ACEUtil->UTF8_To_UniCode(Value);

	ACEDebugLogW(L"[{}] {}",__FUNCTIONW__,data.data());
	return CString(data.data());
}


void MenuHelper::GetMenuItemValueExW(WCHAR* buffer, WCHAR* Item)
{
	auto UTF8_Text = m_ACEUtil->UnicodeToUtf8(Item);

	std::string Value;
	try
	{
		Value = m_data["MU_Item"][UTF8_Text];
	}
	catch (nlohmann::detail::exception& e)
	{
		ACEDebugLog("[{}]:{}", __FUNCTION__, e.what());
		//Value = "Unknown_item";
		Value = UTF8_Text;
	}

	auto Wstr =  m_ACEUtil->UTF8_To_UniCode(Value);

	memcpy(buffer, Wstr.data(),sizeof(WCHAR) * Wstr.length());

}

MENU_LANGUAGE MenuHelper::GetLanguage()
{
	return m_Language;
}

void MenuHelper::SetfunctionIntetface(ULONG Address)
{
	void (_cdecl* fn1)(PVOID pBuf, ULONG Size) = nullptr;
	(ULONG &)fn1 = Address;
	ClientInterface = fn1;

	ACEDebugLog("[{}]:ClientInterface:{:X}",__FUNCTION__,Address);
}

cvar7_s* MenuHelper::GetVarList()
{

	auto lPackWrite = std::make_shared<PackWriter>(1024);


	lPackWrite->WriteByte(FACE_CLIENT::CLIENT_GET_VARLIST);

	ClientInterface(lPackWrite->GetBytes(), lPackWrite->GetSize());

	auto lPackRead = std::make_shared<PackReader>(lPackWrite->GetBytes(), lPackWrite->GetSize());

	lPackRead->SetIndex(lPackWrite->GetBytesLength());

	BYTE Status            =  lPackRead->ReadChar();

	cvar7_s* VarPoint = nullptr;

	if (Status)
	{
		VarPoint = (cvar7_s*)lPackRead->ReadInt();
	}
	return VarPoint;
}

cvar7_s* MenuHelper::GetVar(char* Name)
{
	auto list = GetVarList();

	if (list == nullptr)
	{
		return list;
	}
	do 
	{
		list = list->next;

		if (lstrcmpiA(Name, list->name.data())==0)
		{
			break;
		}

	} while (list->next != nullptr);


	return list;
}

void MenuHelper::GameCommand(char* Text)
{
	static void(_cdecl * fnGameCommand)(char* Text) = nullptr;

	if (fnGameCommand == nullptr)
	{
		auto lPackWrite = std::make_shared<PackWriter>(1024);
		lPackWrite->WriteByte(FACE_CLIENT::COMMADN_FUN);
		ClientInterface(lPackWrite->GetBytes(), lPackWrite->GetSize());


		auto lPackRead = std::make_shared<PackReader>(lPackWrite->GetBytes(), lPackWrite->GetSize());

		lPackRead->SetIndex(lPackWrite->GetBytesLength());
		BYTE Status            = lPackRead->ReadChar();

		if (Status)
		{
			(ULONG &)fnGameCommand = lPackRead->ReadInt();
		}


	}

	return fnGameCommand(Text);
}

void MenuHelper::AlterTeam(BYTE Team)
{
	auto lPackWrite = std::make_shared<PackWriter>(1024);
	//Type
	lPackWrite->WriteByte(FACE_CLIENT::PACK_FUN_ALTER_TEAM);
	// 阵营
	lPackWrite->WriteByte(Team);

	ClientInterface(lPackWrite->GetBytes(), lPackWrite->GetSize());
	
}

void MenuHelper::SetGameNetWordStatus(BOOL Status)
{
	auto lPackWrite = std::make_shared<PackWriter>(1024);
	
	lPackWrite->WriteByte(FACE_CLIENT::SET_NET_STATUS);
	
	lPackWrite->WriteByte(Status);

	ClientInterface(lPackWrite->GetBytes(), lPackWrite->GetSize());

}

BOOL MenuHelper::LoadCFG(ULONG Status)
{

	auto lPackWrite = std::make_shared<PackWriter>(1024);

	lPackWrite->WriteByte(FACE_CLIENT::LOAD_CFG);

	lPackWrite->WriteByte(Status);

	ClientInterface(lPackWrite->GetBytes(), lPackWrite->GetSize());

	auto lPackRead = std::make_shared<PackReader>(lPackWrite->GetBytes(), lPackWrite->GetSize());

	lPackRead->SetIndex(lPackWrite->GetBytesLength());

	
	return lPackRead->ReadChar();
}

std::map<int, PGAME_ITEMMAP>* MenuHelper::GetGameItem()
{
	auto lPackWrite = std::make_shared<PackWriter>(1024);

	lPackWrite->WriteByte(FACE_CLIENT::MAP_GET_ITEM);
	lPackWrite->WriteByte(1);
	ClientInterface(lPackWrite->GetBytes(), lPackWrite->GetSize());

	auto lPackRead = std::make_shared<PackReader>(lPackWrite->GetBytes(), lPackWrite->GetSize());

	lPackRead->SetIndex(lPackWrite->GetBytesLength());

	BYTE Status = lPackRead->ReadChar();

	std::map<int, PGAME_ITEMMAP>* pResult = nullptr;

	if (Status)
	{
		pResult = (std::map<int, PGAME_ITEMMAP>*)lPackRead->ReadInt();
	}
	return pResult;
}

std::map<int, PGAME_MAPINFO>* MenuHelper::GetGameMap()
{
	auto lPackWrite = std::make_shared<PackWriter>(1024);

	lPackWrite->WriteByte(FACE_CLIENT::MAP_GET_ITEM);
	lPackWrite->WriteByte(2);

	ClientInterface(lPackWrite->GetBytes(), lPackWrite->GetSize());

	auto lPackRead = std::make_shared<PackReader>(lPackWrite->GetBytes(), lPackWrite->GetSize());

	lPackRead->SetIndex(lPackWrite->GetBytesLength());

	BYTE Status = lPackRead->ReadChar();

	std::map<int, PGAME_MAPINFO>* pResult = nullptr;

	if (Status)
	{
		pResult = (std::map<int, PGAME_MAPINFO>*)lPackRead->ReadInt();
	}
	return pResult;
}

std::map<int, std::wstring>* MenuHelper::GetGameMode()
{
	auto lPackWrite = std::make_shared<PackWriter>(1024);

	lPackWrite->WriteByte(FACE_CLIENT::MAP_GET_ITEM);
	lPackWrite->WriteByte(3);

	ClientInterface(lPackWrite->GetBytes(), lPackWrite->GetSize());

	auto lPackRead = std::make_shared<PackReader>(lPackWrite->GetBytes(), lPackWrite->GetSize());

	lPackRead->SetIndex(lPackWrite->GetBytesLength());

	BYTE Status = lPackRead->ReadChar();

	std::map<int, std::wstring>* pResult = nullptr;

	if (Status)
	{
		pResult = (std::map<int, std::wstring>*)lPackRead->ReadInt();
	}
	return pResult;
}

std::vector <TAG_LUA_SCRIPT_TABLE>* MenuHelper::GetLuaScriptInfo()
{
	auto lPackWrite = std::make_shared<PackWriter>(1024);

	lPackWrite->WriteByte(FACE_CLIENT::LOAD_LUA_SCRIPT);
	// 全部加载
	lPackWrite->WriteByte(1);

	ClientInterface(lPackWrite->GetBytes(), lPackWrite->GetSize());

	auto lPackRead = std::make_shared<PackReader>(lPackWrite->GetBytes(), lPackWrite->GetSize());

	lPackRead->SetIndex(lPackWrite->GetBytesLength());

	BYTE Status = lPackRead->ReadChar();

	std::vector <TAG_LUA_SCRIPT_TABLE>* pResult = nullptr;

	if (Status)
	{
		pResult = (std::vector<TAG_LUA_SCRIPT_TABLE>*)lPackRead->ReadInt();
	}
	return pResult;
}

BOOL MenuHelper::unAllLoadLuaScript()
{
	auto lPackWrite = std::make_shared<PackWriter>(1024);

	lPackWrite->WriteByte(FACE_CLIENT::LOAD_LUA_SCRIPT);
	// 全部卸载
	lPackWrite->WriteByte(4);

	ClientInterface(lPackWrite->GetBytes(), lPackWrite->GetSize());

	auto lPackRead = std::make_shared<PackReader>(lPackWrite->GetBytes(), lPackWrite->GetSize());

	lPackRead->SetIndex(lPackWrite->GetBytesLength());

	BYTE Status = lPackRead->ReadChar();

	return Status;
}

BOOL MenuHelper::LoadLuaScript(std::string FileName)
{
	auto lPackWrite = std::make_shared<PackWriter>(1024 + FileName.length());

	lPackWrite->WriteByte(FACE_CLIENT::LOAD_LUA_SCRIPT);
	// 全部卸载
	lPackWrite->WriteByte(2);
	lPackWrite->WriteString(FileName);

	ClientInterface(lPackWrite->GetBytes(), lPackWrite->GetSize());

	auto lPackRead = std::make_shared<PackReader>(lPackWrite->GetBytes(), lPackWrite->GetSize());

	lPackRead->SetIndex(lPackWrite->GetBytesLength());

	BYTE Status = lPackRead->ReadChar();

	return Status;
}

BOOL MenuHelper::unLoadLuaScript(std::string FileName)
{
	auto lPackWrite = std::make_shared<PackWriter>(1024 + FileName.length());

	lPackWrite->WriteByte(FACE_CLIENT::LOAD_LUA_SCRIPT);
	// 全部卸载
	lPackWrite->WriteByte(3);
	lPackWrite->WriteString(FileName);

	ClientInterface(lPackWrite->GetBytes(), lPackWrite->GetSize());

	auto lPackRead = std::make_shared<PackReader>(lPackWrite->GetBytes(), lPackWrite->GetSize());

	lPackRead->SetIndex(lPackWrite->GetBytesLength());

	BYTE Status = lPackRead->ReadChar();

	return Status;
}

void MenuHelper::SetGameHwnd(HWND h)
{
	m_GameHwnd = h;
}

HWND MenuHelper::GetGameHwnd()
{
	return m_GameHwnd;
}

cvar_s* MenuHelper::pfnGetCvarPointer(std::string name)
{
	auto lPackWrite = std::make_shared<PackWriter>(1024 + name.length());

	lPackWrite->WriteByte(FACE_CLIENT::GAME_CVAR);

	lPackWrite->WriteByte(1);
	lPackWrite->WriteString(name);
	ClientInterface(lPackWrite->GetBytes(), lPackWrite->GetSize());
	auto lPackRead = std::make_shared<PackReader>(lPackWrite->GetBytes(), lPackWrite->GetSize());
	lPackRead->SetIndex(lPackWrite->GetBytesLength());
	BYTE Status = lPackRead->ReadChar();
	cvar_s* pResult = nullptr;

	if (Status)
	{
		pResult = (cvar_s*)lPackRead->ReadInt();
	}
	return pResult;
}

void MenuHelper::Mp_Command(std::string Text)
{
	static void(_cdecl * fnMpCommand)(std::string Text) = nullptr;

	if (fnMpCommand == nullptr)
	{
		auto lPackWrite = std::make_shared<PackWriter>(1024 + Text.length());
		lPackWrite->WriteByte(FACE_CLIENT::WEAPON_MP);
		ClientInterface(lPackWrite->GetBytes(), lPackWrite->GetSize());

		auto lPackRead = std::make_shared<PackReader>(lPackWrite->GetBytes(), lPackWrite->GetSize());

		lPackRead->SetIndex(lPackWrite->GetBytesLength());
		BYTE Status = lPackRead->ReadChar();

		if (Status)
		{
			(ULONG&)fnMpCommand = lPackRead->ReadInt();
		}

	}

	return fnMpCommand(Text);
}

void MenuHelper::ShowMsg(char* Msg)
{
	auto Var  =  this->GetVar(xorstr_("sk_mfcMenu_enable_tips"));

	if (Var && Var->value)
	{
		AfxMessageBox(g_pTool->GetMenuItemValue(Msg), MB_OK);

	}
	return; 
}

void MenuHelper::ShowMsgKey(char* Msg, CString Key)
{
	auto Var = this->GetVar(xorstr_("sk_mfcMenu_enable_tips"));

	if (Var && Var->value)
	{
		auto Text = fmt::format(g_pTool->GetMenuItemValue(Msg).GetBuffer(), Key);

		AfxMessageBox(Text.data(), MB_OK|MB_ICONINFORMATION);

	}
	return;
}

int MenuHelper::ShowMsgOK(char* Msg)
{
	auto Var = this->GetVar(xorstr_("sk_mfcMenu_enable_tips"));

	int result = 0;

	if (Var && Var->value)
	{
		result = AfxMessageBox(g_pTool->GetMenuItemValue(Msg), MB_YESNO);

	}
	return result;
}

int MenuHelper::ShowMsgEr(char* Msg, BOOL bForce /*= TRUE*/)
{

	auto Var = this->GetVar(xorstr_("sk_mfcMenu_enable_tips"));

	int result = 0;

	if (Var && ((!bForce && Var->value) || bForce))
	{
		result = AfxMessageBox(g_pTool->GetMenuItemValue(Msg), MB_ICONSTOP);

	}
	return result;
}

std::string MenuHelper::GetCFGPath()
{
	auto Path = m_Hook->GetModulePath(m_Hook->GetSelfModuleHandle());

	return  fmt::format("{}\\CFG\\", Path);
}

std::shared_ptr<CACEUtil> MenuHelper::GetACEUtil()
{
	return m_ACEUtil;
}
