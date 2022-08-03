#pragma once
#include "007Helper.h"
#include "GamePlayer.h"

class GameAimBot:public IGameClient,public IGameEngineMSG
{
public:

	static GameAimBot* GetInstance();
	virtual bool vInit();
	virtual void vV_CalcRefdef(ref_params_s* pparams);
	virtual void vCL_CreateMove(float frametime, usercmd_s* cmd, int active);
	virtual void vHUD_Redraw(float time, int intermission);
	virtual int  vKeyBoardProc(int nCode, WPARAM wParam, LPARAM lParam);


	virtual int vInitHUD(const char* pszName, int iSize, void* pbuf);
	virtual int vDeathMsg(const char* pszName, int iSize, void* pbuf);


	// * 获取当前武器反冲补偿
	Vector* GetCurWeaponRecoilCompensation();
private:
	// * 武器是否可以攻击
	bool CanAttack();
	// * 画出后坐力
	void DrawRecoil();
	// * 合理的自瞄
	bool LegitAimbot(usercmd_s* cmd);
	// * 取视角
	float GetAngledeviation(int nIndex);
	// * 是否在视角里
	bool IsInFOV(float* fScreen, float fFov);
	//
	void  MakeAngle(bool addangle, float* angles, struct usercmd_s* cmd);
	void  VectorAngles(const float* forward, float* angles);
	void  SmoothAimAngles(QAngle MyViewAngles, QAngle AimAngles, QAngle& OutAngles, float Smoothing, bool bSpiral, float SpiralX, float SpiralY);
	// * 显示玩家信息
	void  DrawPlayer(int Index);
	// * 命中体积可见检查
	void HitBoxVisible(int index);
	// * 骨骼是否可见检查
	void BonesVisible(int index);
	//
	float Distance(Vector* vInput);
private:
	static inline GameAimBot* m_Instance = nullptr;
	LocalStruct*              m_Local;
	GamePlayer*               m_GamePlayer;

	weapon_data_t*            m_WeaponInfo;
	std::map<int,Vector>	  m_RecoilCompensationMap;
	HITBOX_TYPE				  m_BackupHitBoxType;
};

