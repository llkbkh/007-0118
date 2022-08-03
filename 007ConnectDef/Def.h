#pragma once

enum CALLBACK_TYPE
{
	TYPE_MENUDLG_INIT      = 0,	// menu dlg init
	TYPE_VAR_INIT          = 1,	// var init
	TYPE_CLIENT_INTERFACE  = 2,	// client intetface
	TYPE_GAME_WINDOWS_HWND = 3,	// game windows hwnd

};


//[2/8/2022 007] 菜单语言 - 中文简体 - 中文繁体 - 英文
enum MENU_LANGUAGE
{
	MU_CN = 1,
	MU_TW = 2,
	MU_EN = 3,
};
enum FACE_CLIENT
{
	CLIENT_GET_VARLIST           = 0,		//get varlist
	MAP_GET_ITEM                 = 1,		//get gameItem
	COMMADN_FUN                  = 2,		//game command
	PACK_FUN_ALTER_TEAM          = 3,		//alter team
	LOAD_LUA_SCRIPT				 = 4,		//load lua file
	SET_NET_STATUS               = 5,		//set game network status 
	LOAD_CFG                     = 6,		
	GAME_CVAR                    = 7,		//get game cvar
	WEAPON_MP					 = 8,		//刷枪
};

typedef struct cvar7_s
{
	std::string name;
	std::string string;
	int		flags;
	float	value;
	DWORD	hotkey;
	struct cvar7_s* next;
} cvar7_s;

enum Game_ClassType
{
	ClassType_Unknown,
	ClassType_Equipment,			                    //装备
	ClassType_Class,				                    //角色
	ClassType_PeriodicItem,			                    //喷漆(周期性的)
	ClassType_Tattoo,				                    //纹身
	ClassType_zbRespawnEffect,		                    //生化复活效果
														//太多了 一般就用这两个
};
typedef struct
{
	char	RecourceName[MAX_PATH];				        //资源名
	char	ClassName[MAX_PATH];				        //类名
	ULONG	Category;							        //分类的ID
	ULONG	InGameID;							        //在游戏里的ID
	ULONG	SortingIndex;						        //排序ID
	Game_ClassType RecourceType;				        //资源类型
	WCHAR	ItemSrc[MAX_PATH];							//itemName
}GAME_ITEMMAP, * PGAME_ITEMMAP;


typedef struct LUA_SCRIPT_TABLE
{
	std::string FileName;			//文件名
	std::string Name;				//脚本名
	std::string Auther;	            //作者
	std::string UpdateTime;         //更新时间
	std::string Version;	        //版本号
	std::string Description;	    //使用说明
	std::string Method;				//方法
}TAG_LUA_SCRIPT_TABLE, * PTAG_LUA_SCRIPT_TABLE;

typedef struct
{
	int map_id;					                        //地图ID
	std::string map_name;			                    //地图名
	std::string map_code;			                    //地图代码
	std::wstring map_translation;                       //翻译的地图名
}GAME_MAPINFO, * PGAME_MAPINFO;
