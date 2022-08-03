#include "FyAuthorization.h"
#include <ACEHWID.h>

FyAuthorization::FyAuthorization(void)
{
	m_MAC        = xorstr_("unknown mac");
	m_InitResult = { FALSE,"",nullptr };
	n_pInstance  = this;
}

FyAuthorization::~FyAuthorization(void)
{

}

void FyAuthorization::Init()
{

	if (std::get<0>(m_InitResult) == true)
	{
		//已经成功连接上了.无需再连接了
		return;
	}

	CACEHwid Hwid;
	auto pNet = Hwid.GetActiveNetworkAdapter(FALSE);
	if (true)
	{
		m_MAC = pNet->GetMacAddress();
		//m_MAC = "-2--";
	}

	TAG_ANTI_FREEYUN_PROXY Proxy;
	auto Config = Config::GetInstance()->GetConfig();

	if (Config.Proxy.bEnable)
	{
		Proxy.IP       = Config.Proxy.ip;
		Proxy.Port     = Config.Proxy.port;
		Proxy.User     = Config.Proxy.user;
		Proxy.PassWord = Config.Proxy.password;
	}
	m_InitResult = m_FreeYun.CloudInit(&TAG_ANTI_FREEYUN_INIT_INFO(xorstr_(""), xorstr_(""), xorstr_(""), xorstr_(""), xorstr_(""), m_MAC, 0, Proxy));

	
}

std::tuple<bool, std::string>FyAuthorization::GetNotic()
{
	
	if (std::get<0>(m_InitResult) == false)
	{
		// 网络连接失败
		return {false,std::get<1>(m_InitResult)};
	}
	auto& data = std::get<2>(m_InitResult);

	int nCode = data.at(xorstr_("code"));

	if (nCode != 1003)
	{
		//状态码错误
		return { false,m_FreeYun.GetErrorStr(nCode)};
	}

	return {true,data.at(xorstr_("notic"))};
}

std::tuple<bool, std::string> FyAuthorization::Register(std::string username, std::string password, std::string email)
{
	//手机号为 版本号
	TAG_ANTI_FREEYUN_REG RegisterInfo = TAG_ANTI_FREEYUN_REG(username, password,"",email, UP_007,"","");

	auto result =  m_FreeYun.CloudReg(&RegisterInfo);

	return { std::get<0>(result),std::get<1>(result) };

}

std::tuple<bool, std::string> FyAuthorization::UserPay(std::string username, std::string CardNumber)
{
	auto result = m_FreeYun.CloudPay(username, CardNumber);
	return { std::get<0>(result),std::get<1>(result) };
}

std::tuple<bool, std::string> FyAuthorization::UserLogin(std::string username, std::string password)
{
	auto result = m_FreeYun.CloudLogin(username, password, UP_007);

	if (std::get<0>(result) == false)
	{
		int nCode = m_FreeYun.GetErrorCode();

		if (nCode == 1010)
		{
			//需解绑
			result = m_FreeYun.CloudUserUnBind(username, password);

			if (std::get<0>(result) == true)
			{
				return { false,xorstr_(u8"出现此提示,证明需要账号解绑[本次登陆电脑与上次登录电脑不符],已为您自动解绑.你需要再登陆一次！") };
			}
			else
			{
				return { false,xorstr_(u8"解绑失败,账号过期") };
			}
		}
		return { false,m_FreeYun.GetErrorStr(nCode) };
	}

	return { std::get<0>(result),std::get<1>(result) };
}

std::tuple<bool, std::string> FyAuthorization::CardLogin(std::string CardNumber)
{
	auto result = m_FreeYun.CloudCardLogin(CardNumber, UP_007);

	if (std::get<0>(result) == false)
	{
		int nCode = m_FreeYun.GetErrorCode();

		if (nCode == 1010)
		{
			//需解绑
			result = m_FreeYun.CloudUserUnBind(CardNumber);

			if (std::get<0>(result) == true)
			{
				return { false,xorstr_(u8"出现此提示,证明需要卡号解绑[本次登陆电脑与上次登录电脑不符],已为您自动解绑.你需要再登陆一次！") };
			}
			else
			{
				return { false,xorstr_(u8"解绑失败,卡号过期") };
			}
		}
		
		return { false,m_FreeYun.GetErrorStr(nCode) };
	}

	return { std::get<0>(result),std::get<1>(result) };
}

std::tuple<bool, std::string> FyAuthorization::UserUnbind(std::string username, std::string password)
{
	auto result = m_FreeYun.CloudUserUnBind(username, password);
	return { std::get<0>(result),std::get<1>(result) };
}

std::tuple<bool, std::string> FyAuthorization::VerifyRemoteVariable(std::string username)
{
	VMProtectBegin(__FUNCTION__);
	auto  result = m_FreeYun.CloudGetTeletVar(username, xorstr_("Cstrike_Login"));
	
	if (std::get<0>(result) == false)
	{
		return {false,std::get<1>(result) };
	}
	auto& dataVar = std::get<2>(result);
	std::string VarText = dataVar.at(xorstr_("variable"));
	if (lstrcmpiA(VarText.data(), xorstr_("#Check_OK")) != 0)
	{
		return { false,xorstr_(u8"远程信息效验失败,登陆失败！")};
	}
	VMProtectEnd();
	return { true,std::get<1>(result) };
}

std::tuple<bool, std::string> FyAuthorization::CheckVersion(std::string username, std::string& version)
{
	auto result = m_FreeYun.CloudGetTeletVar(username, xorstr_("CS_Updata_New007"));

	if (std::get<0>(result))
	{

		auto& dataVar = std::get<2>(result);
		std::string VarText = dataVar.at(xorstr_("variable"));

		if (lstrcmpiA(VarText.data(), UP_007) == 0)
		{
			return { true, ""};
		}
		version = VarText;
		
		return { false,xorstr_(u8"版本不匹配,请升级版本！") };
	}

	return { std::get<0>(result) ,std::get<1>(result) };
}

std::tuple<bool, std::string,std::string> FyAuthorization::GetUpdateContent()
{
	auto upResult = m_FreeYun.CloudGetVersionInfo();

	if (std::get<0>(upResult) == false)
	{
		return { false,std::get<1>(upResult),""};
	}
	auto& updataVar = std::get<2>(upResult);
	std::string VarText = updataVar.at(xorstr_("describe"));
	return { true,VarText,updataVar.at(xorstr_("url")) };
}

std::tuple<bool, std::string> FyAuthorization::GetExpireTime(std::string username)
{
	auto Info = m_FreeYun.CloudQueryUserInfo(username);

	if (!std::get<0>(Info))
	{
		return { false,std::get<1>(Info) };
	}
	auto& dataVar = std::get<2>(Info);
	std::string VarText = dataVar.at(xorstr_("timeout"));
	return { true,VarText };
}

std::tuple<bool, std::string> FyAuthorization::RemoteVariable( std::string var)
{
	auto  pVar         = m_FreeYun.CloudGetTeletVar(m_username, var);
	//成功返回空字符串 否则为错误信息
	std::string Result = std::get<1>(pVar);
	BOOL bRet          = std::get<0>(pVar);

	if (bRet)
	{
		Result = std::get<2>(pVar).at(xorstr_("variable"));
	}
	return { bRet ,Result};
}

void FyAuthorization::SetUserName(std::string username)
{
	m_username = username;
}

FreeYun* FyAuthorization::GetFreeYun()
{
	return &m_FreeYun;
}

FyAuthorization* FyAuthorization::GetInstanc()
{
	return n_pInstance;
}
