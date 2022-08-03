#pragma once
//-------------------------------------------------------------------
// FreeYun 网络验证
//-------------------------------------------------------------------
#include <FreeYun.h>
#pragma comment(lib,"FreeYun.lib")
#include "Config.h"

/*
*	a - 主版本号			架构变动
*	b - 次版本号			增删功能
*	c - 阶段版本号			修改BUG
*/

#define UP_007	"2.2.2"
class FyAuthorization
{
public:
	FyAuthorization(void);
	~FyAuthorization(void);

	void Init();

	//* 获取网络公告
	std::tuple<bool, std::string> GetNotic();


	// @用户注册
	std::tuple<bool, std::string> Register(std::string username, std::string password, std::string email);

	// @用户充值
	std::tuple<bool, std::string> UserPay(std::string username, std::string CardNumber);

	// @用户登录
	std::tuple<bool, std::string> UserLogin(std::string username, std::string password);

	// @卡号登陆
	std::tuple<bool, std::string> CardLogin(std::string CardNumber);

	// @账号解绑
	std::tuple<bool, std::string> UserUnbind(std::string username, std::string password);

	// @效验远程变量
	std::tuple<bool, std::string> VerifyRemoteVariable(std::string username);

	// @检查版本更新
	std::tuple<bool, std::string> CheckVersion(std::string username, std::string &version);
	
	// @获取更新内容
	std::tuple<bool, std::string, std::string> GetUpdateContent();

	// @查询到期时间
	std::tuple<bool, std::string> GetExpireTime(std::string username);
	
	// @远程变量
	std::tuple<bool, std::string> RemoteVariable(std::string var);

	// @设置用户名
	void SetUserName(std::string username);

	
	FreeYun* GetFreeYun();

	static FyAuthorization* GetInstanc();
private:
	std::string m_MAC;
	FreeYun m_FreeYun;
	//初始化信息结果
	std::tuple<bool, std::string, nlohmann::json> m_InitResult;
	inline static FyAuthorization* n_pInstance = nullptr;
	std::string m_username;
};

