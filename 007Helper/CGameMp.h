#pragma once
#include "007Helper.h"
#include "CMpEntity.h"

class CGameMp:public IGameClient ,public IGameEngineMSG
{
public:
	CGameMp();
	static CGameMp* GetInstance();

	virtual void vHUD_Frame(double time);
	virtual bool vInit();
	virtual int vInitHUD(const char* pszName, int iSize, void* pbuf);

	static FARPROC WINAPI extGetProcAddress(HMODULE hModule, LPCSTR lpProcName);
	static int __stdcall GiveFnptrsToDll_0(enginefuncs_t* pengfuncsFromEngine, globalvars_t* pglobals);
	static void __stdcall pfnWriteByte(PTAG_PEB_STACK Stack);


	static FARPROC _cdecl extGetEntityAPI(DLL_FUNCTIONS* pFunctionTable, int interfaceVersion);
	static void mp_retroom();


	virtual void vHUD_Redraw(float time, int intermission);
	virtual int  vKeyBoardProc(int nCode, WPARAM wParam, LPARAM lParam);
private:
	static inline CGameMp*      m_pInstance = nullptr;
	std::shared_ptr<CACEHook>	m_Hook;
	std::shared_ptr<CACEMemory>	m_Memory;
	//call 返回地址
	DWORD	                    m_WriteByteCallRetAddress;
	DLL_FUNCTIONS*              m_pFunctionTable;

	//房主本人
	std::shared_ptr<CMpEntity>  m_pMpEntity;

};



