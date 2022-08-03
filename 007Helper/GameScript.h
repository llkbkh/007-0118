#pragma once
#include "007Helper.h"
#include "GamePlayer.h"

class GameScript :public IGameClient, public IGameEngineMSG
{
public:
	GameScript();
	static GameScript* GetInstance();
	//´´½¨ÒÆ¶¯
	virtual void vCL_CreateMove(float frametime, usercmd_s* cmd, int active);
	virtual bool vInit();
	virtual void vHUD_Redraw(float time, int intermission);
	virtual int vMPToCL(const char* pszName, int iSize, void* pbuf);
	virtual int vInitHUD(const char* pszName, int iSize, void* pbuf);
	virtual int  vKeyBoardProc(int nCode, WPARAM wParam, LPARAM lParam);


private:
	void  ZombiAutoGankCT(struct usercmd_s* cmd);
	void  VectorAngles(const float* forward, float* angles);
	void  MakeAngle(bool addangle, float* angles, struct usercmd_s* cmd);

	void  PlayerOriginVisible();
	void  ZbsCounter(usercmd_s* cmd);

	float GetAngledeviation(int nIndex);
private:
	static inline GameScript*   m_pInstance = nullptr;
	GamePlayer*                 m_pGamePlayer;
	LocalStruct*                m_Local;
	std::shared_ptr<CACEMemory> m_Memory;

};

