#pragma once
#include "007Helper.h"
#include <memory>

enum CALLBACK_TYPE
{
	TYPE_MENUDLG_INIT      = 0,	// menu dlg init
	TYPE_VAR_INIT          = 1,	// var init
	TYPE_CLIENT_INTERFACE  = 2,	// client intetface
	TYPE_GAME_WINDOWS_HWND = 3,	// game windows hwnd
	TYPE_MENUDLG_DRAW	   = 4,		// draw

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
	CLIENT_GET_VARLIST  = 0,		//get varlist
	MAP_GET_ITEM	    = 1,		//get gameItem
	COMMADN_FUN		    = 2,		//game command
	PACK_FUN_ALTER_TEAM = 3,		//alter team
	LOAD_LUA_SCRIPT     = 4,		//load lua file
	SET_NET_STATUS      = 5,		//set game network status 
	LOAD_CFG            = 6,		//load cfg
	GAME_CVAR			= 7,		//get game cvar
	WEAPON_MP           = 8,		//刷枪

};


class ExternalMenu : public IGameClient
{
public:
	ExternalMenu();
	static ExternalMenu* GetInstance();
	virtual int vInitialize(cl_enginefunc_t* pEnginefuncs, int iVersion);
	virtual bool vInit();
	virtual int  vKeyBoardProc(int nCode, WPARAM wParam, LPARAM lParam);
	static  void Interface(PVOID pBuf,ULONG Size);

	static void GameCommand(char* Text);
	static void CommandByGiveWeapon(std::string Text);
	
private:
	void LoadExtMenu();
	void LoadArgs();
	void SendAsyncCallMsg(CALLBACK_TYPE Type);
private:
	static inline ExternalMenu* m_pInstance = nullptr;
	std::shared_ptr<CACEHook>   m_Hook;
	//路径
	std::string                 m_Path;
	//模块句柄
	HMODULE                     m_Lib;
	//键盘钩子回调
	typedef  std::function<void WINAPI(int nCode, int keyNum)> KeyBoardProcCallback_t;
	KeyBoardProcCallback_t m_MenuKeyBoardProc;
	//菜单回调
	std::function<void WINAPI(PVOID pBuf, ULONG Size)> m_MenuAsyncCallBack;

};

