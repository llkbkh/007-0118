#pragma once
#include "007Helper.h"

class CMpEntity
{
public:
	CMpEntity(int Index);

	// @初始化
	BOOL Init();

	// @锁血
	void Health(float fHealth = 32000.0f);

	/*
	*	@无敌
	*	@Param: [状态] 1 = 无敌 0 = 取消
	*/
	void God(int nStatus);

	// @设置最大移动速度
	void MaxSpeed(float fNewMaxspeed);

	// @武器无限子弹
	void WeaponAmmoMax();

	// @武器检查
	BOOL WeaponCheck(PTAG_PLAYER_WEAPON pWeapon);

	// @检查是否有效
	BOOL IsEnabled()
	{
		return m_pEdict && m_pVars;
	}
	// @检查是否是玩家
	BOOL IsPlayer();

	// @子弹加速
	void WeaponAmmoSpeed();

	// @反后坐力
	void WeaponRecoil();

	// @人物高跳
	void PlayerHitJump(int nMultiple = 4);

	// * 灵魂杀人
	void SoulToKill(BOOL bStaus);

	// @设置新的坐标
	void pfnSetOrigin(float* NewOrigin);

	// @快速复活
	void PlayerRpn(int Type);


private:
	edict_t*   m_pEdict;
	entvars_t* m_pVars;
	ULONG      m_Index;
};

