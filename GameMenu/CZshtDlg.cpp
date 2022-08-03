// CZshtDlg.cpp: 实现文件
//

#include "pch.h"
#include "GameMenu.h"
#include "afxdialogex.h"
#include "CZshtDlg.h"
#include "MenuHelper.h"


// CZshtDlg 对话框

IMPLEMENT_DYNAMIC(CZshtDlg, CDialogEx)

CZshtDlg::CZshtDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_ZSHT, pParent)
	, m_Edit_ZshtCount(_T(""))
	, m_Edit_ZshtX(_T(""))
	, m_Edit_ZshtY(_T(""))
	, m_Edit_ZshtZ(_T(""))
{

}

CZshtDlg::~CZshtDlg()
{
}

void CZshtDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_ZSHT_RESOURES, m_Check_ZshtReSoures);
	DDX_Control(pDX, IDC_CHECK_ZSHT_TAANSFER, m_Check_ZshtTransfer);
	DDX_Control(pDX, IDC_CHECK_ZSHT_CONTRIBUTE, m_Check_ZshtContribute);
	DDX_Control(pDX, IDC_COMBO_ZSHT_BUILD, m_Combo_ZshtBuild);
	DDX_Text(pDX, IDC_EDIT_ZSHT_COUNT, m_Edit_ZshtCount);
	DDX_Text(pDX, IDC_EDIT_ZSHT_X, m_Edit_ZshtX);
	DDX_Text(pDX, IDC_EDIT_ZSHT_Y, m_Edit_ZshtY);
	DDX_Text(pDX, IDC_EDIT_ZSHT_Z, m_Edit_ZshtZ);
	DDX_Control(pDX, IDC_CHECK_BUILD, m_Check_Build);
}


BEGIN_MESSAGE_MAP(CZshtDlg, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK_ZSHT_RESOURES, &CZshtDlg::OnBnClickedCheckZshtResoures)
	ON_BN_CLICKED(IDC_CHECK_ZSHT_TAANSFER, &CZshtDlg::OnBnClickedCheckZshtTaansfer)
	ON_BN_CLICKED(IDC_CHECK_ZSHT_CONTRIBUTE, &CZshtDlg::OnBnClickedCheckZshtContribute)

	ON_BN_CLICKED(IDC_BUTTON1, &CZshtDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CZshtDlg::OnBnClickedButton2)
	ON_CBN_SELCHANGE(IDC_COMBO_ZSHT_BUILD, &CZshtDlg::OnCbnSelchangeComboZshtBuild)
	ON_BN_CLICKED(IDC_CHECK_BUILD, &CZshtDlg::OnBnClickedCheckBuild)
	ON_EN_CHANGE(IDC_EDIT_ZSHT_COUNT, &CZshtDlg::OnEnChangeEditZshtCount)
	ON_EN_CHANGE(IDC_EDIT_ZSHT_X, &CZshtDlg::OnEnChangeEditZshtX)
	ON_EN_CHANGE(IDC_EDIT_ZSHT_Y, &CZshtDlg::OnEnChangeEditZshtY)
	ON_EN_CHANGE(IDC_EDIT_ZSHT_Z, &CZshtDlg::OnEnChangeEditZshtZ)
END_MESSAGE_MAP()


// CZshtDlg 消息处理程序




BOOL CZshtDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	m_Combo_ZshtBuild.InsertString(0, xorstr_(L"仓库"));
	m_Combo_ZshtBuild.InsertString(1, xorstr_(L"小发电机"));
	m_Combo_ZshtBuild.InsertString(2, xorstr_(L"中发电机"));
	m_Combo_ZshtBuild.InsertString(3, xorstr_(L"大发电机"));
	m_Combo_ZshtBuild.InsertString(4, xorstr_(L"巨发电机"));
	m_Combo_ZshtBuild.InsertString(5, xorstr_(L"机枪炮台"));
	m_Combo_ZshtBuild.InsertString(6, xorstr_(L"火焰炮台"));
	m_Combo_ZshtBuild.InsertString(7, xorstr_(L"多管炮台"));
	m_Combo_ZshtBuild.InsertString(8, xorstr_(L"榴弹炮台"));
	m_Combo_ZshtBuild.InsertString(9, xorstr_(L"磁轨炮台"));
	m_Combo_ZshtBuild.InsertString(10, xorstr_(L"电磁塔"));
	m_Combo_ZshtBuild.SetCurSel(0);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

void CZshtDlg::OnBnClickedCheckZshtResoures()
{
	ULONG ulIndex = m_Check_ZshtReSoures.GetCheck();

	if (ulIndex)
	{
		g_pTool->ShowMsg(xorstr_("#Zsht_Notice_GetReSoures"));
	}
	g_pTool->GetVar(xorstr_("sk_zsht_resource"))->value = ulIndex;

}

void CZshtDlg::OnBnClickedCheckZshtTaansfer()
{
	ULONG ulIndex = m_Check_ZshtTransfer.GetCheck();

	if (ulIndex)
	{
		g_pTool->ShowMsg(xorstr_("#Zsht_Notice_Transfer"));
	}
	g_pTool->GetVar(xorstr_("sk_zsht_transfer"))->value = ulIndex;
}


void CZshtDlg::OnBnClickedCheckZshtContribute()
{
	ULONG ulIndex = m_Check_ZshtContribute.GetCheck();

	if (ulIndex)
	{
		g_pTool->ShowMsg(xorstr_("#Zsht_Notice_Contribute"));
	}
	g_pTool->GetVar(xorstr_("sk_zsht_contribute"))->value = ulIndex;
}




void CZshtDlg::OnBnClickedButton1()
{
	g_pTool->GameCommand(xorstr_("zsht_Request_Skill_Init 1"));
}


void CZshtDlg::OnBnClickedButton2()
{
	g_pTool->GameCommand(xorstr_("zsht_RequestRescue 1"));
}


void CZshtDlg::OnCbnSelchangeComboZshtBuild()
{
	g_pTool->GetVar(xorstr_("sk_zsht_loopbuild"))->flags = m_Combo_ZshtBuild.GetCurSel();
}


void CZshtDlg::OnBnClickedCheckBuild()
{
	ULONG ulIndex = m_Check_Build.GetCheck();

	if (ulIndex)
	{
		UpdateData(TRUE);
		ulIndex = _ttoi(m_Edit_ZshtCount);

		g_pTool->ShowMsg(xorstr_("#Zsht_Notice_Build"));
	}
	g_pTool->GetVar(xorstr_("sk_zsht_loopbuild"))->value = ulIndex;
}


void CZshtDlg::OnEnChangeEditZshtCount()
{
	ULONG ulIndex = m_Check_Build.GetCheck();

	if (ulIndex == 0)
	{
		//g_pTool->ShowMsgEr(xorstr_("#Zsht_Notice_Count_E1"));
		SetDlgItemTextW(IDC_EDIT_ZSHT_COUNT, L"0");
		return;
	}
	UpdateData(TRUE);
	g_pTool->GetVar(xorstr_("sk_zsht_loopbuild"))->value = _ttoi(m_Edit_ZshtCount);
}


void CZshtDlg::OnEnChangeEditZshtX()
{
	UpdateData(TRUE);
	g_pTool->GetVar(xorstr_("sk_zsht_loopbuild_x"))->value = _ttoi(m_Edit_ZshtX);
	
}


void CZshtDlg::OnEnChangeEditZshtY()
{
	UpdateData(TRUE);
	g_pTool->GetVar(xorstr_("sk_zsht_loopbuild_y"))->value = _ttoi(m_Edit_ZshtY);
}


void CZshtDlg::OnEnChangeEditZshtZ()
{
	UpdateData(TRUE);
	g_pTool->GetVar(xorstr_("sk_zsht_loopbuild_z"))->value = _ttoi(m_Edit_ZshtZ);
}
