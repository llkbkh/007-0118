#pragma once
#include "007Helper.h"
class GameVar
{
public:
	GameVar();
	// @注册变量
	bool RegisterVariable();
	// @默认变量
	bool DefaultVariable();
	// @取初始化链表
	cvar7_s* GetInitCvar();
private:
	void RegBasics();
	void RegMenu();
	void RegEsp();
	void RegAimBot();
	void RegWallHack();
	void RegKz();
	void RegScript();
	void RegMFCMenu();
	void RegMpMenu();
	void RegVxl();
	void RegSpeed();
	void RegSurvival();
	void RegZsht();
public:
	BasicsStruct        m_Basi;
	MenuStruct		    m_Menu;
	ESPStruct           m_Esp;
	AimbotStruct	    m_Aimbot;
	WallHackStruct      m_WallHack;
	KreedzStruct	    m_Kz;
	ScriptStruct	    m_Script;
	ExternalMenuStruct  m_MFCMenu;
	MPStruct		    m_Mp;
	VXLStruct			m_Vxl;
	SpeedStruct			m_Speed;
	SurvivalStruct		m_Survial;
	ZshtStruct			m_Zsht;

	//*窗口句柄
	HWND	            m_GameHwnd;
	GAME_USER_INFO      m_UserInfo;

	bool TestValue;

private:
	void SetVarsValue(cvar7_s** Vars, char* name, float Value, DWORD dwHotKey = 0,ULONG Flag = 0);
	
};

extern GameVar g_Vars;