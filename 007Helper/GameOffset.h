#pragma once
#include "007Helper.h"

#define GET_MODULE_SIZE(Address) PIMAGE_NT_HEADERS(Address + (DWORD)PIMAGE_DOS_HEADER(Address)->e_lfanew)->OptionalHeader.SizeOfImage;
typedef struct GAME_MODULE_INFO
{
	HMODULE		hModule;
	ULONG		ulModuleSize;
	ULONG		ulModuleEnd;
	GAME_MODULE_INFO(HMODULE hModule)
	{
		this->hModule = hModule;
		this->ulModuleSize = GET_MODULE_SIZE((DWORD)this->hModule);
		this->ulModuleEnd = (ULONG)hModule + ulModuleSize;
	}
	GAME_MODULE_INFO(char * lpModuleName)
	{
		this->hModule = LoadLibraryA(lpModuleName);
		this->ulModuleSize = GET_MODULE_SIZE((DWORD)this->hModule);
		this->ulModuleEnd = (ULONG)hModule + ulModuleSize;
	}

}GAME_MODULE_INFO, *PGAME_MODULE_INFO;

class GameOffset
{
public:
	GameOffset();
	// * 获取模块信息
	BOOL  GetModuleInfo();
	// * 查找 cl_clientfuncs_s 指针
	BOOL FindClientTable();
	// * 查找 cl_enginefunc_t 指针
	BOOL FindEngineTable();
	// * 查找 StudioModelRenderer_t 指针
	void*  FindStudioTable();
	// * 查找 playermove_t	指针
	BOOL FindPlayermove();
	// *进游戏初始化
	BOOL HUD_Frame();
	// * 查找游戏地址
	BOOL FindGameAddres();
	// * 查找EngMsg
	DWORD  FindUserMsgBase();
	// * 查找本地游戏基址
	DWORD  FindGameMpBase();
	

public:
	//返回模块基址
	PGAME_MODULE_INFO	GetEngineModuleInfo() { return m_pEngine; }
	PGAME_MODULE_INFO	GetClientModuleInfo() { return m_pClient; }
	PGAME_MODULE_INFO	GetGameUiModuleInfo() { return m_pGameUi; }
	PGAME_MODULE_INFO	GetServerModuleInfo() { return m_pServer; }
	inline Gamefunctions*		GetGameFunsInfo()	  { return &m_GameFuns; }
	inline GameAddres*			GetGameAddresInfo()   { return &m_GameAddres; }
	inline GameMpAddres*		GetGameMpAddresInfo() { return &m_GameMpAddres; }
	DWORD				GetSpeedPtr()				  { return SpeedPtr;}
	inline BOOL			GetIsMpStaus()				  { return m_IsMpInit; }
	GameServer	GetGameServer()						  { return m_GameServer; }
	GameServer	SetGameServer(GameServer sv);
	// * 检查地址
	BOOL	CheckModuleAddres(PGAME_MODULE_INFO hModule, ULONG Address);
private:
	PGAME_MODULE_INFO	m_pEngine;
	PGAME_MODULE_INFO	m_pClient;
	PGAME_MODULE_INFO	m_pGameUi;
	PGAME_MODULE_INFO	m_pServer;
	PGAME_MODULE_INFO	m_pVgui2;
	GameAddres			m_GameAddres;
	Gamefunctions		m_GameFuns;
	GameMpAddres		m_GameMpAddres;
	void*				SpeedHackPtr();
	void*				AdSpeedPtr();
	DWORD				SpeedPtr;
	BOOL				m_IsMpInit;
	GameServer			m_GameServer;
private:
	std::unique_ptr<CACEHook>   m_Hook;
	std::unique_ptr<CACEMemory> m_Memory;
	

};

extern GameOffset g_Offset;