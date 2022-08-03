#include "DispatchRecv.h"
#include "GameOffset.h"
#include "Util.h"
#include <regex>
#include "GameCFG.h"
#include "Drawing.h"
#include "GamePlayer.h"
#include "CSagaClient.h"


CDispatchRecv::CDispatchRecv()
{
	m_SeCallAddress = 0;
	m_PacketInfo    = 0;
	m_pGamePack     = nullptr;
	m_ItemCount     = 0;
	m_ItemSite      = 0;

}

CDispatchRecv* CDispatchRecv::GetInstance()
{
	if (m_Instance == nullptr)
	{
		m_Instance = new CDispatchRecv();
	}
	return m_Instance;
}

int CDispatchRecv::vKeyBoardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (wParam == VK_F1)
	{
		CGamePack::GetInstance()->DoubleExp();
	}
	//if (wParam == VK_F2)
	//{
	//	CGamePack::GetInstance()->Room_ChangePlayerCount(1);
	//}
	//if (wParam == VK_F3)
	//{
	//	CGamePack::GetInstance()->Room_GameOver();
	//}

	
	return 0;
}




bool CDispatchRecv::vInit()
{
	InitializeHook();

	Engine::g_Engine.pfnAddCommand("+SuperRetRoom", InterfaceCommandPacketSuperRetRoom);

	m_pGamePack = CGamePack::GetInstance();

	m_ItemSite = 20000;

	m_lPackInventory = std::make_shared<PackWriter>(0x1024 * 100);
		

	return true;
}
GAME_PACKET_TYPE_INFO  GamePacketList[] =
{
#if ANTI_DEBUG
#define xorstr_DEBUG			xorstr_
#else
#define xorstr_DEBUG(str)		xorstr("").crypt_get()
#endif
	GAME_PACKET_TYPE_INFO(Packet_Version	                             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_Version")),
	GAME_PACKET_TYPE_INFO(Packet_Reply	                                 ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_Reply")),
	GAME_PACKET_TYPE_INFO(Packet_Transfer	                             ,(ULONG)CDispatchRecv::Fake_Transfer,false,  xorstr_DEBUG("Packet_Transfer")),
	GAME_PACKET_TYPE_INFO(Packet_ServerList	                             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_ServerList")),
	
	GAME_PACKET_TYPE_INFO(Packet_Crypt	                                 ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_Crypt")),
	GAME_PACKET_TYPE_INFO(Packet_MileageBingo	                         ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_MileageBingo")),
	GAME_PACKET_TYPE_INFO(Packet_SessionID	                             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_SessionID")),
	GAME_PACKET_TYPE_INFO(Packet_UpROOM	                                 ,(ULONG)CDispatchRecv::Fake_UpRoom, true,  xorstr_DEBUG("Packet_UpROOM")),
	GAME_PACKET_TYPE_INFO(Packet_ClientCheck	                         ,(ULONG)CDispatchRecv::Fake_ClientCheck, true,  xorstr_DEBUG("Packet_ClientCheck")),
	GAME_PACKET_TYPE_INFO(Packet_UMsg	                                 ,(ULONG)CDispatchRecv::Fake_UMsg, true,  xorstr_DEBUG("Packet_UMsg")),
	GAME_PACKET_TYPE_INFO(Packet_Host	                                 ,(ULONG)CDispatchRecv::Fake_Host, true,  xorstr_DEBUG("Packet_Host")),
	GAME_PACKET_TYPE_INFO(Packet_UDP	                                 ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_UDP")),
	GAME_PACKET_TYPE_INFO(Packet_Clan	                                 ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_Clan")),
	GAME_PACKET_TYPE_INFO(Packet_Shop	                                 ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_Shop")),
	GAME_PACKET_TYPE_INFO(Packet_Rank	                                 ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_Rank")),
	GAME_PACKET_TYPE_INFO(Packet_Ban	                                 ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_Ban")),
	GAME_PACKET_TYPE_INFO(Packet_Option	                                 ,(ULONG)CDispatchRecv::Fake_Option, true,  xorstr_DEBUG("Packet_Option")),
	GAME_PACKET_TYPE_INFO(Packet_Favorite	                             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_Favorite")),
	GAME_PACKET_TYPE_INFO(Packet_Item	                                 ,(ULONG)CDispatchRecv::Fake_Item, true,  xorstr_DEBUG("Packet_Item")),
	GAME_PACKET_TYPE_INFO(Packet_QuickStart	                             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_QuickStart")),
	GAME_PACKET_TYPE_INFO(Packet_Quest	                                 ,(ULONG)CDispatchRecv::Fake_Quest,	  true,  xorstr_DEBUG("Packet_Quest")),
	GAME_PACKET_TYPE_INFO(Packet_Hack	                                 ,(ULONG)CDispatchRecv::Fake_Hack,	  true,  xorstr_DEBUG("Packet_Hack")),
	GAME_PACKET_TYPE_INFO(Packet_Metadata	                             ,(ULONG)CDispatchRecv::Fake_Metadata, true,  xorstr_DEBUG("Packet_Metadata")),
	GAME_PACKET_TYPE_INFO(Packet_PlayerInfo	                             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_PlayerInfo")),
	GAME_PACKET_TYPE_INFO(Packet_GameMatch	                             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_GameMatch")),
	GAME_PACKET_TYPE_INFO(Packet_UpdateWarehouse	                     ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_UpdateWarehouse")),
	GAME_PACKET_TYPE_INFO(Packet_Ribbon	                                 ,(ULONG)CDispatchRecv::Fake_Ribbon, true,  xorstr_DEBUG("Packet_Ribbon")),
	GAME_PACKET_TYPE_INFO(Packet_VoxelOutUI	                             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_VoxelOutUI")),
	GAME_PACKET_TYPE_INFO(Packet_Analysis	                             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_Analysis")),
	GAME_PACKET_TYPE_INFO(Packet_PopularInfo	                         ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_PopularInfo")),
	GAME_PACKET_TYPE_INFO(Packet_GameMatchRoomList	                     ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_GameMatchRoomList")),
	GAME_PACKET_TYPE_INFO(Packet_UserStart	                             ,(ULONG)CDispatchRecv::Fake_UserStart, true,  xorstr_DEBUG("Packet_UserStart")),
	GAME_PACKET_TYPE_INFO(Packet_Inventory	                             ,(ULONG)CDispatchRecv::Fake_Inventory, true,  xorstr_DEBUG("Packet_Inventory")),
	GAME_PACKET_TYPE_INFO(Packet_Lobby	                                 ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_Lobby")),
	GAME_PACKET_TYPE_INFO(Packet_Inventory2	                             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_Inventory2")),
	GAME_PACKET_TYPE_INFO(Packet_UserUpdateInfo	                         ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_UserUpdateInfo")),
	GAME_PACKET_TYPE_INFO(Packet_Event	                                 ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_Event")),
	GAME_PACKET_TYPE_INFO(Packet_ZombieScenarioMaps	                     ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_ZombieScenarioMaps")),
	GAME_PACKET_TYPE_INFO(Packet_Alarm	                                 ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_Alarm")),
	GAME_PACKET_TYPE_INFO(Packet_VipSystem	                             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_VipSystem")),
	GAME_PACKET_TYPE_INFO(Packet_Kick	                                 ,(ULONG)CDispatchRecv::Fake_Kick, true,  xorstr_DEBUG("Packet_Kick")),
	GAME_PACKET_TYPE_INFO(Packet_Friend	                                 ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_Friend")),
	GAME_PACKET_TYPE_INFO(Packet_Expedition	                             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_Expedition")),
	GAME_PACKET_TYPE_INFO(Packet_League	                                 ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_League")),
	GAME_PACKET_TYPE_INFO(Packet_SantaAnim	                             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_SantaAnim")),
	GAME_PACKET_TYPE_INFO(Packet_Character	                             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_Character")),
    GAME_PACKET_TYPE_INFO(Packet_UpdateInfo	                             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_UpdateInfo")),
    GAME_PACKET_TYPE_INFO(Packet_ClassInven	                             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_ClassInven")),
    GAME_PACKET_TYPE_INFO(Packet_PartsInven	                             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_PartsInven")),
	GAME_PACKET_TYPE_INFO(Packet_ItemInven	                             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_ItemInven")),
	GAME_PACKET_TYPE_INFO(Packet_FreePassWeaponInven	                 ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_FreePassWeaponInven")),
	GAME_PACKET_TYPE_INFO(Packet_Dictionary	                             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_Dictionary")),
	GAME_PACKET_TYPE_INFO(Packet_GuideQuest	                             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_GuideQuest")),
	GAME_PACKET_TYPE_INFO(Packet_Statistico	                             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_Statistico")),
	GAME_PACKET_TYPE_INFO(Packet_MileageShop	                         ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_MileageShop")),
	GAME_PACKET_TYPE_INFO(Packet_SeasonSystem	                         ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_SeasonSystem")),
	GAME_PACKET_TYPE_INFO(Packet_EpicPieceShop	                         ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_EpicPieceShop")),
	GAME_PACKET_TYPE_INFO(Packet_ServerList	                             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_ServerList")),
	GAME_PACKET_TYPE_INFO(Packet_Mobile						             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_Mobile")),
	GAME_PACKET_TYPE_INFO(Packet_Policy					                 ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_Policy")),
	GAME_PACKET_TYPE_INFO(Packet_SearchRoom					             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_SearchRoom")),
	GAME_PACKET_TYPE_INFO(Packet_HostServer					             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_HostServer")),
	GAME_PACKET_TYPE_INFO(Packet_Report					                 ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_Report")),
	GAME_PACKET_TYPE_INFO(Packet_Title					                 ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_Title")),
	GAME_PACKET_TYPE_INFO(Packet_Buff					                 ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_Buff")),
	GAME_PACKET_TYPE_INFO(Packet_UserSurvey					             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_UserSurvey")),
	GAME_PACKET_TYPE_INFO(Packet_MiniGame					             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_MiniGame")),
	GAME_PACKET_TYPE_INFO(Packet_Gift_Item					             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_Gift_Item")),
	GAME_PACKET_TYPE_INFO(Packet_2nd_Password					         ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_2nd_Password")),
	GAME_PACKET_TYPE_INFO(Packet_CleanSystem					         ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_CleanSystem")),
	GAME_PACKET_TYPE_INFO(Packet_WeaponAuctionEvent					     ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_WeaponAuctionEvent")),
	GAME_PACKET_TYPE_INFO(Packet_CoDisassemble					         ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_CoDisassemble")),
	GAME_PACKET_TYPE_INFO(Packet_Help					                 ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_Help")),
	GAME_PACKET_TYPE_INFO(Packet_HonorShop					             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_HonorShop")),
	GAME_PACKET_TYPE_INFO(Packet_Addon					                 ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_Addon")),
	GAME_PACKET_TYPE_INFO(Packet_QuestBadgeShop					         ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_QuestBadgeShop")),
	GAME_PACKET_TYPE_INFO(Packet_ReverseAuctionSystem					 ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_ReverseAuctionSystem")),
	GAME_PACKET_TYPE_INFO(Packet_FabItems					             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_FabItems")),
	GAME_PACKET_TYPE_INFO(Packet_CostumeInven					         ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_CostumeInven")),
	GAME_PACKET_TYPE_INFO(Packet_MonthlyWeapon					         ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_MonthlyWeapon")),
	GAME_PACKET_TYPE_INFO(Packet_ServerLog					             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_ServerLog")),
	GAME_PACKET_TYPE_INFO(Packet_TestPacket					             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_TestPacket")),
	GAME_PACKET_TYPE_INFO(Packet_ZBSGoldenZB					         ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_ZBSGoldenZB")),
	GAME_PACKET_TYPE_INFO(Packet_ScenarioTX					             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_ScenarioTX")),
	GAME_PACKET_TYPE_INFO(Packet_SwitchConfig					         ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_SwitchConfig")),
	GAME_PACKET_TYPE_INFO(Packet_CafeItems					             ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_CafeItems")),
	GAME_PACKET_TYPE_INFO(Packet_DefaultItems					         ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_DefaultItems")),
	GAME_PACKET_TYPE_INFO(Packet_RotationWeaponInven					 ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_RotationWeaponInven")),
	GAME_PACKET_TYPE_INFO(Packet_LifeWeaponInven					     ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_LifeWeaponInven")),
	GAME_PACKET_TYPE_INFO(Packet_EventItemInve					         ,(ULONG)nullptr, false,  xorstr_DEBUG("Packet_EventItemInve")),
};


void _stdcall CDispatchRecv:: DEBUG_PacketDispatch(PTAG_PEB_STACK Stack)
{
	ULONG* Array = (ULONG*)Stack->ESP;

	int Size = Array[1];

	int Index = *(BYTE*)((ULONG)Stack->EBP - 0x10210);

	int Ret = 0;

	for (int i = 0; i < sizeof(GamePacketList) / sizeof(GAME_PACKET_TYPE_INFO); i++)
	{
		if (GamePacketList[i].Type == (Packet_Type)Index)
		{
			Ret = 1;


			//函数功能： 从已知类模板中导出一个类
			//返回值：导出类的地址，如果void* object＝NULL，则返回值＝NULL
			//	参数1：void* object，已知类的地址
			//	参数2：int32_t vfdelta　＝　0（
			//	参数3：void* srctype　源模板
			//	参数4：void* desttype　目的模板
			//	参数5：int isreference　＝　0　（

			//找到 eax + 0x4 = 地址
			ULONG Ecx = (ULONG)Stack->EDI;
			ULONG v1 = *(PULONG)(Ecx + 0xC);
			ULONG funs = 0;
			if (v1)
			{
				ULONG v2 = *(PULONG)(v1 + 0x8);
				funs = *(PULONG)(*(PULONG)(v2) + 0x4);
			}



			ACEWarningFileLog("{} Index:{} String:{} Fun:{:X} SvAddress {:X} funs:{:X}",__FUNCTION__, Index, GamePacketList[i].name, (ULONG)Stack->EDX, GamePacketList[i].SvAddres, funs);




			break;
		}
	}
	if (!Ret)
	{
		ULONG* v2 = (PULONG)(Stack->EDI);
		ACEErrorFileLog("{}-Index:{} Fun:{:X} v2 {:X}", __FUNCTION__, Index, (ULONG)Stack->EDX, v2[0]);
	}

}
void _stdcall CDispatchRecv::PacketDispatch(PTAG_PEB_STACK Stack)
{
	ULONG* Arry = (ULONG*)Stack->ESP;

	CHAR Index = Arry[9] & 0xFF;

	for (int i = 0; i < sizeof(GamePacketList) / sizeof(GAME_PACKET_TYPE_INFO); i++)
	{

		if ((CHAR)GamePacketList[i].Type == Index)
		{
			memcpy(&GamePacketList[i].Stack, Stack, sizeof(TAG_PEB_STACK));

			CallHookAddres(Stack, &GamePacketList[i]);

			break;

		}
	}
}
void CDispatchRecv::CallHookAddres(PTAG_PEB_STACK Stack, PTAG_GAME_PACKET_TYPE_INFO Info)
{
	ULONG* Arry = (ULONG*)Stack->ESP;

	ULONG	src = Arry[1];

	ULONG	ulSize = Arry[2];

	ULONG   CallAddres = Info->Addres;
	__asm
	{
		pushad
		mov eax, Info
		push eax
		mov eax, ulSize
		push eax
		mov eax, src
		push eax
		call CallAddres
		popad
	}
}


PTAG_PEB_STACK CDispatchRecv::GetPacketStack(Packet_Type type)
{
	for (int i = 0; i < sizeof(GamePacketList) / sizeof(GAME_PACKET_TYPE_INFO); i++)
	{
		if (GamePacketList[i].Type == type)
		{
			return &GamePacketList[i].Stack;
		}
	}
	return nullptr;
}

PTAG_GAME_PACKET_TYPE_INFO CDispatchRecv::GetPacketTypeInfo(Packet_Type type)
{
	for (int i = 0; i < sizeof(GamePacketList) / sizeof(GAME_PACKET_TYPE_INFO); i++)
	{

		if ((CHAR)GamePacketList[i].Type == type)
		{
			return  &GamePacketList[i];
		}
	}
	return nullptr;
}

BYTE CDispatchRecv::CallGamePacket(PVOID Dst, int Size, PTAG_PEB_STACK Stack, PTAG_GAME_PACKET_TYPE_INFO pHook)
{
	PVOID pThis = Stack->ECX;
	ULONG pCall = pHook->SvAddres;
	__asm
	{
		mov ecx, dword ptr ss : [ebp + 0xC]
		push ecx
		mov edx, dword ptr ss : [ebp + 0x8]
		push edx
		mov ecx, pThis
		mov edi, ecx
		call pCall
	}
}

BYTE CDispatchRecv::CallGamePacket2(PVOID Dst, int Size, PTAG_PEB_STACK Stack, PTAG_GAME_PACKET_TYPE_INFO pHook)
{
	PVOID pThis = Stack->ECX;
	ULONG pCall = pHook->SvAddres;
	ULONG v2 = CDispatchRecv::GetInstance()->m_SeCallAddress;

	__asm
	{
		mov ecx, dword ptr ss : [ebp + 0xC]
		push ecx
		mov edx, dword ptr ss : [ebp + 0x8]
		push edx
		mov ecx, pThis
		mov edi, ecx
		call pCall
		mov ecx, edi
		call v2
	}

}

ULONG WINAPI CDispatchRecv::extMetadataParsing(PTAG_PEB_STACK Stack)
{
	VMProtectBegin(__FUNCTION__);


	ULONG v1           = (ULONG)Stack->EBP;
	PVOID pBuffer      = NULL;
	DWORD dwBufferSize = 0;
	DWORD dwSrcPoint   = 0;
	DWORD dwSrcSize    = 0;

	// [12/20/2021 007]
	// 韩服更新 解压前的 Size = 94->80  压缩包的的Buff = 78->74 ,Size 74->70 
	// [2/16/2022 007]
	// a8 = 0x pk包
	// a0 = size
	// b8 = src size
	// 9c = src buf
	__asm
	{
		pushad
		mov edx, v1
		mov eax, dword ptr ss : [edx - 0x0000009c]	// 解压后的buff
		lea ecx, pBuffer
		mov[ecx], eax
		mov edx, v1
		mov eax, dword ptr ss : [edx - 0x000000b8]	//解压后的buff大小
		mov dwBufferSize, eax
		mov eax, dword ptr ss : [edx - 0x000000a8]	//压缩文件buff
		mov dwSrcPoint, eax
		mov eax, dword ptr ss : [edx - 0x000000a0]	//压缩文件大小
		mov dwSrcSize, eax
		popad
	}

	auto lPackRead = std::make_shared<PackReader>((BYTE*)pBuffer, dwBufferSize);

	//解析zip 
	auto lPackZip = std::make_shared<PackReader>((BYTE*)dwSrcPoint, dwSrcSize);


	int nPackFlag  = lPackZip->ReadChar();
	short nZipSize = lPackZip->ReadShort();

#pragma pack (1)
	typedef struct ZIP_HEART
	{
		int		header_flag;	            //文件头标识，值固定(0x04034b50)
		short	version;		            //解压文件所需 pkware最低版本
		short	bit_flag;		            //通用比特标志位(置比特0位=加密)
		short	compression_method;         //压缩方式
		short	file_last_modification_time;//文件最后修改时间
		short	file_last_modification_date;//文件最后修改日期
		int		crc32;                      //CRC-32校验码
		int		compressed_size;            //压缩大小
		int		uncompressed_size;          //未压缩的大小
		short	file_name_length;	        //文件名长度
		short	extra_field_length;	        //扩展区长度

	}TAG_ZIP_HEART, * PTAG_ZIP_HEART;
#pragma pack()

	PTAG_ZIP_HEART zip = (PTAG_ZIP_HEART)lPackZip->ReadBytes(sizeof(TAG_ZIP_HEART));

	char szFileName[MAX_PATH] = { 0 };

	strncpy(szFileName, (char*)lPackZip->ReadBytes(zip->file_name_length), zip->file_name_length);

#if ANTI_DEBUG
	auto ACEFile = std::make_shared<CACEFile>();
	int day      = zip->file_last_modification_date & 0x1F;
	int month    = zip->file_last_modification_date >> 5 & 0xF;
	int year     = zip->file_last_modification_date >> 9 & 0x7F;

	/*
	*	curpath+Metedata+date+filename
	*/
	std::string Path = fmt::format(xorstr_("{}Metedata"), ACEFile->GetCurrentDirectoryA());

	std::string FileName = fmt::format(xorstr_("{}\\{}-{}-{}-{}"), Path,year + 1980, month, day, szFileName);

	//创建路径
	CreateDirectoryA(Path.data(), nullptr);

	ACEFile->SetFileName(FileName);

	if (ACEFile->OpenFile(CREATE_ALWAYS) != INVALID_HANDLE_VALUE)
	{
		ACEFile->WriteFile(pBuffer, dwBufferSize);
	}
	ACEFile->CloseFile();

#if 1
	//Resource/cso_chn.txt
	CreateInterfaceFn filesystem_factory = CaptureFactory(("FileSystem_Nar.dll"));
	IFileSystem* g_pFileSystem = (IFileSystem*)(CaptureInterface(filesystem_factory, ("VFileSystem009")));

	FileHandle_t hFile = g_pFileSystem->Open(("resource\\cso_chn.txt"), "rb", NULL);
	if (!hFile)
	{
		hFile = g_pFileSystem->Open(("resource\\cso_koreana.txt"), "rb", NULL);

	}

	int FileSize = g_pFileSystem->Size(hFile);

	std::vector<BYTE> VecText(FileSize);

	g_pFileSystem->Read(VecText.data(), FileSize, hFile);

	ACEFile->SetFileName("chn_txt007.txt");
	ACEFile->OpenFile();
	ACEFile->WriteFile(VecText.data(), FileSize);
	ACEFile->CloseFile();

	//hFile = g_pFileSystem->Open(("resource\\item.csv"), "rb", NULL);
	//FileSize = g_pFileSystem->Size(hFile);
	//std::vector<BYTE> VecTextCsv(FileSize);

	//ACEFile->SetFileName("item007.csv");
	//ACEFile->OpenFile();
	//g_pFileSystem->Read(VecText.data(), FileSize, hFile);
	//ACEFile->WriteFile(VecText.data(), FileSize);
	//ACEFile->CloseFile();

#endif // ANTI_DEBUG











#endif

	std::string str = (char*)pBuffer;

	if (lstrcmpiA(szFileName, xorstr_("progress_unlock.csv")) != 0)
	{
		//if (lstrcmpiA(szFileName, xorstr_("badwordadd.csv")) == 0)
		//{
		//	lPackRead->Clear(0, strlen(szFileName));
		//}

		return 0;
	}

	std::regex r("([0-9]{1,}[,])(([0-9]+[.][0-9]*)|([1-1]+))[,]");

	std::sregex_iterator pos(str.cbegin(), str.cend(), r);
	std::sregex_iterator end;

	for (; pos != end; ++pos)
	{
		std::string Value = pos->str(2);

		int Index = str.find(pos->str(0));

		if (Index == std::string::npos)
			continue;

		//检查itemID是否是近身武器
		std::string ItemId = pos->str(1);
		int nItemID = std::stoi(ItemId.substr(0, ItemId.find(","))) ;
		
		//近身武器
		if (g_pUtil->GetItemMap()->at(nItemID)->SortingIndex != 8)
			continue;

		const char* VTextP = "0.00000000000000";

		//当前位置+item的长度 = 百分率 
		Index += pos->str(1).length();

		str.replace(Index, Value.length(), VTextP, Value.length());

		lPackRead->WriteBuffer(Index, (BYTE*)VTextP, Value.length());

	}

	g_pUtil->GameNoticeBox(g_GameCFG.GetMsgSubItem(xorstr_("#MSG_Notice_ProgressUnLock_Succeed")));


	VMProtectEnd();

	return 0;
}

void __declspec(naked)  Naked_PacketDispatch_DEBUG()
{
	__asm
	{
		pushad
		push esp
		call CDispatchRecv::DEBUG_PacketDispatch
		popad
	}
}
ODG_EXPORT void __declspec(naked)  Naked_PacketDispatch()
{
	__asm
	{
		pushad
		push esp
		call CDispatchRecv:: PacketDispatch
		popad
		ret 8
	}
}

bool CDispatchRecv::InitializeHook()
{
	auto AceHook  =  g_pUtil->GetAceHook();
	auto AceMemory = g_pUtil->GetAceMemory();


	ULONG DispatchRecvAddres = 0;

	if (AceMemory->GetScanCode(xorstr_("50 FF D2 84 C0 74 07"), DispatchRecvAddres, g_Offset.GetEngineModuleInfo()->hModule))
	{
		ULONG uAsmLenght = AceHook->GetDisasmLenght(DispatchRecvAddres, 5);

		m_SeCallAddress = AceHook->GetCallAddress(DispatchRecvAddres + uAsmLenght);


#if ANTI_DEBUG
		AceHook->HookWithNaked(DispatchRecvAddres, 5, Naked_PacketDispatch_DEBUG);
#endif // ANTI_DEBUG

		ACEDebugFileLog("{}  DispatchRecvAddres :{:X} m_SeCallAddress:{:X} AsmLenght:{:X}", __FUNCTION__, DispatchRecvAddres, m_SeCallAddress, uAsmLenght);
	}
	

	if (AceMemory->GetScanCode(xorstr_("56 8B F1 8B 0D ?? ?? ?? ?? 8B 06 FF 70 F4 50"), m_PacketInfo, g_Offset.GetEngineModuleInfo()->hModule))
	{
		m_PacketInfo = *(PDWORD)(m_PacketInfo + 5);
	}

	for (size_t i = 0; i < 0xFF; i++)
	{
		Debug_Packet((Packet_Type)i);
	}

	for (int i = 0; i < sizeof(GamePacketList) / sizeof(GAME_PACKET_TYPE_INFO); i++)
	{
		if (GamePacketList[i].Hook)
		{
			InstallHook(GamePacketList[i].Type, new GAME_PACKET_TYPE_INFO_HOOK((ULONG)Naked_PacketDispatch));
		}
	}





	return true;
}
void CDispatchRecv::Debug_Packet(Packet_Type type)
{

#if ANTI_DEBUG

	ULONG Ecx = (ULONG)GetPackThis(type);

	if (Ecx == 0)
	{
		return;
	}

	ULONG v2 = *(PULONG)(Ecx + 0xC);
	ULONG funs = 0;
	ULONG Addres = *(PULONG)(*(PULONG)Ecx + 0x8);;

	if (v2)
	{
		ULONG v3 = *(PULONG)(v2 + 0x8);
		funs = *(PULONG)(*(PULONG)(v3)+0x4);

	}
	BOOL Status = FALSE;

	for (int i = 0; i < sizeof(GamePacketList) / sizeof(GAME_PACKET_TYPE_INFO); i++)
	{
		if (GamePacketList[i].Type == type)
		{
			Status = TRUE;
			break;
		}
	}

	if (!Status)
	{
		ACEErrorFileLog("Index:{} {:X} funs:{:X}", (ULONG)type, Addres, funs);
	}
#endif
}


BOOL CDispatchRecv::InstallHook(ULONG Index, GAME_PACKET_TYPE_INFO_HOOK* pInfo)
{
	ULONG v1 = *(PULONG)m_PacketInfo + Index * 4 + 0x10;

	ULONG Result = *(PULONG)v1;

	if (!Result)
	{
		return FALSE;
	}
	ULONG SavaAddres = *(PULONG)(*(PULONG)Result + 0x8);
	for (int i = 0; i < sizeof(GamePacketList) / sizeof(GAME_PACKET_TYPE_INFO); i++)
	{
		if (GamePacketList[i].Type == (Packet_Type)Index)
		{
			GamePacketList[i].SvAddres = SavaAddres;
			break;
		}
	}
	ULONG v3 = pInfo->v3;
	//修改地址 保存父类指针
	memcpy(pInfo, (PVOID)(ULONG)(*(PULONG)(Result) - 4 ), sizeof(GAME_PACKET_TYPE_INFO_HOOK));
	pInfo->v3 = v3;
	//当前地址 + 4 = 当前类
	*(PULONG)Result = (ULONG)pInfo + 4;




	return TRUE;
}
void CDispatchRecv::InterfaceCommandPacketSuperRetRoom()
{
	VMProtectBegin(__FUNCTION__);

	cl_entity_s* Entity = Engine::g_Engine.GetLocalPlayer();

	if (Entity && Entity->index)
	{
		hud_player_info_t Info;

		Engine::g_Engine.pfnGetPlayerInfo(Entity->index, &Info);

		if (Info.mark)
		{
			PackWriter Pack = PackWriter(0xC);
			Pack.WriteByte(0x3);
			Pack.WriteInt(Info.mark);
			Pack.WriteInt(Info.mark);

			PTAG_PEB_STACK Stack = GetPacketStack(Packet_Kick);

			CallGamePacket(Pack.GetBytes(), Pack.GetBytesLength(), Stack, GetPacketTypeInfo(Packet_Kick));
		}
	}

	VMProtectEnd();
}


PSHORT CDispatchRecv::GetItemCount()
{
	return &m_ItemCount;
}

PSHORT CDispatchRecv::GetItemSite()
{
	return &m_ItemSite;
}

ODG_EXPORT int _stdcall CDispatchRecv::Fake_UMsg(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook)
{
	auto lPackRead = std::make_shared<PackReader>((BYTE*)Dst, Size);

	UMsgType Type = static_cast<UMsgType>(lPackRead->ReadChar());

	//ACEDebugFileLog("{} Flag :{}", __FUNCTION__, static_cast<int>(Type));

	//重写MSG封包类型
	auto ResetPackType = [&](int Type)
	{
		ULONG v1 = (ULONG)GetPacketStack(Packet_UMsg)->EDI;
		 *(PULONG)(v1 + 0x28) = Type;
	};


	switch (Type)
	{
	case UMsgType::Chat:
	{
		if (g_Vars.m_Basi.sk_basics_maskwords->value || g_Vars.m_Basi.sk_basics_maskallmsg->value)
		{
			ResetPackType(0xFF);
			return 1;
		}
	}
		break;
	case UMsgType::SysNotice:
		if (g_Vars.m_Basi.sk_basics_masknotice->value || g_Vars.m_Basi.sk_basics_maskallmsg->value)
		{
			ResetPackType(0xFF);
			return 1;
		}
		break;
	case UMsgType::MsgStr:
	{
		if (g_Vars.m_Survial.sk_cata_enable->value && g_Vars.m_Survial.sk_cata_start_game->value)
		{
			std::string Text = lPackRead->ReadStr();
			if (Text  == xorstr_("ROOM_START_GUEST_FAILED"))
			{
				//请等待房主开始游戏

				//-延迟10秒 开始游戏
				auto FunsThreadStartGame = []() {

					Sleep(10 * 1000);
					Engine::g_Engine.pfnClientCmd(xorstr_("+PackStartGame"));
				};

				std::thread t(FunsThreadStartGame);
				t.detach();

				return 1;
			}
		}
	}
		break;
	case UMsgType::AdminNotice:
	{
		std::string Text = lPackRead->ReadStr();

		ACEErrorLog("{}", Text);

		if (Text == xorstr_("NEXONGUARD2_BLOCK"))
		{
			//帐号异常 进入黑名单
		}
		else if (Text == xorstr_("NEXONGUARD2_CUT"))
		{
			//异常断开连接
			auto Text = fmt::format(g_GameCFG.GetMsgSubItem(xorstr_("#MSG_GUAD2_CUT")));
			g_pUtil->GameNoticeBox(Text);
			
			return 1;
		}
		break;
	}
	case UMsgType::NoticeBoard:
		break;	
	case UMsgType::Lottery:
	{
		if (g_Vars.m_Basi.sk_basics_masklottery->value || g_Vars.m_Basi.sk_basics_maskallmsg->value)
		{
			ResetPackType(0xFF);
			return 1;
		}
	}
		break;
	case UMsgType::LIKE:
	{
		ResetPackType(0xFF);
		return 1;
	}
		break;
	default:
		ACEErrorFileLog("{} default Flag :{}", __FUNCTION__, static_cast<int>(Type));
		break;
	}

	CallGamePacket(Dst, Size,  GetPacketStack(Packet_UMsg), pHook);
	return 1;
}

ODG_EXPORT int _stdcall CDispatchRecv::Fake_Kick(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook)
{
	auto lPackRead = std::make_shared<PackReader>((BYTE*)Dst, Size);

	KickType Index = static_cast<KickType>(lPackRead->ReadChar());

	ACEDebugFileLog("{} Index :{}", __FUNCTION__, static_cast<int>(Index));

	switch (Index)
	{
	case KickType::Error:
		break;
	case KickType::ReMain:
		break;
	case KickType::KickInfo:
	{
		if (lPackRead->GetSize() <= 8 )
		{
			break;
		}
		//举报人索引
		ULONG uInformanIndex = lPackRead->ReadInt();
		//被举报人索引
		ULONG uTargetIndex   = lPackRead->ReadInt();

		if (g_Vars.m_Basi.sk_basics_displaykick->value)
		{
			//举报人名
			WCHAR* InformanName = g_pUtil->GetPlayerNameByIndex(uInformanIndex);
			//被举报人姓名
			WCHAR* TargetName = g_pUtil->GetPlayerNameByIndex(uTargetIndex);

			char TextName[MAX_PATH] = { 0 };
			auto Util               = std::make_shared<CACEUtil>();
			Util->UnicodeToAnsi(TargetName, TextName);


			if (lstrcmpiA(TextName,g_Vars.m_UserInfo.m_UserName.data()) ==0 )
			{
				//被举报的是自己的话

				//被举报次数+1
				g_Vars.m_UserInfo.m_KickCount++;

				auto Text = fmt::format(g_GameCFG.GetMsgSubItem(xorstr_("#MSG_Kick_Src")), Util->UnicodeToUtf8(InformanName), g_Vars.m_UserInfo.m_KickCount);

				ULONG uColor[3] = { 0,0,0 };

				if (g_Vars.m_UserInfo.m_KickCount >  GamePlayer::GetInstance()->GetPlayerCount() / 2 )
				{
					//超过房间人数一半 改为红色
					uColor[0] = {255};
				}
				else
				{
					//默认绿色
					uColor[1] = { 255 };
				}

				g_Font.FmtDrawHudString(-1, -1, uColor[0], uColor[1], uColor[2], 5, Text);

				//是否自动反踢回去
				if (g_Vars.m_Basi.sk_basics_displaykick->flags & (1 << 0))
				{
					CDispatchRecv::GetInstance()->m_pGamePack->Kick_RoomPlayer(uInformanIndex);

				}

			}

		}

	}
		break;
	case KickType::KickRoom:
		//和谐了.防踢
		break;
	case KickType::KickInit:
		g_Vars.m_UserInfo.m_KickCount = 0;
		break;
	default:
		break;
	}
	CallGamePacket(Dst, Size, GetPacketStack(Packet_Kick), pHook);
	return 1;
}

ODG_EXPORT int _stdcall CDispatchRecv::Fake_UserStart(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook)
{
	auto lPackRead = std::make_shared<PackReader>((BYTE*)Dst, Size);


	int v1             = lPackRead->ReadInt();
	char* user_account = lPackRead->ReadStr();
	char* player_name  = lPackRead->ReadStr();

	if (g_Vars.m_UserInfo.m_UserAccount.empty())
	{
		g_Vars.m_UserInfo.m_UserAccount = std::string(user_account);
		g_Vars.m_UserInfo.m_UserName    = std::string(player_name);

		ACEDebugFileLog("[{}] Account:{} Name:{}", __FUNCTION__, g_pUtil->GetAceTool()->UTF8_To_string(g_Vars.m_UserInfo.m_UserAccount), g_pUtil->GetAceTool()->UTF8_To_string(g_Vars.m_UserInfo.m_UserName));

		//简单写写算了
		if (CDispatchRecv::GetInstance()->CheckLoginUserBlackList())
		{
			MessageBox(0, xorstr_("请不要拿别人的号开挂！谢谢！"), 0, 0);
			exit(0);
		}




	}
	CallGamePacket(Dst, Size, GetPacketStack(Packet_UserStart), pHook);
	return 1;
}

ODG_EXPORT int _stdcall CDispatchRecv::Fake_UpRoom(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook)
{
	auto lPackRead = std::make_shared<PackReader>((BYTE*)Dst, Size);

	UpRoomType Index = static_cast<UpRoomType>(lPackRead->ReadChar());

	//ACEDebugFileLog("{} Index :{}", __FUNCTION__, static_cast<int>(Index));

	switch (Index)
	{
	case UpRoomType::RoomInfo:
	{
		int Flag        = lPackRead->ReadChar();
		ULONG RoomIndex = lPackRead->ReadInt();

		g_Vars.m_Basi.sk_basics_antikick->flags = RoomIndex;

		ULONG ulRoomUn_1 = lPackRead->ReadInt();		//和是不是房主有关系

		BOOL dwIshouseOwner = ulRoomUn_1 == *(PDWORD)(g_Offset.GetGameAddresInfo()->dwIsHomeowners);

		if (g_Vars.m_Mp.sk_mp_enable->value && g_Vars.m_Mp.sk_mp_mode->value && !dwIshouseOwner)
		{
			//g_Vars.m_Mp.sk_mp_enable->value = 0;
			g_Vars.m_Mp.sk_mp_mode->value   = 0;
			//auto Text = fmt::format(g_GameCFG.GetMsgSubItem(xorstr_("#MSG_MP_Detect")));
			//CDispatchRecv::GetInstance()->UMsgNotice(Text);
		}

	}
		break;
	case UpRoomType::RoomPlayerCount:
		break;
	case UpRoomType::RoomGameEnd:
	{
		if (g_Vars.m_Survial.sk_cata_enable->value && g_Vars.m_Survial.sk_cata_start_game->value)
		{
			//-延迟10秒 开始游戏
			auto FunsThreadStartGame = []() 
			{
				Sleep(10 * 1000);
				Engine::g_Engine.pfnClientCmd(xorstr_("+PackStartGame"));
			};

			std::thread t(FunsThreadStartGame);
			t.detach();
		}
	}
		break;
	case UpRoomType::RoomKick:
		if (g_Vars.m_Basi.sk_basics_roomkick->value)
		{
			return 1;
		}
		break;
	case UpRoomType::RoomKickExp:
	{
		if (g_Vars.m_Basi.sk_basics_superretroom->value)
		{
			ULONG Exp  = lPackRead->ReadInt();

			ULONG Gold = lPackRead->ReadInt();

			auto Text = fmt::format(g_GameCFG.GetMsgSubItem(xorstr_("#MSG_SupRetRoom_Src")), Exp, Gold);

			g_pUtil->GameNoticeBox(Text);

			return 1;
		}
	}
		break;
	case UpRoomType::RoomWeaponRate:
		break;
	default:
		break;
	}




	

	CallGamePacket(Dst, Size, GetPacketStack(Packet_UpROOM), pHook);
	return 1;
}


void __declspec(naked)  Naked_Metadata()
{
	__asm
	{
		pushad
		push esp
		call CDispatchRecv::extMetadataParsing
		popad

	}
}

ODG_EXPORT int _stdcall CDispatchRecv::Fake_Metadata(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook)
{
	auto lPackRead = std::make_shared<PackReader>((BYTE*)Dst, Size);


#if ANTI_DEBUG
	if (1)
#else
	if (g_Vars.m_Basi.sk_basics_zbsunlock->value)
#endif
	{
		static bool bInitHookMetadata = false;

		if (!bInitHookMetadata)
		{
			bInitHookMetadata = true;

			auto AceHook   = g_pUtil->GetAceHook();
			auto AceMemory = g_pUtil->GetAceMemory();


			DWORD dwMetadataParsing = 0;
			if (AceMemory->GetScanCode(xorstr_("E8 ?? ?? ?? ?? 8B 85 ?? FF FF FF C6 45 FC 01"), dwMetadataParsing, g_Offset.GetEngineModuleInfo()->hModule))
			{
				dwMetadataParsing += AceHook->GetDisasmLenght(dwMetadataParsing);

				AceHook->HookWithNaked(dwMetadataParsing, 0x6, Naked_Metadata);
			}

		}

	}

	CallGamePacket(Dst, Size, GetPacketStack(Packet_Metadata), pHook);
	return 1;
}


ODG_EXPORT int _stdcall CDispatchRecv::Fake_Inventory(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook)
{
	auto lpStack = GetPacketStack(Packet_Inventory);

	CDispatchRecv* pDispatch = CDispatchRecv::GetInstance();
	pDispatch->m_lPackInventory->Clear();
	pDispatch->m_lPackInventory->WriteBytes((PBYTE)Dst, Size);
	auto lPackRead = std::make_shared<PackReader>(pDispatch->m_lPackInventory->GetBytes(), pDispatch->m_lPackInventory->GetBytesLength());

	int* Array = (int*)lpStack->ECX;

	if (Array[6] == 0x9A)
	{
		lPackRead->ReadInt();
	}

	//读取道具总数量
	*pDispatch->GetItemCount() = lPackRead->ReadShort();

	//应该有校检仓库的数据.因为每次进房间都会重新获取一遍.等有时间逆一遍

	static ULONG ulFlag = (1 << 0)| (1 << 1)| (1 << 2)| (1 << 3) | (1 << 4);

	if (g_Vars.m_Basi.sk_basics_inventory_role->value)
	{
		pDispatch->AddInventory(ClassType_Class,0, lPackRead.get());

		if (ulFlag & (1 << 0))
		{
			pDispatch->UMsgNotice(xorstr_("007"), xorstr_("免费使用以下[角色],有几率掉线."), ClassType_Class);
			ulFlag &= ~(1 << 0);
		}
	}
	if (g_Vars.m_Basi.sk_basics_damageskin->value)
	{
		ULONG uSortingIndex = 2007;

		if (g_pUtil->GetVersion() == GameVer::KR220421)
		{
			uSortingIndex = 120;
		}

		pDispatch->AddInventory(ClassType_PeriodicItem, uSortingIndex, lPackRead.get());
		if (ulFlag & (1 << 1))
		{
			pDispatch->UMsgNotice(xorstr_("007"), xorstr_("免费使用以下[伤害字体],在游戏选项控制台里设置字体"), ClassType_PeriodicItem, uSortingIndex);
			ulFlag &= ~(1 << 1);
		}
	}
	if (g_Vars.m_Basi.sk_basics_inventory_spray->value)
	{
		ULONG uSortingIndex = 2002;

		if (g_pUtil->GetVersion() == GameVer::KR220421)
		{
			uSortingIndex = 30;
		}

		pDispatch->AddInventory(ClassType_PeriodicItem, uSortingIndex, lPackRead.get());

		if (ulFlag & (1 << 2))
		{
			pDispatch->UMsgNotice(xorstr_("007"), xorstr_("免费使用以下[喷漆],在游戏选项控制台里设置喷漆"), ClassType_PeriodicItem, uSortingIndex);
			ulFlag &= ~(1 << 2);
		}
		
	}
	if (g_Vars.m_Basi.sk_basics_inventory_weapon->value)
	{
		pDispatch->AddInventory(ClassType_Equipment, 0, lPackRead.get());
		if (ulFlag & (1 << 3))
		{
			pDispatch->UMsgNotice(xorstr_("007"), xorstr_("获得以下[装备],只能在本地使用,多人模式无法使用！"), ClassType_Equipment, 0);
			ulFlag &= ~(1 << 3);
		}
		
	}
	if (g_Vars.m_Basi.sk_basics_respawneffect->value)
	{
		pDispatch->AddInventory(ClassType_zbRespawnEffect, 0, lPackRead.get(), g_Vars.m_Basi.sk_basics_respawneffect->flags);
		if (ulFlag & (1 << 4))
		{
			pDispatch->UMsgNotice(xorstr_("007"), xorstr_("获得以下[复活特效]"), ClassType_zbRespawnEffect, 0);
			ulFlag &= ~(1 << 4);
		}

	}
	
	CallGamePacket(pDispatch->m_lPackInventory->GetBytes(), pDispatch->m_lPackInventory->GetBytesLength(), lpStack, pHook);
	return 1;
}


BYTE* CDispatchRecv::AddInventoryItem(PackWriter* lPackWrite, short uItemId)
{
	CDispatchRecv* pDispatch = CDispatchRecv::GetInstance();
	PSHORT Count             = pDispatch->GetItemCount();
	(*Count)++;
	PSHORT Site              = pDispatch->GetItemSite();

	lPackWrite->WriteShort((*Site)++);
	lPackWrite->WriteByte(0x1);
	lPackWrite->WriteShort(uItemId);
	lPackWrite->WriteShort(0x0001);		//数量
	lPackWrite->WriteByte(0x01);
	lPackWrite->WriteByte(0x01);
	lPackWrite->WriteInt(0);			//获取时间
	lPackWrite->WriteInt(0);			//到期时间

	lPackWrite->WriteShort(0x0);				//属性
	/*
	命中率 = 8
	伤害 = 7
	后坐力 = 6
	重量 =5
	连射性 = 4
	弹药 = 3*/

	lPackWrite->WriteShort(0);				//彩漆 short
	lPackWrite->WriteShort(0);

	lPackWrite->WriteInt(0);					//v8
	lPackWrite->WriteInt(0);					//v9
	lPackWrite->WriteByte(0x00);
	lPackWrite->WriteInt(0);					//v11
	lPackWrite->WriteByte(0x00);				//武器配件数量

	lPackWrite->WriteByte(0x01);				//v13

	//2021-08-07 07:07:22 更新后
	lPackWrite->WriteInt(0);
	lPackWrite->WriteByte(0);					//v15

	return lPackWrite->GetBytes();
}

BYTE* CDispatchRecv::AddInventory(Game_ClassType Type, int SortingIndex, PackReader* Info, ULONG ItemId)
{
	//m_lPackInventory->Clear();
	////原始拷贝
	//m_lPackInventory->WriteBytes(lPackRead->GetBuffer(), lPackRead->GetSize());

	for (std::map<int, PGAME_ITEMMAP>::iterator iter = g_pUtil->GetItemMap()->begin(); iter != g_pUtil->GetItemMap()->end(); iter++)
	{
		if (iter->second->RecourceType == Type)
		{
			if (SortingIndex)
			{
				if (iter->second->SortingIndex != SortingIndex)
				{
					continue;
				}
			}
			if (ItemId)
			{
				//如果当前item ≠ 则跳出
				if (iter->first != ItemId)
				{
					continue;
				}
			}

			std::wstring ItemName = iter->second->ItemSrc;

			if (ItemName.find(xorstr_(L"CSO_Item")) != ItemName.npos)
			{
				continue;
			}

			AddInventoryItem(m_lPackInventory.get(), iter->first);

		}

	}

	// 重写 count 

	ULONG Offset = m_lPackInventory->GetBytesLength();

	m_lPackInventory->SetOffset(0);

	m_lPackInventory->WriteShort(m_ItemCount);

	m_lPackInventory->SetOffset(Offset);

	return m_lPackInventory->GetBytes();
}

void CDispatchRecv::UMsgNotice(std::string Title, std::string Text, Game_ClassType Type, int SortingIndex /*= 0*/)
{
	PackWriter Pack = PackWriter(1024 * 100);

	Pack.Clear();

	Pack.WriteByte(0x3D);

	Pack.WriteShort(0x01);

	std::vector<ULONG> vecItemIndex;
	for (std::map<int, PGAME_ITEMMAP>::iterator iter = g_pUtil->GetItemMap()->begin(); iter != g_pUtil->GetItemMap()->end(); iter++)
	{
		if (iter->second->RecourceType == Type)
		{
			if (SortingIndex)
			{
				if (iter->second->SortingIndex != SortingIndex)
				{
					continue;
				}
			}

			std::wstring ItemName = iter->second->ItemSrc;

			if (ItemName.find(xorstr_(L"CSO_Item_")) != ItemName.npos)
			{
				continue;
			}
			

			vecItemIndex.push_back(iter->first);
		}

	}

	std::sort(vecItemIndex.begin(), vecItemIndex.end(), std::greater<ULONG>());

	for (auto& itr : vecItemIndex)
	{
		AddUmsgNoticeItem(&Pack, itr);
	}

	ULONG Offset = Pack.GetBytesLength();
	Pack.SetOffset(1);

	Pack.WriteShort(vecItemIndex.size());
	Pack.SetOffset(Offset);

	// 共10个
	Pack.WriteInt(0x00);
	Pack.WriteInt(0x00);
	Pack.WriteByte(0);

	if (g_Offset.GetGameServer() != Chinese)
	{
		Pack.WriteByte(0);
	}
	

	Pack.WriteString(Title);
	Pack.WriteString(Text);
	Pack.WriteByte(0);

	PTAG_PEB_STACK Stack = GetPacketStack(Packet_UMsg);

	Stack->ECX = GetPackThis(Packet_UMsg);

	CallGamePacket2(Pack.GetBytes(), Pack.GetBytesLength(), Stack, GetPacketTypeInfo(Packet_UMsg));
}

void CDispatchRecv::UMsgNotice(std::string Text, UMsgType Type /*= UMsgType::Notice*/)
{
	PackWriter Pack = PackWriter(1024 * 100);

	Pack.Clear();

	Pack.WriteByte(static_cast<ULONG>(Type));

	Pack.WriteString(Text.data());

	Pack.WriteByte(0);

	PTAG_PEB_STACK Stack = GetPacketStack(Packet_UMsg);

	Stack->ECX = GetPackThis(Packet_UMsg);

	CallGamePacket2(Pack.GetBytes(), Pack.GetSize(), Stack, GetPacketTypeInfo(Packet_UMsg));
}

PackWriter* CDispatchRecv::AddUmsgNoticeItem(PackWriter* lPackWrite, ULONG Item)
{
	lPackWrite->WriteShort(Item);
	lPackWrite->WriteShort(0x01);
	lPackWrite->WriteShort(0x01);
	lPackWrite->WriteShort(0x00);
	lPackWrite->WriteInt(0x00);
	lPackWrite->WriteShort(0x00);
	lPackWrite->WriteByte(0);



	return lPackWrite;
}

PVOID CDispatchRecv::GetPackThis(Packet_Type type)
{
	ULONG v1 = *(PULONG)m_PacketInfo + type * 4 + 0x10;

	return (PVOID) * (PULONG)v1;
}



ODG_EXPORT int _stdcall CDispatchRecv::Fake_Quest(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook)
{
	auto lPackRead = std::make_shared<PackReader>((BYTE*)Dst, Size);

	ULONG Index = lPackRead->ReadChar();

	ACEDebugLog("{} Index:{}",__FUNCTION__, Index);

	switch (Index)
	{
	case 0xC:
	{
		int QuestId = lPackRead->ReadInt();

		BYTE v1 = lPackRead->ReadChar();
		BYTE v2 = lPackRead->ReadChar();

		auto QuestName = fmt::format(xorstr_("CSO_Quest{}_Name"), QuestId);

		auto AnsiName = g_pUtil->GetAceTool()->WstringToString(g_pUtil->GetItemName(QuestName.data(), ""));

		QuestName = g_pUtil->GetAceTool()->string_To_UTF8(AnsiName);

		//ACEDebugLog("[{}] id:{} v1:{} v2:{} Quest:{}", __FUNCTION__, QuestId, v1,v2, AnsiName);

		if (v2 & 0x4)
		{
			int nCount = lPackRead->ReadChar();

			//ACEDebugLog("[{}] nCount:{}", __FUNCTION__, nCount);

			for (int i = 0; i < nCount; i++)
			{
				int k = lPackRead->ReadInt();

				//ACEDebugLog("[{}] k:{}",__FUNCTION__, k);
			}

		}
		if (v2 & 0x8)
		{
			int v3 = lPackRead->ReadInt();

			//ACEDebugLog("[{}] v3:{}", __FUNCTION__, v3);

			if (g_Vars.m_Basi.sk_basics_quest->value)
			{
				auto Text = fmt::format("{}->{}", AnsiName, v3);

				g_pUtil->GameNoticeBox(g_pUtil->GetAceTool()->string_To_UTF8(Text));
			}

		}

		//ACEDebugLog(__FUNCTION__);
	}
	break;
	default:
		break;
	}




	CallGamePacket(Dst, Size, GetPacketStack(Packet_Quest), pHook);
	return 1;

}

ODG_EXPORT int _stdcall CDispatchRecv::Fake_Option(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook)
{
	auto lPackRead = std::make_shared<PackReader>((BYTE*)Dst, Size);


	if (g_Vars.m_Basi.sk_basics_console_option->value)
	{
		static bool Status = true;

		if (Status)
		{
			Status = !Status;
			CDispatchRecv::GetInstance()->UMsgNotice(xorstr_("Restore default console confilg success"), UMsgType::SysNotice);
			return 1;
		}

	}

	CallGamePacket(Dst, Size, GetPacketStack(Packet_Option), pHook);
	return 1;
}

ODG_EXPORT int _stdcall CDispatchRecv::Fake_Null(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook)
{

	return 1;
}

ODG_EXPORT int _stdcall CDispatchRecv::Fake_Ribbon(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook)
{
	//自动屏蔽傻逼的成就记录
	return 1;
}

ODG_EXPORT int _stdcall CDispatchRecv::Fake_Hack(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook)
{
	CSagaClient::GetInstance()->HeartBeat();

	CallGamePacket(Dst, Size, GetPacketStack(Packet_Hack), pHook);
	return 1;
}

ODG_EXPORT int _stdcall CDispatchRecv::Fake_Host(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook)
{

	//if (g_Vars.TestValue)
	//{
	//	auto ResetPackType = [&](int Type)
	//	{
	//		ULONG v1 = (ULONG)GetPacketStack(Packet_Host)->EDI;
	//		*(PULONG)(v1 + 0x1C) = Type;
	//	};

	//	ResetPackType(0xFF);
	//	return 1;
	//}
	auto lPackRead = std::make_shared<PackReader>((BYTE*)Dst, Size);

	if (g_Vars.m_Mp.sk_mp_enable->value && g_Vars.m_Mp.sk_mp_mode->value)
	{
		char Flag = lPackRead->ReadChar();

		switch (Flag)
		{
		case 0:
			lPackRead->ReadInt();
			lPackRead->WriteInt(CDispatchRecv::GetInstance()->GetServerOid());
			break;
		case 5:
			lPackRead->WriteBYTE(-1);
			break;
		default:
			break;
		}
	}




	CallGamePacket(Dst, Size, GetPacketStack(Packet_Host), pHook);
	return 1;
}

ODG_EXPORT int _stdcall CDispatchRecv::Fake_Item(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook)
{
	auto lPackRead = std::make_shared<PackReader>((BYTE*)Dst, Size);

#if 1
	if (Size >= 3)
	{
		UCHAR uFlag = lPackRead->ReadChar();
		switch (uFlag)
		{
		case 8:
		{
			UCHAR uV1 = lPackRead->ReadChar();
			UCHAR uV2 = lPackRead->ReadChar();

			// 在线时间已满10分钟 可以领取补给
			CGamePack::GetInstance()->Item_Roulette();

			//可以当做每日回调..
			break;
		}
		default:
			break;
		}

	}
#endif
	
	CallGamePacket(Dst, Size, GetPacketStack(Packet_Item), pHook);
	return 1;
}

ODG_EXPORT int _stdcall CDispatchRecv::Fake_ClientCheck(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook)
{

	//CSagaClient::GetInstance()->HeartBeat();

	CallGamePacket(Dst, Size, GetPacketStack(Packet_ClientCheck), pHook);

	return 1;
}

ODG_EXPORT int _stdcall CDispatchRecv::Fake_Transfer(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook)
{

	CallGamePacket(Dst, Size, GetPacketStack(Packet_Transfer), pHook);
	return 1;
}

int CDispatchRecv::GetServerOid()
{
	VMProtectBegin(__FUNCTION__);


	static DWORD dwServerAddres = 0;

	if (dwServerAddres == 0)
	{
		g_pUtil->GetAceMemory()->GetScanCode (xorstr_("a1 ?? ?? ?? ?? 8b 48 04 8b 01 ff 20"), dwServerAddres, g_Offset.GetEngineModuleInfo()->hModule);
		dwServerAddres = *(PDWORD)(dwServerAddres + 1);

		ACEDebugFileLog("[{}] dwServerAddres:0x{:X}",__FUNCTION__,dwServerAddres);
	}
	__asm
	{
		mov ecx, dword ptr ds : [dwServerAddres]
		mov eax, dword ptr ds : [ecx]
		mov ecx, dword ptr ds : [eax + 0x4]
		mov eax, dword ptr ds : [ecx + 0x34]
	}
	VMProtectEnd();
}

BOOL CDispatchRecv::CheckLoginUserBlackList()
{
	VMProtectBegin(__FUNCTION__);

	BOOL bRet = FALSE;

	//g_Vars.m_UserInfo.m_UserAccount 

	if (g_Offset.GetGameServer() == Chinese)
	{
		//租号黑名单判断.
		CACEFile File;
		CACEHook Hook;
		File.SetFileName(fmt::format(xorstr_("{}\\blacklist.pak"), Hook.GetModulePath(Fake_UserStart)));

		auto hFile = File.OpenFile();

		if (hFile != INVALID_HANDLE_VALUE)
		{
			auto VecBuffer = File.ReadFile();
			auto lPackRead = std::make_shared<PackReader>(VecBuffer);
			char szText[1024] = {0};

			while (lPackRead->ReadStrLine(szText))
			{
				char* Pos = strchr(szText, '\r');

				if (Pos != nullptr)
				{
					std::string Text = std::string(szText, Pos);

					ACEWarningFileLog("Text:{}", Text);

					if (g_Vars.m_UserInfo.m_UserAccount == Text)
					{
						bRet = TRUE;
						break;
					}

				}

			}
		}

	}

	VMProtectEnd();
	return bRet;
}
