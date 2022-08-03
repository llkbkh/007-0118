#pragma once
#include "LCUICommon.h"

enum class NavigationPointId
{
	NAV_NONE,
	NAV_NOTICE,
	NAV_LOGIN,
	NAV_REG,
	NAV_PAY,
	NAV_SETTING,
	NAV_CORE,
	NAV_TOTAL_NUM
};
typedef struct NavigationPointRec_ {
	void(*init)(void);
	void(*free)(void);
} NavigationPointRec, * NavigationPoint;
struct  NaviInfo
{
	LCUI_Widget active;
	NavigationPoint point;
	NavigationPointRec points[(int)NavigationPointId::NAV_TOTAL_NUM];

	inline NaviInfo()
	{
		point = nullptr;
		active = nullptr;
	}
};

// 判断输入内容
enum class CheckInputType
{
	INPUT_USERNAME,
	INPUT_PASSWORD,
	INPUT_REG_USERNAME,
	INPUT_REG_PASSWORD,
	INPUT_REG_EMAIL,
	INPUT_CARDNUMBER,

};

// Http代理
struct Json_HttpProxy
{
	BOOL bEnable;
	std::string ip;
	std::string port;
	std::string user;
	std::string password;
};
// 翻译
struct Json_Translate
{
	std::string language;
	std::string url;
	
};

// *json文件存储的信息
struct JSON_FileInfo
{
	std::string GamePath;
	std::string Card;
	std::string User;
	std::string PASW;
	std::string SteamPath;
	ULONG GameServer;
	Json_HttpProxy Proxy;
	Json_Translate Translate;
	JSON_FileInfo()
	{
		GamePath = "";
		Card = "";
		User = "";
		PASW = "";
		SteamPath = "";
		GameServer = 0;
		Proxy.bEnable = FALSE;
	}
};

struct Register {
	wchar_t wTextUser[1024];
	wchar_t wTextPass[1024];
	wchar_t wTextEmail[1024];
	inline 
		Register()
	{
		RtlZeroMemory(this,sizeof(Register));
	}
};

struct UserPay {
	wchar_t wTextUser[1024];
	wchar_t wCardNumber[1024];
	inline
		UserPay()
	{
		RtlZeroMemory(this, sizeof(UserPay));
	}
};
struct UserLogin {
	wchar_t wUser[1024];
	wchar_t wPassWord[1024];
	inline
		UserLogin()
	{
		RtlZeroMemory(this, sizeof(UserLogin));
	}
};
//交互信息
struct MutualInfo
{
	union 
	{
		UserPay Pay;
		UserLogin Login;
	};
	inline MutualInfo()
	{
		RtlZeroMemory(this, sizeof(MutualInfo));
	}
};

enum class StartGameType
{
	Steam = -1,				//steam 方式启动
	Launcher = 0,				//游戏启动器启动
	CN_D1 = 5,				//游戏主程序启动
	CN_D2 = 6,
	CN_W1 = 15,
};
typedef struct TAG_LAUNCHER_STRING
{
	int		Size;
	char	Name[MAX_PATH];
	TAG_LAUNCHER_STRING(char* Value) : Size(0)
	{
		Size = wsprintfA(Name, "%s", Value);
	}
}*PTAG_LAUNCHER_STRING;

typedef struct TAG_LAUNCHER_PARAM
{
	TAG_LAUNCHER_STRING	Name;
	TAG_LAUNCHER_STRING	Value;
}*PTAG_LAUNCHER_PARAM;