#include "EngineMsg.h"
#include<PackReader.h>
#include "Drawing.h"
#include "GameOffset.h"
#include "Util.h"

std::map<std::string, ULONG> VecBlacklistMap ;

ODG_EXPORT void _stdcall extDispatchUserMsg(PTAG_PEB_STACK Stack)
{
	ULONG* Array = (ULONG*)Stack->ESP;

	char* name = (char*)Array[0];

	
	if (VecBlacklistMap.count(name) <= 0)
	{
		ACEWarningFileLog("[{}] {}",__FUNCTION__, name);
	}

}

void __declspec(naked)  Naked_DispatchUserMsg()
{
	__asm
	{
		pushad
		push esp
		call extDispatchUserMsg
		popad
	}
}


void EngineGameMsg::HookEngineMsg()
{
	VMProtectBegin(__FUNCTION__);

	pPatrolDrone = nullptr;


	g_Offset.FindUserMsgBase();

#if  0

	DWORD  DispatchUserMsg = 0;

	auto pHook   = g_pUtil->GetAceHook();
	auto pMemory = g_pUtil->GetAceMemory();

	if (pMemory->GetScanCode(xorstr_("FF D0 8B 5D FC"), DispatchUserMsg, g_Offset.GetEngineModuleInfo()->hModule))
	{

		VecBlacklistMap.insert(std::map<std::string,ULONG>::value_type( "BotVoice",0));
		VecBlacklistMap.insert(std::map<std::string, ULONG>::value_type("Radar", 0));
		VecBlacklistMap.insert(std::map<std::string, ULONG>::value_type("SendAudio", 0));
		//VecBlacklistMap.insert(std::map<std::string, ULONG>::value_type("WeaponList", 0));
		//VecBlacklistMap.insert(std::map<std::string, ULONG>::value_type("ScoreInfo", 0));
		//VecBlacklistMap.insert(std::map<std::string, ULONG>::value_type("CurWeapon", 0));
		VecBlacklistMap.insert(std::map<std::string, ULONG>::value_type("BotVoice", 0));
		//VecBlacklistMap.insert(std::map<std::string, ULONG>::value_type("MPToCL", 0));
		pHook->HookWithNaked(DispatchUserMsg, 5, Naked_DispatchUserMsg);
	}
#endif // ANTI_DEBUG
	pInitHUD		    =   HookUserMsg(xorstr_("InitHUD"),		g_UserMsg.InitHUD);
	pSetFOV			    =   HookUserMsg(xorstr_("SetFOV"),		g_UserMsg.SetFOV);
	pResetHUD		    =	HookUserMsg(xorstr_("ResetHUD"),		g_UserMsg.ResetHUD);
	pCurWeapon		    =	HookUserMsg(xorstr_("CurWeapon"),	g_UserMsg.CurWeapon);
	pBombDrop		    =	HookUserMsg(xorstr_("BombDrop"),		g_UserMsg.BombDrop);
	//pSayText		    =   HookUserMsg("SayText",		g_UserMsg.SayText);
	pStatusValue	    =   HookUserMsg(xorstr_("StatusValue"),	g_UserMsg.StatusValue);
	pVGUIMenu		    =	HookUserMsg(xorstr_("VGUIMenu"),	g_UserMsg.VGUIMenu);
	pTextMsg		    =	HookUserMsg(xorstr_("TextMsg"),		g_UserMsg.TextMsg);
	pDeathMsg		    =	HookUserMsg(xorstr_("DeathMsg"),		g_UserMsg.DeathMsg);
	//pScoreInfo		=	HookUserMsg("ScoreInfo",	g_UserMsg.ScoreInfo);
	pMPToCL			    =	HookUserMsg(xorstr_("MPToCL"),		g_UserMsg.MPToCL);
	//pMPToCL2             =	HookUserMsg("MPToCL2",		g_UserMsg.MPToCL2);

	pHealth			    =	HookUserMsg(xorstr_("Health"),		g_UserMsg.Health);
	pPainTracer		    =	HookUserMsg(xorstr_("PainTracer"),   g_UserMsg.PainTracer);
	pStatusIcon		    =	HookUserMsg(xorstr_("StatusIcon"),	g_UserMsg.StatusIcon);
	pBattery		    =	HookUserMsg(xorstr_("Battery"),		g_UserMsg.Battery);
	pMoney			    =	HookUserMsg(xorstr_("Money"),		g_UserMsg.Money);
	pBuyEndTime		    =	HookUserMsg(xorstr_("BuyEndTime"),	g_UserMsg.BuyEndTime);
	//pSpectato		    =	HookUserMsg("Spectator",	g_UserMsg.Spectato);
	//pAllowSpec		=	HookUserMsg("PlayerInfo",	g_UserMsg.AllowSpec);
	pKillInfo			=	HookUserMsg(xorstr_("KillInfo"),		g_UserMsg.KillInfo);
	pPlayerInfo         =	HookUserMsg(xorstr_("PlayerInfo"), g_UserMsg.PlayerInfo);

	//pMyInfo			=	HookUserMsg("MyInfo",			g_UserMsg.MyInfo);
	//pTeamInfo		=	HookUserMsg("ItemStatus",		g_UserMsg.TeamInfo);

	//pTempEmit		= HookUserMsg("TempEmit",			g_UserMsg.TempEmit);
	//pPlayerInfo		= HookUserMsg("PlayerInfo",			g_UserMsg.PlayerInfo);

	//Hook ÎÞÈË»ú  pfnHookUserMsg  ScoreInfo ShowMenu
	Engine::g_pEngine->pfnHookUserMsg = PfnHookUserMsg;

	VMProtectEnd();
}

EngineGameMsg::EngineGameMsg()
{
	m_pGameBasics = GameBasics::GetInstance();
	m_pGameEsp    = GameEsp::GetInstance();
	m_pGameAimBot = GameAimBot::GetInstance();
	m_pGameScript = GameScript::GetInstance();
	m_pSurvaval   = CGameSurvival::GetInstance();
	m_pGameMp     = CGameMp::GetInstance();
	m_nVecMsg.push_back(m_pGameBasics);
	m_nVecMsg.push_back(m_pGameEsp);
	m_nVecMsg.push_back(m_pGameAimBot);
	m_nVecMsg.push_back(m_pGameScript);
	m_nVecMsg.push_back(m_pSurvaval);
	m_nVecMsg.push_back(m_pGameMp);
}

int EngineGameMsg::TeamInfo(const char* pszName, int iSize, void* pbuf)
{

	return  g_UserMsg.pTeamInfo(pszName, iSize, pbuf);
}
int EngineGameMsg::DrawHud(const char* pszName, int iSize, void* pbuf)
{
	

	return 1;
}
int EngineGameMsg::TempEmit(const char* pszName, int iSize, void* pbuf)
{


	return  g_UserMsg.pTempEmit(pszName, iSize, pbuf);
}
int EngineGameMsg::PlayerInfo(const char* pszName, int iSize, void* pbuf)
{
	std::vector<IGameEngineMSG*>::iterator itr = g_UserMsg.m_nVecMsg.begin();

	bool v1 = false;

	for (; itr != g_UserMsg.m_nVecMsg.end(); ++itr)
	{
		int result = (*itr)->vPlayerInfo(pszName, iSize, pbuf);

		if (result == 2)
		{
			v1 = true;
		}

	}
	if (v1)
	{
		return 1;
	}

	return  g_UserMsg.pPlayerInfo(pszName, iSize, pbuf);
}

int EngineGameMsg::PfnHookUserMsg(char* szMsgName, pfnUserMsgHook pfn)
{
	if (lstrcmpA(szMsgName, xorstr_("PatrolDrone")) == 0)
	{
		g_UserMsg.pPatrolDrone = pfn;
		pfn = g_UserMsg.PatrolDrone;
	}

	return Engine::g_Engine.pfnHookUserMsg(szMsgName, pfn);
}

int EngineGameMsg::BuyEndTime(const char* pszName, int iSize, void* pbuf)
{
	

	return  g_UserMsg.pBuyEndTime(pszName, iSize, pbuf);
}
int EngineGameMsg::MyInfo(const char* pszName, int iSize, void* pbuf)
{


	return  g_UserMsg.pMyInfo(pszName, iSize, pbuf);
}
int EngineGameMsg::Spectato(const char* pszName, int iSize, void* pbuf)
{

	return  g_UserMsg.pSpectato(pszName, iSize, pbuf);
}
int EngineGameMsg::AllowSpec(const char* pszName, int iSize, void* pbuf)
{

	return  /*g_UserMsg.pAllowSpec(pszName, iSize, pbuf)*/1;
}

int EngineGameMsg::MPToCL(const char* pszName, int iSize, void* pbuf)
{

	std::vector<IGameEngineMSG*>::iterator itr = g_UserMsg.m_nVecMsg.begin();

	bool v1 = false;

	for (; itr != g_UserMsg.m_nVecMsg.end(); ++itr)
	{
		int result = (*itr)->vMPToCL(pszName, iSize, pbuf);

		if (result == 2)
		{
			v1 = true;
		}

	}
	if (v1)
	{
		return 1;
	}
	return g_UserMsg.pMPToCL(pszName, iSize, pbuf);
}





int EngineGameMsg::MPToCL2(const char* pszName, int iSize, void* pbuf)
{
	std::vector<IGameEngineMSG*>::iterator itr = g_UserMsg.m_nVecMsg.begin();

	bool v1 = false;

	for (; itr != g_UserMsg.m_nVecMsg.end(); ++itr)
	{
		int result = (*itr)->vMPToCL2(pszName, iSize, pbuf);

		if (result == 2)
		{
			v1 = true;
		}

	}
	if (v1)
	{
		return 1;
	}
	return g_UserMsg.pMPToCL2(pszName, iSize, pbuf);
}

int EngineGameMsg::HudText(const char* pszName, int iSize, void* pbuf)
{

	return 1;
}
int EngineGameMsg::HudTextArgs(const char* pszName, int iSize, void* pbuf)
{


	return  g_UserMsg.pHudTextArgs(pszName, iSize, pbuf);
}
int EngineGameMsg::ScoreInfo(const char* pszName, int iSize, void* pbuf)
{
	
	

	return g_UserMsg.pScoreInfo(pszName, iSize, pbuf);
}
int EngineGameMsg::Damage(const char* pszName, int iSize, void* pbuf)
{
	return g_UserMsg.pDamage(pszName, iSize, pbuf);
}
int EngineGameMsg::ScreenShake(const char* pszName, int iSize, void* pbuf)
{
	return g_UserMsg.pScreenShake(pszName, iSize, pbuf);
}


int EngineGameMsg::Money(const char* pszName, int iSize, void* pbuf)
{
	std::vector<IGameEngineMSG*>::iterator itr = g_UserMsg.m_nVecMsg.begin();

	bool v1 = false;

	for (; itr != g_UserMsg.m_nVecMsg.end(); ++itr)
	{
		int result = (*itr)->vMoney(pszName, iSize, pbuf);

		if (result == 2)
		{
			v1 = true;
		}
	}
	if (v1)
	{
		return 1;
	}

	return g_UserMsg.pMoney(pszName, iSize, pbuf);
}

int EngineGameMsg::KillInfo(const char* pszName, int iSize, void* pbuf)
{
	std::vector<IGameEngineMSG*>::iterator itr = g_UserMsg.m_nVecMsg.begin();

	bool v1 = false;

	for (; itr != g_UserMsg.m_nVecMsg.end(); ++itr)
	{
		int result = (*itr)->vKillInfo(pszName, iSize, pbuf);

		if (result == 2)
		{
			v1 = true;
		}

	}
	if (v1)
	{
		return 1;
	}
	return g_UserMsg.pKillInfo(pszName, iSize, pbuf);
}

int EngineGameMsg::Battery(const char* pszName, int iSize, void* pbuf)
{
	std::vector<IGameEngineMSG*>::iterator itr = g_UserMsg.m_nVecMsg.begin();

	bool v1 = false;

	for (; itr != g_UserMsg.m_nVecMsg.end(); ++itr)
	{
		int result = (*itr)->vBattery(pszName, iSize, pbuf);

		if (result == 2)
		{
			v1 = true;
		}

	}
	if (v1)
	{
		return 1;
	}


	return g_UserMsg.pBattery(pszName, iSize, pbuf);
}
int EngineGameMsg::StatusIcon(const char* pszName, int iSize, void* pbuf)
{
	std::vector<IGameEngineMSG*>::iterator itr = g_UserMsg.m_nVecMsg.begin();

	bool v1 = false;

	for (; itr != g_UserMsg.m_nVecMsg.end(); ++itr)
	{
		int result = (*itr)->vStatusIcon(pszName, iSize, pbuf);

		if (result == 2)
		{
			v1 = true;
		}

	}
	if (v1)
	{
		return 1;
	}

	return g_UserMsg.pStatusIcon(pszName, iSize, pbuf);
}
int EngineGameMsg::PainTracer(const char* pszName, int iSize, void* pbuf)
{

	int Result = g_UserMsg.pPainTracer(pszName, iSize, pbuf);

	return Result;
}


int EngineGameMsg::DeathMsg(const char* pszName, int iSize, void* pbuf)
{

	std::vector<IGameEngineMSG*>::iterator itr = g_UserMsg.m_nVecMsg.begin();

	for (; itr != g_UserMsg.m_nVecMsg.end(); ++itr)
	{
		(*itr)->vDeathMsg(pszName, iSize, pbuf);
	}

	return g_UserMsg.pDeathMsg(pszName, iSize, pbuf);
}

int EngineGameMsg::PatrolDrone(const char* pszName, int iSize, void* pbuf)
{
	std::vector<IGameEngineMSG*>::iterator itr = g_UserMsg.m_nVecMsg.begin();

	for (; itr != g_UserMsg.m_nVecMsg.end(); ++itr)
	{
		(*itr)->vPatrolDrone(pszName, iSize, pbuf);
	}
	return g_UserMsg.pPatrolDrone(pszName, iSize, pbuf);
}
int EngineGameMsg:: TextMsg(const char* pszName, int iSize, void* pbuf)
{
	

	return g_UserMsg.pTextMsg(pszName, iSize, pbuf);
}


int EngineGameMsg::VGUIMenu(const char* pszName, int iSize, void* pbuf)
{
	
	std::vector<IGameEngineMSG*>::iterator itr = g_UserMsg.m_nVecMsg.begin();

	bool v1 = false;

	for (; itr != g_UserMsg.m_nVecMsg.end(); ++itr)
	{
		int result =  (*itr)->vVGUIMenu(pszName, iSize, pbuf);

		if (result == 2)
		{
			v1 = true;
		}

	}

	if (v1)
	{
		return 1;
	}

	return g_UserMsg.pVGUIMenu(pszName, iSize, pbuf);
}
int EngineGameMsg::Health(const char* pszName, int iSize, void* pbuf)
{
	std::vector<IGameEngineMSG*>::iterator itr = g_UserMsg.m_nVecMsg.begin();

	bool v1 = false;

	for (; itr != g_UserMsg.m_nVecMsg.end(); ++itr)
	{
		(*itr)->vHealth(pszName, iSize, pbuf);
	}
	return g_UserMsg.pHealth(pszName, iSize, pbuf);
}
int EngineGameMsg::StatusValue(const char *pszName, int iSize, void *pbuf)
{
	std::vector<IGameEngineMSG*>::iterator itr = g_UserMsg.m_nVecMsg.begin();

	for (; itr != g_UserMsg.m_nVecMsg.end(); ++itr)
	{
		(*itr)->vStatusValue(pszName, iSize, pbuf);
	}
	return g_UserMsg.pStatusValue(pszName, iSize, pbuf);
}
int EngineGameMsg::WeaponList(const char* pszName, int iSize, void* pbuf)
{
	

	return g_UserMsg.pWeaponList(pszName, iSize, pbuf);
}
int EngineGameMsg::SayText(const char* pszName, int iSize, void* pbuf)
{
	

	return g_UserMsg.pSayText(pszName, iSize, pbuf);
}
int EngineGameMsg::RoundTime(const char *pszName, int iSize, void *pbuf)
{

	return g_UserMsg.pRoundTime(pszName, iSize, pbuf);
}



int EngineGameMsg::BombDrop(const char *pszName, int iSize, void *pbuf)
{
	std::vector<IGameEngineMSG*>::iterator itr = g_UserMsg.m_nVecMsg.begin();

	for (; itr != g_UserMsg.m_nVecMsg.end(); ++itr)
	{
		(*itr)->vBombDrop(pszName, iSize, pbuf);
	}
	return g_UserMsg.pBombDrop(pszName, iSize, pbuf);
}



int EngineGameMsg::InitHUD(const char *pszName, int iSize, void *pbuf)
{
	std::vector<IGameEngineMSG*>::iterator itr = g_UserMsg.m_nVecMsg.begin();

	for (; itr != g_UserMsg.m_nVecMsg.end(); ++itr)
	{
		(*itr)->vInitHUD(pszName, iSize, pbuf);
	}

	if (!g_UserMsg.pPatrolDrone)
	{
		g_UserMsg.pPatrolDrone = g_UserMsg.HookUserMsg(xorstr_("PatrolDrone"), g_UserMsg.PatrolDrone);
	}

	


	return g_UserMsg.pInitHUD(pszName, iSize, pbuf);
}
int EngineGameMsg::SetFOV(const char *pszName, int iSize, void *pbuf)
{
	

	return g_UserMsg.pSetFOV(pszName, iSize, pbuf);
}
int EngineGameMsg::ResetHUD(const char *pszName, int iSize, void *pbuf)
{
	std::vector<IGameEngineMSG*>::iterator itr = g_UserMsg.m_nVecMsg.begin();

	for (; itr != g_UserMsg.m_nVecMsg.end(); ++itr)
	{
		(*itr)->vResetHUD(pszName, iSize, pbuf);
	}
	return g_UserMsg.pResetHUD(pszName, iSize, pbuf);
}
int  EngineGameMsg::CurWeapon(const char *pszName, int iSize, void *pbuf)
{


	return g_UserMsg.pCurWeapon(pszName, iSize, pbuf);
}

pfnUserMsgHook EngineGameMsg::HookUserMsg(char *szMsgName, pfnUserMsgHook pfn)
{
	pfnUserMsgHook Original = NULL;
	PUserMsg Ptr = UserMsgByName(szMsgName);

	if (Ptr->pfn)
	{
		Original = Ptr->pfn;
		Ptr->pfn = pfn;
	}

	return Original;
}
PUserMsg EngineGameMsg::UserMsgByName(char* szMsgName)
{
	PUserMsg Ptr = (PUserMsg) * (PDWORD)(Engine::g_pUserMsgBase);

	while (Ptr->next)
	{
		if (!lstrcmp(Ptr->name, szMsgName))
		{
			return Ptr;
		}

		Ptr = Ptr->next;
	}

	Ptr->pfn = 0;

	return Ptr;
}


EngineGameMsg g_UserMsg;