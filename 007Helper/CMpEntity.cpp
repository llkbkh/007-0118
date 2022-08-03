#include "CMpEntity.h"
#include "GameOffset.h"

CMpEntity::CMpEntity(int Index)
{
	m_Index = Index;
	Init();
}

BOOL CMpEntity::Init()
{
	m_pEdict = Engine::g_Engfuncs.pfnPEntityOfEntIndex(m_Index);
	m_pVars = Engine::g_Engfuncs.pfnGetVarsOfEnt(m_pEdict);

	return IsEnabled();
}

void CMpEntity::Health(float fHealth /*= 32000.0f*/)
{
	*(PFLOAT)(&m_pVars->health) = fHealth;
}

void CMpEntity::God(int nStatus)
{
	*(PDWORD)(&m_pVars->deadflag) = nStatus ? 0 : 64;
}

void CMpEntity::MaxSpeed(float fNewMaxspeed)
{
	Engine::g_Engfuncs.pfnSetClientMaxspeed(m_pEdict, fNewMaxspeed);
}

void CMpEntity::WeaponAmmoMax()
{
	if (!m_pEdict)
		return;
	//主武器
	PTAG_PLAYER_WEAPON pPlayerWeapon = (PTAG_PLAYER_WEAPON)(*(DWORD*)((DWORD)m_pEdict->pvPrivateData + g_Offset.GetGameMpAddresInfo()->dwMainWeaponOffSet));
	if (pPlayerWeapon)
	{
		WeaponCheck(pPlayerWeapon);

	}
	//副武器
	PTAG_PLAYER_WEAPON pPlayerSubWeapon = (PTAG_PLAYER_WEAPON)(*(DWORD*)((DWORD)m_pEdict->pvPrivateData + g_Offset.GetGameMpAddresInfo()->dwSubWeaponOffSet));
	if (pPlayerSubWeapon)
	{
		WeaponCheck(pPlayerSubWeapon);
	}
}
//无限子弹 武器检查
BOOL CMpEntity::WeaponCheck(PTAG_PLAYER_WEAPON pWeapon)
{
	BOOL bRet = TRUE;

	switch (pWeapon->InGameID)
	{
	case 327:	//spsg
		*(int*)(&pWeapon->iClip) = 2;
		break;
	case 373:	//永恒
		*(int*)(&pWeapon->iClip) = 7;
		break;
	case 431:	//幽影
	case 460:	//鼠神
		*(int*)(&pWeapon->iClip) = 300;
		*(int*)(&pWeapon->ClientWeaponState) = 300;
		break;

	default:
		*(int*)(&pWeapon->iClip) = 300;
		*(int*)(&pWeapon->iSpecialClip) = 100;
		*(int*)(&pWeapon->ClientWeaponState) = 100;
		break;
	}
	return bRet;
}

BOOL CMpEntity::IsPlayer()
{
	const char* ClassName = Engine::g_Engfuncs.pfnSzFromIndex(m_pVars->classname);
	return lstrcmpA(ClassName, xorstr_("player")) == 0;
}

void CMpEntity::WeaponAmmoSpeed()
{
	if (!m_pEdict)
		return;
	//主武器
	PTAG_PLAYER_WEAPON pPlayerWeapon = (PTAG_PLAYER_WEAPON)(*(DWORD*)((DWORD)m_pEdict->pvPrivateData + g_Offset.GetGameMpAddresInfo()->dwMainWeaponOffSet));
	if (pPlayerWeapon)
	{
		//左键加速
		*(float*)(&pPlayerWeapon->flNextPrimaryAttack) = 0.0f;
		//右键加速
		*(float*)(&pPlayerWeapon->flNextSecondaryAttack) = 0.0f;
	}
	//副武器
	PTAG_PLAYER_WEAPON pPlayerSubWeapon = (PTAG_PLAYER_WEAPON)(*(DWORD*)((DWORD)m_pEdict->pvPrivateData + g_Offset.GetGameMpAddresInfo()->dwSubWeaponOffSet));
	if (pPlayerSubWeapon)
	{
		// 深渊右键不能加速
		if (pPlayerSubWeapon->InGameID == 395)
		{
			*(float*)(&pPlayerSubWeapon->flNextPrimaryAttack) = 0.0f;
			return;
		}

		//左键加速
		*(float*)(&pPlayerSubWeapon->flNextPrimaryAttack) = 0.0f;
		//右键加速
		*(float*)(&pPlayerSubWeapon->flNextSecondaryAttack) = 0.0f;
	}
	//近身武器
	PTAG_PLAYER_WEAPON pPlayeknife = (PTAG_PLAYER_WEAPON)(*(DWORD*)((DWORD)m_pEdict->pvPrivateData + g_Offset.GetGameMpAddresInfo()->dwKnifeOffset));
	if (pPlayeknife)
	{
		*(float*)(&pPlayeknife->flNextPrimaryAttack) = 0.0f;
		*(float*)(&pPlayeknife->flNextSecondaryAttack) = 0.0f;
	}


}

void CMpEntity::WeaponRecoil()
{
	if (!m_pEdict)
		return;
	//主武器
	PTAG_PLAYER_WEAPON pPlayerWeapon = (PTAG_PLAYER_WEAPON)(*(DWORD*)((DWORD)m_pEdict->pvPrivateData + g_Offset.GetGameMpAddresInfo()->dwMainWeaponOffSet));
	if (pPlayerWeapon)
	{
		*(float*)(&pPlayerWeapon->fInRecoil) = 0.0f;
	}
	//副武器
	PTAG_PLAYER_WEAPON pPlayerSubWeapon = (PTAG_PLAYER_WEAPON)(*(DWORD*)((DWORD)m_pEdict->pvPrivateData + g_Offset.GetGameMpAddresInfo()->dwSubWeaponOffSet));
	if (pPlayerSubWeapon)
	{
		*(float*)(&pPlayerSubWeapon->fInRecoil) = 0.0f;
	}

	//人物视角调整
	*(PFLOAT)(&m_pVars->punchangle.x) = 0;
	*(PFLOAT)(&m_pVars->punchangle.y) = 0;
	*(PFLOAT)(&m_pVars->punchangle.z) = 0;
}

void CMpEntity::PlayerHitJump(int nMultiple)
{
	float fMultiple = 0.8;
	switch (nMultiple)
	{
	case 1:
		fMultiple = 0.7;
		break;
	case 2:
		fMultiple = 0.5;
		break;
	case 3:
		fMultiple = 0.3;
		break;
	case 4:
		fMultiple = 0.1;	//10倍
		break;
	}
	*(PFLOAT)(&m_pVars->gravity) = fMultiple;
}

void CMpEntity::SoulToKill(BOOL bStaus)
{
	*(PDWORD)(&m_pVars->iuser1) = bStaus ? 3 : 0;
}

void CMpEntity::pfnSetOrigin(float* NewOrigin)
{
	Engine::g_Engfuncs.pfnSetOrigin(m_pEdict, NewOrigin);
}

void CMpEntity::PlayerRpn(int Type)
{
	/*
	1:瞬移到指定地点 选择菜单 选择人物后复活
	2:瞬移到指定地点 等一会选择菜单 选择人物后复活
	3:瞬移到指定地点 选择菜单 选择人物后复活        默认选择这个
	4:忘了
	5:快速复活 不会切换阵营
	*/
	*(PDWORD)((DWORD)Engine::g_Engfuncs.pfnPvEntPrivateData(m_pEdict) + g_Offset.GetGameMpAddresInfo()->dwPlayerRpnOffset) = Type;
}

