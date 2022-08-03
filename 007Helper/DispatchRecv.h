#pragma once
#include "007Helper.h"
#include "PacketDef.h"
#include "GamePack.h"


class CDispatchRecv:public IGameClient
{
public:
	CDispatchRecv();
	static CDispatchRecv* GetInstance();

	virtual int  vKeyBoardProc(int nCode, WPARAM wParam, LPARAM lParam);

	virtual bool vInit();
	//*初始化Hook
	bool InitializeHook();
	//*安装Hook
	BOOL InstallHook(ULONG Index, GAME_PACKET_TYPE_INFO_HOOK* pInfo);

	static void _stdcall DEBUG_PacketDispatch(PTAG_PEB_STACK Stack);
	static void _stdcall PacketDispatch(PTAG_PEB_STACK Stack);
	static void CallHookAddres(PTAG_PEB_STACK Stack, PTAG_GAME_PACKET_TYPE_INFO Info);

	static PTAG_PEB_STACK GetPacketStack(Packet_Type type);
	static PTAG_GAME_PACKET_TYPE_INFO GetPacketTypeInfo(Packet_Type type);

	static BYTE CallGamePacket(PVOID Dst, int Size, PTAG_PEB_STACK Stack, PTAG_GAME_PACKET_TYPE_INFO pHook);
	static BYTE CallGamePacket2(PVOID Dst, int Size, PTAG_PEB_STACK Stack, PTAG_GAME_PACKET_TYPE_INFO pHook);

	static ULONG  WINAPI extMetadataParsing(PTAG_PEB_STACK Stack);

	//hook的派发封包功能
	ODG_EXPORT static int _stdcall Fake_UMsg(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook);
	ODG_EXPORT static int _stdcall Fake_Kick(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook);
	ODG_EXPORT static int _stdcall Fake_UserStart(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook);
	ODG_EXPORT static int _stdcall Fake_UpRoom(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook);
	ODG_EXPORT static int _stdcall Fake_Metadata(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook);
	ODG_EXPORT static int _stdcall Fake_Inventory(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook);
	ODG_EXPORT static int _stdcall Fake_Quest(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook);
	ODG_EXPORT static int _stdcall Fake_Option(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook);
	ODG_EXPORT static int _stdcall Fake_Null(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook);
	ODG_EXPORT static int _stdcall Fake_Ribbon(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook);
	ODG_EXPORT static int _stdcall Fake_Hack(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook);
	ODG_EXPORT static int _stdcall Fake_Host(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook);
	ODG_EXPORT static int _stdcall Fake_Item(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook);
	ODG_EXPORT static int _stdcall Fake_ClientCheck(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook);
	ODG_EXPORT static int _stdcall Fake_Transfer(PVOID Dst, int Size, PTAG_GAME_PACKET_TYPE_INFO pHook);
	
	//*添加仓库物品
	BYTE* AddInventoryItem(PackWriter* lPackWrite, short uItemId);

	/*
	*	@添加仓库道具
	*	@Param:资源类型
	*	@Param:Id == 0
	*	@Param:添加指定的道具ID 默认为0
	*/
	BYTE* AddInventory(Game_ClassType Type,int SortingIndex, PackReader* Info, ULONG ItemId = 0);

	/*
	*	@消息道具通知
	*	@Param:标题 GBK
	*	@Param:内容
	*	@Param:道具类型
	*/
	void UMsgNotice(std::string Title, std::string Text, Game_ClassType Type, int SortingIndex = 0);

	/*
	*	@公告通知
	*/
	void UMsgNotice(std::string Text,UMsgType Type = UMsgType::Notice);


	//@添加公告通知 道具项
	PackWriter* AddUmsgNoticeItem(PackWriter* lPackWrite, ULONG Item);

	//@返回Pack包指针
	PVOID GetPackThis(Packet_Type type);

	void Debug_Packet(Packet_Type type);

	int GetServerOid();

	// @检查登陆用户黑名单
	BOOL CheckLoginUserBlackList();


private:
	//*接口命令.超级返回房间
	static void InterfaceCommandPacketSuperRetRoom();

	PSHORT GetItemCount();

	PSHORT GetItemSite();
private:
	static inline CDispatchRecv* m_Instance = nullptr;
	ULONG                        m_PacketInfo;
	CGamePack*					 m_pGamePack;
	//道具数量
	short	                     m_ItemCount;
	//道具标志
	short	                     m_ItemSite;
	std::shared_ptr<PackWriter>  m_lPackInventory;
	//二级地址					
	ULONG						m_SeCallAddress;
	

};

