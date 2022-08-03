// CAim.cpp: 实现文件
//

#include "pch.h"
#include "GameMenu.h"
#include "afxdialogex.h"
#include "CAim.h"
#include "MenuHelper.h"
#include <thread>


// CAim 对话框

IMPLEMENT_DYNAMIC(CAim, CDialogEx)

CAim::CAim(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_AIM, pParent)
	, m_Edit_Aim_Angle(_T(""))
	, m_Edit_Aim_Pitch(_T(""))
	, m_Edit_Aim_Yaw(_T(""))
	, m_Edit_ShootAngle(_T(""))
{

}

CAim::~CAim()
{
}

void CAim::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_AIM_KEY, m_Check_Aim_Key);
	DDX_Control(pDX, IDC_COMBO_AIM_KEY, m_Combo_Aim_Key);
	DDX_Control(pDX, IDC_COMBO_AIM_TEAM, m_Combo_Aim_Team);
	DDX_Control(pDX, IDC_COMBO_AIM_POS, m_Combo_Aim_Pos);
	DDX_Control(pDX, IDC_COMBO_AIM_TYPE, m_Combo_Aim_Type);
	DDX_Control(pDX, IDC_COMBO_AIM_MODE, m_Combo_Aim_Mode);
	//  DDX_Control(pDX, IDC_EDIT_AIM_ANGLE, m_Edit_Aim_Angle);
	DDX_Text(pDX, IDC_EDIT_AIM_ANGLE, m_Edit_Aim_Angle);
	DDX_Text(pDX, IDC_EDIT_AIM_PITCH, m_Edit_Aim_Pitch);
	DDX_Text(pDX, IDC_EDIT_AIM_YAW, m_Edit_Aim_Yaw);
	DDX_Control(pDX, IDC_COMBO_AIM_RECOILMODE, m_Combo_Aim_RecoilMode);
	DDX_Control(pDX, IDC_CHECK_AIM_NORECOIL, m_Check_Aim_NoRecoil);
	DDX_Control(pDX, IDC_CHECK_AIM_NORECOIL_VL, m_Check_NoRecoil_Visual);
	DDX_Control(pDX, IDC_SLIDER1, m_Slider);
	DDX_Control(pDX, IDC_COMBO_AIM_MOUSE_TYPE, m_Combo_Aim_MouseType);
	DDX_Control(pDX, IDC_COMBO_AIM_HIXBOX, m_Combo_Aim_HixBox);
	DDX_Control(pDX, IDC_COMBO_AIM_HIXBOX_TYPE, m_Combo_Aim_HixBoxType);
	DDX_Control(pDX, IDC_CHECK_AIM_SHOWPLAYER, m_Check_Aim_ShowPlayer);
	DDX_Control(pDX, IDC_CHECK_AIM_SHOW_SITION, m_Check_Aim_ShowPos);
	DDX_Control(pDX, IDC_CHECK_AIM_SHOW_RECOIL, m_Check_Aim_ShowRecoil);
	DDX_Control(pDX, IDC_CHECK_AIM_CAN_WEAPON, m_Check_Aim_WeaponCan);
	DDX_Control(pDX, IDC_CHECK_AIM_ZBS_NOAIM, m_Check_Aim_ZbsNoAim);
	DDX_Control(pDX, IDC_CHECK_AIM_NO4, m_Check_Aim_NoGrenade);
	DDX_Control(pDX, IDC_CHECK_AIM_BARRIER, m_Check_Aim_Barrier);
	DDX_Text(pDX, IDC_EDIT_AIM_SHOOT_ANGLE, m_Edit_ShootAngle);
	DDX_Control(pDX, IDC_CHECK_AIM_SHOOT, m_Check_Aim_Shoot);
}


BEGIN_MESSAGE_MAP(CAim, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK_AIM_KEY, &CAim::OnBnClickedCheckAimKey)
	ON_CBN_SELCHANGE(IDC_COMBO_AIM_KEY, &CAim::OnCbnSelchangeComboAimKey)
	ON_CBN_SELCHANGE(IDC_COMBO_AIM_TYPE, &CAim::OnCbnSelchangeComboAimType)
	ON_CBN_SELCHANGE(IDC_COMBO_AIM_TEAM, &CAim::OnCbnSelchangeComboAimTeam)
	ON_CBN_SELCHANGE(IDC_COMBO_AIM_POS, &CAim::OnCbnSelchangeComboAimPos)
	ON_CBN_SELCHANGE(IDC_COMBO_AIM_MODE, &CAim::OnCbnSelchangeComboAimMode)
	ON_EN_CHANGE(IDC_EDIT_AIM_ANGLE, &CAim::OnEnChangeEditAimAngle)
	ON_EN_CHANGE(IDC_EDIT_AIM_PITCH, &CAim::OnEnChangeEditAimPitch)
	ON_EN_CHANGE(IDC_EDIT_AIM_YAW, &CAim::OnEnChangeEditAimYaw)
	ON_CBN_SELCHANGE(IDC_COMBO_AIM_RECOILMODE, &CAim::OnCbnSelchangeComboAimRecoilmode)
	ON_BN_CLICKED(IDC_CHECK_AIM_NORECOIL, &CAim::OnBnClickedCheckAimNorecoil)
	ON_BN_CLICKED(IDC_CHECK_AIM_NORECOIL_VL, &CAim::OnBnClickedCheckAimNorecoilVl)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CAim::OnNMCustomdrawSlider1)
	ON_CBN_SELCHANGE(IDC_COMBO_AIM_HIXBOX, &CAim::OnCbnSelchangeComboAimHixbox)
	ON_BN_CLICKED(IDC_CHECK_AIM_SHOWPLAYER, &CAim::OnBnClickedCheckAimShowplayer)
	ON_BN_CLICKED(IDC_CHECK_AIM_SHOW_SITION, &CAim::OnBnClickedCheckAimShowSition)
	ON_BN_CLICKED(IDC_CHECK_AIM_SHOW_RECOIL, &CAim::OnBnClickedCheckAimShowRecoil)
	ON_BN_CLICKED(IDC_CHECK_AIM_CAN_WEAPON, &CAim::OnBnClickedCheckAimCanWeapon)
	ON_BN_CLICKED(IDC_CHECK_AIM_ZBS_NOAIM, &CAim::OnBnClickedCheckAimZbsNoaim)
	ON_BN_CLICKED(IDC_CHECK_AIM_NO4, &CAim::OnBnClickedCheckAimNo4)
	ON_BN_CLICKED(IDC_CHECK_AIM_BARRIER, &CAim::OnBnClickedCheckAimBarrier)
	ON_CBN_SELCHANGE(IDC_COMBO_AIM_MOUSE_TYPE, &CAim::OnCbnSelchangeComboAimMouseType)
	ON_BN_CLICKED(IDC_CHECK_AIM_SHOOT, &CAim::OnBnClickedCheckAimShoot)
	ON_EN_CHANGE(IDC_EDIT_AIM_SHOOT_ANGLE, &CAim::OnEnChangeEditAimShootAngle)
	ON_BN_CLICKED(IDC_BUTTON1, &CAim::OnBnClickedButton1)
END_MESSAGE_MAP()


// CAim 消息处理程序


void CAim::OnBnClickedCheckAimKey()
{
	ULONG uStatus = m_Check_Aim_Key.GetCheck();

	if (uStatus)
	{
		OnCbnSelchangeComboAimKey();
	}
	else
	{
		g_pTool->GetVar(xorstr_("sk_aimbot_key"))->value = 0;
	}

}


void CAim::OnCbnSelchangeComboAimKey()
{
	g_pTool->GetVar(xorstr_("sk_aimbot_key"))->value = m_Combo_Aim_Key.GetCurSel()+1;
}


BOOL CAim::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	for (size_t i = 0; i < 6; i++)
	{
		auto Text = fmt::format(xorstr_("#Aim_Combo_Key0{}"),i+1);
		m_Combo_Aim_Key.InsertString(i, g_pTool->GetMenuItemValue(Text));
	}
	m_Combo_Aim_Key.SetCurSel(4);
	

	for (size_t i = 0; i < 3; i++)
	{
		auto Text = fmt::format(xorstr_("#Aim_Combo_Team_{}"), i);
		m_Combo_Aim_Team.InsertString(i, g_pTool->GetMenuItemValue(Text));
	}
	m_Combo_Aim_Team.SetCurSel(0);

	for (size_t i = 0; i < 6; i++)
	{
		auto Text = fmt::format(xorstr_("#Aim_Combo_Pos_{}"), i + 1);
		m_Combo_Aim_Pos.InsertString(i, g_pTool->GetMenuItemValue(Text));
	}
	m_Combo_Aim_Pos.SetCurSel(0);

	for (size_t i = 0; i < 3; i++)
	{
		auto Text = fmt::format(xorstr_("#Aim_Combo_Type_{}"), i + 1);
		m_Combo_Aim_Type.InsertString(i, g_pTool->GetMenuItemValue(Text));
	}
	m_Combo_Aim_Type.SetCurSel(0);

	m_Combo_Aim_Mode.InsertString(0, g_pTool->GetMenuItemValue(xorstr_("#Aim_Combo_Mode_1")));
	m_Combo_Aim_Mode.InsertString(1, g_pTool->GetMenuItemValue(xorstr_("#Aim_Combo_Mode_2")));
	m_Combo_Aim_Mode.SetCurSel(0);

	CString TextAngle;
	TextAngle.Format(L"%d", (int)g_pTool->GetVar(xorstr_("sk_aimbot_angle"))->value);
	m_Edit_Aim_Angle = TextAngle;


	m_Combo_Aim_RecoilMode.InsertString(0, g_pTool->GetMenuItemValue(xorstr_("#Aim_Combo_Recoil_Mode_1")));
	m_Combo_Aim_RecoilMode.InsertString(1, g_pTool->GetMenuItemValue(xorstr_("#Aim_Combo_Recoil_Mode_2")));
	m_Combo_Aim_RecoilMode.SetCurSel(0);

	m_Slider.SetRange(0, 100);
	m_Slider.SetPos(0);

	m_Combo_Aim_MouseType.InsertString(0, g_pTool->GetMenuItemValue(xorstr_("#Aim_Combo_Mouse_Type_1")));
	m_Combo_Aim_MouseType.InsertString(1, g_pTool->GetMenuItemValue(xorstr_("#Aim_Combo_Mouse_Type_2")));
	m_Combo_Aim_MouseType.SetCurSel(0);

	m_Combo_Aim_HixBox.InsertString(0, xorstr_(L"自动判断"));
	m_Combo_Aim_HixBox.InsertString(1, xorstr_(L"手动调整"));
	m_Combo_Aim_HixBox.SetCurSel(0);
	m_Combo_Aim_HixBoxType.InsertString(0, xorstr_(L"HixBox"));
	m_Combo_Aim_HixBoxType.InsertString(1, xorstr_(L"Bone"));
	m_Combo_Aim_HixBoxType.InsertString(2, xorstr_(L"Origin"));
	m_Combo_Aim_HixBoxType.SetCurSel(0);


	m_Edit_ShootAngle = L"45";
	m_Edit_Aim_Pitch  = L"2";
	m_Edit_Aim_Yaw    = L"2";

	UpdateData(FALSE);
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CAim::OnCbnSelchangeComboAimType()
{
	// TODO: 在此添加控件通知处理程序代码 sk_aimbot_aimtype

	g_pTool->GetVar(xorstr_("sk_aimbot_aimtype"))->value = m_Combo_Aim_Type.GetCurSel() + 1;
}


void CAim::OnCbnSelchangeComboAimTeam()
{
	g_pTool->GetVar(xorstr_("sk_aimbot_camp"))->value = m_Combo_Aim_Team.GetCurSel() + 1;
}


void CAim::OnCbnSelchangeComboAimPos()
{
	g_pTool->GetVar(xorstr_("sk_aimbot_position"))->value = m_Combo_Aim_Pos.GetCurSel() + 1;
}


void CAim::OnCbnSelchangeComboAimMode()
{
	g_pTool->GetVar(xorstr_("sk_aimbot_aimmode"))->value = m_Combo_Aim_Mode.GetCurSel() + 1;
}


void CAim::OnEnChangeEditAimAngle()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	UpdateData(TRUE);

	int Value = _wtoi(m_Edit_Aim_Angle);

	if (Value > 360)
	{
		Value = 360;
	}
	else if (Value < 0)
	{
		Value = 0;
	}

	g_pTool->GetVar(xorstr_("sk_aimbot_angle"))->value = Value;

}


void CAim::OnEnChangeEditAimPitch()
{
	UpdateData(TRUE);
	CString csAreaS = m_Edit_Aim_Pitch;
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
				m_Edit_Aim_Pitch = csTmp;
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
			m_Edit_Aim_Pitch = csAreaS;
			goto Lab_Exit;
		}
	}

Lab_Exit:
	g_pTool->GetVar(xorstr_("sk_aimbot_RecoilCompensationPitch"))->value =_ttof(m_Edit_Aim_Pitch);;

	UpdateData(FALSE);
}


void CAim::OnEnChangeEditAimYaw()
{
	UpdateData(TRUE);
	CString csAreaS = m_Edit_Aim_Yaw;
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
				m_Edit_Aim_Yaw = csTmp;
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
			m_Edit_Aim_Yaw = csAreaS;
			goto Lab_Exit;
		}
	}

Lab_Exit:
	g_pTool->GetVar(xorstr_("sk_aimbot_RecoilCompensationYaw"))->value = _ttof(m_Edit_Aim_Yaw);;

	UpdateData(FALSE);
}


void CAim::OnCbnSelchangeComboAimRecoilmode()
{
	ULONG uIndex = m_Combo_Aim_RecoilMode.GetCurSel();

	g_pTool->GetVar(xorstr_("sk_aimbot_RecoilCompensationMode"))->value = uIndex;

	// 全局模式关闭调试模式  枪支模式 开启调试模式
	g_pTool->GetVar(xorstr_("sk_aimbot_RecoilCompensationMode_DEBUG"))->value = uIndex;

	if (uIndex)
	{
		g_pTool->ShowMsg(xorstr_("#Aim_Notice_RecoilCompensationMode"));

	}

}


void CAim::OnBnClickedCheckAimNorecoil()
{
	int Status = m_Check_Aim_NoRecoil.GetCheck();

	auto Var = g_pTool->GetVar(xorstr_("sk_aimbot_NoRecoil"));

	if (Status)
	{
		g_pTool->ShowMsg(xorstr_("#Aim_Notice_NoRecoil"));
	}

	Var->value = Status;
}


void CAim::OnBnClickedCheckAimNorecoilVl()
{
	int Status = m_Check_NoRecoil_Visual.GetCheck();

	auto Var = g_pTool->GetVar(xorstr_("sk_aimbot_NoRecoil_Visual"));

	if (Status)
	{
		g_pTool->ShowMsg(xorstr_("#Aim_Notice_NoRecoil_Visual"));
	}

	Var->value = Status;
}


void CAim::OnNMCustomdrawSlider1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	static bool Status = false;
	int Pos = m_Slider.GetPos();
	if (!Status && Pos >= 1)
	{
		Status ^= true;

		//g_pTool->ShowMsg(xorstr_("#Aim_Notice_MouseSpeed"));
	}


	CString Text;
	Text.Format(L"%d", Pos);

	SetDlgItemTextW(IDC_STATIC_MOUSE, Text);

	auto Var = g_pTool->GetVar(xorstr_("sk_aimbot_MouseMoveSpeed"));
	Var->value = Pos;

	*pResult = 0;
}


void CAim::OnCbnSelchangeComboAimHixbox()
{
	static bool Status = false;

	if (!Status)
	{
		Status ^= true;

		g_pTool->ShowMsg(xorstr_("#Aim_Notice_HixBox_ER"));

		m_Combo_Aim_HixBox.EnableWindow(FALSE);
		m_Combo_Aim_HixBoxType.EnableWindow(FALSE);
	}
	
}


void CAim::OnBnClickedCheckAimShowplayer()
{
	int Status = m_Check_Aim_ShowPlayer.GetCheck();

	auto Var = g_pTool->GetVar(xorstr_("sk_aimbot_showplayer"));

	if (Status)
	{
		g_pTool->ShowMsg(xorstr_("#Aim_Notice_ShowPlayer"));
	}

	Var->value = Status;
}


void CAim::OnBnClickedCheckAimShowSition()
{
	int Status = m_Check_Aim_ShowPos.GetCheck();

	auto Var = g_pTool->GetVar(xorstr_("sk_aimbot_showposition"));

	Var->value = Status;
}


void CAim::OnBnClickedCheckAimShowRecoil()
{
	int Status = m_Check_Aim_ShowRecoil.GetCheck();

	auto Var = g_pTool->GetVar(xorstr_("sk_aimbot_showrecoil"));

	Var->value = Status;
}


void CAim::OnBnClickedCheckAimCanWeapon()
{
	int Status = m_Check_Aim_WeaponCan.GetCheck();

	auto Var = g_pTool->GetVar(xorstr_("sk_aimbot_WeaponCanAttack"));

	if (Status)
	{
		g_pTool->ShowMsg(xorstr_("#Aim_Notice_WeaponCan"));
	}

	Var->value = Status;
}


void CAim::OnBnClickedCheckAimZbsNoaim()
{
	int Status = m_Check_Aim_ZbsNoAim.GetCheck();

	auto Var = g_pTool->GetVar(xorstr_("sk_aimbot_zb_notaim"));

	if (Status)
	{
		g_pTool->ShowMsg(xorstr_("#Aim_Notice_ZBSNoAim"));
	}

	Var->value = Status;
}


void CAim::OnBnClickedCheckAimNo4()
{
	int Status = m_Check_Aim_NoGrenade.GetCheck();

	auto Var = g_pTool->GetVar(xorstr_("sk_aimbot_no_grenade"));

	Var->value = Status;
}


void CAim::OnBnClickedCheckAimBarrier()
{

	int Status = m_Check_Aim_Barrier.GetCheck();

	auto Var = g_pTool->GetVar(xorstr_("sk_aimbot_barrier"));

	if (Status)
	{
		g_pTool->ShowMsg(xorstr_("#Aim_Notice_Visible"));
	}

	Var->value = Status;


}


void CAim::OnCbnSelchangeComboAimMouseType()
{
	auto Var = g_pTool->GetVar(xorstr_("sk_aimbot_MouseMoveSpeed"));
	Var->flags = m_Combo_Aim_MouseType.GetCurSel();
	
}


void CAim::OnBnClickedCheckAimShoot()
{
	int Status = m_Check_Aim_Shoot.GetCheck();

	auto Var = g_pTool->GetVar(xorstr_("sk_aimbot_shoot"));

	if (Status)
	{
		g_pTool->ShowMsg(xorstr_("#Aim_Notice_Shoot"));
	}

	Var->value = Status;
}


void CAim::OnEnChangeEditAimShootAngle()
{
	UpdateData(TRUE);

	int Value = _wtoi(m_Edit_ShootAngle);

	if (Value > 360)
	{
		Value = 360;
	}
	else if (Value < 0)
	{
		Value = 0;
	}

	g_pTool->GetVar(xorstr_("sk_aimbot_shoot_fov"))->value = Value;
}


void CAim::OnBnClickedButton1()
{
	auto Funs = []()->void
	{
		auto FileCmdLine = fmt::format(xorstr_("notepad.exe {}AimTuto.txt"),g_pTool->GetCFGPath());

		::system(FileCmdLine.data());
	};
	std::thread th(Funs);
	th.detach();
}
