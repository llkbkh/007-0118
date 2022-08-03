#pragma once
#include "007Helper.h"

/*
*	1 - 正常执行游戏回调
*	2 - 正常执行游戏回调,最后不执行游戏回调
*	3 - 跳过之后的虚函数,最后执行游戏回调
*	4 - 跳过之后的虚函数,最后不执行游戏回调
*	
*/


class IGameEngineMSG
{

public:
	//@初始化
	virtual int vInitHUD(const char* pszName, int iSize, void* pbuf);
	//@状态值-当鼠标移动到人物时显示 或状态发生改变时
	virtual	int vStatusValue(const char* pszName, int iSize, void* pbuf);
	//@无人机
	virtual int vPatrolDrone(const char* pszName, int iSize, void* pbuf);
	//@Gui菜单
	virtual int vVGUIMenu(const char* pszName, int iSize, void* pbuf);
	//@本地转客户端消息
	virtual int vMPToCL(const char* pszName, int iSize, void* pbuf);
	//@本地转客户端消息
	virtual int vMPToCL2(const char* pszName, int iSize, void* pbuf);
	//@炸弹掉落信息
	virtual int vBombDrop(const char* pszName, int iSize, void* pbuf);
	//@重置HUD
	virtual int vResetHUD(const char* pszName, int iSize, void* pbuf);
	//@击杀消息
	virtual int vDeathMsg(const char* pszName, int iSize, void* pbuf);
	//@击杀信息
	virtual int vKillInfo(const char* pszName, int iSize, void* pbuf);
	//@金钱
	virtual int vMoney(const char* pszName, int iSize, void* pbuf);
	//@护甲
	virtual int vBattery(const char* pszName, int iSize, void* pbuf);
	//@状态图标
	virtual int vStatusIcon(const char* pszName, int iSize, void* pbuf);
	//@健康
	virtual int vHealth(const char* pszName, int iSize, void* pbuf);
	//@玩家信息
	virtual int vPlayerInfo(const char* pszName, int iSize, void* pbuf);

	
	
};

