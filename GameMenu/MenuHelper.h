#pragma once
#include "nlohmann/json.hpp"
#include "nlohmann/fifo_map.hpp"

typedef struct cvar_s
{
	char* name;
	char* string;
	int		flags;
	float	value;
	struct cvar_s* next;
} cvar_t;


template<class K, class V, class dummy_compare, class A>
using my_workaround_fifo_map = nlohmann::fifo_map<K, V, nlohmann::fifo_map_compare<K>, A>;
using my_json = nlohmann::basic_json<my_workaround_fifo_map>;

class MenuHelper
{
public:
	MenuHelper();

	//*加载菜单配置
	bool LoadMenuCFG(MENU_LANGUAGE Language);

	//获取菜单项
	CString GetMenuItemValue(std::string Item);

	CString GetMenuItemValueW(CString Item);

	void  GetMenuItemValueExW(WCHAR* buffer, WCHAR* Item);

	//设置窗口控件文本
	//BOOL SetDlgItemText(int nID);

	MENU_LANGUAGE GetLanguage();

	//*设置函数接口
	void SetfunctionIntetface(ULONG Address);

	cvar7_s* GetVarList();
	
	cvar7_s* GetVar(char*Name);

	void GameCommand(char* Text);

	void AlterTeam(BYTE Team);

	void SetGameNetWordStatus(BOOL Status);

	BOOL LoadCFG(ULONG Status);


	std::map<int, PGAME_ITEMMAP>* GetGameItem();

	std::map<int, PGAME_MAPINFO>* GetGameMap();

	std::map<int, std::wstring>* GetGameMode();

	// @加载Lua并且获取lua脚本信息
	std::vector <TAG_LUA_SCRIPT_TABLE>* GetLuaScriptInfo();

	// @卸载全部lua脚本
	BOOL unAllLoadLuaScript();


	// @加载指定Lua脚本
	BOOL LoadLuaScript(std::string FileName);

	// @卸载指定Lua脚本
	BOOL unLoadLuaScript(std::string FileName);


	void SetGameHwnd(HWND h);
	HWND GetGameHwnd();

	// @取游戏变量指针
	cvar_s* pfnGetCvarPointer(std::string name);

	void Mp_Command(std::string Text);

	void ShowMsg(char* Msg);
	void ShowMsgKey(char* Msg, CString Key);
	int  ShowMsgOK(char* Msg);

	// @ 错误信息
	int  ShowMsgEr(char* Msg,BOOL bForce = TRUE);

	// @获取配置路径
	std::string GetCFGPath();

	std::shared_ptr<CACEUtil> GetACEUtil();

private:
	std::shared_ptr<CACEHook>            m_Hook;
	std::string                          m_MenuCfgPath;
	my_json                              m_data;
	MENU_LANGUAGE			             m_Language;
	std::map<MENU_LANGUAGE, std::string> m_MenuName;
	std::shared_ptr<CACEUtil>			 m_ACEUtil;
	//接口回调
	std::function<void _cdecl (PVOID pBuf, ULONG Size)> ClientInterface;
	HWND                                 m_GameHwnd;
};

extern MenuHelper* g_pTool;