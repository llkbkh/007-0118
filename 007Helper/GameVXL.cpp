#include "GameVXL.h"
#include "DispatchRecv.h"

CGameVXL::CGameVXL()
{
	m_pACEUtil = nullptr;
}

CGameVXL* CGameVXL::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new CGameVXL();
	}
	return m_pInstance;
}

int CGameVXL::vKeyBoardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (g_Vars.m_Vxl.sk_vxl_enable->value)
	{
		if (g_Vars.m_Vxl.sk_vxl_destroy->value && wParam == g_Vars.m_Vxl.sk_vxl_destroy->hotkey)
		{
			static bool Status = true;

			auto TextCommand = fmt::format(xorstr_("vxl_humandestroy {};vxl_zombidestroy {}"), (ULONG)Status, (ULONG)Status);

			Engine::g_Engine.pfnClientCmd(TextCommand.data());

			auto TextNotice = fmt::format(xorstr_("Map Destroy {}"), (ULONG)Status);

			CDispatchRecv::GetInstance()->UMsgNotice(TextNotice, UMsgType::SysNotice);

			Status ^= true;
		}
		else if (g_Vars.m_Vxl.sk_vxl_restart->value && wParam == g_Vars.m_Vxl.sk_vxl_restart->hotkey)
		{
			Engine::g_Engine.pfnClientCmd(xorstr_("vxl_restart"));
		}

	}

	return 0;
}

void CGameVXL::vHUD_Frame(double time)
{
	if (!g_Vars.m_Vxl.sk_vxl_enable->value)
	{
		return;
	}


	if (g_Vars.m_Vxl.sk_vxl_sky->value)
	{
		ULONG Index = m_pACEUtil->GetRandomNumber(0, 9);

		switch (Index)
		{
		case 0:
			Engine::g_Engine.pfnClientCmd(xorstr_("vxl_skyname de_2storm"));
			break;
		case 1:
			Engine::g_Engine.pfnClientCmd(xorstr_("vxl_skyname de_storm"));
			break;
		case 2:
			Engine::g_Engine.pfnClientCmd(xorstr_("vxl_skyname Redsky"));
			break;
		case 3:
			Engine::g_Engine.pfnClientCmd(xorstr_("vxl_skyname NTDM2_"));
			break;
		case 4:
			Engine::g_Engine.pfnClientCmd(xorstr_("vxl_skyname Moon"));
			break;
		case 5:
			Engine::g_Engine.pfnClientCmd(xorstr_("vxl_skyname hs_"));
			break;
		case 6:
			Engine::g_Engine.pfnClientCmd(xorstr_("vxl_skyname hs4_"));
			break;
		case 7:
			Engine::g_Engine.pfnClientCmd(xorstr_("vxl_skyname tomb"));
			break;
		case 9:
			Engine::g_Engine.pfnClientCmd(xorstr_("vxl_skyname BlueSky"));
			break;
		default:
			break;
		}

	}
	if (g_Vars.m_Vxl.sk_vxl_weather->value)
	{
		static int Index = 1;

		Engine::g_Engine.pfnClientCmd(fmt::format(xorstr_("vxl_weather {}"), Index + 1).data());

		Index ^= 1;

	}
	if (g_Vars.m_Vxl.sk_vxl_light->value)
	{
		static int Index = 1;

		Engine::g_Engine.pfnClientCmd(fmt::format(xorstr_("vxl_remove_dim_light {0:};vxl_daylight {0:}"), Index).data());

		Index ^= 1;

	}

}

bool CGameVXL::vInit()
{
	m_pACEUtil = std::make_shared<CACEUtil>();

	return true;
}
