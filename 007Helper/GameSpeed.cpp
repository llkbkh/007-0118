#include "GameSpeed.h"
#include "GameOffset.h"
#include "Util.h"

GameSpeed::GameSpeed()
{

}

GameSpeed* GameSpeed::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new GameSpeed();
	}
	return m_pInstance;
}

bool GameSpeed::vInit()
{
	HMODULE hModule = g_Offset.GetEngineModuleInfo()->hModule;

	//初始化加速地址
	__try
	{
		m_pACEMemory->GetScanCode(xorstr_("A0 ?? ?? ?? ?? 32 05 ?? ?? ?? ?? A2 ?? ?? ?? ?? A0 ?? ?? ?? ?? 32 05 ?? ?? ?? ??"), m_dwSpeed[0], hModule);
		m_dwSpeed[0] = *(PDWORD)(m_dwSpeed[0] + 1);

		m_pACEMemory->GetScanCode(xorstr_("C3 0F 57 C0 F2 0F 11 05 ?? ?? ?? ?? F2 0F 11 05 ?? ?? ?? ??"), m_dwSpeed[1], hModule);
		m_dwSpeed[1] = *(PDWORD)(m_dwSpeed[1] + 8);


		m_pACEMemory->GetScanCode(xorstr_("FF FF FF FF C7 05 ?? ?? ?? ?? ff ff ff ff f3 0f 11 05 ?? ?? ?? ??"), m_dwSpeed[2], hModule);
		m_dwSpeed[2] = *(PDWORD)(m_dwSpeed[2] + 6);

		if (m_pACEMemory->GetScanCode(xorstr_("F2 0F 10 05 ?? ?? ?? ?? 66 0F 2F C1 72 07 C6 81 ?? ?? ?? ?? 01 C3 CC"), m_dwSpeed[3], hModule))
		{
			//m_dwSpeed[3] = *(PDWORD)(m_dwSpeed[3] + 4);
		}
		for (int i = 0; i < 5; i++)
		{
			g_Offset.GetGameAddresInfo()->dwSpeed[i] = m_dwSpeed[i];

			ACEDebugLog("[{}]->dwSpeedAddres {}--{:X}", __FUNCTION__, i, m_dwSpeed[i]);
		}
	}
	__except (1)
	{

		return false;
	}

	return true;
}

int GameSpeed::vInitialize(cl_enginefunc_t* pEnginefuncs, int iVersion)
{
	m_pACEMemory = g_pUtil->GetAceMemory();


	return 1;
}

void GameSpeed::vHUD_Redraw(float time, int intermission)
{

	if (!g_Vars.m_Speed.sk_speed_enable->value)
		return;
	// mode  1 人物微加速  2 子弹加速 3 超级加速

	int nSpeedMode = g_Vars.m_Speed.sk_speed_mode->value;


	switch (nSpeedMode)
	{
	case 1:
		PlayerSpeed();
		break;
	case 2:
		TimeSpeed();
		break;
	case 3:
		GameMemorySpeed();
		break;
	default:
		break;
	}

}

void GameSpeed::GameMemorySpeed()
{
	static BOOL isSpeedReconnect = false;
	static DWORD SpeedKey = 'E';
	if (g_Vars.m_Speed.sk_speed_key->value == 0)
		SpeedKey = 'E';
	else if (g_Vars.m_Speed.sk_speed_key->value == 1)
		SpeedKey = VK_SHIFT;
	else if (g_Vars.m_Speed.sk_speed_key->value == 2)
		SpeedKey = VK_LBUTTON;
	else if (g_Vars.m_Speed.sk_speed_key->value == 3)
		SpeedKey = VK_RBUTTON;
	else if (g_Vars.m_Speed.sk_speed_key->value == 4)
		SpeedKey = VK_MBUTTON;

	if ((GetKeyState(SpeedKey) < 0) || g_Vars.m_Speed.sk_speed_key->value == 5 && ((GetKeyState(VK_LBUTTON) < 0) || (GetKeyState(VK_RBUTTON) < 0)))
	{
		memset((PVOID)m_dwSpeed[0], 0, 8);
		memset((PVOID)m_dwSpeed[1], 0, 8);

		memset((PVOID)m_dwSpeed[2], 1, 1);
		memset((PVOID)m_dwSpeed[2], 0, 1);

		memset((PVOID)m_dwSpeed[0], 0, 8);
	
		//memset((PVOID)m_dwSpeed[3], 0, 3);

		isSpeedReconnect = true;
		g_Vars.m_Speed.IsSpeed = TRUE;
	}
	else
	{
		g_Vars.m_Speed.IsSpeed = FALSE;

		if (isSpeedReconnect && g_Vars.m_Speed.sk_speed_reconnect->value)
		{
			isSpeedReconnect = false;

			auto delayConnectRoom = [&]()
			{
				Sleep(g_Vars.m_Speed.sk_speed_reconnect_timer->value * 1000);
				//重连房间
				Engine::g_Engine.pfnClientCmd(xorstr_("reconnect"));
			};

			std::thread th(delayConnectRoom);
			th.detach();

		}
		isSpeedReconnect = false;
	}

}

DWORD WINAPI Reconnect(LPVOID lpThreadParameter)
{
	return 1;
}

void  GameSpeed::AdjustSpeed(double Speed)
{
	*(double*)g_Offset.GetGameAddresInfo()->dwAdSpeed = Speed * 1000;
}

void GameSpeed::TimeSpeed()
{
	static DWORD SpeedKey = 'E';
	if (g_Vars.m_Speed.sk_speed_key->value == 0)
		SpeedKey = 'E';
	else if (g_Vars.m_Speed.sk_speed_key->value == 1)
		SpeedKey = VK_SHIFT;
	else if (g_Vars.m_Speed.sk_speed_key->value == 2)
		SpeedKey = VK_LBUTTON;
	else if (g_Vars.m_Speed.sk_speed_key->value == 3)
		SpeedKey = VK_RBUTTON;
	else if (g_Vars.m_Speed.sk_speed_key->value == 4)
		SpeedKey = VK_MBUTTON;
	else if (g_Vars.m_Speed.sk_speed_key->value == 6)
	{
		m_fSpeedTiemr = g_Vars.m_Speed.sk_speed_value->value;
		return;
	}

	if ((GetKeyState(SpeedKey) < 0) || g_Vars.m_Speed.sk_speed_key->value == 5 && ((GetKeyState(VK_LBUTTON) < 0) || (GetKeyState(VK_RBUTTON) < 0)))
	{

		m_fSpeedTiemr = g_Vars.m_Speed.sk_speed_value->value;

	}
	else
	{
		m_fSpeedTiemr = 1.0f;

	}
}

BOOL WINAPI GameSpeed::extueryPerformanceCounter(LARGE_INTEGER* lp)
{
	BOOL  ret = QueryPerformanceCounter(lp);

	static LONG64 last_real = 0;
	static LONG64 last_fake = 0;

	LONG64 new_real = lp->QuadPart;
	double factor =  m_fSpeedTiemr;
	DWORD diff_real = (DWORD)(new_real - last_real);
	DWORD diff_fake = (DWORD)(factor * double(diff_real));
	lp->QuadPart = last_fake + diff_fake;
	last_fake += diff_fake;
	last_real += diff_real;

	return ret;
}

void GameSpeed::PlayerSpeed()
{
	static DWORD SpeedKey = 'E';
	if (g_Vars.m_Speed.sk_speed_key->value == 0)
		SpeedKey = 'E';
	else if (g_Vars.m_Speed.sk_speed_key->value == 1)
		SpeedKey = VK_SHIFT;
	else if (g_Vars.m_Speed.sk_speed_key->value == 2)
		SpeedKey = VK_LBUTTON;
	else if (g_Vars.m_Speed.sk_speed_key->value == 3)
		SpeedKey = VK_RBUTTON;
	else if (g_Vars.m_Speed.sk_speed_key->value == 4)
		SpeedKey = VK_MBUTTON;
	else if (g_Vars.m_Speed.sk_speed_key->value == 6)
	{
		AdjustSpeed(g_Vars.m_Speed.sk_speed_value->value);

		return;
	}
	if ((GetKeyState(SpeedKey) < 0) || g_Vars.m_Speed.sk_speed_key->value == 5 && ((GetKeyState(VK_LBUTTON) < 0) || (GetKeyState(VK_RBUTTON) < 0)))
	{
		AdjustSpeed(g_Vars.m_Speed.sk_speed_value->value);
	}
	else
	{
		AdjustSpeed(1);
	}
}
