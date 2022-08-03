#pragma once
#include "007Helper.h"
#include "GameDef.h"

class Util
{



public:
	Util();

	/*
	*	@获取道具名称(翻译后的)
	*	@Param:代码名
	*	@返回UTF8编码
	*/
	std::string GetCSOItemName(char* ItemCode);

	/*
	*	@获取道具名称
	*	@Param:道具代码
	*	@Param:道具代码前缀
	*/
	wchar_t* GetItemName(const char* pItemCode, const char* pItemname);


	/*
	*	@返回游戏服务器
	*	@返回<游戏服务器,字符串说明>
	*/
	std::tuple<GameServer, std::string> GetGameServer();

	/*
	*	@获取游戏大区
	*	@返回<大区索引,大区,字符串>
	*/
	std::tuple<ULONG, GameServerRegion, std::string > GetGameServerRegion();

	/*
	*	@获取游戏文件版本号
	*/
	GameVer GetGameDllVersion();

	GameVer GetVersion();


	std::shared_ptr<CACEHook> GetAceHook();
	std::shared_ptr<CACEMemory> GetAceMemory();
	std::shared_ptr<CACEUtil> GetAceTool();

	// @ 初始化游戏Item
	void InitGameItemInfo();

	// @ 初始化武器Item
	void InitinWeaponItem();

	// @ 判断当前武器ID 是否是手雷
	BOOL	IsGranade(int iWeaponID);
	// @ 获取武器项信息
	PTAG_GAME_ITEMS GetGameWeaponItemInfo(int iWeaponID);
	// @ 判断当前模式是否为生化模式
	bool GameModeIsZombi();
	std::map<int, PTAG_GAME_ITEMS>* GetItemWeaponInfo();

	// @ 根据索引返回玩家名称
	wchar_t* GetPlayerNameByIndex(int Index);

	// @ 游戏通知框
	void GameNoticeBox(std::string TextMsg);

	// @ 取游戏道具
	std::map<int, PGAME_ITEMMAP>* GetItemMap();

	// @ 通过武器ID 获取武器信息
	PGAME_ITEMMAP GetWeaponInfoByWeaponID(int iWeaponID);

	// @获取Vgui鼠标
	PTAG_VGUI_INPUT GetVguiMousetimer();
	// @Vgui输入时间
	double GetVguiIpunttimer();

	// @初始化游戏地图
	void InitGameMap();

	// @返回游戏地图信息
	std::map<int, PGAME_MAPINFO>* GetGameMap();

	// @初始化游戏模式
	void InitGameMode();

	// @返回游戏模式
	std::map<int, std::wstring>* GetGameMode();

private:
	// @查找游戏资源文件
	bool FindGameResourceFile(std::string FileName);

private:
	std::shared_ptr<CACEHook>        m_Hook;
	std::shared_ptr<CACEMemory>      m_Memory;
	std::shared_ptr<CACEUtil>        m_Tool;
	// InGameID,武器信息
	std::map<int, PTAG_GAME_ITEMS>   m_ItemWeaponInfo;
	//通过inGameId 找到武器索引 key= InGameId,Value = Index
	std::map<int, int>				 m_ItemWeaponToIndex;
	//游戏物品项(包含所有的)
	std::map<int, PGAME_ITEMMAP>     m_ItemMap;
	//游戏地图信息
	std::map<int, PGAME_MAPINFO>	 m_GameMap;
	//游戏模式
	std::map<int, std::wstring>		 m_GameMode;
	GameVer                          m_GameVer;
};

extern Util* g_pUtil;