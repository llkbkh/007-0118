// CKzDlg.cpp: 实现文件
//

#include "pch.h"
#include "GameMenu.h"
#include "afxdialogex.h"
#include "CKzDlg.h"
#include "MenuHelper.h"


// CKzDlg 对话框

IMPLEMENT_DYNAMIC(CKzDlg, CDialogEx)

CKzDlg::CKzDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_KZ, pParent)
	, m_Edit_SpeedValue(_T(""))
	, m_Edit_SpeedReConnectTime(_T(""))
{

}

CKzDlg::~CKzDlg()
{
}

void CKzDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_KZ_278, m_Check_278);
	DDX_Control(pDX, IDC_CHECK_KZ_SPINHACK, m_Check_SpinHack);
	DDX_Control(pDX, IDC_CHECK_KZ_JUMPBUG, m_Check_JumpBug);
	DDX_Control(pDX, IDC_CHECK_KZ_INFO, m_Check_KzInfo);
	DDX_Control(pDX, IDC_CHECK_KZ_HOP, m_Check_BunnyHop);
	DDX_Control(pDX, IDC_CHECK_KZ_AUTO_STRAFE, m_Check_AutoStrafe);
	DDX_Control(pDX, IDC_CHECK_MCJ, m_Check_Mcj);
	DDX_Control(pDX, IDC_COMBO_KZ_MCJ_KEY, m_Combo_McjKey);
	DDX_Control(pDX, IDC_LIST_ZBS_AUTO_GANK, m_List_ZbsAutoGank);
	DDX_Control(pDX, IDC_CHECK_ZBS_GANK, m_Check_ZbsAutoGank);
	DDX_Control(pDX, IDC_LIST_ZBS_AUTO_COUNTER, m_List_ZbsAutoCounter);
	DDX_Control(pDX, IDC_COMBO_COUNTER_MODE, m_Combo_CounterMode);
	DDX_Control(pDX, IDC_CHECK_ZBS_COUNTER, m_Check_ZbsCounter);
	DDX_Control(pDX, IDC_CHECK_ZBS_S_AUTO_SKILL, m_Check_ZBS_AutoSkill);
	DDX_Control(pDX, IDC_COMBO_GAME_SPEED, m_Combo_GameSpeed);
	DDX_Control(pDX, IDC_CHECK_SPEED_ENABLE, m_Check_SpeedEnable);
	DDX_Control(pDX, IDC_COMBO_GAME_KEY, m_Combo_GameSpeedKey);
	DDX_Text(pDX, IDC_EDIT_SPEED_VALUE, m_Edit_SpeedValue);
	DDX_Text(pDX, IDC_EDIT_SPEED_RECONNECT_VALUE, m_Edit_SpeedReConnectTime);
	DDX_Control(pDX, IDC_CHECK_SPEED_RECON, m_Check_SpeedReConnectEnable);
	DDX_Control(pDX, IDC_CHECK_KZMODE, m_Check_KZMode);
	DDX_Control(pDX, IDC_COMBO_KZ_MODE, m_Combo_KZMode);
}


BEGIN_MESSAGE_MAP(CKzDlg, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK_KZ_278, &CKzDlg::OnBnClickedCheckKz278)
	ON_BN_CLICKED(IDC_CHECK_KZ_SPINHACK, &CKzDlg::OnBnClickedCheckKzSpinhack)
	ON_BN_CLICKED(IDC_CHECK_KZ_JUMPBUG, &CKzDlg::OnBnClickedCheckKzJumpbug)
	ON_BN_CLICKED(IDC_CHECK_KZ_INFO, &CKzDlg::OnBnClickedCheckKzInfo)
	ON_BN_CLICKED(IDC_CHECK_KZ_HOP, &CKzDlg::OnBnClickedCheckKzHop)
	ON_BN_CLICKED(IDC_CHECK_KZ_AUTO_STRAFE, &CKzDlg::OnBnClickedCheckKzAutoStrafe)
	ON_BN_CLICKED(IDC_CHECK_MCJ, &CKzDlg::OnBnClickedCheckMcj)
	ON_CBN_SELCHANGE(IDC_COMBO_KZ_MCJ_KEY, &CKzDlg::OnCbnSelchangeComboKzMcjKey)
	ON_BN_CLICKED(IDC_CHECK_ZBS_GANK, &CKzDlg::OnBnClickedCheckZbsGank)
	ON_LBN_SELCHANGE(IDC_LIST_ZBS_AUTO_GANK, &CKzDlg::OnLbnSelchangeListZbsAutoGank)
	ON_BN_CLICKED(IDC_CHECK_ZBS_COUNTER, &CKzDlg::OnBnClickedCheckZbsCounter)
	ON_CBN_SELCHANGE(IDC_COMBO_COUNTER_MODE, &CKzDlg::OnCbnSelchangeComboCounterMode)
	ON_LBN_SELCHANGE(IDC_LIST_ZBS_AUTO_COUNTER, &CKzDlg::OnLbnSelchangeListZbsAutoCounter)
	ON_BN_CLICKED(IDC_CHECK_ZBS_S_AUTO_SKILL, &CKzDlg::OnBnClickedCheckZbsSAutoSkill)
	ON_CBN_SELCHANGE(IDC_COMBO_GAME_SPEED, &CKzDlg::OnCbnSelchangeComboGameSpeed)
	ON_BN_CLICKED(IDC_CHECK_SPEED_ENABLE, &CKzDlg::OnBnClickedCheckSpeedEnable)
	ON_EN_CHANGE(IDC_EDIT_SPEED_VALUE, &CKzDlg::OnEnChangeEditSpeedValue)
	ON_CBN_SELCHANGE(IDC_COMBO_GAME_KEY, &CKzDlg::OnCbnSelchangeComboGameKey)
	ON_BN_CLICKED(IDC_CHECK_SPEED_RECON, &CKzDlg::OnBnClickedCheckSpeedRecon)
	ON_EN_CHANGE(IDC_EDIT_SPEED_RECONNECT_VALUE, &CKzDlg::OnEnChangeEditSpeedReconnectValue)
	ON_CBN_SELCHANGE(IDC_COMBO_KZ_MODE, &CKzDlg::OnCbnSelchangeComboKzMode)
END_MESSAGE_MAP()


// CKzDlg 消息处理程序


void CKzDlg::OnBnClickedCheckKz278()
{
	g_pTool->GetVar(xorstr_("sk_kreedz_groundspeed"))->value = m_Check_278.GetCheck(); 
}


void CKzDlg::OnBnClickedCheckKzSpinhack()
{
	g_pTool->GetVar(xorstr_("sk_kreedz_spinhack"))->value = m_Check_SpinHack.GetCheck();
}


void CKzDlg::OnBnClickedCheckKzJumpbug()
{
	g_pTool->GetVar(xorstr_("sk_kreedz_jumpbug"))->value = m_Check_JumpBug.GetCheck();
}


void CKzDlg::OnBnClickedCheckKzInfo()
{
	g_pTool->GetVar(xorstr_("sk_kreedz_info"))->value = m_Check_KzInfo.GetCheck();
}


void CKzDlg::OnBnClickedCheckKzHop()
{
	g_pTool->GetVar(xorstr_("sk_kreedz_bunnyhop"))->value = m_Check_BunnyHop.GetCheck();
}


void CKzDlg::OnBnClickedCheckKzAutoStrafe()
{
	g_pTool->GetVar(xorstr_("sk_kreedz_autostrafe"))->value = m_Check_AutoStrafe.GetCheck();
}


void CKzDlg::OnBnClickedCheckMcj()
{
	OnCbnSelchangeComboKzMcjKey();
}


BOOL CKzDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_Combo_McjKey.InsertString(0, xorstr_(L"空格键"));
	m_Combo_McjKey.InsertString(1, xorstr_(L"Alt键"));
	m_Combo_McjKey.InsertString(2, xorstr_(L"Shift键"));
	m_Combo_McjKey.SetCurSel(1);




	m_List_ZbsAutoGank.InsertString(0, xorstr_(L"敌人隐身不抓"));
	m_List_ZbsAutoGank.InsertString(1, xorstr_(L"圣剑盾反不抓"));
	m_List_ZbsAutoGank.InsertString(2, xorstr_(L"敌人隔墙不抓"));
	m_List_ZbsAutoGank.InsertString(3, xorstr_(L"背后敌人不抓"));

	m_List_ZbsAutoCounter.InsertString(0, xorstr_(L"自动切圣剑"));
	m_List_ZbsAutoCounter.InsertString(1, xorstr_(L"自动切盾反"));
	m_List_ZbsAutoCounter.InsertString(2, xorstr_(L"自动使用技能5"));
	m_List_ZbsAutoCounter.InsertString(3, xorstr_(L"自动使用技能6"));
	m_List_ZbsAutoCounter.InsertString(4, xorstr_(L"自动使用技能7"));
	m_List_ZbsAutoCounter.InsertString(5, xorstr_(L"自动使用技能8"));
	m_List_ZbsAutoCounter.InsertString(6, xorstr_(L"生化S自动按V"));
	m_List_ZbsAutoCounter.InsertString(7, xorstr_(L"自动切小圣剑"));
	
	m_Combo_CounterMode.InsertString(0, xorstr_(L"V1模式"));
	m_Combo_CounterMode.InsertString(1, xorstr_(L"V2模式"));
	m_Combo_CounterMode.SetCurSel(1);



	m_Combo_GameSpeed.InsertString(0, xorstr_(L"人物微加速"));
	m_Combo_GameSpeed.InsertString(1, xorstr_(L"子弹微加速"));
	m_Combo_GameSpeed.InsertString(2, xorstr_(L"缓冲加速"));
	m_Combo_GameSpeed.SetCurSel(0);

	m_Combo_GameSpeedKey.InsertString(0, xorstr_(L"E键"));
	m_Combo_GameSpeedKey.InsertString(1, xorstr_(L"Shift键"));
	m_Combo_GameSpeedKey.InsertString(2, xorstr_(L"鼠标左键"));
	m_Combo_GameSpeedKey.InsertString(3, xorstr_(L"鼠标右键"));
	m_Combo_GameSpeedKey.InsertString(4, xorstr_(L"鼠标中键"));
	m_Combo_GameSpeedKey.InsertString(5, xorstr_(L"左键或右键"));
	m_Combo_GameSpeedKey.InsertString(6, xorstr_(L"一直加速"));
	m_Combo_GameSpeedKey.SetCurSel(0);

	m_Combo_KZMode.InsertString(0, xorstr_(L"KZ模式1"));
	m_Combo_KZMode.InsertString(1, xorstr_(L"KZ模式2"));
	m_Combo_KZMode.SetCurSel(1);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CKzDlg::OnCbnSelchangeComboKzMcjKey()
{
	ULONG Status = m_Check_Mcj.GetCheck();

	if (Status)
	{
		Status = m_Combo_McjKey.GetCurSel() + 1;

		//自动同步 开启
		g_pTool->GetVar(xorstr_("sk_kreedz_autostrafe"))->value = 1;





	}
	g_pTool->GetVar(xorstr_("sk_kreedz_groundstrafe"))->value = Status;
}


void CKzDlg::OnBnClickedCheckZbsGank()
{
	int Status = m_Check_ZbsAutoGank.GetCheck();

	m_List_ZbsAutoGank.EnableWindow(Status);

	auto pVar = g_pTool->GetVar(xorstr_("sk_script_zbs_autogank_ct"));

	if (Status)
	{
		int nSel = m_List_ZbsAutoGank.GetCount();

		for (int i = 0; i < nSel; i++)
		{
			m_List_ZbsAutoGank.SetSel(i, pVar->flags & (1 << i));
		}
		g_pTool->ShowMsg(xorstr_("#Kz_Notice_AutoGankCT"));

	}
	if (pVar)
	{
		pVar->value = Status;
	}
}


void CKzDlg::OnLbnSelchangeListZbsAutoGank()
{
	int nSel = m_List_ZbsAutoGank.GetSelCount();

	auto pVar = g_pTool->GetVar(xorstr_("sk_script_zbs_autogank_ct"));

	if (pVar)
	{
		CArray< int, int& > arrayListSel;

		arrayListSel.SetSize(nSel);

		m_List_ZbsAutoGank.GetSelItems(nSel, arrayListSel.GetData());

		pVar->flags = 0;

		for (int i = 0; i < nSel; i++)
		{
			int Index = arrayListSel[i];

			pVar->flags |= (1 << Index);

		}

		pVar->value = m_Check_ZbsAutoGank.GetCheck();
	}
}


void CKzDlg::OnBnClickedCheckZbsCounter()
{
	int Status = m_Check_ZbsCounter.GetCheck();

	m_List_ZbsAutoCounter.EnableWindow(Status);

	auto pVar = g_pTool->GetVar(xorstr_("sk_script_zbs_counter"));

	if (Status)
	{
		int nSel = m_List_ZbsAutoCounter.GetCount();

		for (int i = 0; i < nSel; i++)
		{
			m_List_ZbsAutoCounter.SetSel(i, pVar->flags & (1 << i));
		}
		g_pTool->ShowMsg(xorstr_("#Kz_Notice_Counter"));


		Status = m_Combo_CounterMode.GetCurSel() + 1;

	}
	if (pVar)
	{

		pVar->value = Status;
	}
}


void CKzDlg::OnCbnSelchangeComboCounterMode()
{
	int Status = m_Check_ZbsCounter.GetCheck();

	if (Status)
	{
		g_pTool->GetVar(xorstr_("sk_script_zbs_counter"))->value = m_Combo_CounterMode.GetCurSel() + 1;
		g_pTool->ShowMsg(xorstr_("#Kz_Notice_CounterMode"));
		
	}

}


void CKzDlg::OnLbnSelchangeListZbsAutoCounter()
{
	int nSel = m_List_ZbsAutoCounter.GetSelCount();

	auto pVar = g_pTool->GetVar(xorstr_("sk_script_zbs_counter"));

	if (pVar)
	{
		CArray< int, int& > arrayListSel;

		arrayListSel.SetSize(nSel);

		m_List_ZbsAutoCounter.GetSelItems(nSel, arrayListSel.GetData());

		pVar->flags = 0;

		for (int i = 0; i < nSel; i++)
		{
			int Index = arrayListSel[i];

			pVar->flags |= (1 << Index);

		}

	}
}


void CKzDlg::OnBnClickedCheckZbsSAutoSkill()
{
	int Status = m_Check_ZBS_AutoSkill.GetCheck();

	if (Status)
	{
		g_pTool->ShowMsg(xorstr_("#Kz_Notice_AutoSkill"));
	}
	g_pTool->GetVar(xorstr_("sk_script_zb5_auto_skill"))->value = Status;
}


void CKzDlg::OnCbnSelchangeComboGameSpeed()
{
	ULONG ulIndex = m_Combo_GameSpeed.GetCurSel();

	g_pTool->GetVar(xorstr_("sk_speed_mode"))->value = ulIndex + 1;
}


void CKzDlg::OnBnClickedCheckSpeedEnable()
{
	ULONG ulStatus = m_Check_SpeedEnable.GetCheck();

	if (ulStatus)
	{
		g_pTool->ShowMsg(xorstr_("#Speed_Notice_Mode"));
	}
	g_pTool->GetVar(xorstr_("sk_speed_enable"))->value = ulStatus;

}


void CKzDlg::OnEnChangeEditSpeedValue()
{
	UpdateData(TRUE);
	CString csAreaS = m_Edit_SpeedValue;
	// 只允许输数据  
	int nStringLength = csAreaS.GetLength();
	int nDotCount = 0;
	// 点字符不能多于1个  
	for (int nIndex = 0; nIndex < nStringLength; nIndex++)
	{
		if (csAreaS[nIndex] == '.')
		{
			nDotCount++;
			if (nDotCount > 1)
			{
				CString csTmp;
				csTmp = csAreaS.Left(nIndex);
				csTmp += csAreaS.Right(csAreaS.GetLength() - nIndex - 1);
				//csRadius = csRadius.Left( nIndex + 1 ) + csRadius.Right( nStringLength - ( nIndex + 1 ) - 1 );  
				m_Edit_SpeedValue = csTmp;
				goto Lab_Exit;
			}
		}
	}

	// 不允许输入数字和点以外的字符  
	for (int nIndex = 0; nIndex < nStringLength; nIndex++)
	{
		if (csAreaS[nIndex] != '.' && (csAreaS[nIndex] > '9' || csAreaS[nIndex] < '0'))
		{
			csAreaS = csAreaS.Left(nIndex) + csAreaS.Right(csAreaS.GetLength() - nIndex - 1);
			m_Edit_SpeedValue = csAreaS;
			goto Lab_Exit;
		}
	}

Lab_Exit:
	g_pTool->GetVar(xorstr_("sk_speed_value"))->value = _ttof(m_Edit_SpeedValue);;

	UpdateData(FALSE);


}


void CKzDlg::OnCbnSelchangeComboGameKey()
{
	ULONG ulIndex = m_Combo_GameSpeedKey.GetCurSel();

	g_pTool->GetVar(xorstr_("sk_speed_key"))->value = ulIndex + 1;
}


void CKzDlg::OnBnClickedCheckSpeedRecon()
{
	// TODO: 在此添加控件通知处理程序代码
	ULONG ulStatus = m_Check_SpeedReConnectEnable.GetCheck();

	if (ulStatus)
	{
		g_pTool->ShowMsg(xorstr_("#Speed_Notice_reconnect"));
	}
	g_pTool->GetVar(xorstr_("sk_speed_reconnect"))->value = ulStatus;


	OnEnChangeEditSpeedReconnectValue();

}


void CKzDlg::OnEnChangeEditSpeedReconnectValue()
{
	UpdateData(TRUE);

	int Value = _wtoi(m_Edit_SpeedReConnectTime);


	g_pTool->GetVar(xorstr_("sk_speed_reconnect_timer"))->value = Value;
}


void CKzDlg::OnCbnSelchangeComboKzMode()
{
	ULONG ulIndex = m_Combo_KZMode.GetCurSel() + 1;

	if (ulIndex == 1)
	{
		g_pTool->ShowMsg(xorstr_("#Kz_Notice_KZMode_1"));
	}
	g_pTool->GetVar(xorstr_("sk_kreedz_mode"))->value = ulIndex;
}
