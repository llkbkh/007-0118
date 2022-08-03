#include "CGameMp.h"
#include "GameOffset.h"
#include "Util.h"
#include "GamePlayer.h"

FARPROC(__stdcall* GiveFnptrsToDll)(enginefuncs_t* pengfuncsFromEngine, globalvars_t* pglobals) = nullptr;
FARPROC(_cdecl* GetEntityAPI)(DLL_FUNCTIONS* pFunctionTable, int interfaceVersion) = nullptr;


CGameMp::CGameMp()
{
	m_WriteByteCallRetAddress = 0;
	m_pMpEntity               = nullptr;
	m_pFunctionTable          = nullptr;

}

CGameMp* CGameMp::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new CGameMp();
	}

	return m_pInstance;
}

void CGameMp::vHUD_Frame(double time)
{
	if (!g_Vars.m_Mp.sk_mp_enable->value)
	{
		return;
	}

	if (g_Vars.m_Mp.sk_mp_gamemode->value)
	{
		//锁定房间数据
		*(PWORD)g_Offset.GetGameAddresInfo()->dwRoomMode = g_Vars.m_Mp.sk_mp_gamemode->flags;
	}
	if (g_Vars.m_Mp.sk_mp_gamemap->value)
	{
		//锁定房间数据
		*(PWORD)(g_Offset.GetGameAddresInfo()->dwRoomMode + 2) = g_Vars.m_Mp.sk_mp_gamemap->flags;
	}

	//auto Var = Engine::g_Engfuncs.pfnPEntityOfEntIndex(1);

	
	//ACEDebugLog("{}",__FUNCTION__, Var);


}

bool CGameMp::vInit()
{
	m_Hook           = g_pUtil->GetAceHook();
	m_Memory         = g_pUtil->GetAceMemory();


	static std::string TextCommand = xorstr_("mp_retroom");


	Engine::g_Engine.pfnAddCommand(TextCommand.data(), mp_retroom);


	return true;
}

void CGameMp::mp_retroom()
{
	*(PDWORD)(g_Offset.GetGameMpAddresInfo()->dwReleaseHold) = 0;
}

void CGameMp::vHUD_Redraw(float time, int intermission)
{
	if (!g_Vars.m_Mp.sk_mp_enable->value || m_pMpEntity == nullptr)
		return ;


	if (!m_pMpEntity->IsEnabled() || !m_pMpEntity->IsPlayer())
		return;
	//锁血
	if (g_Vars.m_Mp.sk_mp_health->value)
		m_pMpEntity->Health(32000.0f);
	//无敌
		m_pMpEntity->God(g_Vars.m_Mp.sk_mp_god->value);
	//飞奔
	if (g_Vars.m_Mp.sk_mp_speed->value)
		m_pMpEntity->MaxSpeed(390.0f);
	//无限弹药
	if (g_Vars.m_Mp.sk_mp_ammo_max->value)
		m_pMpEntity->WeaponAmmoMax();
	//弹药加速
	if (g_Vars.m_Mp.sk_mp_ammo_speed->value)
		m_pMpEntity->WeaponAmmoSpeed();
	//反后坐力
	if (g_Vars.m_Mp.sk_mp_weapon_recoil->value)
		m_pMpEntity->WeaponRecoil();
	//人物高跳
	if (g_Vars.m_Mp.sk_mp_highjump->value)
		m_pMpEntity->PlayerHitJump(2);


}

int CGameMp::vKeyBoardProc(int nCode, WPARAM keynum, LPARAM lParam)
{
	if (!g_Vars.m_Mp.sk_mp_enable->value || m_pMpEntity == nullptr)
		return 1;

	if (!m_pMpEntity->IsEnabled() || !m_pMpEntity->IsPlayer())
		return 1 ;

	if (g_Vars.m_Mp.sk_mp_soul->value && keynum == g_Vars.m_Mp.sk_mp_soul->hotkey)
	{
		static bool bSoulWalls = false;
		m_pMpEntity->SoulToKill(bSoulWalls^= 1);
	}
	else if (g_Vars.m_Mp.sk_mp_sop->value && keynum == g_Vars.m_Mp.sk_mp_sop->hotkey)
	{
		for (size_t i = 2; i <= Engine::g_Engine.GetMaxClients(); i++)
		{
			std::shared_ptr<CMpEntity>  pMpEntity = std::make_shared<CMpEntity>(i);

			if (!pMpEntity->IsEnabled() || !pMpEntity->IsPlayer())
				continue;

			pMpEntity->pfnSetOrigin(GamePlayer::GetInstance()->GetLocal()->Entity->origin + vec3_t(100,00,0));
		}

	}
	else if (g_Vars.m_Mp.sk_mp_rpn->value && keynum == g_Vars.m_Mp.sk_mp_rpn->hotkey)
	{
		m_pMpEntity->PlayerRpn(3);
	}


	return 1;
}

int CGameMp::vInitHUD(const char* pszName, int iSize, void* pbuf)
{
	if (!g_Vars.m_Mp.sk_mp_enable->value)
		return 1;

	if (g_Vars.m_Mp.sk_mp_mode->value)
	{
		//超级房间
		*(PDWORD)(g_Offset.GetGameMpAddresInfo()->dwReleaseHold) = 1;
	}
	
	


	//初始化房主信息
	if (m_pMpEntity == nullptr)
	{
		m_pMpEntity = std::make_shared<CMpEntity>(1);
	}
	else
	{
		m_pMpEntity->Init();
	}

	return 1;
}



void __stdcall CGameMp::pfnWriteByte(PTAG_PEB_STACK Stack)
{
	ULONG* Array = (ULONG*)Stack->ESP;

	if (g_Vars.m_Mp.sk_mp_enable->value && g_Vars.m_Mp.sk_mp_GainMut->value && Array[0] == CGameMp::GetInstance()->m_WriteByteCallRetAddress)
	{
		ULONG PlayerAddres = (ULONG)Stack->EBX;
		*(PWORD)(PlayerAddres + 0x20) = 70;

		//static ULONG points       = 0;

		//ULONG Curpoints = points++;

		//if (Curpoints > 52)
		//{
		//	Curpoints = points = 0;
		//}
		//Stack->EAX = (VOID*)Curpoints;
	}
LAB_exit:
	Engine::g_Engfuncs.pfnWriteByte(Array[1]);
}

void __declspec(naked)  Naked_WriteByte()
{
	__asm
	{
		pushad
		push esp
		call CGameMp::pfnWriteByte
		popad
		ret
	}
}

int __stdcall  CGameMp::GiveFnptrsToDll_0(enginefuncs_t* pengfuncsFromEngine, globalvars_t* pglobals)
{
	if (!Engine::g_pEngfuncs)
	{
		ACEDebugFileLog("[{}] PengfuncsFromEngine=0x{:X} pglobals=0x{:X}", __FUNCTION__, (DWORD)pengfuncsFromEngine, (DWORD)pglobals);

		Engine::g_pEngfuncs = pengfuncsFromEngine;

		RtlCopyMemory(&Engine::g_Engfuncs, Engine::g_pEngfuncs, sizeof(enginefuncs_t));

		(DWORD&)pengfuncsFromEngine->pfnWriteByte = (DWORD)Naked_WriteByte;

		ULONG CallAddress = 0;
		ULONG Offset      = 0;
		while (g_pUtil->GetAceMemory()->GetScanCodeAddress(xorstr_("6A 01 FF 15 ?? ?? ?? ?? 8b 45 f8 0F B6 C0"), CallAddress, g_Offset.GetServerModuleInfo()->hModule, Offset))
		{
			Offset++;
			CallAddress += g_pUtil->GetAceHook()->GetDisasmLenght(CallAddress, 2);
			CGameMp::GetInstance()->m_WriteByteCallRetAddress = CallAddress;
			break;

			/*
			* 
			*	下断找这个 0019E09C   2118A320   "ZBZGainMut"

				push 0x1
				call dword ptr ds:[<&void __cdecl Naked_WriteByte(void)>]
				mov eax,dword ptr ss:[ebp-0x8]
				movzx eax,al
			*/

		}

	}

	return (int)GiveFnptrsToDll(pengfuncsFromEngine, pglobals);
}

FARPROC WINAPI CGameMp::extGetProcAddress(HMODULE hModule, LPCSTR lpProcName)
{

	if (lstrcmpiA(lpProcName, xorstr_("GiveFnptrsToDll")) == 0)
	{
		// 获取 enginefuncs_t的地址
		/*
		https://tieba.baidu.com/p/5272108911?red_tag=2935570143

		或第一个函数的字符串 PF_precache_model_I: NULL pointer

		特征码 End of List  name kill status 往下找

		*/
		(FARPROC&)GiveFnptrsToDll = ::GetProcAddress(hModule, lpProcName);

	

		return (FARPROC)GiveFnptrsToDll_0;
	}
	else if (lstrcmpiA(lpProcName, xorstr_("GetEntityAPI")) == 0)
	{
		(FARPROC&)GetEntityAPI = ::GetProcAddress(hModule, lpProcName);
		return (FARPROC)extGetEntityAPI;

	}
	return ::GetProcAddress(hModule, lpProcName);
}
FARPROC _cdecl CGameMp::extGetEntityAPI(DLL_FUNCTIONS* pFunctionTable, int interfaceVersion)
{
	if (pFunctionTable && interfaceVersion == INTERFACE_VERSION)
	{
		ACEDebugFileLog("[{}] pFunctionTable=0x{:X} ", __FUNCTION__, (DWORD)pFunctionTable);

		CGameMp::GetInstance()->m_pFunctionTable = pFunctionTable;
	}
	return GetEntityAPI(pFunctionTable, interfaceVersion);
}

