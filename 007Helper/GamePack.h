#pragma once
#include "007Helper.h"
#include "PacketDef.h"



class CGamePack :public IGameClient
{
public:
	CGamePack();
	static CGamePack* GetInstance();

	virtual bool vInit();



	static int WINAPIV extEVP_EncryptInit(VOID* ctx, const VOID* type, unsigned char* key, unsigned char* iv);
	static int WSAAPI  extSend(SOCKET s, const char* buf, int len, int flags);
	static int WINAPIV extEVP_EncryptUpdate(VOID* ctx, unsigned char* out, int* outl, unsigned char* in, int inl);

	static int WSAAPI FAR extsendto(
		_In_ SOCKET s,
	 const char FAR* buf,
		_In_ int len,
		_In_ int flags,
	 const struct sockaddr FAR* to,
		_In_ int tolen);

	void PostOnDispatch(unsigned char* out, int inl);


	// *举报踢出房间用户
	// @Param:玩家标志
	bool Kick_RoomPlayer(int PlayerSign);

	/*
	*	@切换阵营
	*/
	bool Room_AlterTeam(TEAM Value);

	// @ 离开房间
	bool Room_Disconnect();

	// @ 进入房间
	bool Room_joinRoom(ULONG RoomIndex);

	// @开始游戏
	bool Room_StartGame();

	// @领取每日补给
	bool Item_Roulette();

	// @刷战场纪律
	bool Discipline(std::string name);

	// @三倍经验
	bool DoubleExp();
	
	// @修改玩家人数
	bool Room_ChangePlayerCount(int Value);

	// @游戏结算
	bool Room_GameOver();

private:
	// *生成Pkg包
	bool  GeneratePkgBuffer(PackWriter* lPack);
	std::tuple<bool,BYTE> GetPacketSeq();
	bool SendToPacket(PackWriter* lWriterPack);


	//*接口命令.离开房间
	static void InterfaceDisconnectRoom();
	//*接口命令.加入房间
	static void InterfaceJoinRoom();
	//*接口命令.开始游戏
	static void InterfaceStartGame();

private:
	static inline CGamePack*    m_pInstance = nullptr;
	DWORD	                    m_PackSeqAddress;
	std::shared_ptr<CACEMemory>	m_AceMemory;
	evp_cipher_ctx_st           m_ctx;
	SOCKET                      m_Socket;
	std::string	                m_IDisconnectRoomText;
	std::string	                m_IJoinRoomText;
	std::string	                m_IStartGameText;

};

