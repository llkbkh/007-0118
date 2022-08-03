/***
*
*	Copyright (c) 1999, 2000, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/
#if !defined ( WEAPONINFOH )
#define WEAPONINFOH
#ifdef _WIN32
#pragma once
#endif

// Info about weapons player might have in his/her possession
typedef struct weapon_data_s
{
	int			m_iId;
	int			m_iClip;				//子弹数量

	float		m_flNextPrimaryAttack;
	float		m_flNextSecondaryAttack;
	float		m_flTimeWeaponIdle;

	int			m_fInReload;
	int			m_fInSpecialReload;	//换子弹
	float		m_flNextReload;
	float		m_flPumpTime;
	float		m_fReloadTime;

	float		m_fAimedDamage;
	float		m_fNextAimBonus;
	float		m_fun1;		//? 2022-01-19 17:52:45

	int			m_fInZoom;			//开火时间
	int			m_iWeaponState;		//消音器

	int			iuser1;
	int			iuser2;
	int			iuser3;
	int			iuser4;
	float		fuser1;
	float		fuser2;
	float		fuser3;
	float		fuser4;
} weapon_data_t;

#endif