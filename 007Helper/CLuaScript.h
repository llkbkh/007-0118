#pragma once
#include "007Helper.h"
#include "ThreadPool.h"
extern "C"
{
	#include "lua\lua.h"
	#include "lua\lualib.h"
	#include "lua\lauxlib.h"
}
#pragma comment(lib,"lua.lib")


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

class CLuaScript 
{
public:
	CLuaScript();
	virtual ~CLuaScript();
	// @装载Lua文件
	BOOL LoadLuaFile(char* FileName);

	// @调用lua函数 键盘回调
	int Lua_Function_KeyBoard(int KeyCode);


	// @获取脚本信息
	LUA_SCRIPT_TABLE* GetScritpInfo();

	std::string GetErrorMsg();

	// @设置状态
	BOOL SetStatus(BOOL Status);
private:
	// @注册函数
	void RegisterFuns();

	// @游戏指令
	static int l_GameCommand(lua_State* L);

	// @游戏控制台输出
	static int l_DPrintf(lua_State* L);

	// @暂停
	static int l_Sleep(lua_State* L);

	// @取当前武器索引
	static int l_GetKnifeWeaponIndex(lua_State* L);

	// @调用lua函数 获取脚本信息
	LUA_SCRIPT_TABLE* Lua_Function_ScritpInfo();

	// @获取luaTable Value
	std::string Lua_GetTableString(char* Key);

	

private:
	lua_State*          m_l;
	std::string         m_Error;
	std::string         m_LuaFileName;
	LUA_SCRIPT_TABLE	m_ScriptInfo;
	BOOL                m_Status;
};



class CLuaScriptManage : public IGameClient
{
public:
	CLuaScriptManage();
	static CLuaScriptManage* GetInstance();

	virtual bool vInit();
	virtual int  vKeyBoardProc(int nCode, WPARAM wParam, LPARAM lParam);
	virtual int vInitialize(cl_enginefunc_t* pEnginefuncs, int iVersion);
	virtual void vHUD_Redraw(float time, int intermission);
	/*
	*	@创建Lua
	*	@Param:脚本路径
	*/
	BOOL CreateLua(std::string FileName);

	// @返回错误码
	std::string GetError();


	// @本地加载lua脚本
	BOOL LocalLoadLuaScript();

	// @取所有lua脚本信息
	std::vector<TAG_LUA_SCRIPT_TABLE>* GetAllLuaScriptInfo();

	// @卸载所有脚本
	BOOL UnLoadAllLuaScript();

	// @卸载脚本
	BOOL UnLoadLuaScript(std::string FileName);

private:
	static inline CLuaScriptManage*              m_pInstance = nullptr;
	std::map<ULONG, CLuaScript*>                 m_MapLua;
	ULONG                                        m_ulCount;
	std::string                                  m_Error;
	// * 是否游戏控制台输出日志
	BOOL	                                     m_GameConsoleOut;
	ThreadPool*		                             m_pThreadPool;
	std::vector <TAG_LUA_SCRIPT_TABLE>	         m_VecLuaScript;
};