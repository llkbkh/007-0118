#pragma once
#include "007Helper.h"
#include "GamePlayer.h"
#include "IGameEngineMSG.h"

class GameBasics :public IGameClient, public IGameEngineMSG
{
public:
	GameBasics();
	static GameBasics* GetInstance();

	virtual bool vInit();

	virtual int  vKeyBoardProc(int nCode, WPARAM wParam, LPARAM lParam);
	virtual void vCL_CreateMove(float frametime, usercmd_s* cmd, int active);
	virtual void vV_CalcRefdef(ref_params_s* pparams);
	virtual void vHUD_Redraw(float time, int intermission) ;

	//------------MSG---------------
	virtual	int vStatusValue(const char* pszName, int iSize, void* pbuf);
	virtual int vInitHUD(const char* pszName, int iSize, void* pbuf) ;
	virtual int vVGUIMenu(const char* pszName, int iSize, void* pbuf);
	virtual int vMPToCL(const char* pszName, int iSize, void* pbuf);
	virtual int vMPToCL2(const char* pszName, int iSize, void* pbuf);
	virtual int vDeathMsg(const char* pszName, int iSize, void* pbuf);
	virtual int vKillInfo(const char* pszName, int iSize, void* pbuf);
	virtual int vPlayerInfo(const char* pszName, int iSize, void* pbuf);
private:
	void FreeLookFunction(float frametime, usercmd_s* cmd);
	//人物卡空
	void PlayerCard();
	// 夜视
	void NightVision();
	//地图高亮
	void MapLight();
	//方框显血
	void BoxShowHp();
	//左上角显示玩家健康信息
	void DrawHealth(INT x                , INT y, INT width, INT height);
	//显示全图玩家健康信息                         ,并没有什么用
	void AllDrawHealth();
	//显示自绘的显血.目前不启用这个
	void DrawHealth();
	//显示血条
	void DrawHPGaugeInfo();
	//返回MSG函数地址
	pfnUserMsgHook GetpfnUserMsg(const char* Msg);
private:
	std::shared_ptr<CACEMemory>     m_Memory;
	bool                            m_bFreeLook;
	GamePlayer*                     m_Player;
	BOOL                            m_NightVision;
	static inline GameBasics*       m_pInstance = nullptr;
	pfnUserMsgHook                  m_HPGaugeInfo;
	PackWriter						m_HPGaugeInfoPack;
	int	                            m_DeathMsg_iKillCount;
	cvar_s*                         m_pCl_killeffect;
};



