#pragma once
#include "007Helper.h"
#include "IGameClient.h"
#include "GameBasics.h"
#include "GamePlayer.h"
#include "GameMenu.h"
#include "GameEsp.h"
#include "GameAimBot.h"
#include "GameWallHack.h"
#include "GameSpeed.h"
#include "GameKreedz.h"
#include "GameScript.h"
#include "ExternalMenu.h"
#include "DispatchRecv.h"
#include "GameVXL.h"
#include "GamePack.h"
#include "CLuaScript.h"
#include "CSagaClient.h"
#include "GameSurvival.h"
#include "GameZsht.h"
#include "CGameMp.h"

class GameClient:public IGameClient
{
public:
	GameClient();
	static GameClient* GetInstance();
	void  HookClientFunction();
	bool  HookStudioFunction();
	void  Hud_Console007Info();
	GLdouble		 m_modelview[16]; GLdouble m_projview[16]; GLint m_viewport[4];
private:
	static void HUD_PostRunCmd(struct local_state_s* from, struct local_state_s* to, struct usercmd_s* cmd, int runfuncs, double time, unsigned int random_seed);
	static void HUD_Redraw(float time, int intermission);
	static void HUD_Frame_Init(double time);
	static void HUD_Frame(double time);
	static void IN_MouseEvent(int mstate);
	static int  HUD_Key_Event(int down, int keynum, const char* pszCurrentBinding);
	static int  HUD_AddEntity(int type, cl_entity_s* ent, const char* modelname);
	static void HUD_PlayerMove(struct playermove_s* ppmove, int server);
	static int  CL_IsThirdPerson();
	static void CL_CreateMove(float frametime, usercmd_s* cmd, int active);
	static void V_CalcRefdef(ref_params_s* pparams);
	static void RestoreRenderer();
	static int  EnginetStudioCheckBBox();
	static void StudioDrawPoints();
	static void	StudioDynamicLight(struct cl_entity_s* ent, struct alight_s* plight);
	static int  HUD_GetStudioModelInterface(int version, struct r_studio_interface_s** ppinterface, struct engine_studio_api_s* pstudio);
	static int  Initialize(cl_enginefunc_t* pEnginefuncs, int iVersion);
public:
	virtual bool vInit();
	virtual void vHUD_Frame(double time);
	virtual int  vGameUI_WndProc(HWND hwnd, int nCode, WPARAM wParam, LPARAM lParam);
	virtual int  vKeyBoardProc(int nCode, WPARAM wParam, LPARAM lParam);
	virtual void vCL_CreateMove(float frametime, usercmd_s* cmd, int active);
	virtual void vV_CalcRefdef(ref_params_s* pparams);
	virtual void vHUD_AddEntity(int type, cl_entity_s* ent, const char* modelname) ;
	virtual void vHUD_Redraw(float time, int intermission);
	virtual void vHUD_PlayerMove(struct playermove_s* ppmove, int server);
	virtual int  vHUD_Key_Event(int down, int keynum, const char* pszCurrentBinding);
	virtual void vHUD_PostRunCmd(struct local_state_s* from, struct local_state_s* to, struct usercmd_s* cmd, int runfuncs, double time, unsigned int random_seed);
	virtual int vHUD_GetStudioModelInterface(int version, struct r_studio_interface_s** ppinterface, struct engine_studio_api_s* pstudio);
	virtual int  vInitialize(cl_enginefunc_t* pEnginefuncs, int iVersion);



private:
	static inline GameClient*        m_pInstance = nullptr;
	std::vector<IGameClient*>        m_nVecClientFun;
	GameBasics*                      m_pGameBasics;
	GamePlayer*                      m_pGamePlayer;
	GameMenu*                        m_pGameMenu;
	GameEsp*                         m_pGameEsp;
	GameAimBot*                      m_pGameAimbot;
	std::unique_ptr<CACEHook>        m_Hook;
	GameWallHack*                    m_pGameWall;
	GameSpeed*                       m_pGameSpeed;
	GameKreedz*				         m_pGameKreedz;
	GameScript*                      m_pGameScript;
	ExternalMenu*                    m_pExtMenu;
	std::unique_ptr<CACEProcess>     m_Process;
	CDispatchRecv*                   m_pDispatchRecv;
	CGameVXL*		                 m_pGameVXL;
	CGamePack*					     m_pGamePack;
	CLuaScriptManage*                m_pLuaScriptManage;
	CSagaClient*					 m_pSagaClient;
	CGameSurvival*                   m_pGameSurvival;
	CGameZsht*						 m_pGameZsht;
	CGameMp*                         m_pGameMp;
};

