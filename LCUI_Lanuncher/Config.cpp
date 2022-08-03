#include "Config.h"
#include <xorstr.hpp>

Config::Config()
{
	m_Instance = this;
	m_Filename = xorstr_("C:\\Loginkss.json");
}

Config::~Config()
{

}

Config* Config::GetInstance()
{
	return m_Instance;
}

void to_json(nlohmann::json& j, const JSON_FileInfo& p)
{
	j = nlohmann::json
	{
		{"Card", p.Card},
		{"GamePath", p.GamePath},
		{"PASW", p.PASW},
		{"SteamPath", p.SteamPath} ,
		{"User", p.User},
		{"GameServer", p.GameServer},
	};
	j["HttpProxy"] = { {"enable", p.Proxy.bEnable}, {"ip", p.Proxy.ip} ,{"port", p.Proxy.port} ,{"user", p.Proxy.user} ,{"password", p.Proxy.password} };
	j["Translate"] = { {"language", p.Translate.language}, {"url", p.Translate.url} };

}

void from_json(const nlohmann::json& j, JSON_FileInfo& p)
{
	j.at("Card").get_to(p.Card);
	j.at("GamePath").get_to(p.GamePath);
	j.at("PASW").get_to(p.PASW);
	j.at("SteamPath").get_to(p.SteamPath);
	j.at("User").get_to(p.User);

	if (j.count("GameServer"))
	{
		j.at("GameServer").get_to(p.GameServer);
	}
	if (j.count("HttpProxy"))
	{
		j.at("HttpProxy").at("enable").get_to(p.Proxy.bEnable);
		j.at("HttpProxy").at("ip").get_to(p.Proxy.ip);
		j.at("HttpProxy").at("port").get_to(p.Proxy.port);
		j.at("HttpProxy").at("user").get_to(p.Proxy.user);
		j.at("HttpProxy").at("password").get_to(p.Proxy.password);
	}
	if (j.count("Translate"))
	{
		j.at("Translate").at("language").get_to(p.Translate.language);
		j.at("Translate").at("url").get_to(p.Translate.url);

	}
}
void Config::Load()
{
	auto pFile = std::make_shared<CACEFile>(m_Filename);

	HANDLE hFile = pFile->OpenFile();

	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBoxW(0, xorstr_(L"读取文件配置失败！请右键辅助 [以管理员身份运行]！"), 0, MB_ICONSTOP);
		return;
	}
	//读取文件
	auto VecResult = pFile->ReadFile();
	try
	{
		if (VecResult.size())
		{
			m_JsonInfo = nlohmann::json::parse(VecResult.begin(), VecResult.end());
			m_Config = m_JsonInfo.get<JSON_FileInfo>();
		}
		else
		{
			//写入新的文件 struct->josn
			m_JsonInfo = m_Config;
			auto info = m_JsonInfo.dump(4);
			pFile->WriteFile(info.data(), info.size());
		}
	}
	catch (nlohmann::detail::exception& e)
	{
		MessageBoxW(0, CACEInterface::GetInstance()->GetUtil()->UTF8_To_UniCode(e.what()).data(), 0, MB_ICONSTOP);
	}
	pFile->CloseFile();
}

void Config::Save()
{
	auto pFile = std::make_shared<CACEFile>(m_Filename);

	if (DeleteFileA(m_Filename.data()) == FALSE )
	{
		//删除失败了.
		MessageBoxW(0, xorstr_(L"保存配置失败,请重试！"), 0, MB_ICONSTOP);
		return;
	}


	HANDLE hFile = pFile->OpenFile();

	if (hFile == INVALID_HANDLE_VALUE)
		return;
	//重写
	pFile->SetFilePointer(FILE_BEGIN);

	m_JsonInfo = m_Config;
	auto info = m_JsonInfo.dump(4);
	pFile->WriteFile(info.data(), info.size());

	pFile->CloseFile();
}

std::string Config::GetRandTitle()
{
	auto pUtil = CACEInterface::GetInstance()->GetUtil();
	
	//随机字符
	std::string str = xorstr_("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");


	int nCount = pUtil->GetRandomNumber(10, str.length() - 1);;

	std::string result;

	// len = 5
	for (size_t i = 0; i < 5; i++)
	{
		ULONG k = pUtil->GetRandomNumber(0, str.length() - 1);

		result.push_back(str.at(k));
	}
	return result;
}

JSON_FileInfo& Config::GetConfig()
{
	return m_Config;
}

