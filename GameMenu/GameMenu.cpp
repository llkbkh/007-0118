// GameMenu.cpp: 定义 DLL 的初始化例程。
//

#include "pch.h"
#include "framework.h"
#include "GameMenu.h"
#include <PackReader.h>
#include "MenuHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO:  如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如: 
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。  这意味着
//		它必须作为以下项中的第一个语句:
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CGameMenuApp

BEGIN_MESSAGE_MAP(CGameMenuApp, CWinApp)
END_MESSAGE_MAP()


// CGameMenuApp 构造

CGameMenuApp::CGameMenuApp()
{
	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中

	TabDlg = nullptr;

}


// 唯一的 CGameMenuApp 对象

CGameMenuApp theApp;


// CGameMenuApp 初始化

BOOL CGameMenuApp::InitInstance()
{
	CWinApp::InitInstance();

	TAG_ACE_HOOK_MODULE_INFO AppList[] =
	{
		ACE_HOOK_MODULE_INFO("cstrike-online.exe"),
	};

	if (!CACEModule::GetModuleDetect(AppList, sizeof(AppList) / sizeof(TAG_ACE_HOOK_MODULE_INFO)))  return FALSE;


	TabDlg = new CTabDlg();
	TabDlg->Create(IDD_DIALOG_TABDLG);

	return TRUE;
}

void CGameMenuApp::CreateTabDlg()
{
	if (TabDlg)
	{
		TabDlg->Init();
		TabDlg->ShowWindow(SW_SHOW);

	}
}



CTabDlg* CGameMenuApp::GetTabDlg()
{
	return TabDlg;
}

extern "C" void WINAPI Menu_KeyBoardProc(int nCode, int keyNum)
{

	//ACEDebugLog("[{}] {} {}  Hwnd:{:X}",__FUNCTION__, nCode,keyNum, (DWORD)GetActiveWindow());
	static auto mfcMenu = g_pTool->GetVar(xorstr_("sk_mfcMenu_hotkey"));
	static auto weapon = g_pTool->GetVar(xorstr_("sk_mp_weapon"));



	if (keyNum == (mfcMenu->hotkey & 0xFF))
	{
		auto pDlg = theApp.GetTabDlg();

		if (pDlg)
		{
			if (/*g_pTool->GetGameHwnd() == GetActiveWindow()*/1)
			{
				BOOL bVisible = pDlg->IsWindowVisible();
				int nCmdShow = bVisible ? SW_HIDE : SW_SHOW;
				pDlg->ShowWindow(nCmdShow);

			}

		}
	}
	else if (weapon->value && (weapon->hotkey & 0xFF) == keyNum)
	{
		auto pDlg = theApp.GetTabDlg();
		pDlg->GetWeaponDlg()->WeaponSetF();
	}



}

extern "C" void WINAPI Menu_AsyncCallBack(PVOID pBuf,ULONG Size)
{
	if (Size)
	{
		auto lReadPack             = std::make_shared<PackReader>((PBYTE)pBuf, Size);
		CALLBACK_TYPE Type         = (CALLBACK_TYPE)lReadPack->ReadChar();

		switch (Type)
		{
		case TYPE_MENUDLG_INIT:  	//初始化
		{
			theApp.CreateTabDlg();
		}
		break;
		case TYPE_VAR_INIT:
		{
			MENU_LANGUAGE Language = (MENU_LANGUAGE)lReadPack->ReadChar();
			g_pTool                = new MenuHelper();
			g_pTool->LoadMenuCFG(Language);
		}
		break;
		case TYPE_CLIENT_INTERFACE:
		{
			g_pTool->SetfunctionIntetface(lReadPack->ReadInt());
		}
		break;
		case TYPE_GAME_WINDOWS_HWND:
		{
			g_pTool->SetGameHwnd((HWND)lReadPack->ReadInt());
		}
		break;

		default:
			break;
		}


	}
}



#pragma comment(linker, "/EXPORT:Menu_KeyBoardProc=_Menu_KeyBoardProc@8")
#pragma comment(linker, "/EXPORT:Menu_AsyncCallBack=_Menu_AsyncCallBack@8")