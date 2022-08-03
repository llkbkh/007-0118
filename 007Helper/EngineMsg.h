#pragma once
#include "007Helper.h"
#include "GameBasics.h"
#include "GameEsp.h"
#include "GameAimBot.h"
#include "GameScript.h"
#include "GameSurvival.h"
#include "CGameMp.h"

class EngineGameMsg
{
private:
	pfnUserMsgHook HookUserMsg(char *szMsgName, pfnUserMsgHook pfn);
	PUserMsg UserMsgByName(char* szMsgName);
public:
	pfnUserMsgHook pInitHUD;
	pfnUserMsgHook pSetFOV;
	pfnUserMsgHook pResetHUD;
	pfnUserMsgHook pCurWeapon;
	pfnUserMsgHook pBombDrop;
	pfnUserMsgHook pRoundTime;
	pfnUserMsgHook pSayText;
	pfnUserMsgHook pWeaponList;
	pfnUserMsgHook pHealth;
	pfnUserMsgHook pStatusValue;
	pfnUserMsgHook pVGUIMenu;
	pfnUserMsgHook pTextMsg;
	pfnUserMsgHook pPatrolDrone;
	pfnUserMsgHook pDeathMsg;
	pfnUserMsgHook pScoreInfo;
	pfnUserMsgHook pMPToCL;
	pfnUserMsgHook pMPToCL2;

	pfnUserMsgHook pHudTextArgs;
	pfnUserMsgHook pDamage;
	pfnUserMsgHook pPainTracer;
	pfnUserMsgHook pStatusIcon;
	pfnUserMsgHook pBattery;
	pfnUserMsgHook pMoney;
	pfnUserMsgHook pScreenShake;
	pfnUserMsgHook pBuyEndTime;
	pfnUserMsgHook pMyInfo;
	
	pfnUserMsgHook pSpectato;
	pfnUserMsgHook pAllowSpec;
	
	pfnUserMsgHook pTeamInfo;
	pfnUserMsgHook pHudText;
	pfnUserMsgHook pTempEmit;
	pfnUserMsgHook pPlayerInfo;
	pfnUserMsgHook pKillInfo;

	
private:
	static int InitHUD(const char *pszName, int iSize, void *pbuf);
	static int SetFOV(const char *pszName, int iSize, void *pbuf);
	static int ResetHUD(const char *pszName, int iSize, void *pbuf);
	static int CurWeapon(const char *pszName, int iSize, void *pbuf);
	static int BombDrop(const char *pszName, int iSize, void *pbuf);
	static int RoundTime(const char *pszName, int iSize, void *pbuf);
	static int SayText(const char* pszName, int iSize, void* pbuf);
	static int WeaponList(const char* pszName, int iSize, void* pbuf); 
	static int Health(const char* pszName, int iSize, void* pbuf);
	static int StatusValue(const char *pszName, int iSize, void *pbuf);
	static int VGUIMenu(const char* pszName, int iSize, void* pbuf);
	static int TextMsg(const char* pszName, int iSize, void* pbuf);
	static int DeathMsg(const char* pszName, int iSize, void* pbuf);
	static int ScoreInfo(const char* pszName, int iSize, void* pbuf);
	static int MPToCL(const char* pszName, int iSize, void* pbuf);
	static int MPToCL2(const char* pszName, int iSize, void* pbuf);
	static int Damage(const char* pszName, int iSize, void* pbuf);
	static int PainTracer(const char* pszName, int iSize, void* pbuf);
	static int StatusIcon(const char* pszName, int iSize, void* pbuf);
	static int Battery(const char* pszName, int iSize, void* pbuf);
	static int Money(const char* pszName, int iSize, void* pbuf);
	static int KillInfo(const char* pszName, int iSize, void* pbuf);
	

	static int DrawHud(const char* pszName, int iSize, void* pbuf);
	static int BuyEndTime(const char* pszName, int iSize, void* pbuf);
	static int MyInfo(const char* pszName, int iSize, void* pbuf);
	static int Spectato(const char* pszName, int iSize, void* pbuf);
	static int AllowSpec(const char* pszName, int iSize, void* pbuf);
	static int TeamInfo(const char* pszName, int iSize, void* pbuf);
	static int HudText(const char* pszName, int iSize, void* pbuf);
	static int TempEmit(const char* pszName, int iSize, void* pbuf);
	static int PlayerInfo(const char* pszName, int iSize, void* pbuf);
	static int PfnHookUserMsg(char* szMsgName, pfnUserMsgHook pfn);
public:
	static int PatrolDrone(const char* pszName, int iSize, void* pbuf);
	static int HudTextArgs(const char* pszName, int iSize, void* pbuf);
	static int ScreenShake(const char* pszName, int iSize, void* pbuf);
	void HookEngineMsg();
	EngineGameMsg();
private:
	pfnUserMsgHook	             m_MsgFunction;
	size_t			             m_MsgDataSize;
	char*			             m_MsgName;
	std::vector<IGameEngineMSG*> m_nVecMsg;
	GameBasics*		             m_pGameBasics;
	GameEsp*                     m_pGameEsp;
	GameAimBot*                  m_pGameAimBot;
	GameScript*                  m_pGameScript;
	CGameSurvival*               m_pSurvaval;
	CGameMp*                     m_pGameMp;

};

extern EngineGameMsg g_UserMsg;