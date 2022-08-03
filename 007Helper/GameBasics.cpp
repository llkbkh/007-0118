#include "GameBasics.h"
#include "GameOffset.h"
#include <imm.h>
#include "Drawing.h"
#include <PackReader.h>
#include "Util.h"
#include "GamePack.h"
#include "drawimage.h"
#include "cSprites.h"
#include "GameCFG.h"
GameBasics::GameBasics()
{
	m_bFreeLook           = false;
	m_Player              = nullptr;
	m_NightVision         = false;
	m_HPGaugeInfo         = nullptr;
	m_DeathMsg_iKillCount = 0;
}

GameBasics* GameBasics::GetInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new GameBasics();
	}
	return m_pInstance;
}

int	pfnClientCmd(char* szCmdString)
{
	ACEInfoFileLog("[{}]:{}", __FUNCTION__, szCmdString);

	return Engine::g_Engine.pfnClientCmd(szCmdString);
}

void (*Pfsub_375C2FC0)(int index, int len, BYTE* buf, int sendtype, int a6, int a7, int a8, int a9) = nullptr;
unsigned int __cdecl sub_375C2FC0(BYTE* a1, int a2, int a3)
{
	unsigned int result; // eax
	int* v4; // ebx
	int v5; // edi
	signed int v6; // ecx
	BYTE* v7; // esi
	char v8; // dl
	bool v9; // zf
	int v10; // [esp+4h] [ebp-8h]
	int v11; // [esp+8h] [ebp-4h]

	if (a2)
	{
		std::vector<BYTE> VecPack(a2);

		memcpy(VecPack.data(), a1,a2);

		PackReader Pack = PackReader(VecPack);
		ULONG ulFlags = Pack.ReadChar();



		//ACEWarningFileLog("sendto:{:02X}", fmt::join(VecPack.begin(), VecPack.end(), " "));

		if (ulFlags == 3)
		{
			//命令执行
			ACEWarningFileLog("command:{}", Pack.ReadStr());
		}
		else if (ulFlags == 2)
		{
			//封包结构
			BYTE v1 = Pack.ReadChar();
			
			//序列
			if (g_Vars.TestValue)
			{

				switch (v1)
				{
				case 0:
				case 1:
				case 2:
				case 3:
				//case 0x23:
					//Pack.WriteBYTE(0x4);
					break;

				//case 0x21:	//与人物移动开枪有关
				//case 0x22:
				//case 0x23:
				//case 0x24:
				//{
				//	Pack.WriteBYTE(0x4);
				//	break;
				//}
				default:

					if (v1 >= *(PBYTE)(0x39100000))
					{
						Pack.WriteBYTE(0x4);

					}


					break;
				}
			}

			//if (v1 == /*0x20*/*(PBYTE)(0x39100000))
			//{
			//	
			//}

		}

	
		memcpy((PVOID)a1, VecPack.data(), a2);

		ACEWarningFileLog("sendto:{:02X}", fmt::join(VecPack.begin(), VecPack.end(), " "));
	}


	result = a2 & 0xFFFFFFFC;

	v11 = (signed int)(a2 & 0xFFFFFFFC) / 4;

	if (v11 > 0)
	{
		v4 = (int*)a1;
		result = ~a3;
		v10 = ~a3;
		v5 = -(signed int)&a2;
		do
		{
			a2 = (unsigned __int8)((result ^ *v4) >> 24)+ (((unsigned __int8)((result ^ *v4) >> 16) + ((((unsigned __int8)(result ^ *(BYTE*)v4) << 8) + ((unsigned __int16)(result ^ *(WORD*)v4) >> 8)) << 8)) << 8);
			v6 = 0;
			do
			{
				v7 = (BYTE*)&a2 + v6;
				BYTE k = ((BYTE)v5 + (unsigned __int8)((char*)&a2 + v6)) & 0xF;
				v8 = v6 | *(PBYTE)(0x37BCBF14 + k) | ((BYTE)v6 << v6);
				++v6;
				*v7 ^= v8 | 0xA5;
			} while (v6 < 4);
			v5 = v5 + 1;
			*v4 = a3 ^ a2;
			++v4;
			v9 = v11-- == 1;
			result = v10;
		} while (!v9);
	}
	return result;
}



void (*PfnextHookSend)(int index, int len, BYTE* buf, int sendtype, int a6, int a7, int a8, int a9) = nullptr;

void  __cdecl extHookSend(int index, int len, BYTE* buf, int sendtype, int a6, int a7, int a8, int a9)
{

	//ACEDebugFileLog("index:{} len:{} sendtype:{} a6:{} a7:{} a8:{} a9:{}",index, len, sendtype, a6, a7, a8, a9);


	std::vector<BYTE> VecPack(len);

	memcpy(VecPack.data(), buf, len);

	//ACEWarningFileLog("sendto:{:02X}", fmt::join(VecPack.begin(), VecPack.end(), " "));


	//RtlZeroMemory(VecPack.data(),0x10);

	////PackReader Pack = PackReader(VecPack);

	////ULONG ulFlags = Pack.ReadChar();


	////if (ulFlags == 0x56)
	////{
	////	ULONG v1 = Pack.ReadInt();
	////	BYTE  v2 = Pack.ReadChar();



	////}


	//if (g_Vars.TestValue)
	//{
	//	
	//
	//	return;



	//}

	PfnextHookSend(index, VecPack.size(), VecPack.data(), sendtype, a6, a7, a8, a9);
}


bool GameBasics::vInit()
{
	m_Memory          = g_pUtil->GetAceMemory();
	m_Player          = GamePlayer::GetInstance();
	m_HPGaugeInfo     = GetpfnUserMsg(xorstr_("HPGaugeInfo"));
	m_HPGaugeInfoPack = PackWriter(1024);

	//Engine::g_pEngine->pfnClientCmd = pfnClientCmd;

	//g_pUtil->GetAceHook()->InlineHook(0x3761AF70, 0, extHookSend, &PfnextHookSend);
	//g_pUtil->GetAceHook()->InlineHook(0x375CA890, 0, sub_375C2FC0, &Pfsub_375C2FC0);


#if 0
	//cvar_s* pVar = Engine::g_Engine.pfnGetCvarList();

	//while (pVar)
	//{
	//	//ACEDebugFileLog("{} {}", pVar->name, pVar->value);
	//	if (lstrcmpi(pVar->name, xorstr_("cl_killeffect")) == 0)
	//	{
	//		m_pCl_killeffect = pVar;
	//	}
	//	pVar = pVar->next;
	//}

	PUserMsg Ptr = (PUserMsg) * (PDWORD)(Engine::g_pUserMsgBase);

	while (Ptr->next)
	{
		ACEDebugFileLog("{} {:X}", Ptr->name, (ULONG)Ptr->pfn);

		Ptr = Ptr->next;
	}

	

	cmd_s* pCmd = Engine::g_Engine.pfnGetCmdList();

	while (pCmd)
	{

		ACEDebugFileLog("{} {}", pCmd->name, (ULONG)pCmd->function);

		pCmd = pCmd->next;
	}

#endif

#if 0
	CACEMemory m_Memory;
	CACEHook m_Hook;
	DWORD dwFpsLimit = 0;
	//锁30FPS限制 - 但是会增加CPU负担
	if (m_Memory.GetScanCode("E8 ?? ?? ?? ?? F3 0F 10 05 ?? ?? ?? ?? F3 0F 10 0D ?? ?? ?? ?? 0F 2F C1 76 1A", dwFpsLimit, g_Offset.GetEngineModuleInfo()->hModule))
	{

		// 下断最大FPS值 找跳转即可
		dwFpsLimit += m_Hook.GetDisasmLenght(dwFpsLimit, 4);

		DWORD OldProtect;
		VirtualProtect((PVOID)dwFpsLimit, 1024, 64, &OldProtect);

		*(PBYTE)(dwFpsLimit) = 0xEB;

		VirtualProtect((PVOID)dwFpsLimit, 64, OldProtect, &OldProtect);

		ACEDebugFileLog("dwFpsLimit:{:X}", dwFpsLimit);

		Engine::g_pEngine->pfnGetCvarPointer("fps_max")->value = 101.0f;
	}
#endif
	




	return true;
}
std::map<std::string, float> m_MapGameVar;

int GameBasics::vKeyBoardProc(int nCode, WPARAM keynum, LPARAM lParam)
{
	if (!g_Vars.m_Basi.sk_basics_enable->value)
	{
		return 0;
	}
#if 0

	if (keynum /*== g_Vars.m_Basi.sk_basics_reconnect->hotkey*/==VK_F1)
	{

		CGamePack::GetInstance()->Item_Roulette();


		//PUserMsg Ptr = (PUserMsg) * (PDWORD)(Engine::g_pUserMsgBase);

		//while (Ptr->next)
		//{
		//	ACEDebugFileLog("{} {:X}", Ptr->name, (ULONG)Ptr->pfn);

		//	Ptr = Ptr->next;
		//}



		//cvar_s* pVar = Engine::g_Engine.pfnGetCvarList();
		//while (pVar)
		//{
		//	if (!m_MapGameVar.count(pVar->name))
		//	{
		//		m_MapGameVar.insert(std::map<std::string, float>::value_type(pVar->name, pVar->value));
		//	}
		//	else
		//	{
		//		if (m_MapGameVar.at(pVar->name) != pVar->value)
		//		{
		//			ACEDebugFileLog("{} {}", pVar->name, pVar->value);
		//		}

		//	}

		//	pVar = pVar->next;
		//}
	}
	
#endif
	

	//软键盘0 重连房间
	if (g_Vars.m_Basi.sk_basics_reconnect->value && keynum == g_Vars.m_Basi.sk_basics_reconnect->hotkey)
	{
		Engine::g_Engine.pfnClientCmd(xorstr_("retry")); //reconnect retry
	}
	//返回房间
	else if (g_Vars.m_Basi.sk_basics_retroom->value && keynum == g_Vars.m_Basi.sk_basics_retroom->hotkey)
	{

		auto ReRoomFuns = [&]()->void
		{
			for (int i = 0; i < 1024; i++)
			{
				*(BYTE*)(g_Offset.GetGameAddresInfo()->dwRetRoom + 3) = 0;
			}
	
		};
		Engine::g_Engine.pfnClientCmd(xorstr_("mp_retroom"));
		
		std::thread th(ReRoomFuns);
		th.detach();
	}
	//屏蔽菜单
	else if (g_Vars.m_Basi.sk_basics_maskgamemenu->value && keynum == g_Vars.m_Basi.sk_basics_maskgamemenu->hotkey)
	{
		DWORD dwBase = m_Memory->Read<DWORD>(g_Offset.GetGameAddresInfo()->dwGuiMenu, 0x28);
		if (dwBase && dwBase != 0xFFFFFFFF)
		{
			*(WORD*)(dwBase + 0x42) = !(*(WORD*)(dwBase + 0x42));
		}

	}
	//复活
	else if (g_Vars.m_Basi.sk_basics_rpn->value && keynum == g_Vars.m_Basi.sk_basics_rpn->hotkey)
	{
		Engine::g_Engine.pfnClientCmd(xorstr_("rpn"));
	}
	//强制购买
	else if (g_Vars.m_Basi.sk_basics_buymenu->value && keynum == g_Vars.m_Basi.sk_basics_buymenu->hotkey)
	{
		Engine::g_Engine.pfnClientCmd(xorstr_("vxl_BuyZone"));
	}
	//无限无人机
	else if (g_Vars.m_Basi.sk_basics_drone->value && keynum == g_Vars.m_Basi.sk_basics_drone->hotkey)
	{
		g_Vars.m_Basi.sk_basics_drone->flags = 1;
		Engine::g_Engine.pfnClientCmd(xorstr_("vxl_BuyZone"));
	}
	//快捷买刀
	else if (g_Vars.m_Basi.sk_basics_itemknifeid->value && keynum == g_Vars.m_Basi.sk_basics_itemknifeid->hotkey)
	{
		auto Text =  fmt::format(xorstr_("item_{}"), (int)g_Vars.m_Basi.sk_basics_itemknifeid->flags);
		Engine::g_Engine.pfnClientCmd(Text.data());
	}
	//卡空
	else if (g_Vars.m_Basi.sk_basics_cardenable->value && g_Vars.m_Basi.sk_basics_cardenable->hotkey == keynum)
	{
		g_Vars.m_Basi.sk_basics_play_card->value = !g_Vars.m_Basi.sk_basics_play_card->value;
	}
	//去黑雾
	else if (g_Vars.m_Basi.sk_basics_noblackfog->value && g_Vars.m_Basi.sk_basics_noblackfog->hotkey == keynum)
	{
		Engine::g_Engine.pfnClientCmd(xorstr_("gl_fog 0"));
	}
	//缔造者关灯
	else if (g_Vars.m_Basi.sk_basics_vxllight->value && g_Vars.m_Basi.sk_basics_vxllight->hotkey == keynum)
	{
		static int nFlag = 1;
		auto Text = fmt::format(xorstr_("vxl_remove_dim_light {}"), (int)nFlag ^= 1);
		Engine::g_Engine.pfnClientCmd(Text.data());
	}
	//第三人称
	else if (g_Vars.m_Basi.sk_basics_thirdperson->value && g_Vars.m_Basi.sk_basics_thirdperson->hotkey == keynum)
	{

		//取第三位当做判断位
		// 1<<0 true 第三人称  fase = 自由视角
		// 1<<1 判断开关
		g_Vars.m_Basi.sk_basics_thirdperson->flags ^= (1 << 1);
	}
	//超级回房
	else if (g_Vars.m_Basi.sk_basics_superretroom->value && g_Vars.m_Basi.sk_basics_superretroom->hotkey == keynum)
	{
		Engine::g_Engine.pfnClientCmd(xorstr_("+SuperRetRoom"));
	}
	//飞天
	else if (g_Vars.m_Basi.sk_basics_fly->value && g_Vars.m_Basi.sk_basics_fly->hotkey == keynum)
	{
		auto Flying = [&]()->void
		{
			Engine::g_Engine.pfnClientCmd(xorstr_("+jump"));

			*(double*)(g_Offset.GetGameAddresInfo()->dwAdSpeed) = 1.0;

			//g_Vars.m_Basi.sk_basics_play_card->value = 1;

			Sleep(g_Vars.m_Basi.sk_basics_fly->flags);

			//g_Vars.m_Basi.sk_basics_play_card->value = 0;

			*(double*)(g_Offset.GetGameAddresInfo()->dwAdSpeed) = 1000.0;

			Engine::g_Engine.pfnClientCmd(xorstr_("-jump"));
		

		};
		std::thread th(Flying);
		th.detach();
	}
	//飞天漫步
	else if (g_Vars.m_Basi.sk_basics_fly2->value && g_Vars.m_Basi.sk_basics_fly2->hotkey == keynum)
	{
		static int bIsSpeedFlg = 1;
		*(double*)(g_Offset.GetGameAddresInfo()->dwAdSpeed) = (bIsSpeedFlg ^= 1) ?  1000: 100;
	
	}
	else if (g_Vars.m_Basi.sk_basics_discipline->value && keynum == 'P')
	{
		for (int Index = 1; Index <= Engine::g_Engine.GetMaxClients(); ++Index)
		{

			if (Index != m_Player->GetLocal()->Player.Index)
			{
				auto Player = m_Player->GetPlayer(Index);

				if (Player->Entity == NULL || Player->Info.name == NULL)
					continue;;

				CGamePack::GetInstance()->Discipline(g_pUtil->GetAceTool()->UTF8_To_string(m_Player->GetPlayer(Index)->Info.name));
			}
		}


	}
#if ANTI_DEBUG
	if (keynum == VK_F1)
	{
		//CGamePack::GetInstance()->Kick_RoomPlayer(nCode);
		g_Vars.TestValue = !g_Vars.TestValue;
		//ACEDebugFileLog("{} TestValue:{}", __FUNCTION__, g_Vars.TestValue);

	}
	if (keynum == VK_F2)
	{
		//CGamePack::GetInstance()->Room_StartGame();
	}

#endif

	return 1;
}

void GameBasics::vCL_CreateMove(float frametime, usercmd_s* cmd, int active)
{
	if (!g_Vars.m_Basi.sk_basics_enable->value)
		return;

	if (g_Vars.m_Basi.sk_basics_thirdperson->value)
	{
		//第三视角自由视角
		if ((g_Vars.m_Basi.sk_basics_thirdperson->flags & ((1 << 0) | (1 << 1))) == ((1 << 0) | (1 << 1)))
			this->FreeLookFunction(frametime, cmd);
		else
			this->m_bFreeLook = false;
	}

}

void GameBasics::vV_CalcRefdef(ref_params_s* pparams)
{
	if (!g_Vars.m_Basi.sk_basics_enable->value)
		return;

	if (g_Vars.m_Basi.sk_basics_thirdperson->value && g_Vars.m_Basi.sk_basics_thirdperson->flags & (1 << 1))
	{

		Vector Offset(0, 0, 0);
		Vector r, u, b;

		VectorMul(pparams->right, g_Vars.m_Basi.sk_basics_idealyaw->value, r);
		VectorMul(pparams->up, g_Vars.m_Basi.sk_basics_viewhighly->value, u);
		VectorMul(pparams->forward, -(g_Vars.m_Basi.sk_basics_viewdistance->value), b);

		Offset = Offset + r;
		Offset = Offset + u;
		Offset = Offset + b;

		pparams->vieworg[0] += Offset[0];
		pparams->vieworg[1] += Offset[1];
		pparams->vieworg[2] += Offset[2];

		//自由视角
		if (g_Vars.m_Basi.sk_basics_thirdperson->flags & (1 << 0))
		{
			VectorCopy(m_Player->GetLocal()->FreeLookOffset, pparams->vieworg);
		}

	}


	//改变视角
	if (g_Vars.m_Basi.sk_basics_ballistic->value)
	{
		float fValue = 90;

		switch (int(g_Vars.m_Basi.sk_basics_ballistic->value))
		{
		case 1:
			fValue = 180;
			break;
		case 2:
			fValue = 91;
			break;
		case 3:
			fValue = 90;
			break;
		default:
			break;
		}

		*(PFLOAT)(g_Offset.GetGameAddresInfo()->dwBallistic) = fValue;

	}
}


void GameBasics::vHUD_Redraw(float time, int intermission)
{
	g_flTime = time;
	//画出击杀图标
	DrawImageRedraw();

	if (!g_Vars.m_Basi.sk_basics_enable->value)
	{
		return;
	}

	//人物卡空
	if (g_Vars.m_Basi.sk_basics_play_card->value)
	{
		PlayerCard();
	}

	//夜视
	NightVision();

	//地图高亮
	if (g_Vars.m_Basi.sk_basics_maplighten->value)
	{
		MapLight();
	}

	//方框显血
	BoxShowHp();

}

int GameBasics::vStatusValue(const char* pszName, int iSize, void* pbuf)
{
	auto lReader = std::make_shared<PackReader>((BYTE*)pbuf, iSize);


	if (lReader->GetSize() >= 3 && g_Vars.m_Basi.sk_basics_healthmode->value)
	{
		int nFlag  = lReader->ReadChar();
		int nValue = lReader->ReadShort();

		switch (nFlag)
		{
		case 0:	//激活
			g_Vars.m_Basi.HeadlthArray[0].Activate = nValue;
			//lReader->WriteShort(1);	//一直锁定显示
			break;
		case 1:	//团队关系		
			//g_Vars.m_Basi.HeadlthArray[0].TeamRelationship = nValue;
			if (g_Vars.m_Basi.sk_basics_healthmode->value &&  g_Vars.m_Basi.sk_basics_healthmode->flags & (1 << 1))
			{
				lReader->WriteShort(1);
			}
			break;
		case 2:	//当前激活的玩家索引
			if (nValue < 33)
			{
				g_Vars.m_Basi.m_CurActivateHeadlthPlayer = nValue;
			}

			break;
		case 3:	//玩家的健康状态
			g_Vars.m_Basi.HeadlthArray[g_Vars.m_Basi.m_CurActivateHeadlthPlayer].Health = nValue;
			g_Vars.m_Basi.HeadlthArray[0].Health = nValue;
			break;
		default:
			
			break;
		}

	}

	return 1;
}

int GameBasics::vInitHUD(const char* pszName, int iSize, void* pbuf)
{
	g_Vars.m_Basi.m_CurActivateHeadlthPlayer = 0;
	RtlZeroMemory(g_Vars.m_Basi.HeadlthArray, sizeof(g_Vars.m_Basi.HeadlthArray));

	if (g_Vars.m_Basi.sk_basics_enable->value)
	{
		if (g_Vars.m_Basi.sk_basics_antikick->value)
		{
			Engine::g_Engine.pfnClientCmd(xorstr_("+PackDisconnectRoom"));
		}
	}

	//初始化击杀消息
	DrawImageInitialize();
	if (g_Vars.m_Basi.sk_basics_deathbox->value)
	{
		// 屏蔽游戏的成就提示  不好屏蔽.声音也没了
		//m_pCl_killeffect->value = 0;

	}

	if (g_Vars.m_Basi.sk_basics_fps->value)
	{
		Engine::g_Engine.pfnGetCvarPointer(xorstr_("fps_max"))->value = g_Vars.m_Basi.sk_basics_fps->flags;

		if (g_Vars.m_Basi.sk_basics_fps->flags >= 101)
		{
			Engine::g_Engine.pfnGetCvarPointer(xorstr_("developer"))->value = 1;

		}
	}

	return 1;
}


int GameBasics::vVGUIMenu(const char* pszName, int iSize, void* pbuf)
{
	auto lReader = std::make_shared<PackReader>((BYTE*)pbuf, iSize);


	//无限无人机原理:先购买手雷,然后再购买无人机
	if (lReader->GetSize() > 3)
	{
		lReader->ReadShort();

		int Type = lReader->ReadChar();

		// 0 = 阵营 2 = 购买菜单

		if (Type == 0 && g_Vars.m_Basi.sk_basics_obstp->value)
		{
			Engine::g_Engine.pfnClientCmd(xorstr_("vxl_BuyZone;item_8286"));
			return 1;
		}

		if (Type == 2)
		{
			//无限无人机
			if (g_Vars.m_Basi.sk_basics_drone->flags)
			{
				if (g_Vars.m_Basi.sk_basics_drone->flags == 1)
				{
					Engine::g_Engine.pfnClientCmd(xorstr_("item_31;vxl_BuyZone;item_8286"));			
					g_Vars.m_Basi.sk_basics_drone->flags = 2;
				}
				else if (g_Vars.m_Basi.sk_basics_drone->flags == 2)
				{
					g_Vars.m_Basi.sk_basics_drone->flags = 0;
				}
				return 2;
			}
			//神鬼开天无敌宏
			if (g_Vars.m_Basi.sk_basics_item8530->flags)
			{
				if (g_Vars.m_Basi.sk_basics_item8530->flags == 1)
				{
					g_Vars.m_Basi.sk_basics_item8530->flags = 2;
					Engine::g_Engine.pfnClientCmd(xorstr_("item_8047;vxl_BuyZone;item_8530"));
				}
				else if (g_Vars.m_Basi.sk_basics_item8530->flags == 2)
				{
					g_Vars.m_Basi.sk_basics_item8530->flags = 0;
				
				}

				return 2;
			}
			//圣剑无限格挡
			if (g_Vars.m_Basi.sk_basics_item8133->flags || g_Vars.m_Basi.sk_basics_item8133->value)
			{
				if (g_Vars.m_Basi.sk_basics_item8133->flags == 1)
				{
					Engine::g_Engine.pfnClientCmd(xorstr_("item_8047;vxl_BuyZone;item_8133"));
					g_Vars.m_Basi.sk_basics_item8133->flags = 2;
					//Engine::g_Engine.pfnClientCmd(xorstr_("vxl_BuyZone"));
				}
				//else if (g_Vars.m_Basi.sk_basics_item8133->flags == 2)
				//{
				//	g_Vars.m_Basi.sk_basics_item8133->flags = 0;
				//	//Engine::g_Engine.pfnClientCmd(xorstr_("item_8133"));
				//}
				return 2;
			}
			//小圣剑无限购买
			if (g_Vars.m_Script.m_buy_excalibur_ex)
			{
				if (g_Vars.m_Script.m_buy_excalibur_ex == 1)
				{
					Engine::g_Engine.pfnClientCmd(xorstr_("item_8047;vxl_BuyZone;item_8634"));
					g_Vars.m_Script.m_buy_excalibur_ex = 2;
				}
				return 2;
			}
		}
	}

	return 1;
}

int GameBasics::vMPToCL(const char* pszName, int iSize, void* pbuf)
{

	auto lReader = std::make_shared<PackReader>((BYTE*)pbuf, iSize);


	int MsgType = lReader->ReadChar();

	if (MsgType == 37)
	{
		if (g_Vars.m_Basi.sk_basics_drone->value || g_Vars.m_Basi.sk_basics_item8530->value || g_Vars.m_Basi.sk_basics_item8133->value || g_Vars.m_Script.m_buy_excalibur_ex)
		{


			return 2;
		}
		

	}
	//团队被击杀框
	if (g_Vars.m_Basi.sk_basics_deathbox->value && MsgType == 40)
	{
		return 2;
	}

	//ACEDebugLog("{} msg:{}",__FUNCTION__,MsgType);


	if (g_Vars.m_Basi.sk_basics_enable->value)
	{
	
		switch (MsgType)
		{
		case 1:	//开局或者復活?
		{
			if (g_Vars.m_Basi.sk_basics_antikick->value)
			{
				// 需要离开房间

				auto DisconnectRoomFuns = [&]()->void
				{
					Engine::g_Engine.pfnClientCmd("record 1.dem");
					Sleep(1500);
					Engine::g_Engine.pfnClientCmd(xorstr_("+PackDisconnectRoom"));
					Engine::g_Engine.pfnClientCmd("stop");
				};
				std::thread th(DisconnectRoomFuns);
				th.detach();
			}

			//恢复
			if (g_Vars.m_Script.m_buy_excalibur_ex)
				g_Vars.m_Script.m_buy_excalibur_ex = 0;
		}
			break;
		case 0x7D:// 每局结束后的结算信息
		{
			if (g_Vars.m_Basi.sk_basics_antikick->value)
			{
				//重新进入房间
				Engine::g_Engine.pfnClientCmd(xorstr_("+PackJoinRoom"));
			}
			//恢复
			if (g_Vars.m_Script.m_buy_excalibur_ex)
				g_Vars.m_Script.m_buy_excalibur_ex = 0;

		}
			break;
		default:
			break;
		}


	}

	if (MsgType == 124)
	{
		m_DeathMsg_iKillCount = 0;

		int v249 = lReader->ReadChar();
		int v250 = 0;
		if (v249 > 0)
		{
			do
			{
				v250 = lReader->ReadChar();

				switch (v250)
				{
				case 4:
					m_DeathMsg_iKillCount = 1;
					break;
				case 5:
					m_DeathMsg_iKillCount = 2;
					break;
				case 6:
					m_DeathMsg_iKillCount = 3;
					break;
				default:
					break;
				}

				--v249;
			} while (v249);
		}
	}
	


	return 1;
}

int GameBasics::vMPToCL2(const char* pszName, int iSize, void* pbuf)
{

	return 1;
}

int GameBasics::vDeathMsg(const char* pszName, int iSize, void* pbuf)
{
	if (g_Vars.m_Basi.sk_basics_enable->value == 0)
	{
		return 1;
	}

	auto lPackReader = std::make_shared<PackReader>((BYTE*)pbuf, iSize);

	//击杀者
	int iKiller   = lPackReader->ReadChar();
	//被击杀
	int iVictim   = lPackReader->ReadChar();
	//助攻
	int iAssist   = lPackReader->ReadChar();
	//是否被爆头
	int iHeadShot = lPackReader->ReadChar();

	auto Local = Engine::g_Engine.GetLocalPlayer();

	//ACEDebugFileLog("{} {} {} {} {} Size:{}", __FUNCTION__, iKiller, iVictim, iAssist, iHeadShot, iSize);

	if (Local == nullptr)
	{
		return 1;
	}
	int nMeIndex = Local->index;


	//被击杀的是自己
	if (iVictim == nMeIndex)
	{
		//如果是人类感染为僵尸的话.不会重新进入房间
		if (g_Vars.m_Basi.sk_basics_antikick->value)
		{
			//击杀者是人类的话.
			if (m_Player->GetPlayer(iKiller)->Team == TEAM::CT)
			{
				//重新进入房间
				Engine::g_Engine.pfnClientCmd(xorstr_("+PackJoinRoom"));
			}
		}
	}
	//我击杀了XX
	if (g_Vars.m_Basi.sk_basics_deathico->value && iKiller == nMeIndex)
	{
		hud_player_info_t hPlayer;

		if (iKiller != iVictim)
		{
			Engine::g_Engine.pfnGetPlayerInfo(iKiller, &hPlayer);

			m_DeathMsg_iKillCount = m_DeathMsg_iKillCount > 4 ? 4 : m_DeathMsg_iKillCount;

			auto szSpr1 = fmt::format(xorstr_("KM_Number{}"), m_DeathMsg_iKillCount);

			DrawImageItem rgTempDrawImage;
			//计算数字和文字的宽度和间距
			wrect_t* TempRect;


			TempRect = &g_Sprite.GetRc(g_Sprite.GetSpriteIndex(szSpr1.data()));
			int NumWidth = (TempRect->right) - (TempRect->left);
			int NumHeight = (TempRect->bottom - TempRect->top);
			//int NumWidth = 41;
			//int NumHeight = 68;
			TempRect = &g_Sprite.GetRc(g_Sprite.GetSpriteIndex(xorstr_("KM_KillText")));
			int TextWidth = (TempRect->right) - (TempRect->left);
			int TextHeight = (TempRect->bottom - TempRect->top);
			int iSprAllWidthHalf = (NumWidth + TextWidth + 20) / 2;
			//确定X中间
			int x = Engine::g_Screen.iWidth / 2 - iSprAllWidthHalf;
			//确定Y合适
			int iY = (int)Engine::g_Screen.iHeight * 0.15;
			int y = iY;//min(iY,100);
			//Kill Number
			rgTempDrawImage.iFunction = 1;
			rgTempDrawImage.iCheck = 1;
			rgTempDrawImage.iCenter = 0;
			rgTempDrawImage.iSprIndex = g_Sprite.GetSpriteIndex(szSpr1.data());
			rgTempDrawImage.hl_hSprites = g_Sprite.GethSpr(rgTempDrawImage.iSprIndex);
			rgTempDrawImage.x = x;
			rgTempDrawImage.y = (int)y - (NumHeight - 68) / 2;
			rgTempDrawImage.color.r = 255;
			rgTempDrawImage.color.g = 255;
			rgTempDrawImage.color.b = 255;
			rgTempDrawImage.iMode = 2;
			rgTempDrawImage.flStartDisplayTime = g_flTime;
			rgTempDrawImage.flEndDisplayTime = 3.0 + g_flTime;
			rgTempDrawImage.iChanne = 51;
			rgTempDrawImage.iLength = -1;
			DrawImageAdd(rgTempDrawImage);
			//Kill Text
			y = iY;//min(iY,100);
			rgTempDrawImage.iFunction = 1;
			rgTempDrawImage.iCheck = 1;
			rgTempDrawImage.iCenter = 0;
			rgTempDrawImage.iSprIndex = g_Sprite.GetSpriteIndex(xorstr_("KM_KillText"));
			rgTempDrawImage.hl_hSprites = g_Sprite.GethSpr(rgTempDrawImage.iSprIndex);
			rgTempDrawImage.x = x + NumWidth + 5;
			//rgTempDrawImage.y = y+ (NumHeight/2)-(TextHeight/2);
			rgTempDrawImage.y = y + (68 / 2) - (TextHeight / 2);
			rgTempDrawImage.color.r = 255;
			rgTempDrawImage.color.g = 255;
			rgTempDrawImage.color.b = 255;
			rgTempDrawImage.iMode = 2;
			rgTempDrawImage.flStartDisplayTime = g_flTime;
			rgTempDrawImage.flEndDisplayTime = 3.0 + g_flTime;
			rgTempDrawImage.iChanne = 50;
			rgTempDrawImage.iLength = -1;
			DrawImageAdd(rgTempDrawImage);

			if (iHeadShot)
			{
				rgTempDrawImage.iFunction = 1;
				rgTempDrawImage.iCheck = 1;
				rgTempDrawImage.iCenter = 0;
				rgTempDrawImage.iSprIndex = g_Sprite.GetSpriteIndex(xorstr_("KM_Icon_Head"));
				rgTempDrawImage.hl_hSprites = g_Sprite.GethSpr(rgTempDrawImage.iSprIndex);
				wrect_t* TempRect;
				TempRect = &g_Sprite.GetRc(g_Sprite.GetSpriteIndex(xorstr_("KM_Icon_Head")));
				int tWidth = (TempRect->right) - (TempRect->left);
				rgTempDrawImage.x = (Engine::g_Screen.iWidth - tWidth) / 2;
				rgTempDrawImage.y = y + 60;
				rgTempDrawImage.color.r = 255;
				rgTempDrawImage.color.g = 255;
				rgTempDrawImage.color.b = 255;
				rgTempDrawImage.iMode = 2;
				rgTempDrawImage.flStartDisplayTime = g_flTime;
				rgTempDrawImage.flEndDisplayTime = 5.0 + g_flTime;
				rgTempDrawImage.iChanne = 49;
				rgTempDrawImage.iLength = -1;
				DrawImageAdd(rgTempDrawImage);
			}


		}
	}




	return 1;
}

int GameBasics::vKillInfo(const char* pszName, int iSize, void* pbuf)
{
	// 屏蔽生化被击杀信息
	if (g_Vars.m_Basi.sk_basics_deathbox->value)
	{
		return 2;
	}

	return 1;
}

int GameBasics::vPlayerInfo(const char* pszName, int iSize, void* pbuf)
{
	//;屏蔽 足球
	if (g_Vars.m_Basi.sk_basics_soccer->value)
	{
		return 2;
	}
	return 1;
}

void GameBasics::FreeLookFunction(float frametime, usercmd_s* cmd)
{

	if (!m_bFreeLook && m_Player->GetLocal()->Entity != NULL)
	{
		Engine::g_Engine.GetViewAngles(m_Player->GetLocal()->FreeLookAngles);
		m_Player->GetLocal()->FreeLookOffset[0] = m_Player->GetLocal()->Entity->origin.x;
		m_Player->GetLocal()->FreeLookOffset[1] = m_Player->GetLocal()->Entity->origin.y;
		m_Player->GetLocal()->FreeLookOffset[2] = m_Player->GetLocal()->Entity->origin.z + 80.0f;

		VectorCopy(m_Player->GetLocal()->FreeLookAngles, cmd->viewangles);

		m_bFreeLook = true;
	}

	Vector curMove(0.0f, 0.0f, 0.0f);

	Vector vr = m_Player->GetLocal()->Right;
	Vector vf = m_Player->GetLocal()->Forward;

	if (cmd->buttons & IN_MOVELEFT)curMove = curMove + vr * -1.0f;
	if (cmd->buttons & IN_MOVERIGHT)curMove = curMove + vr * 1.0f;
	if (cmd->buttons & IN_FORWARD)curMove = curMove + vf * 1.0f;
	if (cmd->buttons & IN_BACK)curMove = curMove + vf * -1.0f;

	curMove = curMove * (/*cvar.misc_FreeLookSpeed*/700 * frametime);

	m_Player->GetLocal()->FreeLookOffset[0] += curMove.x;
	m_Player->GetLocal()->FreeLookOffset[1] += curMove.y;
	m_Player->GetLocal()->FreeLookOffset[2] += curMove.z;

	cmd->buttons = 0;
	cmd->sidemove = 0;
	cmd->forwardmove = 0;

}

void GameBasics::PlayerCard()
{

	//加速地址 + 3 = 卡空地址
	*(PBYTE)(g_Offset.GetGameAddresInfo()->dwSpeed[2] + 3) = 1;
}

void GameBasics::NightVision()
{

	static	ULONG NightBase = m_Memory->Read<ULONG>(g_Offset.GetGameAddresInfo()->dwNightVision + 2);

	if (g_Vars.m_Basi.sk_basics_nightvision->value)	//开启夜视仪
	{
		m_NightVision = TRUE;

		memset((VOID*)NightBase, 1, 4);

		if (g_Vars.m_Basi.sk_basics_nightvision->flags)
		{
			//永远为白色夜视仪
			RtlZeroMemory((PVOID)g_Offset.GetGameAddresInfo()->dwNightVisionColor, 4);
		}
	}
	else if (m_NightVision)
	{
		m_NightVision = !m_NightVision;

		memset((VOID*)NightBase, 0, 4);
	}
}

void GameBasics::MapLight()
{
	/*
		97 = 最黑
		默认= 115
		最高= 130
		团队模式 = 109
	*/
	memset((VOID*)(g_Offset.GetGameAddresInfo()->dwMapBrightness), 115, 4);
}

void GameBasics::BoxShowHp()
{
	if (g_Vars.m_Basi.sk_basics_healthmode->value )
	{
		if (g_Vars.m_Basi.sk_basics_healthmode->flags & (1 << 0))
		{
			DrawHealth(300, 230, 200, 30);
		}

		//全图显血好像没必要.
		//AllDrawHealth();

		//游戏显血
		if (g_Vars.m_Basi.sk_basics_healthmode->flags & (1 << 2))
		{
			DrawHPGaugeInfo();

		}

	}
}


void GameBasics::DrawHealth(INT x, INT y, INT width, INT height)
{

	DWORD centerx = width / 2;
	DWORD centery = height / 2;
	g_Drawing.FillArea(x - centerx, y - centery, width, 2, 255, 255, 255, 200);
	g_Drawing.FillArea(x - centerx, y - centery, 2, height, 255, 255, 255, 200);
	g_Drawing.FillArea(x - centerx, y - centery + height, width, 2, 255, 255, 255, 200);
	g_Drawing.FillArea(x - centerx + width, y - centery, 2, height + 1, 255, 255, 255, 200);

	BYTE colour[3] = { 255,0,0 };

	int  Health = g_Vars.m_Basi.HeadlthArray[g_Vars.m_Basi.m_CurActivateHeadlthPlayer].Health;

	BOOL IsEnemy = !m_Player->GetTeamRelationship(g_Vars.m_Basi.m_CurActivateHeadlthPlayer);


	if (IsEnemy && Health > 70)	//绿色
	{
		colour[0] = 0;
		colour[1] = 220;
		colour[2] = 0;
	}
	else if (IsEnemy && Health < 70 && Health > 30)	//黄色 
	{
		colour[0] = 255;
		colour[1] = 255;
		colour[2] = 0;
	}
	else if (IsEnemy && Health < 30)	//红色
	{
		colour[0] = 255;
		colour[1] = 0;
		colour[2] = 0;
	}
	else
	{
		// 友军一直是绿色
		colour[0] = 0;
		colour[1] = 220;
		colour[2] = 0;
	}

	DWORD DrawHealthlength = Health * 2 - 2; if (DrawHealthlength > 198)DrawHealthlength = 198;

	if (Health)g_Drawing.FillArea(x - 98, y - 13, DrawHealthlength, height - 2, colour[0], colour[1], colour[2], 150);

	//字体颜色设置
	BYTE font_colour[3] = { 255, 255, 255 };
	if (!IsEnemy)
	{
		font_colour[0] = 0;
		font_colour[1] = 220;
		font_colour[2] = 0;
	}

	//显示 血量 与 阵营信息
	g_Font.FmtPrint(x - 100, y + 5, font_colour[0], font_colour[1], font_colour[2], 255, FL_CENTER, xorstr_("{} Hp:{}"), IsEnemy ? xorstr_("Enemy") : xorstr_("Friend"), Health);

}

void GameBasics::DrawHealth()
{
	if (g_Vars.m_Basi.m_CurActivateHeadlthPlayer == 0 )
		return;

	Vector  Screen;

	int i = g_Vars.m_Basi.m_CurActivateHeadlthPlayer;


	//阵营
	if (m_Player->GetTeamRelationship(i) == 1)
		return;
	//休眠状态
	if (m_Player->GetPlayer(i)->Dormant)
		return;
	//活死 或 ID = 自己
	if (!m_Player->GetPlayer(i)->Valid || m_Player->GetLocal()->Player.Index == i) 
		return;
	//默认就是头部
	auto Status = m_Player->GetScreenXy(i, m_Player->GetBonePartIndex(i, HITBOX_TYPE::HEAD).at(0));
	if (std::get<bool>(Status) == false)
		return;

	Screen = std::get<Vector>(Status);

	BYTE colour[3] = { 255,255,0 };
	// 0 就是当前的
	int  Health = g_Vars.m_Basi.HeadlthArray[0].Health;


	DWORD DrawHealthlength = Health;
	if (DrawHealthlength > 100) DrawHealthlength = 100;
	if (DrawHealthlength <= 0) DrawHealthlength = 0;

	g_Drawing.FillArea(Screen.x - 50, Screen.y - 28, 100, 10, 255, 0, 0, 255);
	g_Drawing.FillArea(Screen.x - 50, Screen.y - 28, DrawHealthlength, 10, colour[0], colour[1], colour[2], 255);}



void GameBasics::DrawHPGaugeInfo()
{

	m_HPGaugeInfoPack.Clear();
	m_HPGaugeInfoPack.WriteByte(g_Vars.m_Basi.m_CurActivateHeadlthPlayer);

	m_HPGaugeInfoPack.WriteInt(0x40000000);
	m_HPGaugeInfoPack.WriteByte(g_Vars.m_Basi.HeadlthArray[0].Health);
	m_HPGaugeInfoPack.WriteByte(1);

	m_HPGaugeInfo(xorstr_("HPGaugeInfo"), m_HPGaugeInfoPack.GetBytesLength(), m_HPGaugeInfoPack.GetBytes());

}

pfnUserMsgHook GameBasics::GetpfnUserMsg(const char* Msg)
{
	PUserMsg Ptr = (PUserMsg) * (PDWORD)(Engine::g_pUserMsgBase);

	while (Ptr->next)
	{
		if (!lstrcmp(Ptr->name, Msg))
		{

			return Ptr->pfn;
		}

		Ptr = Ptr->next;
	}

	return nullptr;
}

void GameBasics::AllDrawHealth()
{
	for (int i = 1; i <= Engine::g_Engine.GetMaxClients(); i++)
	{
		Vector  Screen;

		//阵营
		if (m_Player->GetTeamRelationship(i) == 1)
			continue;
		//休眠状态
		if (m_Player->GetPlayer(i)->Dormant)
			continue;
		//活死 或 ID = 自己
		if (!m_Player->GetPlayer(i)->Valid || m_Player->GetLocal()->Player.Index == i) continue;


		auto Status =  m_Player->GetScreenXy(i,7);
		if (std::get<bool>(Status) == false)
			continue;

		Screen = std::get<Vector>(Status);

		BYTE colour[3] = { 255,255,0 };

		int  Health = g_Vars.m_Basi.HeadlthArray[i].Health;


		DWORD DrawHealthlength = Health;
		if (DrawHealthlength > 100) DrawHealthlength = 100;
		if (DrawHealthlength <= 0) DrawHealthlength = 0;


		g_Drawing.FillArea(Screen.x- 50, Screen.y - 28, 100, 10, 255, 0, 0, 255);


		g_Drawing.FillArea(Screen.x - 50, Screen.y - 28, DrawHealthlength, 10, colour[0], colour[1], colour[2], 255);

		//g_Font.Print(int(ScreenTop[0]) - 50, int(PlayerBoxHeight + ScreenTop[1]) - 38, colour[0], colour[1], colour[2], 255, FL_CENTER, "PlayerBoxHeight %f", PlayerBoxHeight);
	}
}
