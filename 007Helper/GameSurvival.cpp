#include "GameSurvival.h"
#include "Util.h"

CGameSurvival::CGameSurvival()
{
	m_Player = nullptr;

}

CGameSurvival* CGameSurvival::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new CGameSurvival();
	}
	return m_pInstance;
}

bool CGameSurvival::vInit()
{
	m_Player = GamePlayer::GetInstance();

	return true;
}

void CGameSurvival::vHUD_Redraw(float time, int intermission)
{
	if (!g_Vars.m_Survial.sk_cata_enable->value)
	{
		return;
	}

	// 子弹小于1 且在购买区
	if (g_Vars.m_Survial.sk_cata_autobullet->value	&& m_Player->GetWeaponInfo()->m_iClip < 1 && g_Vars.m_Survial.m_IsbuyZone)
	{
		Engine::g_Engine.pfnClientCmd(xorstr_("primammo"));
		Engine::g_Engine.pfnClientCmd(xorstr_("secammo"));
	}

	// 
	if (g_Vars.m_Survial.sk_cata_afk->value)
	{

		PTAG_VGUI_INPUT info   = g_pUtil->GetVguiMousetimer();
		double iput_time       = g_pUtil->GetVguiIpunttimer();
		(info->v1)             = iput_time;
		(info->keyboard_timer) = iput_time;
		(info->mouse_timer)    = iput_time;
	}


	CalcDrugCD();
}

void CGameSurvival::vHUD_Frame(double time)
{
	//灾变自动买防弹衣
	if (g_Vars.m_Survial.sk_cata_enable->value && g_Vars.m_Survial.sk_cata_autoarmor->value == 1 && g_Vars.m_Survial.m_IsbuyZone)
	{
		if (g_Vars.m_Survial.m_CurArmor < g_Vars.m_Survial.m_MaxArmor && g_Vars.m_Survial.m_CurMoney >= 1000)
		{
			Engine::g_Engine.pfnClientCmd(xorstr_("item_28"));
		}
	}
}

int CGameSurvival::vKeyBoardProc(int nCode, WPARAM keynum, LPARAM lParam)
{
	if (!g_Vars.m_Survial.sk_cata_enable->value)
	{
		return 1;
	}
	//防弹衣
	if (g_Vars.m_Survial.sk_cata_armor->value && keynum == g_Vars.m_Survial.sk_cata_armor->hotkey)
	{
		Engine::g_Engine.pfnClientCmd(xorstr_("item_28"));
	}
	else if (g_Vars.m_Survial.sk_cata_attack->value && keynum == g_Vars.m_Survial.sk_cata_attack->hotkey)
	{
		Engine::g_Engine.pfnClientCmd(xorstr_("dmgup"));
	}
	else if (g_Vars.m_Survial.sk_cata_hp->value && keynum == g_Vars.m_Survial.sk_cata_hp->hotkey)
	{
		Engine::g_Engine.pfnClientCmd(xorstr_("hup"));
	}
	else if (g_Vars.m_Survial.sk_cata_grenades->value && keynum == g_Vars.m_Survial.sk_cata_grenades->hotkey)
	{
		std::string Text = fmt::format("item_{}", g_Vars.m_Survial.sk_cata_grenades->flags);
		Engine::g_Engine.pfnClientCmd(Text.data());
	}
	else if (g_Vars.m_Survial.sk_cata_bullet->value && keynum == g_Vars.m_Survial.sk_cata_bullet->hotkey)
	{
		Engine::g_Engine.pfnClientCmd(xorstr_("primammo"));
	}
	else if (g_Vars.m_Survial.sk_cata_bullet2->value && keynum == g_Vars.m_Survial.sk_cata_bullet2->hotkey)
	{
		Engine::g_Engine.pfnClientCmd(xorstr_("secammo"));
	}

	return 1;
}

int CGameSurvival::vMPToCL(const char* pszName, int iSize, void* pbuf)
{
	auto lPackRead = std::make_shared<PackReader>((PBYTE)pbuf, iSize);

	int MsgType = lPackRead->ReadChar();

	if (MsgType == 27)
	{
		int result = lPackRead->ReadChar();

		switch (result)
		{
		case 1:
		{
			if (g_Vars.m_Survial.sk_cata_atck_up->value)
			{
				int		ItemTIndex = lPackRead->ReadShort();
				if (ItemTIndex == 0x64)
				{
					//攻击力100 
					lPackRead->ReadShort();
					lPackRead->ReadChar();
					//攻击倍数
					g_Vars.m_Survial.m_CurAtUpLv = lPackRead->ReadChar();
				}
			}

		}
		break;
		case 4:
		{
			//灾变信息获取
			int		ItemType = lPackRead->ReadChar();
			int		UseCd    = (int)lPackRead->ReadFloat() * 1000;
			switch (ItemType)
			{
			case 0:
				g_Vars.m_Survial.m_ZBS_HP30.ItemId     = ItemType;
				g_Vars.m_Survial.m_ZBS_HP30.UseCd      = UseCd;
				g_Vars.m_Survial.m_ZBS_HP30.TickCount  = GetTickCount();
				g_Vars.m_Survial.m_ZBS_HP30.Count      = lPackRead->ReadShort();
				break;
			case 1:
				g_Vars.m_Survial.m_ZBS_HP100.ItemId    = ItemType;
				g_Vars.m_Survial.m_ZBS_HP100.UseCd     = UseCd;
				g_Vars.m_Survial.m_ZBS_HP100.TickCount = GetTickCount();
				g_Vars.m_Survial.m_ZBS_HP100.Count     = lPackRead->ReadShort();
				break;
			}
		}
		break;
		}

	}
	return 1;
}

int CGameSurvival::vMoney(const char* pszName, int iSize, void* pbuf)
{
	auto lPackRead = std::make_shared<PackReader>((PBYTE)pbuf, iSize);

	g_Vars.m_Survial.m_CurMoney = lPackRead->ReadInt();

	// * 自动升级攻击力
	if (g_Vars.m_Survial.sk_cata_enable->value && g_Vars.m_Survial.sk_cata_atck_up->value)
	{
		if (g_Vars.m_Survial.m_CurMoney >= g_Vars.m_Survial.sk_cata_atck_up->flags && g_Vars.m_Survial.m_CurAtUpLv < 0x29)
		{
			Engine::g_Engine.pfnClientCmd(xorstr_("dmgup"));
		}
	}

	return 1;
}

int CGameSurvival::vBattery(const char* pszName, int iSize, void* pbuf)
{
	auto lPackRead = std::make_shared<PackReader>((PBYTE)pbuf, iSize);

	g_Vars.m_Survial.m_CurArmor = lPackRead->ReadInt();
	g_Vars.m_Survial.m_MaxArmor = lPackRead->ReadInt();

	return 1;
}

int CGameSurvival::vStatusIcon(const char* pszName, int iSize, void* pbuf)
{
	auto lPackRead   = std::make_shared<PackReader>((PBYTE)pbuf, iSize);
	int		Status   = lPackRead->ReadChar();
	char* SpriteName = lPackRead->ReadStr();

	if (SpriteName && lstrcmpA(SpriteName, xorstr_("buyzone")) == 0)
	{
		g_Vars.m_Survial.m_IsbuyZone = Status;
	}
	return 1;
}

int CGameSurvival::vHealth(const char* pszName, int iSize, void* pbuf)
{
	auto lPackRead = std::make_shared<PackReader>((PBYTE)pbuf, iSize);

	g_Vars.m_Survial.m_CurHealth = lPackRead->ReadInt();
	g_Vars.m_Survial.m_MaxHealth = lPackRead->ReadInt();

	if (g_Vars.m_Survial.sk_cata_enable->value)
	{
		// 当前HP 小于 最大的Hp
		if (g_Vars.m_Survial.m_CurHealth < g_Vars.m_Survial.m_MaxHealth)
		{

			if (m_Player->GetLocal()->Player.isAlive == false)
			{
				return 1;
			}

			// 当前HP占用总血量的百分比
			ULONG Percentage = ((float)g_Vars.m_Survial.m_CurHealth / (float)g_Vars.m_Survial.m_MaxHealth) * 100;

			if (g_Vars.m_Survial.sk_cata_drug_30->value && Percentage <= (ULONG)g_Vars.m_Survial.sk_cata_drug_30->flags)
			{
				if (g_Vars.m_Survial.m_ZBS_HP30.UseCd == 0 && g_Vars.m_Survial.m_ZBS_HP30.Count >= 1)
				{
					// 低于血量%d 使用了恢复30%药品还剩余%个
					Engine::g_Engine.pfnClientCmd(xorstr_("useitem \"zbs3\""));
				}
			}
			if (g_Vars.m_Survial.sk_cata_drug_100->value && Percentage <= (ULONG)g_Vars.m_Survial.sk_cata_drug_100->flags)
			{
				if (g_Vars.m_Survial.m_ZBS_HP100.UseCd == 0 && g_Vars.m_Survial.m_ZBS_HP100.Count >= 1)
				{
					Engine::g_Engine.pfnClientCmd(xorstr_("useitem \"zbs10\""));
				}
			}

		}
	}



	return 1;
}

int CGameSurvival::vInitHUD(const char* pszName, int iSize, void* pbuf)
{
	if (!g_Vars.m_Survial.sk_cata_enable->value)
		return 1;

	if (g_Vars.m_Survial.sk_cata_role->value)
	{
		auto Text = fmt::format(xorstr_("joinclass {}"), g_Vars.m_Survial.sk_cata_role->flags);
		Engine::g_Engine.pfnClientCmd(Text.data());
	}



	return 1;
}

void CGameSurvival::CalcDrugCD()
{
	if (g_Vars.m_Survial.m_ZBS_HP30.UseCd != 0)
	{
		if (g_Vars.m_Survial.m_ZBS_HP30.TickCount + g_Vars.m_Survial.m_ZBS_HP30.UseCd < GetTickCount())
		{
			g_Vars.m_Survial.m_ZBS_HP30.UseCd = 0;
		}
	}
	if (g_Vars.m_Survial.m_ZBS_HP100.UseCd != 0)
	{
		if (g_Vars.m_Survial.m_ZBS_HP100.TickCount + g_Vars.m_Survial.m_ZBS_HP100.UseCd < GetTickCount())
		{
			g_Vars.m_Survial.m_ZBS_HP100.UseCd = 0;
		}
	}
}
