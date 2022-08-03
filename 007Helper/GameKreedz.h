#pragma once
#include "007Helper.h"
#include "GamePlayer.h"

class GameKreedz :public IGameClient,public IGameEngineMSG
{
public:
	GameKreedz();
	static GameKreedz* GetInstance();

	virtual bool vInit();
	virtual void vCL_CreateMove(float frametime, usercmd_s* cmd, int active);
	virtual void vHUD_Redraw(float time, int intermission);



private:
	void Bhop(float frametime, struct usercmd_s* cmd);
	void AdjustSpeed(double x);
	void Gstrafe(struct usercmd_s* cmd);

	bool CheckKzKeyStatus(usercmd_s* cmd);
	void RemoveShiftKey();
	void RemoveAltKey();
	void StrafeHack(float frametime, struct usercmd_s* cmd);
	void StrafeHelper(float frametime, struct usercmd_s* cmd);

	void ShowLongJump(usercmd_s* cmd);
	int JumpBug(float frametime, usercmd_s* cmd);
	void SpinHack(usercmd_s* cmd);
	void FastRun(float frametime, struct usercmd_s* cmd);
	void DrawShowKeys();
	void DrawKreedzInfo();
	void KzStrafe(usercmd_s* cmd);
private:
	static inline GameKreedz* m_Instance = nullptr;
	GamePlayer*	              m_pGamePlayer;
	DWORD                     m_dwSpeedptr;
	LocalStruct*              m_Local;
	bool                      m_bJumped ;
	bool                      m_balance_bh;
	float                     m_InterpFps;
	float                     m_FrameCnt;
	float                     m_FpsCnt;
	float                     m_PreStrafe;
	float                     m_JumpOff;
	bool Forward, Backward;
	bool MoveLeft, MoveRight;
	bool Duck, Jump;
};

