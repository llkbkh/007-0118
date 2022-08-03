// CWeapon.cpp: 实现文件
//

#include "pch.h"
#include "GameMenu.h"
#include "afxdialogex.h"
#include "CWeaponDlg.h"
#include "MenuHelper.h"


// CWeapon 对话框

IMPLEMENT_DYNAMIC(CWeaponDlg, CDialogEx)

CWeaponDlg::CWeaponDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_WEAPON, pParent)
{

}

CWeaponDlg::~CWeaponDlg()
{
}

void CWeaponDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List01);
	DDX_Control(pDX, IDC_LIST3, m_List02);
	DDX_Control(pDX, IDC_LIST4, m_List03);
	DDX_Control(pDX, IDC_LIST5, m_List04);
	DDX_Control(pDX, IDC_LIST6, m_List05);
	DDX_Control(pDX, IDC_LIST7, m_List06);
	DDX_Control(pDX, IDC_CHECK_WEAPON, m_Check_Weapon);
	DDX_Control(pDX, IDC_HOTKEY_MP_WEAPON, m_HotKey_Weapon);
}


BEGIN_MESSAGE_MAP(CWeaponDlg, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK_WEAPON, &CWeaponDlg::OnBnClickedCheckWeapon)
	ON_LBN_SELCHANGE(IDC_LIST1, &CWeaponDlg::OnLbnSelchangeList1)
	ON_LBN_SELCHANGE(IDC_LIST3, &CWeaponDlg::OnLbnSelchangeList3)
	ON_LBN_SELCHANGE(IDC_LIST4, &CWeaponDlg::OnLbnSelchangeList4)
	ON_LBN_SELCHANGE(IDC_LIST5, &CWeaponDlg::OnLbnSelchangeList5)
	ON_LBN_SELCHANGE(IDC_LIST6, &CWeaponDlg::OnLbnSelchangeList6)
	ON_LBN_SELCHANGE(IDC_LIST7, &CWeaponDlg::OnLbnSelchangeList7)
END_MESSAGE_MAP()


// CWeapon 消息处理程序


BOOL CWeaponDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	auto FiltrationWeapon = [&](ULONG Index) ->BOOL
	{
		switch (Index)
		{
		case 757:	//Index=757 Src 空手
		case 758:	//Index=758 Src CSO_Item_Name_STANDALONEKNIFE
		case 795:	//Index = 795 Src 空手
		case 8305:	//Index=8305 Src 大型急救包
		case 8307:	//Index=8307 Src 空手
			return FALSE;
		default:
			break;
		}
		return TRUE;
	};

	m_Item = g_pTool->GetGameItem();

	for (auto &itr : *m_Item)
	{
		if (!FiltrationWeapon(itr.first))
		{
			continue;
		}

		if (itr.second->Category == 1)
		{
			m_List01.AddString(itr.second->ItemSrc);
		}
		else if(itr.second->Category == 2)
		{
			m_List02.AddString(itr.second->ItemSrc);
		}
		else if (itr.second->Category == 3)
		{
			m_List03.AddString(itr.second->ItemSrc);
		}
		else if (itr.second->Category == 4)
		{
			m_List04.AddString(itr.second->ItemSrc);
		}
		else if (itr.second->Category == 5)
		{
			m_List05.AddString(itr.second->ItemSrc);
		}
		else if (itr.second->Category == 6)
		{
			m_List06.AddString(itr.second->ItemSrc);
		}

	}
	m_List01.SetTopIndex(m_List01.GetCount() - 1);
	m_List02.SetTopIndex(m_List02.GetCount() - 1);
	m_List03.SetTopIndex(m_List03.GetCount() - 1);
	m_List04.SetTopIndex(m_List04.GetCount() - 1);
	m_List05.SetTopIndex(m_List05.GetCount() - 1);
	m_List06.SetTopIndex(m_List06.GetCount() - 1);



	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CWeaponDlg::OnBnClickedCheckWeapon()
{
	int Status = m_Check_Weapon.GetCheck();

	auto Var = g_pTool->GetVar(xorstr_("sk_mp_weapon"));

	if (Status)
	{
		g_pTool->ShowMsgKey(xorstr_("#Mp_Notice_BuyWeapon"), m_HotKey_Weapon.GetHotKeyName());
	}

	Var->value = Status;
}


BOOL CWeaponDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	switch (LOWORD(wParam))//wParam的低字是控件ID，lParam是发送这个WM_COMMAND消息的控件的句柄。
	{
	case IDC_HOTKEY_MP_WEAPON:
		g_pTool->GetVar(xorstr_("sk_mp_weapon"))->hotkey = m_HotKey_Weapon.GetHotKey();
		break;
	default:
		break;
	}



	return CDialogEx::OnCommand(wParam, lParam);
}


void CWeaponDlg::OnLbnSelchangeList1()
{
	UpdateData(TRUE);

	m_Weapon_Array[0].clear();

	int nSel = m_List01.GetSelCount();

	CArray< int, int& > arrayListSel;

	arrayListSel.SetSize(nSel);

	m_List01.GetSelItems(nSel, arrayListSel.GetData());

	CString s = _T("");

	for (int i = 0; i < nSel; i++)
	{
		m_List01.GetText(arrayListSel[i], s);

		for (auto& itr : *m_Item)
		{
			if (itr.second->Category == 1)
			{
				if (lstrcmpiW(itr.second->ItemSrc, s) == 0)
				{
					m_Weapon_Array[0].push_back(itr.first);
				}
;
			}
		}
	}
}


void CWeaponDlg::OnLbnSelchangeList3()
{
	UpdateData(TRUE);

	m_Weapon_Array[1].clear();

	int nSel = m_List02.GetSelCount();

	CArray< int, int& > arrayListSel;

	arrayListSel.SetSize(nSel);

	m_List02.GetSelItems(nSel, arrayListSel.GetData());

	CString s = _T("");

	for (int i = 0; i < nSel; i++)
	{
		m_List02.GetText(arrayListSel[i], s);

		for (auto& itr : *m_Item)
		{
			if (itr.second->Category == 2)
			{
				if (lstrcmpiW(itr.second->ItemSrc, s) == 0)
				{
					m_Weapon_Array[1].push_back(itr.first);
				}
				;
			}
		}
	}
}


void CWeaponDlg::OnLbnSelchangeList4()
{
	UpdateData(TRUE);

	m_Weapon_Array[2].clear();

	int nSel = m_List03.GetSelCount();

	CArray< int, int& > arrayListSel;

	arrayListSel.SetSize(nSel);

	m_List03.GetSelItems(nSel, arrayListSel.GetData());

	CString s = _T("");

	for (int i = 0; i < nSel; i++)
	{
		m_List03.GetText(arrayListSel[i], s);

		for (auto& itr : *m_Item)
		{
			if (itr.second->Category == 3)
			{
				if (lstrcmpiW(itr.second->ItemSrc, s) == 0)
				{
					m_Weapon_Array[2].push_back(itr.first);
				}
				;
			}
		}
	}
}


void CWeaponDlg::OnLbnSelchangeList5()
{
	UpdateData(TRUE);

	m_Weapon_Array[3].clear();

	int nSel = m_List04.GetSelCount();

	CArray< int, int& > arrayListSel;

	arrayListSel.SetSize(nSel);

	m_List04.GetSelItems(nSel, arrayListSel.GetData());

	CString s = _T("");

	for (int i = 0; i < nSel; i++)
	{
		m_List04.GetText(arrayListSel[i], s);

		for (auto& itr : *m_Item)
		{
			if (itr.second->Category == 4)
			{
				if (lstrcmpiW(itr.second->ItemSrc, s) == 0)
				{
					m_Weapon_Array[3].push_back(itr.first);
				}
				;
			}
		}
	}
}


void CWeaponDlg::OnLbnSelchangeList6()
{
	UpdateData(TRUE);

	m_Weapon_Array[4].clear();

	int nSel = m_List05.GetSelCount();

	CArray< int, int& > arrayListSel;

	arrayListSel.SetSize(nSel);

	m_List05.GetSelItems(nSel, arrayListSel.GetData());

	CString s = _T("");

	for (int i = 0; i < nSel; i++)
	{
		m_List05.GetText(arrayListSel[i], s);

		for (auto& itr : *m_Item)
		{
			if (itr.second->Category == 5)
			{
				if (lstrcmpiW(itr.second->ItemSrc, s) == 0)
				{
					m_Weapon_Array[4].push_back(itr.first);
				}
				;
			}
		}
	}
}


void CWeaponDlg::OnLbnSelchangeList7()
{
	UpdateData(TRUE);

	m_Weapon_Array[5].clear();

	int nSel = m_List06.GetSelCount();

	CArray< int, int& > arrayListSel;

	arrayListSel.SetSize(nSel);

	m_List06.GetSelItems(nSel, arrayListSel.GetData());

	CString s = _T("");

	for (int i = 0; i < nSel; i++)
	{
		m_List06.GetText(arrayListSel[i], s);

		for (auto& itr : *m_Item)
		{
			if (itr.second->Category == 6)
			{
				if (lstrcmpiW(itr.second->ItemSrc, s) == 0)
				{
					m_Weapon_Array[5].push_back(itr.first);
				}
				;
			}
		}
	}
}

void CWeaponDlg::WeaponSetF()
{
	m_WeaponList.clear();


	for (auto &iter : m_Weapon_Array)
	{
		for (auto & it :iter)
		{
			auto Text = fmt::format(xorstr_("item_{};"), it);

			m_WeaponList.append(Text);
		}

	}

	ACEDebugLog("[{}] {}",__FUNCTION__, m_WeaponList);

	if (m_WeaponList.empty())
	{
		return;
	}

	g_pTool->Mp_Command(m_WeaponList);
}
