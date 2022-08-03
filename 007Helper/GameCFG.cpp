#include "GameCFG.h"
#include<fstream>

GameCFG g_GameCFG;

GameCFG::GameCFG()
{
	m_Key = xorstr_("C1F0A83803EA3F3940C89A3195E879DB");


}
std::vector<BYTE> GameCFG::SRC4Decode(std::vector<BYTE> CipherByte)
{
	RC4_KEY s_table;
	RC4_set_key(&s_table, m_Key.length(), (unsigned char*)m_Key.c_str());
	std::vector<BYTE>DecodeText(CipherByte.size());
	RC4(&s_table, CipherByte.size(), (unsigned char*)CipherByte.data(), DecodeText.data()); 
	return DecodeText;
}

std::vector<BYTE> GameCFG::SRC4Encrypt(std::vector<BYTE> CipherByte)
{
	RC4_KEY s_table;
	RC4_set_key(&s_table, m_Key.length(), (unsigned char*)m_Key.c_str());
	std::vector<BYTE> Rc4Data(CipherByte.size());
	RC4(&s_table, CipherByte.size(), (unsigned char*)CipherByte.data(), Rc4Data.data());
	return Rc4Data;
}




void to_json(my_json& j, cvar7_s* p)
{
	CACEUtil Util;

	// 字符串RC4加密后 16进制字符串
	std::vector<BYTE> VecText(p->name.length());
	memcpy(VecText.data(), p->name.data(), VecText.size());
	auto VecEncryptByte = g_GameCFG.SRC4Encrypt(VecText);
	auto HexName =  Util.ByteToHex(VecEncryptByte.data(), VecEncryptByte.size());

	j =
	{
		{"name",	HexName},
		{"string",  p->string},
		{"flags",	p->flags},
		{"value",	p->value},
		{"HotKey",	p->hotkey}
	};
}

void from_json(const my_json & j, cvar7_s& s)
{
	j.at("name").get_to(s.name);
	j.at("string").get_to(s.string);
	j.at("flags").get_to(s.flags);
	j.at("value").get_to(s.value);
	j.at("HotKey").get_to(s.hotkey);
}



bool GameCFG::SavaCVarToJsonFile()
{
	//序列化
	CACEModule ACEModule;
	auto Path = fmt::format(xorstr_("{}\\cvar7.json"), ACEModule.GetModuleInfo()->ModulePath);

	std::ofstream of(Path);

	if (!of.is_open())
	{
		g_pConsole->DPrintf(xorstr_("open File is error\r\n"));
		return FALSE;
	}

	my_json cvardate;
	auto  pVars = g_Vars.GetInitCvar();
	try
	{

		while (pVars)
		{
			cvardate += pVars;
			pVars = pVars->next;
		}
		of << cvardate/*.dump(4)*/;
	}
	catch (nlohmann::detail::exception& e)
	{

#if ANTI_DEBUG
		g_pConsole->DPrintf("%s\r\n", e.what());
#endif
		g_pConsole->DPrintf(xorstr_("sava cvar error ! \r\n"));

		of.close();

		return false;
	}
	of.close();

	g_pConsole->DPrintf(xorstr_("sava cvar ok ! \r\n"));

	return true;
}

bool GameCFG::LoadJsonFile()
{
	bool bRet = false;
	CACEModule ACEModule;
	auto Path = fmt::format(xorstr_("{}\\cvar7.json"), ACEModule.GetModuleInfo()->ModulePath);
	auto pACEFile = std::make_shared<CACEFile>(Path);

	if (pACEFile->OpenFile(OPEN_EXISTING) == INVALID_HANDLE_VALUE)
	{
		ACEErrorLog("{}:open file error!", Path);
		return false;
	}
	auto  VecByte = pACEFile->ReadFile();
	my_json v1  =  nlohmann::json::parse(VecByte.begin(), VecByte.end());
	
	std::vector<cvar7_s> cvar = v1.get<std::vector<cvar7_s>>();


	for (std::vector<cvar7_s>::const_iterator iter = cvar.cbegin(); iter != cvar.cend(); iter++)
	{
		SearchVars(&(*iter));
	}

	bRet = true;
	g_pConsole->DPrintf(xorstr_("load cvar ok ! \r\n"));

	return bRet;
}

cvar7_s* GameCFG::SearchVars(const cvar7_s* p)
{
	cvar7_s* pVars = g_Vars.GetInitCvar();

	CACEUtil Util;
	
	while (pVars)
	{
		std::vector<BYTE> VecText(p->name.length() / 2);

		Util.HexToByte(p->name, VecText.data());

		auto VecDeCodeByte = g_GameCFG.SRC4Decode(VecText);


		std::string NodeName = std::string(VecDeCodeByte.begin(), VecDeCodeByte.end());

	
		if (lstrcmpA(pVars->name.data(), NodeName.data()) == 0)
		{
			pVars->value = p->value;

			pVars->flags = p->flags;

			pVars->hotkey = p->hotkey;


			return pVars;
		}
		pVars = pVars->next;
	}

	return pVars;
}


bool GameCFG::LoadCore()
{
	auto Hook = CACEHook();

	auto CorePath = fmt::format(xorstr_("{}\\CFG\\Core.json"), Hook.GetModulePath(to_json));

	std::ifstream is(CorePath);

	if (!is.is_open())
	{
		g_pConsole->DPrintf(xorstr_("Open File is error\r\n"));

		return FALSE;
	}

	is >> m_CoreJson;

	return true;
}

std::string GameCFG::GetInMenuSubItem(std::string Item)
{
	std::string SubItem;
	try
	{
		if (g_Vars.m_Menu.sk_menu_language->value == 1)
		{
			SubItem = m_CoreJson[xorstr_("menu_cn")][Item];
		}
		else if (g_Vars.m_Menu.sk_menu_language->value == 2)
		{
			SubItem = m_CoreJson[xorstr_("menu_tw")][Item];
		}
		else
		{
			SubItem = m_CoreJson[xorstr_("menu_en")][Item];
		}


	}
	catch (nlohmann::detail::exception& e)
	{

#if ANTI_DEBUG
		g_pConsole->DPrintf("%s\r\n", e.what());
#endif
		SubItem = Item;
	}

	return SubItem;
}

std::string GameCFG::GetMsgSubItem(std::string Msg)
{
	std::string SubItem;
	try
	{
		if (g_Vars.m_Menu.sk_menu_language->value == 1)
		{
			SubItem = m_CoreJson[xorstr_("msg_cn")][Msg];
		}
		else if (g_Vars.m_Menu.sk_menu_language->value == 2)
		{
			SubItem = m_CoreJson[xorstr_("msg_tw")][Msg];
		}
		else
		{
			SubItem = m_CoreJson[xorstr_("msg_en")][Msg];
		}
	}
	catch (nlohmann::detail::exception& e)
	{

#if ANTI_DEBUG
		g_pConsole->DPrintf("%s\r\n", e.what());
#endif
		SubItem = Msg;
	}

	return SubItem;
}
