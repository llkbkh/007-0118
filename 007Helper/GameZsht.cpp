#include "GameZsht.h"

CGameZsht* CGameZsht::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new CGameZsht();
	}
	return m_pInstance;
}

int CGameZsht::vKeyBoardProc(int nCode, WPARAM keynum, LPARAM lParam)
{
	if (!g_Vars.m_Zsht.sk_zsht_enable->value)
		return 1;

	if (g_Vars.m_Zsht.sk_zsht_transfer->value && keynum == g_Vars.m_Zsht.sk_zsht_transfer->hotkey)			// 材料转移到仓库
	{
		auto Text = fmt::format(xorstr_("zsht_warehouse 0 15 0;zsht_warehouse 1 15 0"));
		Engine::g_Engine.pfnClientCmd(Text.data());
	}
	else if (g_Vars.m_Zsht.sk_zsht_contribute->value && keynum == g_Vars.m_Zsht.sk_zsht_contribute->hotkey)			// 刷贡献
	{
		Engine::g_Engine.pfnClientCmd(xorstr_("zsht_playergetresource 0;zsht_playergetresource 1"));
	}
	else if (g_Vars.m_Zsht.sk_zsht_resource->value && keynum == g_Vars.m_Zsht.sk_zsht_resource->hotkey)			// get 资源与金钱
	{
		Engine::g_Engine.pfnClientCmd(xorstr_("zsht_picklegacy 32000 15 15"));
	}
	else if (g_Vars.m_Zsht.sk_zsht_loopbuild->value && keynum == g_Vars.m_Zsht.sk_zsht_loopbuild->hotkey)			
	{

		for (int i = 0; i < g_Vars.m_Zsht.sk_zsht_loopbuild->value; i++)
		{
			vec3_t origin;
			origin.x = m_Player->GetLocal()->Entity->origin.x + g_Vars.m_Zsht.sk_zsht_loopbuild_x->value * i - g_Vars.m_Zsht.sk_zsht_loopbuild_x->value;
			origin.y = m_Player->GetLocal()->Entity->origin.y + g_Vars.m_Zsht.sk_zsht_loopbuild_y->value * i - g_Vars.m_Zsht.sk_zsht_loopbuild_y->value;
			origin.z = m_Player->GetLocal()->Entity->origin.z + g_Vars.m_Zsht.sk_zsht_loopbuild_z->value * i - 30;

			ZshtBuild(g_Vars.m_Zsht.sk_zsht_loopbuild->flags, origin);
			Sleep(20);
		}
	}

	return 1;
}

bool CGameZsht::vInit()
{
	m_Player = GamePlayer::GetInstance();

	return true;
}

void CGameZsht::ZshtBuild(ULONG ulIndex, vec3_t origin)
{
	static LONG Build_Id[] = { 18, 19,20, 21, 33,12,13, 14, 28, 31, 32 };

	if (ulIndex > (sizeof(Build_Id) / sizeof(Build_Id[0])))
	{
		return;
	}

	auto CommandText = fmt::format(xorstr_("zsht_picklegacy 32000 15 15;zsht_BuildConfirm {} {} {} {} 0"), Build_Id[ulIndex], origin.x, origin.y, origin.z);

	Engine::g_Engine.pfnClientCmd(CommandText.data());
}
