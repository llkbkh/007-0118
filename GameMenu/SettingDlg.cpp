// SettingDlg.cpp: 实现文件
//

#include "pch.h"
#include "GameMenu.h"
#include "afxdialogex.h"
#include "SettingDlg.h"
#include "MenuHelper.h"


// SettingDlg 对话框

IMPLEMENT_DYNAMIC(CSettingDlg, CDialogEx)

CSettingDlg::CSettingDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SETTING, pParent)
	, m_Edit_GameFps(_T(""))
{

}

CSettingDlg::~CSettingDlg()
{
}

void CSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_CHECK_ShowMenuTip, m_EnalbeMenuTips);


	DDX_Control(pDX, IDC_CHECK_BASEFUN, m_Check_BaseFun);
	DDX_Control(pDX, IDC_CHECK_ESPFUN, m_Check_EspFun);
	DDX_Control(pDX, IDC_CHECK_AIMFUN, m_Check_AimFun);
	DDX_Control(pDX, IDC_CHECK_WALLFUN, m_Check_WallFun);
	DDX_Control(pDX, IDC_CHECK_KZFUN, m_Check_KZFun);
	DDX_Control(pDX, IDC_CHECK_SCRIPTFUN, m_Check_ScriptFun);
	DDX_Control(pDX, IDC_CHECK_MPFUN, m_Check_MpFun);
	DDX_Control(pDX, IDC_HOTKEY_MENU, m_HotKey_Menu);
	DDX_Control(pDX, IDC_CHECK_ZSFUN, m_Check_Survival);
	DDX_Control(pDX, IDC_CHECK_SOCCER, m_Check_Soccer);
	DDX_Control(pDX, IDC_CHECK_DEATHBOX, m_Check_DeathBox);
	DDX_Control(pDX, IDC_CHECK_KILLICO, m_Check_KillIco);
	DDX_Control(pDX, IDC_CHECK_GAME_FPS, m_Check_GameFps);
	DDX_Text(pDX, IDC_EDIT_GAME_FPS, m_Edit_GameFps);
	DDX_Control(pDX, IDC_CHECK_ACTIVATE_WINDOW, m_Check_ActvateWindows);
	DDX_Control(pDX, IDC_CHECK_NIGHT, m_Check_Zsht);
	DDX_Control(pDX, IDC_CHECK_LIKE, m_Check_Like);
}



BEGIN_MESSAGE_MAP(CSettingDlg, CDialogEx)
	
	ON_BN_CLICKED(IDC_STATIC_CFG, &CSettingDlg::OnBnClickedStaticCfg)
	ON_BN_CLICKED(IDC_CHECK_ShowMenuTip, &CSettingDlg::OnBnClickedCheckShowmenutip)
//	ON_NOTIFY(NM_OUTOFMEMORY, IDC_HOTKEY1, &CSettingDlg::OnNMOutofmemoryHotkey1)
	ON_BN_CLICKED(IDC_BUTTON_LoadCFG, &CSettingDlg::OnBnClickedButtonLoadcfg)
	ON_BN_CLICKED(IDC_BUTTON_SavaCFG, &CSettingDlg::OnBnClickedButtonSavacfg)
	ON_BN_CLICKED(IDC_BUTTON_DefaultCFG, &CSettingDlg::OnBnClickedButtonDefaultcfg)
	
//	ON_NOTIFY(NM_THEMECHANGED, IDC_HOTKEY1, &CSettingDlg::OnThemechangedHotkey1)
//	ON_WM_HOTKEY()
//ON_WM_CHAR()
//ON_EN_CHANGE(IDC_EDIT1, &CSettingDlg::OnEnChangeEdit1)
//ON_WM_CHAR()
ON_BN_CLICKED(IDC_CHECK_BASEFUN, &CSettingDlg::OnBnClickedCheckBasefun)
ON_BN_CLICKED(IDC_CHECK_ESPFUN, &CSettingDlg::OnBnClickedCheckEspfun)
ON_BN_CLICKED(IDC_CHECK_AIMFUN, &CSettingDlg::OnBnClickedCheckAimfun)
ON_BN_CLICKED(IDC_CHECK_WALLFUN, &CSettingDlg::OnBnClickedCheckWallfun)
ON_BN_CLICKED(IDC_CHECK_KZFUN, &CSettingDlg::OnBnClickedCheckKzfun)
ON_BN_CLICKED(IDC_CHECK_ZSFUN, &CSettingDlg::OnBnClickedCheckZsfun)
ON_BN_CLICKED(IDC_CHECK_KILLICO, &CSettingDlg::OnBnClickedCheckKillico)
ON_BN_CLICKED(IDC_CHECK_DEATHBOX, &CSettingDlg::OnBnClickedCheckDeathbox)
ON_BN_CLICKED(IDC_CHECK_SOCCER, &CSettingDlg::OnBnClickedCheckSoccer)
ON_BN_CLICKED(IDC_CHECK_GAME_FPS, &CSettingDlg::OnBnClickedCheckGameFps)
ON_EN_CHANGE(IDC_EDIT_GAME_FPS, &CSettingDlg::OnEnChangeEditGameFps)
ON_BN_CLICKED(IDC_CHECK_ACTIVATE_WINDOW, &CSettingDlg::OnBnClickedCheckActivateWindow)
ON_BN_CLICKED(IDC_CHECK_NIGHT, &CSettingDlg::OnBnClickedCheckNight)
ON_BN_CLICKED(IDC_CHECK_MPFUN, &CSettingDlg::OnBnClickedCheckMpfun)
ON_BN_CLICKED(IDC_CHECK_LIKE, &CSettingDlg::OnBnClickedCheckLike)
END_MESSAGE_MAP()


// SettingDlg 消息处理程序


BOOL CSettingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();







	return TRUE;  
}


void CSettingDlg::OnBnClickedStaticCfg()
{
	// TODO: 在此添加控件通知处理程序代码

}


void CSettingDlg::OnBnClickedCheckShowmenutip()
{
	int nState =  m_EnalbeMenuTips.GetCheck();

	if (nState != BST_CHECKED)
	{
		int  result = AfxMessageBox(g_pTool->GetMenuItemValueW(xorstr_(L"#Setting_Notice_CloseMenu")), MB_YESNO);	// ::MessageBox(0, g_pUtil->GetMenuItemValueW(xorstr_(L"#Setting_CloseMenuTips")), L"007", MB_YESNO);

		if (result == IDYES)
		{
			
		}
		else
		{
			m_EnalbeMenuTips.SetCheck(BST_CHECKED);
			return;
		}
	}
	g_pTool->GetVar(xorstr_("sk_mfcMenu_enable_tips"))->value = nState;
}



void CSettingDlg::OnBnClickedButtonLoadcfg()
{
	BOOL Status = g_pTool->LoadCFG(2);

	if (Status)
	{
		AfxMessageBox(xorstr_(L"加载配置文件成功"), MB_OK);
		//加载完毕要重新显示一下
		theApp.GetTabDlg()->LoadCFGGui();
	}
	else
	{
		AfxMessageBox(xorstr_(L"加载配置文件失败"), MB_ICONSTOP);
	}
}


void CSettingDlg::OnBnClickedButtonSavacfg()
{
	//
	CACEModule ACEModule;
	auto Path = fmt::format(xorstr_("{}\\cvar7.json"), ACEModule.GetModuleInfo()->ModulePath);

	CACEFile ACEFile(Path);

	if (ACEFile.OpenFile(OPEN_EXISTING) != INVALID_HANDLE_VALUE)
	{
		int result = AfxMessageBox(xorstr_(L"配置文件已存在,你确定要覆盖配置么?"), MB_YESNO);

		if (result != IDYES)
		{
			return;

		}

	}

	BOOL Status = g_pTool->LoadCFG(1);

	if (Status)
	{
		AfxMessageBox(xorstr_(L"保存配置文件成功"), MB_OK);
	}
	else
	{
		AfxMessageBox(xorstr_(L"保存配置文件失败"), MB_ICONSTOP);
	}
}


void CSettingDlg::OnBnClickedButtonDefaultcfg()
{
	g_pTool->LoadCFG(0);
	AfxMessageBox(xorstr_(L"恢复默认配置成功"), MB_OK);
	theApp.GetTabDlg()->LoadCFGGui();
}





BOOL CSettingDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类

	switch (LOWORD(wParam))//wParam的低字是控件ID，lParam是发送这个WM_COMMAND消息的控件的句柄。
	{
	case IDC_HOTKEY_MENU:
	{
		//保存hotkey
		g_pTool->GetVar(xorstr_("sk_mfcMenu_hotkey"))->hotkey = m_HotKey_Menu.GetHotKey();
		break;
	}
	default:
		break;
	}


	return __super::OnCommand(wParam, lParam);
}


void CSettingDlg::OnBnClickedCheckBasefun()
{
	int Status = m_Check_BaseFun.GetCheck();
	g_pTool->GetVar(xorstr_("sk_basics_enable"))->value = Status;
}


void CSettingDlg::OnBnClickedCheckEspfun()
{
	int Status = m_Check_EspFun.GetCheck();
	g_pTool->GetVar(xorstr_("sk_esp_enable"))->value = Status;
}


void CSettingDlg::OnBnClickedCheckAimfun()
{
	int Status = m_Check_AimFun.GetCheck();
	g_pTool->GetVar(xorstr_("sk_aimbot_enable"))->value = Status;
}


void CSettingDlg::OnBnClickedCheckWallfun()
{
	int Status = m_Check_WallFun.GetCheck();
	g_pTool->GetVar(xorstr_("sk_wall_enable"))->value = Status;
}


void CSettingDlg::OnBnClickedCheckKzfun()
{
	int Status = m_Check_KZFun.GetCheck();
	g_pTool->GetVar(xorstr_("sk_kreedz_enable"))->value = Status;
}


void CSettingDlg::OnBnClickedCheckZsfun()
{
	int Status = m_Check_Survival.GetCheck();
	g_pTool->GetVar(xorstr_("sk_cata_enable"))->value = Status;
}


void CSettingDlg::OnBnClickedCheckKillico()
{
	int Status = m_Check_KillIco.GetCheck();
	g_pTool->GetVar(xorstr_("sk_basics_deathico"))->value = Status;

	if (Status)
	{
		g_pTool->ShowMsg(xorstr_("#Setting_Notice_DeathIco"));
	}
}


void CSettingDlg::OnBnClickedCheckDeathbox()
{
	int Status = m_Check_DeathBox.GetCheck();
	g_pTool->GetVar(xorstr_("sk_basics_deathbox"))->value = Status;

	if (Status)
	{
		g_pTool->ShowMsg(xorstr_("#Setting_Notice_blockDeathBox"));
	}
}


void CSettingDlg::OnBnClickedCheckSoccer()
{
	int Status = m_Check_Soccer.GetCheck();
	g_pTool->GetVar(xorstr_("sk_basics_soccer"))->value = Status;

	if (Status)
	{
		g_pTool->ShowMsg(xorstr_("#Setting_Notice_blockSoccer"));
	}
}


void CSettingDlg::OnBnClickedCheckGameFps()
{
	ULONG ulStatus = m_Check_GameFps.GetCheck();

	g_pTool->GetVar(xorstr_("sk_basics_fps"))->value = ulStatus;

	if (ulStatus)
	{
		g_pTool->ShowMsg(xorstr_("#Setting_Notice_fps"));
	}
}


void CSettingDlg::OnEnChangeEditGameFps()
{
	UpdateData(TRUE);

	int Value = _wtoi(m_Edit_GameFps);

	g_pTool->GetVar(xorstr_("sk_basics_fps"))->flags = Value;

	cvar_s* pCvar = g_pTool->pfnGetCvarPointer(xorstr_("fps_max"));

	if (m_Check_GameFps.GetCheck() && pCvar)
	{
		pCvar->value = Value;
	}
}


void CSettingDlg::OnBnClickedCheckActivateWindow()
{
	int Status = m_Check_ActvateWindows.GetCheck();
	g_pTool->GetVar(xorstr_("sk_basics_activate_window"))->value = Status;

	if (Status)
	{
		g_pTool->ShowMsg(xorstr_("#Setting_Notice_ActivateWindows"));
	}


}


void CSettingDlg::OnBnClickedCheckNight()
{
	int Status = m_Check_Zsht.GetCheck();
	g_pTool->GetVar(xorstr_("sk_zsht_enable"))->value = Status;
}


void CSettingDlg::OnBnClickedCheckMpfun()
{
	int Status = m_Check_MpFun.GetCheck();
	g_pTool->GetVar(xorstr_("sk_mp_enable"))->value = Status;
}


void CSettingDlg::OnBnClickedCheckLike()
{
	int Status = m_Check_Like.GetCheck();
	g_pTool->GetVar(xorstr_("sk_basics_discipline"))->value = Status;

	if (Status)
	{
		g_pTool->ShowMsg(xorstr_("#Base_Notice_Like"));
	}
}
