#include "GameOffset.h"
GameOffset g_Offset;

GameOffset::GameOffset()
{
	m_Hook   = std::make_unique<CACEHook>();
	m_Memory = std::make_unique<CACEMemory>();

}

BOOL  GameOffset::GetModuleInfo()
{
	this->m_pEngine = new GAME_MODULE_INFO(xorstr_("hw.dll"));
	this->m_pClient = new GAME_MODULE_INFO(xorstr_("client.dll"));
	this->m_pGameUi = new GAME_MODULE_INFO(xorstr_("GameUI.dll"));
	this->m_pServer = new GAME_MODULE_INFO(xorstr_("mp.dll"));
	this->m_pVgui2 =  new GAME_MODULE_INFO(xorstr_("vgui2.dll"));


	ACEDebugLog("[{}]m_pEngine hModule {:X} ulModuleSize {:X} ulModuleEnd {:X}", __FUNCTION__, (ULONG)m_pEngine->hModule, m_pEngine->ulModuleSize, m_pEngine->ulModuleEnd);
	ACEDebugLog("[{}]m_pClient hModule {:X} ulModuleSize {:X} ulModuleEnd {:X}", __FUNCTION__, (ULONG)m_pClient->hModule, m_pClient->ulModuleSize, m_pClient->ulModuleEnd);
	ACEDebugLog("[{}]m_pGameUi hModule {:X} ulModuleSize {:X} ulModuleEnd {:X}", __FUNCTION__, (ULONG)m_pGameUi->hModule, m_pGameUi->ulModuleSize, m_pGameUi->ulModuleEnd);
	ACEDebugLog("[{}]m_pServer hModule {:X} ulModuleSize {:X} ulModuleEnd {:X}", __FUNCTION__, (ULONG)m_pServer->hModule, m_pServer->ulModuleSize, m_pServer->ulModuleEnd);
	ACEDebugLog("[{}]m_pVgui2  hModule {:X} ulModuleSize {:X} ulModuleEnd {:X}", __FUNCTION__, (ULONG)m_pVgui2->hModule, m_pVgui2->ulModuleSize, m_pVgui2->ulModuleEnd);

	
	return m_pEngine && m_pClient && m_pGameUi && m_pServer && m_pVgui2;
}

BOOL GameOffset::FindClientTable()
{
	DWORD ClientTablePtr = 0;

	if (m_Memory->GetScanCode(xorstr_("6A 07 68 ?? ?? ?? ?? FF 15 ?? ?? ?? ??"), ClientTablePtr, m_pEngine->hModule))
	{
		ClientTablePtr = *(PDWORD)(ClientTablePtr + 0x09);
	}
	Engine::g_pClient = (cl_clientfunc_t*)ClientTablePtr;

	RtlCopyMemory(&Engine::g_Client, Engine::g_pClient, sizeof(cl_clientfunc_t));


	ACEDebugLog("[{}]->ClientTablePtr {:X} Initialize {:X}", __FUNCTION__, ClientTablePtr, (ULONG)Engine::g_pClient->Initialize);

	return Engine::g_Client.Initialize != nullptr;
}

BOOL GameOffset::FindEngineTable()
{
	DWORD dwOffset = /*g_Vars.m_GameServer == Chinese ? 0x1E : */0x34;


	DWORD dwInitialize = (DWORD)m_Hook->GetProcAddress(m_pClient->hModule, xorstr_("Initialize"));

	
	PVOID EnginePtr = (cl_enginefunc_t*)*(PDWORD)((DWORD)dwInitialize + dwOffset);

	ACEDebugLog("[{}]->EnginePtr {:X} ", __FUNCTION__, (ULONG)EnginePtr);


	Engine::g_pEngine = (cl_enginefunc_t*)EnginePtr;

	// 到这里是 地址还没填充.时机不对.要到Initialize那里Hook
	//RtlCopyMemory(&Engine::g_Engine, Engine::g_pEngine, sizeof(cl_enginefunc_t));

	return /*Engine::g_Engine.V_CalcShake*/EnginePtr != nullptr;
}


void* GameOffset::FindStudioTable()
{
	DWORD dwOffSet = 0x34;

	DWORD StudioTablePtr = *(DWORD*)((DWORD)Engine::g_Client.HUD_GetStudioModelInterface + dwOffSet);

	ACEDebugLog("[{}]->StudioTablePtr {:X} ", __FUNCTION__, StudioTablePtr);

	Engine::g_pStudio = (engine_studio_api_t*)StudioTablePtr;
	RtlCopyMemory(&Engine::g_Studio, Engine::g_pStudio, sizeof(engine_studio_api_t));

	return Engine::g_Studio.StudioDrawPoints;
}
BOOL GameOffset::FindPlayermove()
{
	DWORD dwPlayerMove = 0;
	if (m_Memory->GetScanCode(xorstr_("83 C4 ?? 85 C0 74 0A 68 ?? ?? ?? ?? FF D0 83 C4 04 E8"), dwPlayerMove, m_pEngine->hModule))
	{
		Engine::g_pPlayerMove = (playermove_t*)(*(PDWORD)(dwPlayerMove + 0x08));
	}
	ACEDebugLog("[{}]->PlayerMovePtr {:X} ", __FUNCTION__, (ULONG)Engine::g_pPlayerMove);

	/*	韩服 特征码 2019年7月8日 19:11:49
		6A 07 68 ?? ?? ?? ?? FF 15 ?? ?? ?? ?? 68 ?? ?? ?? ?? E8 ?? ?? ?? ?? 83 C4 2C
	.text:374315FD 6A 07                                                           push    7               ; _DWORD
	.text:374315FF 68 C8 09 8F 37                                                  push    offset off_378F09C8 ; _DWORD
	.text:37431604 FF 15 60 3C DB 39                                               call    dword_39DB3C60
	.text:3743160A 68 A0 E5 61 39                                                  push    offset unk_3961E5A0
	.text:3743160F E8 FC FB FF FF                                                  call    sub_37431210
	.text:37431614 83 C4 2C                                                        add     esp, 2Ch
	85 C0                 - test eax,eax
	74 0A                 - je hw.pplx::details::windows_scheduler::schedule+3629C4
	68 E019973A           - push hw.dll+38719E0		<-
	FF D0                 - call eax
	83 C4 04              - add esp,04
	E8 E7F70000           - call hw.pplx::details::windows_scheduler::schedule+3721B0

	*/
	return Engine::g_pPlayerMove->Con_DPrintf != NULL;
}
void* GameOffset::AdSpeedPtr()
{
	//PCHAR String = "Texture load: %6.1fms";

	//DWORD Address = (DWORD)FindMemoryClone(m_pEngine->hModule, m_pEngine->hModule + m_pEngine->ulModuleSize, String, strlen(String));

	//PVOID pSpeedPtr = (PVOID)*(DWORD*)(FindReference(m_pEngine->hModule, m_pEngine->hModule + m_pEngine->ulModuleSize, Address) - 9);

	DWORD Address;
	if (m_Memory->GetScanCode(xorstr_("F2 0F 59 05 ?? ?? ?? ?? F2 0F 11 04 24 68"), Address, m_pEngine->hModule))
	{
		Address = m_Memory->Read<DWORD>((Address + 4));

		DWORD OldProtect;
		VirtualProtect((PVOID)Address, 1, PAGE_READWRITE, &OldProtect);
	}

	return PVOID (Address);
}
void* GameOffset::SpeedHackPtr()
{
	DWORD Address   = 0;
	PVOID pSpeedPtr = NULL;
	if (m_Memory->GetScanCode(xorstr_("CC F2 0F 10 05 ?? ?? ?? ?? 66 0F 5A C0"), Address,m_pEngine->hModule))
	{
		pSpeedPtr = (PVOID)*(PDWORD)(Address + 5);
	}
	SpeedPtr = (DWORD)pSpeedPtr;
	return pSpeedPtr;
}
BOOL GameOffset::HUD_Frame()
{
	g_Offset.SpeedPtr = (DWORD)g_Offset.SpeedHackPtr();


	ACEDebugLog("[{}]g_Net {:X}", __FUNCTION__, (DWORD)Engine::g_Engine.pNetAPI->SendRequest + 0x73);
	ACEDebugLog("[{}]SpeedPtr{:X}", __FUNCTION__, g_Offset.SpeedPtr);

	return TRUE;
}

GameServer GameOffset::SetGameServer(GameServer sv)
{
	return m_GameServer = sv;
}

BOOL GameOffset::CheckModuleAddres(PGAME_MODULE_INFO pModuleInfo, ULONG Address)
{
	return Address > (DWORD)pModuleInfo->hModule && Address <= (DWORD)pModuleInfo->ulModuleEnd;
}



BOOL GameOffset::FindGameAddres()
{
	VMProtectBegin(__FUNCTION__);

	m_GameAddres.dwAdSpeed= (ULONG)AdSpeedPtr();


	if (m_Memory->GetScanCode(xorstr_("8B 0D ?? ?? ?? ?? 53 89 B5 EC FE FF FF E8 ?? ?? ?? ??"), m_GameAddres.dwPlayerMgrCall, m_pEngine->hModule))
	{
		/*
		37626072    8B0D 94278F38   mov ecx,dword ptr ds:[0x388F2794]
		37626078    53              push ebx
		37626079    89B5 ECFEFFFF   mov dword ptr ss:[ebp-0x114],esi
		3762607F    E8 3CB40600     call hw.376914C0                         ; 获取他的

		查找Kick  Index = 3
		376260EB    68 14E08C37     push hw.378CE014                         ; #CSO_VoteKick_Proposed
		*/
		m_GameAddres.dwPlayerMgrAddres = m_Memory->Read<DWORD>(m_GameAddres.dwPlayerMgrCall + 2);
		//找到下一个CALL 就是
		auto result =  m_Hook->GetScanOpCodeByAddress(m_GameAddres.dwPlayerMgrCall,0x20,1,ACE_SCAN_OPCODE_TYPE_CALL);

		if (std::get<0>(result))
		{
			m_GameAddres.dwPlayerMgrCall = std::get<4>(result);
		}
	}

	
	for (ULONG Section = 0,Offset = 0, Count = 0; m_Memory->GetScanCodeAddress(xorstr_("CC A1 ?? ?? ?? ?? C3 CC"), m_GameAddres.dwPlayerSequence, m_pClient->hModule, Offset, Section); Count++, Offset++)
	{
		if (Count == 4)
		{
			m_GameAddres.dwPlayerSequence = m_Memory->Read<DWORD>(m_GameAddres.dwPlayerSequence + 2);

			break;
		}
		/*
			获取人物动作序列_ 主要判断圣剑盾反状态
			随便找个人物_开枪或不开枪
		*/

	}

	if (m_Memory->GetScanCode(xorstr_("89 9F ?? ?? ?? ??  FF 15 ?? ?? ?? ??"), m_GameAddres.dwRandCallRetAddres, m_pEngine->hModule))
	{
		m_GameAddres.dwRandCallRetAddres += m_Hook->GetDisasmLenght(m_GameAddres.dwRandCallRetAddres,2);

		/*
			写入地图  下断Rand
		*/

	}

	// 视角
	if (m_Memory->GetScanCode(xorstr_("56 57 8B F2 BF ?? ?? ?? ?? F3 A5"), m_GameAddres.dwAngletr, m_pEngine->hModule))
	{
		m_GameAddres.dwAngletr = m_Memory->Read<DWORD>(m_GameAddres.dwAngletr + 5) + 0xC;
	}
	else
	{
		//韩服
		if (m_Memory->GetScanCode(xorstr_("F3 A5 68 ?? ?? ?? ?? E8 ?? ?? ?? ?? 8b 83"), m_GameAddres.dwAngletr, m_pEngine->hModule))
		{
			m_GameAddres.dwAngletr = m_Memory->Read<DWORD>(m_GameAddres.dwAngletr + 3)+ 0x10;
		}
	}
	// FOV
	if (m_Memory->GetScanCode(xorstr_("F3 0F 10 05 ?? ?? ?? ?? F3 0F 10 0D ?? ?? ?? ?? 0F 2F C1 C7 05"), m_GameAddres.dwCameraFOV, m_pEngine->hModule))
	{
		m_GameAddres.dwCameraFOV = m_Memory->Read<DWORD>(m_GameAddres.dwCameraFOV + 12);
	}
	//自己的骨骼点
	if (m_Memory->GetScanCode(xorstr_("50 51 c7 04 24 ?? ?? ?? ?? 50 E8 ?? ?? ?? ?? F3 0F 10 05 ?? ?? ?? ??"), m_GameAddres.dwMyBone, m_pEngine->hModule))
	{
		m_GameAddres.dwMyBone = m_Memory->Read<DWORD>(m_GameAddres.dwMyBone + 19);
	}
	if (m_Memory->GetScanCode(xorstr_("CC 55 8B EC 8B 45 08 83 C0 FF 56 78 54 3B 05 ?? ?? ?? ?? 7D 4C"), m_GameAddres.dwValid, m_pEngine->hModule))
	{
		m_GameAddres.dwValid = m_Memory->Read<DWORD>(m_GameAddres.dwValid + 15)+ 0x24 + 4;
	}
	else
	{
		//steam
		if (m_Memory->GetScanCode(xorstr_("CC 55 8B EC 8B 45 08 83 C0 FF 56 78 51 3B 05 ?? ?? ?? ?? 7D 49"), m_GameAddres.dwValid, m_pEngine->hModule))
		{
			m_GameAddres.dwValid = m_Memory->Read<DWORD>(m_GameAddres.dwValid + 15)+ 0x24 + 4;
		}
	}
	m_GameAddres.dwDormant = m_GameAddres.dwValid + 0x8;
	//是否在游戏中
	if (m_Memory->GetScanCode(xorstr_("83 C4 10 89 04 FD ?? ?? ?? ?? 5f 5E C3"), m_GameAddres.dwIsGame, m_pEngine->hModule))
	{
		m_GameAddres.dwIsGame = m_Memory->Read<DWORD>(m_GameAddres.dwIsGame + 6);
	}
	//混房功能
	if (m_Memory->GetScanCode(xorstr_("68 ?? ?? ?? ?? E8 ?? ?? ?? ?? 68 ?? ?? ?? ?? 6A 01 E8"), m_GameAddres.dwMixRoom, m_pEngine->hModule))
	{
		//m_GameAddres.dwMixRoom = *(PDWORD)(m_GameAddres.dwMixRoom + 0x0B);
		//m_GameAddres.dwMixRoom = m_GameAddres.dwMixRoom + 0x4F;
		//DWORD OldProtect;
		//VirtualProtect((PVOID)m_GameAddres.dwMixRoom, 64, 64, &OldProtect);
	}
	//夜视功能
	if (m_Memory->GetScanCode(xorstr_("83 3D ?? ?? ?? ?? 00 74 08 FF 35 ?? ?? ?? ??"), m_GameAddres.dwNightVision, m_pEngine->hModule))
	{

		//查找 夜视仪颜色设置
		m_GameAddres.dwNightVisionColor = m_GameAddres.dwNightVision + m_Hook->GetDisasmLenght(m_GameAddres.dwNightVision, 2);

		m_GameAddres.dwNightVisionColor = m_Memory->Read<DWORD>(m_GameAddres.dwNightVisionColor + 2);

		/*
		83 3D 7C442138 00     - cmp dword ptr [hw.AmdPowerXpressRequestHighPerformance+65C690],00
		74 08                 - je hw.pplx::details::windows_scheduler::schedule+3BC924
		FF 35 E882A339        - push [hw.dll+29382E8]	//夜视颜色
		EB 0A                 - jmp hw.pplx::details::windows_scheduler::schedule+3BC92E
		*/
	}
	//第三人称
	ULONG ulCL_IsThirdPerson = (ULONG)GetProcAddress(m_pClient->hModule, xorstr_("CL_IsThirdPerson"));
	ulCL_IsThirdPerson += m_Hook->GetDisasmLenght(ulCL_IsThirdPerson,6);
	m_GameAddres.dwThirdPerson = m_Memory->Read<DWORD>(ulCL_IsThirdPerson + 2);
	//push ebp
	//mov ebp, esp
	//push ecx
	//mov eax, [21B1EFB4] 
	//mov eax, [eax + 3C]
	//call eax
	//cmp dword ptr[21D937A8], 00
	// 
	//获得MSG弹窗
	//if (m_Memory->GetScanCode(xorstr_("8b 0d ?? ?? ?? ?? 6A FF 68 ?? ?? ?? ?? 68 ?? ?? ?? ?? 8b 01"), m_GameAddres.dwMsg, m_pEngine->hModule))
	//{
	//	m_GameAddres.dwMsg = m_Memory->Read<DWORD>(m_GameAddres.dwMsg + 2);
	//}
	//else
	{

		/*
		*
		8B 0D E0840009        - mov ecx,[hw.AmdPowerXpressRequestHighPerformance+D7204]
		8D 55 FC              - lea edx,[ebp-04]
		6A FF                 - push -01
		68 C486D808           - push hw.vgui::Frame::operator=+38A0C4
		68 D886D808           - push hw.vgui::Frame::operator=+38A0D8
		8B 01                 - mov eax,[ecx]
		52                    - push edx
		FF 90 AC000000        - call dword ptr [eax+000000AC]
		E8 CDFC1F00           - call hw.vgui::Frame::operator=+D6420

		*/
		//  韩服 新加了一个参数 [12/17/2021 007]
		//8b 0d ?? ?? ?? ?? 8D 55 FC 6A FF 68 ?? ?? ?? ?? 68 ?? ?? ?? ?? 8b 01

		if (m_Memory->GetScanCode(xorstr_("8b 0d ?? ?? ?? ?? 8D 55 FC 6A FF 68 ?? ?? ?? ?? 68 ?? ?? ?? ?? 8b 01"), m_GameAddres.dwMsg, m_pEngine->hModule))
		{
			m_GameAddres.dwMsg = m_Memory->Read<DWORD>(m_GameAddres.dwMsg + 2);
		}

	}


	/*
	2ECF30B0    55              push ebp
	2ECF30B1    8BEC            mov ebp,esp
	2ECF30B3    A1 70F2402F     mov eax,dword ptr ds:[0x2F40F270]
	2ECF30B8    8B80 B0000000   mov eax,dword ptr ds:[eax+0xB0]
	2ECF30BE    5D              pop ebp
	2ECF30BF  - FFE0            jmp eax

	*/
	//人物射线
	if (m_Memory->GetScanCode(xorstr_("A1 ?? ?? ?? ?? 8B 80 B0 00 00 00"), m_GameAddres.dwBeamPoints, m_pClient->hModule))
	{
		m_GameAddres.dwBeamPoints = m_Memory->Read<DWORD>(m_GameAddres.dwBeamPoints + 1);
	}
	//屏蔽菜单
	if (m_Memory->GetScanCode(xorstr_("E9 ?? ?? ?? ?? 8B 0D ?? ?? ?? ?? 8B 01 FF 50 24"), m_GameAddres.dwGuiMenu, m_pClient->hModule))
	{
		m_GameAddres.dwGuiMenu = m_Memory->Read<DWORD>(m_GameAddres.dwGuiMenu + 0x7)+ 0x18;

	}
	//挂机.没用
	if (m_Memory->GetScanCode(xorstr_("66 0F 2F D0 76 7B"), m_GameAddres.dwNoKeyInput, m_pClient->hModule))
	{
		//m_GameAddres.dwNoKeyInput += 4;
		//DWORD OldProtect;
		//VirtualProtect((PVOID)m_GameAddres.dwNoKeyInput, 64, 64, &OldProtect);
	}	
	//防踢挂机.收益减少.
	if (m_Memory->GetScanCode(xorstr_("E8 ?? ?? ?? ?? 80 3D ?? ?? ?? ?? 00 0F 85 ?? ?? ?? ?? F3 0F 10 4D C8"), m_GameAddres.dwAFK, m_pClient->hModule))
	{
		m_GameAddres.dwAFK += m_Hook->GetDisasmLenght(m_GameAddres.dwAFK);

		m_GameAddres.dwAFK = m_Memory->Read<ULONG>(m_GameAddres.dwAFK+2);
	}
	//联赛挂机.vgui.没用收益减少
	if (m_Memory->GetScanCode(xorstr_("0F B6 DB 83 F8 26 0F 44 D9 E8 ?? ?? ?? ?? 8B C8"), m_GameAddres.dwGameVguiIpunt, m_pClient->hModule))
	{
		//找到第一个CALL
		/*
		  8B C8                 - mov ecx,eax
		  8B 10                 - mov edx,[eax]
		  8B 42 44              - mov eax,[edx+44]
		  FF D0                 - call eax

		*/
		if (auto result = m_Hook->GetScanOpCodeByAddress(m_GameAddres.dwGameVguiIpunt, 0x50, 1, ACE_SCAN_OPCODE_TYPE_CALL);std::get<0>(result))
		{
			m_GameAddres.dwGameVguiMouseIpunt = std::get<4>(result);

		}
		//找到第三个CALL
		/*	
			0F5A C0               - cvtps2pd xmm0,xmm0
			DD 5D C0              - fstp qword ptr [ebp-40]
			F2 0F5F 45 C0         - maxsd xmm3,[ebp-40]
			F2 0F11 45 C0         - movsd [ebp-40],xmm0
			E8 2A931900           - call client.dll+862AF0

		*/
		if (auto result = m_Hook->GetScanOpCodeByAddress(m_GameAddres.dwGameVguiIpunt, 0x50, 3, ACE_SCAN_OPCODE_TYPE_CALL); std::get<0>(result))
		{
			m_GameAddres.dwGameVguiIpuntTime = std::get<4>(result);

		}

	}
	//房间模式
	if (m_Memory->GetScanCode(xorstr_("83 F8 01 A0 ?? ?? ?? ??"), m_GameAddres.dwRoomMode, m_pEngine->hModule))
	{
		m_GameAddres.dwRoomMode = m_Memory->Read<ULONG>(m_GameAddres.dwRoomMode + 4);

		/*
			short	=	Mode
			ULong	=	Map
			Ulong	=	MapPic
		*/

	}
	//
	if (m_Memory->GetScanCode(xorstr_("56 8B F1 57 83 7E 38 00 75 14"), m_GameFuns.ItemIdToItemCode, m_pEngine->hModule))
	{
			//物品ID->物品代码

			/*
				RelationProductRecord Class
				text "UTF-16LE", '(%d) has not CSO_Item_Name_',0
			*/
	}
	else
	{
		//if (g_Vars.m_GameServer == Steam)
		{
			if (m_Memory->GetScanCode(xorstr_("56 8B F1 57 83 7E 34 00 75 14"), m_GameFuns.ItemIdToItemCode, m_pEngine->hModule))
			{

			}
		}
	}



	if (m_Memory->GetScanCode(xorstr_("C6 45 FC 07 E8 ?? ?? ?? ?? 56"), m_GameFuns.GetItemEntryCall, m_pEngine->hModule))
	{
		//获取物品ID call
		m_GameFuns.GetItemEntryCall = m_Hook->GetCallAddress(m_GameFuns.GetItemEntryCall + 4);

		/*
			特征码
			Resource/relation_product_ver2.csv
		*/


	}

	for (ULONG Offset = 0; m_Memory->GetScanCodeAddress(xorstr_("50 68 ?? ?? ?? ?? E8 ?? ?? ?? ?? 83 C4 0C 85"), m_GameFuns.ItemCodeToItemName, m_pEngine->hModule, Offset); Offset++)
	{
		/*
		3760576B - 50                    - push eax
		3760576C - 68 C4F38037           - push 3780F3C4 : ["CSO_Item_Name_"]
		37605771 - E8 4A8EF4FF           - call 3754E5C0
		37605776 - 83 C4 0C              - add esp,0C
		37605779 - 85 C0                 - test eax,eax
		*/
		char* StrAddres = (char*)*(PULONG)((ULONG)m_GameFuns.ItemCodeToItemName + 2);

		if (CheckModuleAddres(m_pEngine, (ULONG)StrAddres))
		{
			if (StrAddres[0] == '\0')
				continue;

			if (lstrcmpA(StrAddres, xorstr_("CSO_Item_Desc_")) == 0)
			{
				m_GameFuns.ItemCodeToItemName = m_Hook->GetCallAddress(m_GameFuns.ItemCodeToItemName + 6);
				break;
			}
		}
	}



	//游戏对OpenGL导出函数进行检查.但是并没有上报服务器
	if (m_Memory->GetScanCode(xorstr_("81 F 14 10 19 12 51 E8"), m_GameAddres.dwCheckOpengl, m_pEngine->hModule))
	{
		m_GameAddres.dwCheckOpengl += 7;
	}
	if (m_Memory->GetScanCode(xorstr_("B8 50 47 00 00 E8 ?? ?? ?? ??"), m_GameAddres.dwCheckProcess, m_pEngine->hModule))
	{
		/*	DWORD OldProtect;
		VirtualProtect((PVOID)m_GameAddres.dwCheckProcess, 64, 64, &OldProtect);*/
		//检查进程.不过并没有上报服务器
	}
	//地图高亮
	if (m_Memory->GetScanCode(xorstr_("0F BE 84 32 ?? ?? ?? ??"), m_GameAddres.dwMapBrightness, m_pEngine->hModule))
	{
		//2019年7月23日 14:23:54
		m_GameAddres.dwMapBrightness = m_Memory->Read<ULONG>(m_GameAddres.dwMapBrightness + 4);

		/*
		37436086
		37436083 - 99                    - cdq
		37436084 - F7 FF                 - idiv edi
		37436086 - 0FBE 84 32 442E6B39   - movsx eax,byte ptr [edx+esi+396B2E44]
		3743608E - 83 E8 61              - sub eax,61
		37436091 - 6B C0 16              - imul eax,eax,16
		37436094 - 89 01                 - mov [ecx],eax

		99 F7 FF 0F BE 84 32 ?? ?? ?? ??
		*/
	}
	// 拦截控制台命令
	if (m_Memory->GetScanCode(xorstr_("C3 50 E8 ?? ?? ?? ?? 68 ?? ?? ?? ?? E8 ?? ?? ?? ?? 83 C4 08 B8 01"), m_GameAddres.dwCommandHook, m_pEngine->hModule))
	{
		m_GameAddres.dwCommandHook += m_Hook->GetDisasmLenght(m_GameAddres.dwCommandHook, 2);
		m_GameAddres.dwCommandHook = m_Hook->GetCallAddress(m_GameAddres.dwCommandHook);
	}

	if (m_Memory->GetScanCode(xorstr_("50 8D 87 ?? ?? ?? ?? 50 8D 04 52"), m_GameAddres.dwItemPoint, m_pEngine->hModule))
	{
		/*

			37555952 - 8B 80 308F7D37        - mov eax,[eax+377D8F30]
			37555958 - 89 45 08              - mov [ebp+08],eax
			8B 80 ?? ?? ?? ?? 89 45 08 8D 45 08 50

			//2020-08-21 10:26:11 更新后的特征码
			50                    - push eax
			8D 87 A8327E08        - lea eax,[edi+hw.AmdPowerXpressRequestHighPerformance+338C]
			50                    - push eax
			8D 04 52              - lea eax,[edx+edx*2]
			C1 E0 04              - shl eax,04
			03 05 F0B96E09        - add eax,[hw.AmdPowerXpressRequestHighPerformance+F0BAD4]
			50                    - push eax
			E8 0D552300           - call hw.vgui::Frame::operator=+274D0A


	*/
		m_GameAddres.dwItemPoint = m_Memory->Read<ULONG>(m_GameAddres.dwItemPoint + 3);

	}
	else
	{
		if (m_Memory->GetScanCode(xorstr_("BF ?? ?? ?? ?? 8B 77 04 85 F6"), m_GameAddres.dwItemPoint, m_pEngine->hModule))
		{

			m_GameAddres.dwItemPoint = m_Memory->Read<ULONG>(m_GameAddres.dwItemPoint + 1);
			
			/*
				.rdata:377FBEB8 ; int dword_377FBEB8[]
			.rdata:377FBEB8 dword_377FBEB8  dd 1                    ; DATA XREF: sub_37535EE0+20↑o
			.rdata:377FBEBC                                         ; sub_375385A0+D1↑r
			.rdata:377FBEBC                                         ; "p228"
			.rdata:377FBEC0                 dd offset aP228         ; "p228"
			.rdata:377FBEC4                 dd offset aP228         ; "p228"
			.rdata:377FBEC8                 dd offset aCsoP228      ; "#CSO_P228"

			*	原理还是找 memcpy 
			* .text:023217DE 50											 push    eax             ; Size
			.text:023217DF 8B 45 E8                                      mov     eax, [ebp+var_18]
			.text:023217E2 8D 80 F0 01 7F 02                             lea     eax, unk_27F01F0[eax]		数组 377FBEB8
			.text:023217E8 50                                            push    eax             ; Src
			.text:023217E9 8D 04 52                                      lea     eax, [edx+edx*2]
			.text:023217EC C1 E0 04                                      shl     eax, 4
			.text:023217EF 03 05 20 C2 70 03                             add     eax, dword_370C220
			.text:023217F5 50                                            push    eax             ; Dst
			.text:023217F6 E8 57 35 25 00                                call    memcpy
				*/
				
		}
		else
		{
			if (m_Memory->GetScanCode(xorstr_("50 8B 45 E8 8D 80 ?? ?? ?? ?? 50 8D 04 52"), m_GameAddres.dwItemPoint, m_pEngine->hModule))
			{
				m_GameAddres.dwItemPoint = m_Memory->Read<ULONG>(m_GameAddres.dwItemPoint + 6);
			}
			else if (m_Memory->GetScanCode(xorstr_("68 ?? ?? ?? ?? FF 35 ?? ?? ?? ?? 89 7D E8"), m_GameAddres.dwItemPoint, m_pEngine->hModule))
			{
				/*
				* 2022-05-19 19:00:44 韩服更新后特征码
				.text:02343BA3 E8 48 AD FF FF                                                  call    sub_233E8F0
				.text:02343BA8 8D 3C 7F                                                        lea     edi, [edi+edi*2]
				.text:02343BAB C1 E7 04                                                        shl     edi, 4
				.text:02343BAE 57                                                              push    edi             ; Size
				.text:02343BAF 68 E0 FE 84 02                                                  push    offset unk_284FEE0 ; Src
				.text:02343BB4 FF 35 68 21 77 03                                               push    dword_3772168   ; Dst
				.text:02343BBA 89 7D E8                                                        mov     [ebp+var_18], edi
				.text:02343BBD E8 B9 61 27 00                                                  call    memcpy
				*/

				m_GameAddres.dwItemPoint = m_Memory->Read<ULONG>(m_GameAddres.dwItemPoint + 1);
				
			}


		}


	}


	if (m_Memory->GetScanCode(xorstr_("57 8B D9 E8 ?? ?? ?? ?? 8B 7D 08"), m_GameAddres.dwItemEntry, m_pEngine->hModule))
	{
		/*
			//57 8B D9 E8 ?? ?? ?? ?? 8B 7D 08
			3753A6F0  /.  55            push ebp
			3753A6F1  |.  8BEC          mov ebp,esp
			3753A6F3  |.  53            push ebx                                         ;  kernel32.74CA1401
			3753A6F4  |.  56            push esi
			3753A6F5  |.  57            push edi                                         ;  hw.37928F58
			3753A6F6  |.  8BD9          mov ebx,ecx
			3753A6F8  |.  E8 A35A0100   call hw.375501A0
			3753A6FD  |.  8B7D 08       mov edi,[arg.1]
			3753A700  |.  8BF0          mov esi,eax
			3753A702  |.  6A 05         push 0x5                                         ; /maxlen = 0x5
			3753A704  |.  68 C0C98037   push hw.3780C9C0                                 ; |s2 = "item_"
			3753A709  |.  57            push edi                                         ; |s1 = "H?7?"
			3753A70A  |.  FF15 8CED7B37 call dword ptr ds:[<&api-ms-win-crt-string-l1-1->; \strncmp
			3753A710  |.  83C4 0C       add esp,0xC
			3753A713  |.  85C0          test eax,eax
			3753A715  |.  75 56         jnz short hw.3753A76D
			3753A717  |.  8D47 05       lea eax,dword ptr ds:[edi+0x5]
			3753A71A  |.  50            push eax                                         ; /s = NULL
			3753A71B  |.  FF15 4CEB7B37 call dword ptr ds:[<&api-ms-win-crt-convert-l1-1>; \atol
			3753A721  |.  8B4D 1C       mov ecx,[arg.6]                                  ;  hw.374E0BC0
			3753A724  |.  83C4 04       add esp,0x4
			3753A727  |.  8901          mov dword ptr ds:[ecx],eax
			3753A729  |.  85C0          test eax,eax
			3753A72B  |.  0F84 83000000 je hw.3753A7B4
		*/
		m_GameAddres.dwItemEntry = m_Hook->GetCallAddress(m_GameAddres.dwItemEntry + 3);
	}
	if (m_Memory->GetScanCode(xorstr_("68 ?? ?? ?? ?? 68 ?? ?? ?? ?? E8 ?? ?? ?? ?? 83 C4 0C E8 ?? ?? ?? ?? 6A 00"), m_GameAddres.dwItemCount, m_pEngine->hModule))
	{

		/*
		//第二个入口
	// 68 ?? ?? ?? ?? 68 ?? ?? ?? ?? E8 ?? ?? ?? ?? 83 C4 0C E8 ?? ?? ?? ?? 6A 00
	375FDA92  |.  50            push eax
	375FDA93  |.  68 C8688237   push hw.378268C8                                 ;  ASCII "GameMode : %s\n"
	375FDA98  |.  68 D0588238   push hw.388258D0
	375FDA9D  |.  E8 3E730400   call hw.37644DE0
	375FDAA2  |.  83C4 0C       add esp,0xC
	375FDAA5  |>  E8 C620F4FF   call hw.3753FB70
	375FDAAA  |.  6A 00         push 0x0
		*/
		m_GameAddres.dwItemCount = m_Hook->GetCallAddress(m_GameAddres.dwItemCount + 18);

	}
	if (m_Memory->GetScanCode(xorstr_("C6 45 FC 00 39 7E 0C 0F 8E"), m_GameAddres.dwbadWord, m_pEngine->hModule))
	{
		/*
		//解除脏话
	375483A6    FF50 2C         call dword ptr ds:[eax+0x2C]                               ; hw.3741C0F0
	375483A9    85C0            test eax,eax                                               ; hw.3797B30C
	375483AB    0F84 95010000   je hw.37548546
	375483B1    50              push eax                                                   ; hw.3797B30C
	375483B2    8D45 0C         lea eax,dword ptr ss:[ebp+0xC]
	375483B5    50              push eax                                                   ; hw.3797B30C
	37548368    C645 FC 00      mov byte ptr ss:[ebp-0x4],0x0
	3754836C    397E 0C         cmp dword ptr ds:[esi+0xC],edi           ; hw.3797B928
	3754836F    0F8E DE010000   jle hw.37548553
		*/

		//m_GameAddres.dwbadWord +=AntiHookGetDisasm(m_GameAddres.dwbadWord);
		//m_GameAddres.dwbadWord += AntiHookGetDisasm(m_GameAddres.dwbadWord);
		//m_GameAddres.dwbadWordJmp = m_GameAddres.dwbadWord + *(PDWORD)(m_GameAddres.dwbadWord + 2) + 6;

	}
	//按键消息
	if (m_Memory->GetScanCode(xorstr_("55 8B EC 8B 0D ?? ?? ?? ?? 85 C9 74 06 5D"), m_GameAddres.dwCallEngineSurfaceProc, m_pEngine->hModule))
	{

	}
	if (m_Memory->GetScanCode(xorstr_("89 0C 85 ?? ?? ?? ?? 0F B7 05"), m_GameAddres.dwPlayerWeaponInfo, m_pClient->hModule))
	{
		m_GameAddres.dwPlayerWeaponInfo = m_Memory->Read<ULONG>(m_GameAddres.dwPlayerWeaponInfo + 0x3);
	
	}
	/*
		退出房间的搜索方法:

		关键词  进入房间的地方 第二种就是搜浮点数  37
		37463D6F - 68 744C8837           - push 37884C74 : ["CL_CSOConnect - CL_CSODisconnect failed
		"]
		37463DAA - 68 B02CE739           - push 39E72CB0 : ["124.129.17.95:41681 "]


		37463E19 - C7 05 7C2CE739 00000000 - mov [39E72C7C],00000000
		37463E23 - C7 05 809F8738 00000000 - mov [38879F80],00000000
		37463E2D - B8 01000000           - mov eax,00000001
		37463E32 - C7 05 9815AC37 00000000 - mov [37AC1598],00000000
		37463E3C - 5D                    - pop ebp
		37463E3D - C3                    - ret
		//B8 01 00 00 00 C7 05 ?? ?? ?? ?? 00 00 00 00 5D C3
	*/
	if (m_Memory->GetScanCode(xorstr_("B8 01 00 00 00 C7 05 ?? ?? ?? ?? 00 00 00 00 5D C3"), m_GameAddres.dwRetRoom, m_pEngine->hModule))
	{
		m_GameAddres.dwRetRoom = m_Memory->Read<ULONG>(m_GameAddres.dwRetRoom + 0x7);

	}

	if (m_Memory->GetScanCode(xorstr_("E8 ?? ?? ?? ?? 89 46 10  E8 ?? ?? ?? ?? 89 46 14"), m_GameAddres.dwGetRoundCall, m_pClient->hModule))
	{
		m_GameAddres.dwGetRoundCall = m_Hook->GetCallAddress(m_GameAddres.dwGetRoundCall);
	}
	if (m_Memory->GetScanCode(xorstr_("D9 05 ?? ?? ?? ?? 33 CD E8 ?? ?? ?? ??"), m_GameAddres.dwBallistic, m_pClient->hModule))
	{
		m_GameAddres.dwBallistic = m_Memory->Read<DWORD>(m_GameAddres.dwBallistic + 2);

		/*
		*	浮点数 默认90
		*	暴走 180
		*	隐身 91
		*/

	}

	if (m_GameServer == Kor || m_GameServer == Steam)
	{

		if (m_Memory->GetScanCode(xorstr_("69 05 ?? ?? ?? ?? 6D 4E C6 41"), m_GameAddres.dwCheckEngineAddres, m_pEngine->hModule))
		{
			m_GameAddres.dwCheckEngineAddres = m_Memory->Read<DWORD>(m_GameAddres.dwCheckEngineAddres + 2);
		}
	}
	else
	{
		//国服,台服
		if (m_Memory->GetScanCode(xorstr_("69 0D ?? ?? ?? ?? 6D 4E C6 41"), m_GameAddres.dwCheckEngineAddres, m_pEngine->hModule))
		{
			m_GameAddres.dwCheckEngineAddres = m_Memory->Read<DWORD>(m_GameAddres.dwCheckEngineAddres + 2);
		}
	}

	/*
		CL_Parse_Sound: ent = %i, cl.max_edicts %i 往上找第一个调用CALL 就是
	*/

	if (m_Memory->GetScanCode(xorstr_("52 50 51 57 E8 ?? ?? ?? ?? 83 C4 20"), m_GameAddres.PreS_DynamicSoundCall, m_pEngine->hModule))
	{

		m_GameAddres.PreS_DynamicSoundCall += m_Hook->GetDisasmLenght(m_GameAddres.PreS_DynamicSoundCall,4);
		m_GameAddres.PreS_DynamicSoundCall = m_Hook->GetCallAddress(m_GameAddres.PreS_DynamicSoundCall);
	}
	if (m_Memory->GetScanCode(xorstr_("8B 7D 08 3B 3D ?? ?? ?? ?? 0F 84"), m_GameAddres.dwIsHomeowners, m_pEngine->hModule))
	{

		/*
		*	搜索这个数值 找到37开头的地址
		*	特征码 8B 7D 08 3B 3D ?? ?? ?? ?? 0F 84
		*
		*
		*	8B 7D 08			  - mov edi,[ebp+08]
			3B 3D 40DCB737        - cmp edi,[37B7DC40] : [023A93C2]
			0F 84 CA000000        - je 376715B0
		*
		*/
		m_GameAddres.dwIsHomeowners = m_Memory->Read<DWORD>(m_GameAddres.dwIsHomeowners + 5);
	}
	
	ACEInfoFileLog("[{}] PreS_DynamicSoundCall:{:X}", __FUNCTION__, m_GameAddres.PreS_DynamicSoundCall);
	ACEInfoFileLog("[{}] dwCheckEngineAddres:{:X}", __FUNCTION__, m_GameAddres.dwCheckEngineAddres);
	ACEInfoFileLog("[{}] dwPlayerMgrAddres:{:X} dwPlayerMgrCall:{:X}", __FUNCTION__, m_GameAddres.dwPlayerMgrAddres, m_GameAddres.dwPlayerMgrCall);
	ACEInfoFileLog("[{}] dwPlayerSequence:{:X}", __FUNCTION__, m_GameAddres.dwPlayerSequence);
	ACEInfoFileLog("[{}] dwRandCallRetAddres:{:X}", __FUNCTION__, m_GameAddres.dwRandCallRetAddres);
	ACEInfoFileLog("[{}] dwAngletr:{:X}", __FUNCTION__, m_GameAddres.dwAngletr);
	ACEInfoFileLog("[{}] dwCameraFOV:{:X}", __FUNCTION__, m_GameAddres.dwCameraFOV);
	ACEInfoFileLog("[{}] dwMyBone:{:X}", __FUNCTION__, m_GameAddres.dwMyBone);
	ACEInfoFileLog("[{}] dwValid:{:X}", __FUNCTION__, m_GameAddres.dwValid);
	ACEInfoFileLog("[{}] dwIsGame:{:X}", __FUNCTION__, m_GameAddres.dwIsGame);
	ACEInfoFileLog("[{}] dwNightVision:{:X}", __FUNCTION__, m_GameAddres.dwNightVision);
	ACEInfoFileLog("[{}] dwThirdPerson:{:X}", __FUNCTION__, m_GameAddres.dwThirdPerson);
	ACEInfoFileLog("[{}] dwMsg:{:X}", __FUNCTION__, m_GameAddres.dwMsg);
	ACEInfoFileLog("[{}] dwBeamPoints:{:X}", __FUNCTION__, m_GameAddres.dwBeamPoints);
	ACEInfoFileLog("[{}] dwGuiMenu:{:X}", __FUNCTION__, m_GameAddres.dwGuiMenu);
	ACEInfoFileLog("[{}] dwNoKeyInput:{:X}", __FUNCTION__, m_GameAddres.dwNoKeyInput);
	ACEInfoFileLog("[{}] dwGameVguiIpunt:{:X} dwGameVguiMouseIpunt:{:X} dwGameVguiIpuntTime:{:X}", __FUNCTION__, m_GameAddres.dwGameVguiIpunt, m_GameAddres.dwGameVguiMouseIpunt, m_GameAddres.dwGameVguiIpuntTime);
	ACEInfoFileLog("[{}] dwAFK:{:X}", __FUNCTION__, m_GameAddres.dwAFK);
	ACEInfoFileLog("[{}] dwRoomMode:{:X}", __FUNCTION__, m_GameAddres.dwRoomMode);
	ACEInfoFileLog("[{}] ItemIdToItemCode:{:X}", __FUNCTION__, m_GameFuns.ItemIdToItemCode);
	ACEInfoFileLog("[{}] GetItemEntryCall:{:X}", __FUNCTION__, m_GameFuns.GetItemEntryCall);
	ACEInfoFileLog("[{}] ItemCodeToItemName:{:X}", __FUNCTION__, m_GameFuns.ItemCodeToItemName);
	ACEInfoFileLog("[{}] dwCheckOpengl:{:X}", __FUNCTION__, m_GameAddres.dwCheckOpengl);
	ACEInfoFileLog("[{}] dwCheckProcess:{:X}", __FUNCTION__, m_GameAddres.dwCheckProcess);
	ACEInfoFileLog("[{}] dwMapBrightness:{:X}", __FUNCTION__, m_GameAddres.dwMapBrightness);
	ACEInfoFileLog("[{}] dwCommandHook:{:X}", __FUNCTION__, m_GameAddres.dwCommandHook);
	ACEInfoFileLog("[{}] dwItemPoint:{:X}", __FUNCTION__, m_GameAddres.dwItemPoint);
	ACEInfoFileLog("[{}] dwItemEntry:{:X}", __FUNCTION__, m_GameAddres.dwItemEntry);
	ACEInfoFileLog("[{}] dwbadWord:{:X}", __FUNCTION__, m_GameAddres.dwbadWord);
	ACEInfoFileLog("[{}] dwCallEngineSurfaceProc:{:X}", __FUNCTION__, m_GameAddres.dwCallEngineSurfaceProc);
	ACEInfoFileLog("[{}] dwRetRoom:{:X}", __FUNCTION__, m_GameAddres.dwRetRoom);
	ACEInfoFileLog("[{}] dwGetRoundCall:{:X}", __FUNCTION__, m_GameAddres.dwGetRoundCall);
	ACEInfoFileLog("[{}] dwBallistic:{:X}", __FUNCTION__, m_GameAddres.dwBallistic);
	ACEInfoFileLog("[{}] dwIsHomeowners:{:X}", __FUNCTION__, m_GameAddres.dwIsHomeowners);


	FindGameMpBase();
	VMProtectEnd();
	return true;
}
DWORD  GameOffset::FindUserMsgBase()
{

	//81 FE 00 01 00 00 0F 8D ?? ?? ?? ?? 8B 3D ?? ?? ?? ??
	DWORD UserMsgBase = 0;

	//韩服
	if (m_Memory->GetScanCode(xorstr_("3D C1 00 00 00 0F 87 ?? ?? ?? ?? 56 8B 35 ?? ?? ?? ?? 85 F6"), UserMsgBase, m_pEngine->hModule))
	{
		// 韩服特征码更新 2019年7月8日 17:44:54
		UserMsgBase += m_Hook->GetDisasmLenght(UserMsgBase,3);

	}
	else
	{
		if (m_Memory->GetScanCode(xorstr_("81 FE 00 01 00 00 0F 8D ?? ?? ?? ?? 8B 3D ?? ?? ?? ??"), UserMsgBase, m_pEngine->hModule))
		{
			UserMsgBase += m_Hook->GetDisasmLenght(UserMsgBase, 2);

		}
	}
	UserMsgBase = m_Memory->Read<DWORD>(UserMsgBase + 0x02);
	Engine::g_pUserMsgBase = (PUserMsg)(UserMsgBase);

	/*
	目前按照这个特征码来搜索 韩服 "3D C1 00 00 00 0F 87 ?? ?? ?? ?? 56 8B 35 ?? ?? ?? ?? 85 F6"
.text:37445AAA 3D C1 00 00 00                                                  cmp     eax, 0C1h
.text:37445AAF 0F 87 2B 01 00 00                                               ja      loc_37445BE0
.text:37445AB5 56                                                              push    esi
.text:37445AB6 8B 35 70 75 9D 37                                               mov     esi, dword_379D7570
.text:37445ABC 85 F6                                                           test    esi, esi
.text:37445ABE 74 0B                                                           jz      short loc_37445ACB
	*/

	ACEInfoFileLog("[{}] g_pUserMsgBase:0x{:X}",__FUNCTION__, (ULONG)Engine::g_pUserMsgBase);


	/*
.text:3743E6C0 sub_3743E6C0    proc near               ; CODE XREF: sub_37434600+2FB↑p
.text:3743E6C0                                         ; sub_3744DF60+15D↓p
.text:3743E6C0
.text:3743E6C0 arg_0           = dword ptr  8
.text:3743E6C0
.text:3743E6C0                 push    ebp
.text:3743E6C1                 mov     ebp, esp
.text:3743E6C3                 mov     eax, [ebp+arg_0]
.text:3743E6C6                 push    eax
.text:3743E6C7                 push    eax
.text:3743E6C8                 call    sub_3745B370
.text:3743E6CD                 add     esp, 4
.text:3743E6D0                 push    eax
.text:3743E6D1                 push    offset aHudtext ; "HudText"			找这个特征码即可
.text:3743E6D6                 call    sub_37445A40
.text:3743E6DB                 add     esp, 0Ch
.text:3743E6DE                 pop     ebp
.text:3743E6DF                 retn
.text:3743E6DF sub_3743E6C0    endp
	*/

	return (DWORD)Engine::g_pUserMsgBase;
}


DWORD  GameOffset::FindGameMpBase()
{
	VMProtectBegin(__FUNCTION__);

	if (m_IsMpInit)
		return 0;
	HMODULE hModuleMp = GetServerModuleInfo()->hModule;
	HMODULE hModuleHw = GetEngineModuleInfo()->hModule;




	if (m_Memory->GetScanCode(xorstr_("55 8B EC B9 ?? ?? ?? ?? E8 ?? ?? ?? ?? 8B 0D"), m_GameMpAddres.dwReleaseHold, hModuleHw))
	{
		//解除限制偏移  56 8B F1 88 86 ?? ?? ?? ?? 84 C0
		DWORD dwReleaseHoldOffset = 0;
		if (m_Memory->GetScanCode(xorstr_("56 8B F1 88 86 ?? ?? ?? ?? 84 C0"), dwReleaseHoldOffset, hModuleHw))
		{
			m_GameMpAddres.dwReleaseHold = *(DWORD*)(m_GameMpAddres.dwReleaseHold + 4) + *(DWORD*)(dwReleaseHoldOffset + 5);
		}
		//2019年7月23日 13:19:38
		//3759C6B0 - 55 - push ebp
		//3759C6B1 - 8B EC - mov ebp, esp
		//3759C6B3 - B9 F88D8138 - mov ecx, 38818DF8 : [37814500]
		//3759C6B8 - E8 03F0FFFF - call 3759B6C0
		//3759C6BD - 8B 0D 04908138 - mov ecx, [38819004] : [061EE828]
		//3759C6C3 - FF 35 38908138 - push[38819038] : [00000005]

		//本地教程下写入断点

	}
	if (m_Memory->GetScanCode(xorstr_("0F 44 F0 8B 87 ?? ?? ?? ?? 85 C0"), m_GameMpAddres.dwMainWeaponOffSet, hModuleMp))
	{
		m_GameMpAddres.dwMainWeaponOffSet = *(DWORD*)(m_GameMpAddres.dwMainWeaponOffSet + 5);

		//2021-07-31 09:32:05 后续加4
		m_GameMpAddres.dwMainWeaponOffSet += 4;
		//2022-04-16 13:03:38 0xC->0x10
		m_GameMpAddres.dwSubWeaponOffSet = m_GameMpAddres.dwMainWeaponOffSet + 0x10;
		m_GameMpAddres.dwKnifeOffset     = m_GameMpAddres.dwSubWeaponOffSet + 0x10;

	}
	//获取人物复活偏移____私有数据+ 偏移
	if (m_Memory->GetScanCode(xorstr_("E8 ?? ?? ?? ?? 83 F8 04 74 12 83 BF"), m_GameMpAddres.dwPlayerRpnOffset, hModuleMp))
	{
		/*
		1E9F6FCC - E8 9F690400           - call 1EA3D970 : [->375481F0]
		1E9F6FD1 - 83 F8 04              - cmp eax,04
		1E9F6FD4 - 74 12                 - je 1E9F6FE8
		1E9F6FD6 - 83 BF 14020000 00     - cmp dword ptr [edi+00000214],00
		1E9F6FDD - 74 43                 - je 1E9F7022

		*/

		m_GameMpAddres.dwPlayerRpnOffset += m_Hook->GetDisasmLenght(m_GameMpAddres.dwPlayerRpnOffset, 3);
		m_GameMpAddres.dwPlayerRpnOffset = *(DWORD*)(m_GameMpAddres.dwPlayerRpnOffset + 2);
		m_GameMpAddres.dwPlayerRpnOffset = 0x214;
	}
	m_GameMpAddres.dwPlayerRpnOffset = 0x2D0;

	ACEInfoFileLog("[{}] dwReleaseHold:0x{:X}", __FUNCTION__, (ULONG)m_GameMpAddres.dwReleaseHold);
	ACEInfoFileLog("[{}] dwMainWeaponOffSet{:X}", __FUNCTION__, (ULONG)m_GameMpAddres.dwMainWeaponOffSet);
	ACEInfoFileLog("[{}] dwPlayerRpnOffset{:X}", __FUNCTION__, (ULONG)m_GameMpAddres.dwPlayerRpnOffset);

#if 0

	if (AntiHookGetScanCode("85 C0 74 1A 80 3D ?? ?? ?? ?? 00 74 1C", m_GameMpAddres.Map, hModuleHw))
	{
		m_GameMpAddres.Map = *(DWORD*)(m_GameMpAddres.Map + 6);
	}

	if (AntiHookGetScanCode("FF 35 ?? ?? ?? ?? FF 15 ?? ?? ?? ?? 59 C3", m_GameMpAddres.dwBase, hModuleHw))
	{
		m_GameMpAddres.dwBase = *(DWORD*)(m_GameMpAddres.dwBase + 2);
	}
	if (AntiHookGetScanCode("C3 CC CC 83 3D ?? ?? ?? ?? 00 0F 84", m_GameMpAddres.dwIsMpRoom, hModuleHw))
	{
		m_GameMpAddres.dwIsMpRoom = *(DWORD*)(m_GameMpAddres.dwIsMpRoom + 5);
	}
	if (AntiHookGetScanCode("55 8B EC B9 ?? ?? ?? ?? E8 ?? ?? ?? ?? 8B 0D", m_GameMpAddres.dwReleaseHold, hModuleHw))
	{
		//解除限制偏移  56 8B F1 88 86 ?? ?? ?? ?? 84 C0
		DWORD dwReleaseHoldOffset = 0;
		if (AntiHookGetScanCode("56 8B F1 88 86 ?? ?? ?? ?? 84 C0", dwReleaseHoldOffset, hModuleHw))
		{
			m_GameMpAddres.dwReleaseHold = *(DWORD*)(m_GameMpAddres.dwReleaseHold + 4) + *(DWORD*)(dwReleaseHoldOffset + 5);
		}
		//2019年7月23日 13:19:38
		//3759C6B0 - 55 - push ebp
		//3759C6B1 - 8B EC - mov ebp, esp
		//3759C6B3 - B9 F88D8138 - mov ecx, 38818DF8 : [37814500]
		//3759C6B8 - E8 03F0FFFF - call 3759B6C0
		//3759C6BD - 8B 0D 04908138 - mov ecx, [38819004] : [061EE828]
		//3759C6C3 - FF 35 38908138 - push[38819038] : [00000005]

	}

	if (AntiHookGetScanCode("0F 44 F0 8B 87 ?? ?? ?? ?? 85 C0", m_GameMpAddres.dwMainWeaponOffSet, hModuleMp))
	{
		m_GameMpAddres.dwMainWeaponOffSet = *(DWORD*)(m_GameMpAddres.dwMainWeaponOffSet + 5);

		//2021-07-31 09:32:05 后续加4
		m_GameMpAddres.dwMainWeaponOffSet += 4;
	}
	if (AntiHookGetScanCode("8B 55 FC 3B 97 ?? 00 00 00", m_GameMpAddres.dwAttackCall, hModuleMp))
	{
		// 子弹偏移从这里更新
		m_GameMpAddres.dwWeaponBulletOffSet = *(DWORD*)(m_GameMpAddres.dwAttackCall + 5);
	}
	if (AntiHookGetScanCode("83 C4 08 3B B7 ?? 00 00 00", m_GameMpAddres.dwAttack2Call, hModuleMp))
	{


	}
	//Hook 伤害Call地址 
	if (AntiHookGetScanCode("FF 75 0C FF 75 08 FF 50 ?? FF 75 0C", m_GameMpAddres.dwhookDamage, hModuleMp))
	{
		//g_phookDamage = g_Util.inject((BYTE*)m_GameMpAddres.dwhookDamage + 6, (LPBYTE)&Hooked_FilterDamageCall, 6);
	}
	//限制同一刷枪地址  
	if (AntiHookGetScanCode("39 9E ?? 00 00 00 74 74", m_GameMpAddres.dwAlreadyOwnWeapon, hModuleMp))
	{
		DWORD dwback;
		VirtualProtect((PVOID)m_GameMpAddres.dwAlreadyOwnWeapon, 0x1024, PAGE_EXECUTE_READWRITE, &dwback);


		/*
		1DAC6BB0   .  E8 61047CFF   call mp.1D287016
		1DAC6BB5   .  84C0          test al,al
		1DAC6BB7      0F84 98000000 je mp.1DAC6C55

		倒数后不会刷枪
		*/
		if (AntiHookGetScanCode("84 C0 0F ?? ?? ?? ?? ?? 8B 5D 0C 83 FB 7C", m_GameMpAddres.dwMpRoomCheck, hModuleMp))
		{
			VirtualProtect((PVOID)m_GameMpAddres.dwMpRoomCheck, 0x1024, PAGE_EXECUTE_READWRITE, &dwback);

		}
		//解除购买武器限制 ->
		//if (AntiHookGetScanCode("38 05 ?? ?? ?? ?? 74 22", m_GameMpAddres.dwBuyThis, hModuleMp))
		//{
		//	VirtualProtect((PVOID)m_GameMpAddres.dwBuyThis, 0x1024, PAGE_EXECUTE_READWRITE, &dwback);
		//	//*(PBYTE)(m_GameMpAddres.dwBuyThis) = 0xEB;
		//}
		//解除购买武器限制2 -> 0F 85 ?? ?? ?? ?? 38 05 ?? ?? ?? ?? 74 22
		if (AntiHookGetScanCode("0F 85 ?? ?? ?? ?? 38 05 ?? ?? ?? ?? 74 22", m_GameMpAddres.dwBuyThisCallCheck, hModuleMp))
		{
			VirtualProtect((PVOID)m_GameMpAddres.dwBuyThisCallCheck, 0x1024, PAGE_EXECUTE_READWRITE, &dwback);

			//for (int i = 0; i < 6; i++)
			//{
			//	m_GameMpAddres.dwBuyThisCallCheck += AntiHookGetDisasm(m_GameMpAddres.dwBuyThisCallCheck);
			//}
			/*
			  获取跳转地址 = 当前地址+ 6 +偏移
			2F2A2837     /0F85 B6030000        jnz mp.2F2A2BF3
			*/
			m_GameMpAddres.dwBuyThisJmpAddres = m_GameMpAddres.dwBuyThisCallCheck + 6 + *(PDWORD)(m_GameMpAddres.dwBuyThisCallCheck + 2);
		}
		//资金不足限制 ->
		if (AntiHookGetScanCode("39 87 ?? ?? ?? ?? 7D 4E", m_GameMpAddres.dwEnoughMoney, hModuleMp))
		{
			VirtualProtect((PVOID)m_GameMpAddres.dwEnoughMoney, 0x1024, PAGE_EXECUTE_READWRITE, &dwback);
		}
		//丢弃武器限制 ->
		if (AntiHookGetScanCode("8B 8E ?? ?? ?? ?? 85 C9 74 33", m_GameMpAddres.dwCannotBeDropped, hModuleMp))
		{
			//*(PBYTE)(m_GameMpAddres.dwCannotBeDropped + 8) = 0xEB;
			VirtualProtect((PVOID)m_GameMpAddres.dwCannotBeDropped, 0x1024, PAGE_EXECUTE_READWRITE, &dwback);
		}
		if (AntiHookGetScanCode("74 1E 80 BB ?? ?? 00 00", m_GameMpAddres.dwCheckBuyTimer, hModuleMp))
		{
			VirtualProtect((PVOID)m_GameMpAddres.dwCheckBuyTimer, 0x1024, PAGE_EXECUTE_READWRITE, &dwback);
		}


	}
	//买刀限制
	if (AntiHookGetScanCode("0F 84 ?? ?? ?? ?? 6A 01 57", m_GameMpAddres.dwBuyKnife, hModuleMp))
	{

		/*
		近身武器选择 跟进CALL
		1F31E869  |> \8B45 F8       mov eax,[local.2]
		1F31E86C  |.  0FB7C0        movzx eax,ax
		1F31E86F  |.  50            push eax
		1F31E870  |.  57            push edi
		1F31E871  |.  53            push ebx
		1F31E872  |.  E8 47AD7BFF   call mp.1EAD95BE

		关键跳转 nop
		1F3166C9     /0F84 35010000 je mp.1F316804
		1F3166CF  |. |6A 01         push 0x1
		1F3166D1  |. |57            push edi                                 ;  client.1DE3184C
		1F3166D2  |. |8BCE          mov ecx,esi
		1F3166D4  |. |C686 00040000>mov byte ptr ds:[esi+0x400],0x1

		*/
		DWORD dwback;
		VirtualProtect((PVOID)m_GameMpAddres.dwBuyKnife, 0x1024, PAGE_EXECUTE_READWRITE, &dwback);
	}


	//致命打击技能 解除限制
	if (AntiHookGetScanCode("84 C0 0F 84 ?? ?? ?? ?? 6A 08 68 ?? ?? ?? ??", m_GameMpAddres.dwHm_skill_Z, hModuleMp))
	{


		DWORD dwback;
		VirtualProtect((PVOID)m_GameMpAddres.dwHm_skill_Z, 0x1024, PAGE_EXECUTE_READWRITE, &dwback);
		//生化Z 限制解除
		//memset((void*)(m_GameMpAddres.dwHm_skill_Z + 0x2),0x90,6);


		if (AntiHookGetScanCode("80 BF ?? ?? ?? ?? 00 0f 84 ?? ?? ?? ?? 8B 15", m_GameMpAddres.dwHm_skill, hModuleMp))
		{
			m_GameMpAddres.dwHeadShot_Offset = *(PDWORD)(m_GameMpAddres.dwHm_skill + 2);

		}
		if (AntiHookGetScanCode("F6 C1 02 74 ?? F3 0F 10 87 ?? ?? ?? ??", m_GameMpAddres.dwCantHeadShotUsed, hModuleMp))
		{
			//致命打击是否可用
			//*(PBYTE)((m_GameMpAddres.dwCantHeadShotUsed + 0x3)) = 0xEB;
		}
		if (AntiHookGetScanCode("8B CF C7 87 ?? ?? ?? ?? 00 00 00 00 F3 0F 11 87 ?? ?? ?? ??", m_GameMpAddres.dwHm_skillDuration_Hook, hModuleMp))
		{

			//致命打击持续时间偏移
			//m_GameMpAddres.dwHm_skillDuration_Offset = *(PDWORD)(m_GameMpAddres.dwHm_skillDuration_Hook + 4);

		}


	}

	// 2019年11月12日 11:22:25 更新后 PVE 伤害CALL
	ULONG	OffSet = 0;
	while (AntiHookGetScanCodeEx("6A FF 68 ?? ?? ?? ?? 8B 01", m_GameMpAddres.PVEDamageCall, hModuleMp, OffSet))
	{

		/*  查找Class 这个cur_skill字符串
		1EEE9DF9 - 6A FF                 - push -01
		1EEE9DFB - 68 14E35D1F           - push 1F5DE314 : ["cur_skill"]
		1EEE9E00 - 8B 01                 - mov eax,[ecx]
		6A FF 68 ?? ?? ?? ?? 8B 01
		*/

		m_GameMpAddres.PVEDamageCall += AntiHookGetDisasm(m_GameMpAddres.PVEDamageCall);

		char* Str = (char*)(*(PULONG)(m_GameMpAddres.PVEDamageCall + 1));

		if (lstrcmpA(Str, "cur_skill") == 0)
		{
			m_GameMpAddres.PVEDamageCall = /**(DWORD*)*/((ULONG)Str + 0x3C + 0x40);
			break;
		}
		OffSet++;
	}

	//PVE 伤害Call
	if (/*AntiHookGetScanCode("8D B7 ?? ?? 00 00 C7 07 ?? ?? ?? ?? 6A 08 C6", m_GameMpAddres.PVEDamageCall, hModuleMp)*/0)
	{
		/*	mp.roamingpoint + 1084 - 8D B7 08080000 - lea esi, [edi + 00000808]
			mp.roamingpoint + 108A - C7 07 64F0C020 - mov[edi], mp.ZBBot::BotTouch + 64E94
			mp.roamingpoint + 1090 - 6A 08 - push 08


			*/
			// 8D B7 08 08 00 00 C7 07 ?? ?? ?? ?? 6A 08




			/* 从这里 可以找到所有伤害CALL 也就是修改伤害哪里
	2F770A31    F3:0F1005 54E7B42F        movss xmm0,dword ptr ds:[0x2FB4E754]
	2F770A39    8B01                      mov eax,dword ptr ds:[ecx]
	2F770A3B    56                        push esi
	2F770A3C    FF35 58E7B42F             push dword ptr ds:[0x2FB4E758]
	2F770A42    51                        push ecx
	2F770A43    F3:0F110424               movss dword ptr ss:[esp],xmm0
	2F770A48    FF75 0C                   push dword ptr ss:[ebp+0xC]              ; hw.374E0BC0
	2F770A4B    FF75 08                   push dword ptr ss:[ebp+0x8]              ; hw.37928484
	2F770A4E    FF50 40                   call dword ptr ds:[eax+0x40]				;这是偏移   这里去EAX的值 + 0x40
	2F770A51    FF75 0C                   push dword ptr ss:[ebp+0xC]              ; hw.374E0BC0
	2F770A54    8BF0                      mov esi,eax
	2F770A56    FF75 08                   push dword ptr ss:[ebp+0x8]              ; hw.37928484
	2F770A59    E8 62D9A4FF               call mp.2F1BE3C0
	2F770A5E    83C4 08                   add esp,0x8
	2F770A61    8BC6                      mov eax,esi
	2F770A63    5E                        pop esi                                  ; hw.374E0BC0
	2F770A64    5D                        pop ebp                                  ; hw.374E0BC0
	2F770A65    C3                        retn
	FF 75 0C FF 75 08 FF 50 40 FF 75 0C
			*/


		m_GameMpAddres.PVEDamageCall = *(DWORD*)(m_GameMpAddres.PVEDamageCall + 8) + 0x40;
	}
	//PVP 伤害Call
	if (AntiHookGetScanCode("55 8B EC 83 EC 08 8B 45 08 53 8B D8 89 5D FC", m_GameMpAddres.PVPDamageCall, hModuleMp))
	{


	}

	//获取刀具偏移
	if (AntiHookGetScanCode("6A 01 68 ?? ?? ?? ?? C7 80 ?? ?? ?? ?? 00 00 00 00", m_GameMpAddres.dwKnifeOffset, hModuleMp))
	{
		m_GameMpAddres.dwKnifeOffset = *(DWORD*)(m_GameMpAddres.dwKnifeOffset + 9);
	}

	//获取人物角色数组地址
	if (AntiHookGetScanCode("8D 04 40 8B 3C 85", m_GameMpAddres.dwPlayerRoleOffset, hModuleMp))
	{
		m_GameMpAddres.dwPlayerRoleOffset = *(DWORD*)(m_GameMpAddres.dwPlayerRoleOffset + 6);
	}
	//获取人物复活偏移____私有数据+ 偏移
	if (AntiHookGetScanCode("E8 ?? ?? ?? ?? 83 F8 04 74 12 83 BF", m_GameMpAddres.dwPlayerRpnOffset, hModuleMp))
	{
		/*
		1E9F6FCC - E8 9F690400           - call 1EA3D970 : [->375481F0]
		1E9F6FD1 - 83 F8 04              - cmp eax,04
		1E9F6FD4 - 74 12                 - je 1E9F6FE8
		1E9F6FD6 - 83 BF 14020000 00     - cmp dword ptr [edi+00000214],00
		1E9F6FDD - 74 43                 - je 1E9F7022

		*/
		for (int i = 0; i < 3; i++)
		{
			m_GameMpAddres.dwPlayerRpnOffset += AntiHookGetDisasm(m_GameMpAddres.dwPlayerRpnOffset);
		}
		m_GameMpAddres.dwPlayerRpnOffset = *(DWORD*)(m_GameMpAddres.dwPlayerRpnOffset + 2);
	}


	//重新选择角色限制
//	if (AntiHookGetScanCode("FF 15 ?? ?? ?? ?? 83 C4 04 83 BF ?? ?? ?? ?? 03", m_GameMpAddres.dwPlayerRoleDetection, hModuleMp))
//	{
//		/*
//// 解除限制
//1E84C3A1    68 9435AC1E     push mp.1EAC3594                                      ; ASCII "joinclass"
//1E84C3A6    53              push ebx                                              ; kernel32.762A1400
//1E84C3A7    E8 6464B1FF     call mp.1E362810
//1E84C3AC    83C4 08         add esp,0x8
//1E84C3AF    85C0            test eax,eax                                          ; hw.3745358F
//1E84C3B1    0F84 8E000000   je mp.1E84C445
//1E84C3B7    803D 40D1CB1E 0>cmp byte ptr ds:[0x1ECBD140],0x0
//1E84C3BE    74 07           je short mp.1E84C3C7
//1E84C3C0    A1 34D1CB1E     mov eax,dword ptr ds:[0x1ECBD134]
//1E84C3C5    EB 0B           jmp short mp.1E84C3D2
//1E84C3C7    6A 01           push 0x1
//1E84C3C9    FF15 E88BD01E   call dword ptr ds:[0x1ED08BE8]                        ; hw.37415240
//1E84C3CF    83C4 04         add esp,0x4
//1E84C3D2    50              push eax                                              ; hw.3745358F
//1E84C3D3    FF15 78D2A01E   call dword ptr ds:[<&api-ms-win-crt-convert-l1-1-0.at>; ucrtbase.atol
//1E84C3D9    83C4 04         add esp,0x4
//1E84C3DC    83BF 64030000 0>cmp dword ptr ds:[edi+0x364],0x3
//1E84C3E3  ^ 0F85 40FCFFFF   jnz mp.1E84C029
//
//FF 15 ?? ?? ?? ?? 83 C4 04 83 BF ?? ?? ?? ?? 03
//		*/
//		m_GameMpAddres.dwPlayerRoleDetection += AntiHookGetDisasm(m_GameMpAddres.dwPlayerRoleDetection);
//		m_GameMpAddres.dwPlayerRoleDetection += AntiHookGetDisasm(m_GameMpAddres.dwPlayerRoleDetection);
//
//		m_GameMpAddres.dwPlayerRoleDetection = *(DWORD*)(m_GameMpAddres.dwPlayerRoleDetection + 2);
//	}
	if (AntiHookGetScanCode("8B F0 83 BB ?? ?? ?? ?? 03 0F 85", m_GameMpAddres.dwPlayerRoleDetection, hModuleMp))
	{
		m_GameMpAddres.dwPlayerRoleDetection += AntiHookGetDisasm(m_GameMpAddres.dwPlayerRoleDetection);
		m_GameMpAddres.dwPlayerRoleDetection = *(DWORD*)(m_GameMpAddres.dwPlayerRoleDetection + 2);

	}

	//获取阵营偏移
	/*
	2F45F51B  |> \8B46 5C                 mov eax,dword ptr ds:[esi+0x5C]
	2F45F51E  |.  3B47 5C                 cmp eax,dword ptr ds:[edi+0x5C]
	2F45F521  |.  75 25                   jnz short mp.2F45F548
	8B 46 ?? 3B 47 ?? 75 25
	*/
	if (AntiHookGetScanCode("8B 46 ?? 3B 47 ?? 75 25", m_GameMpAddres.dwCampOffSet, hModuleMp))
	{
		m_GameMpAddres.dwCampOffSet = *(PBYTE)(m_GameMpAddres.dwCampOffSet + 2);
	}

	/*
	1BED2F76    8B01            mov eax,dword ptr ds:[ecx]
	1BED2F78    8B80 D0020000   mov eax,dword ptr ds:[eax+0x2D0]
	1BED2F7E    FFD0            call eax
	1BED2F80    84C0            test al,al
	1BED2F82    0F85 77010000   jnz client.1BED30FF
	*/

	if (AntiHookGetScanCode("8B 80 D0 02 00 00 FF D0 84 C0 0F 85", m_GameMpAddres.dwReleaseHoldEx, hModuleMp))
	{
		//m_GameMpAddres.dwCampOffSet = *(PBYTE)(m_GameMpAddres.dwCampOffSet + 2);
	}



	//m_GameMpAddres.dwPlayerOffSet = 0x7c;
	//m_GameMpAddres.dwCampOffSet = 0x44;
	//m_GameMpAddres.dwMoneyOffSet = 0x1D0;
	m_GameMpAddres.dwSubWeaponOffSet = m_GameMpAddres.dwMainWeaponOffSet + 0xC;

	m_GameMpAddres.dwWeaponRecoilOffSet = m_GameMpAddres.dwWeaponBulletOffSet + 0x40;
	m_GameMpAddres.dwWeaponSpeedOffSet = m_GameMpAddres.dwWeaponBulletOffSet - 0x18;
	//m_GameMpAddres.dwNextPlayerOffSet = 0x34C + 0x8;
#endif

	

#if 0
	AntiFileLog("[%s]->Map %X", __FUNCTION__, m_GameMpAddres.Map);
	AntiFileLog("[%s]->dwBase %X", __FUNCTION__, m_GameMpAddres.dwBase);
	AntiFileLog("[%s]->dwIsMpRoom %X", __FUNCTION__, m_GameMpAddres.dwIsMpRoom);
	AntiFileLog("[%s]->dwReleaseHold %X", __FUNCTION__, m_GameMpAddres.dwReleaseHold);

	AntiFileLog("[%s]->dwMainWeaponOffSet %X", __FUNCTION__, m_GameMpAddres.dwMainWeaponOffSet);
	AntiFileLog("[%s]->dwSubWeaponOffSet %X", __FUNCTION__, m_GameMpAddres.dwSubWeaponOffSet);

	AntiFileLog("[%s]->dwAttackCall %X", __FUNCTION__, m_GameMpAddres.dwAttackCall);
	AntiFileLog("[%s]->dwAttack2Call %X", __FUNCTION__, m_GameMpAddres.dwAttack2Call);

	AntiFileLog("[%s]->PVEDamageCall %X", __FUNCTION__, m_GameMpAddres.PVEDamageCall);
	AntiFileLog("[%s]->PVPDamageCall %X", __FUNCTION__, m_GameMpAddres.PVPDamageCall);
	AntiFileLog("[%s]->dwKnifeOffset %X", __FUNCTION__, m_GameMpAddres.dwKnifeOffset);
	AntiFileLog("[%s]->dwhookDamage %X", __FUNCTION__, m_GameMpAddres.dwhookDamage);


	AntiFileLog("[%s]->dwAlreadyOwnWeapon %X", __FUNCTION__, m_GameMpAddres.dwAlreadyOwnWeapon);
	AntiFileLog("[%s]->dwBuyThis %X", __FUNCTION__, m_GameMpAddres.dwBuyThis);
	AntiFileLog("[%s]->dwMpRoomCheck %X", __FUNCTION__, m_GameMpAddres.dwMpRoomCheck);
	AntiFileLog("[%s]->dwEnoughMoney %X", __FUNCTION__, m_GameMpAddres.dwEnoughMoney);
	AntiFileLog("[%s]->dwCannotBeDropped %X", __FUNCTION__, m_GameMpAddres.dwCannotBeDropped);
	AntiFileLog("[%s]->dwCheckBuyTimer %X", __FUNCTION__, m_GameMpAddres.dwCheckBuyTimer);
	AntiFileLog("[%s]->dwBuyThisJmpAddres %X", __FUNCTION__, m_GameMpAddres.dwBuyThisJmpAddres);
	AntiFileLog("[%s]->dwBuyThisCallCheck %X", __FUNCTION__, m_GameMpAddres.dwBuyThisCallCheck);
	AntiFileLog("[%s]->dwBuyKnife %X", __FUNCTION__, m_GameMpAddres.dwBuyKnife);



	AntiFileLog("[%s]->dwHm_skill %X", __FUNCTION__, m_GameMpAddres.dwHm_skill);
	AntiFileLog("[%s]->dwHm_skill_Z %X", __FUNCTION__, m_GameMpAddres.dwHm_skill_Z);
	AntiFileLog("[%s]->dwCantHeadShotUsed %X", __FUNCTION__, m_GameMpAddres.dwCantHeadShotUsed);
	AntiFileLog("[%s]->dwHeadShot_Offset %X", __FUNCTION__, m_GameMpAddres.dwHeadShot_Offset);
	AntiFileLog("[%s]->dwHm_skillDuration_Offset %X", __FUNCTION__, m_GameMpAddres.dwHm_skillDuration_Offset);
	AntiFileLog("[%s]->dwHm_skillDuration_Hook %X", __FUNCTION__, m_GameMpAddres.dwHm_skillDuration_Hook);

	AntiFileLog("[%s]->dwPlayerRoleOffset %X", __FUNCTION__, m_GameMpAddres.dwPlayerRoleOffset);
	AntiFileLog("[%s]->dwPlayerRpnOffset %X", __FUNCTION__, m_GameMpAddres.dwPlayerRpnOffset);
	AntiFileLog("[%s]->dwPlayerRoleDetection %X", __FUNCTION__, m_GameMpAddres.dwPlayerRoleDetection);

	AntiFileLog("[%s]->dwCampOffSet %X", __FUNCTION__, m_GameMpAddres.dwCampOffSet);


#endif // ANTI_DEBUG

	m_IsMpInit = TRUE;



	VMProtectEnd();
	return true;

}
