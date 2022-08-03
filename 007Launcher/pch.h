// pch.h: 这是预编译标头文件。
// 下方列出的文件仅编译一次，提高了将来生成的生成性能。
// 这还将影响 IntelliSense 性能，包括代码完成和许多代码浏览功能。
// 但是，如果此处列出的文件中的任何一个在生成之间有更新，它们全部都将被重新编译。
// 请勿在此处添加要频繁更新的文件，这将使得性能优势无效。

#ifndef PCH_H
#define PCH_H

// 添加要在此处预编译的标头
#include "framework.h"
#include <ACEConnect.h>

#include <FreeYun.h>
#pragma comment(lib,"FreeYun.lib")

extern 	FreeYun Verify;


struct JSON_HttpProxy
{
	BOOL bEnable;
	std::string ip;
	std::string port;
	std::string user;
	std::string password;
};
// *json文件存储的信息
struct JSON_FileInfo
{
	std::string GamePath;
	std::string Card;
	std::string User;
	std::string PASW;
	ULONG SAVA;
	std::string SteamPath;
	ULONG GameServer;
	JSON_HttpProxy Proxy;
};

// * 保存json信息
void SavaJsonInfo();
void from_json(const nlohmann::json& j, JSON_FileInfo& p);
void to_json(nlohmann::json& j, const JSON_FileInfo& p);

/*
*	a - 主版本号			架构变动
*	b - 次版本号			增删功能
*	c - 阶段版本号			修改BUG
*	d - 日期				修改日期
*	e - 软件版本阶段说明	Alpha Beta	RC	(R)
*/

#define UP_007	"1.0.1.220526"


#endif //PCH_H
