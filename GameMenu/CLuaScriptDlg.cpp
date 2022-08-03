// CLuaScript.cpp: 实现文件
//

#include "pch.h"
#include "GameMenu.h"
#include "afxdialogex.h"
#include "CLuaScriptDlg.h"
#include "MenuHelper.h"


// CLuaScript 对话框

IMPLEMENT_DYNAMIC(CLuaScriptDlg, CDialogEx)

CLuaScriptDlg::CLuaScriptDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_LUA, pParent)
{

}

CLuaScriptDlg::~CLuaScriptDlg()
{
}

void CLuaScriptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Control(pDX, IDC_LIST2, m_List_Lua);
	DDX_Control(pDX, IDC_CHECK_LUA_GOD, m_Check_LuaGod);
	DDX_Control(pDX, IDC_HOTKEY_GOD, m_HotKey_God);
	DDX_Control(pDX, IDC_CHECK_LUA_GOD_8133, m_Check_God8133);
	DDX_Control(pDX, IDC_HOTKEY_GOD_8133, m_HotKey_God8133);
}


BEGIN_MESSAGE_MAP(CLuaScriptDlg, CDialogEx)
	
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST2, &CLuaScriptDlg::OnLvnItemchangedList2)
	ON_NOTIFY(NM_CLICK, IDC_LIST2, &CLuaScriptDlg::OnClickList2)
	ON_BN_CLICKED(IDC_CHECK_LUA_GOD, &CLuaScriptDlg::OnBnClickedCheckLuaGod)
	ON_BN_CLICKED(IDC_CHECK_LUA_GOD_8133, &CLuaScriptDlg::OnBnClickedCheckLuaGod8133)
END_MESSAGE_MAP()


// CLuaScript 消息处理程序


BOOL CLuaScriptDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_List_Lua.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES| LVS_EX_CHECKBOXES); // 整行选择、网格线 每一行的最前面带个复选框


	m_List_Lua.InsertColumn(0,xorstr_(_T("脚本名")) , LVCFMT_LEFT, 150);

	m_List_Lua.InsertColumn(1, xorstr_(_T("作者")), LVCFMT_LEFT, 50);

	m_List_Lua.InsertColumn(2, xorstr_(_T("更新时间")), LVCFMT_LEFT, 100);

	m_List_Lua.InsertColumn(3, xorstr_(_T("版本号")), LVCFMT_LEFT, 60);

	m_List_Lua.InsertColumn(4, xorstr_(_T("类型")), LVCFMT_LEFT, 50);

	m_List_Lua.InsertColumn(5, xorstr_(_T("文件名")), LVCFMT_LEFT, 300);

	



	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CLuaScriptDlg::OnLbnSelchangeList1()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CLuaScriptDlg::OnLvnItemchangedList2(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	int itemIndex = pNMLV->iItem;


	if ((pNMLV->uOldState & INDEXTOSTATEIMAGEMASK(1)) /* old state : unchecked */
		&& (pNMLV->uNewState & INDEXTOSTATEIMAGEMASK(2)) /* new state : checked */
		)
	{
		auto ACETool = std::make_shared<CACEUtil>();

		for (auto& it : m_VecListScript)
		{
			CString ItemFileName = m_List_Lua.GetItemText(itemIndex, 5);

			int Pos = it.FileName.rfind("\\") + 1;

			auto FileName = it.FileName.substr(Pos, it.FileName.length() - Pos);

			auto FileNameText = ACETool->WstringToString(ItemFileName.GetBuffer());

			if (FileName == FileNameText)
			{

				if (g_pTool->LoadLuaScript(it.FileName))
				{
					AfxMessageBox(ACETool->UTF8_To_UniCode(it.Description).data(), MB_OK);
				}
				else
				{
					AfxMessageBox(xorstr_(L"加载lua脚本失败"), MB_OK);
				}

				break;
			}
		}

	}
	else if ((pNMLV->uOldState & INDEXTOSTATEIMAGEMASK(2)) /* old state : checked */
		&& (pNMLV->uNewState & INDEXTOSTATEIMAGEMASK(1)) /* new state : unchecked */
		)
	{
		//取消选中

		auto ACETool = std::make_shared<CACEUtil>();

		for (auto& it : m_VecListScript)
		{
			CString ItemFileName = m_List_Lua.GetItemText(itemIndex, 5);

			int Pos = it.FileName.rfind("\\") + 1;

			auto FileName = it.FileName.substr(Pos, it.FileName.length() - Pos);

			auto FileNameText = ACETool->WstringToString(ItemFileName.GetBuffer());

			if (FileName == FileNameText)
			{

				if (g_pTool->unLoadLuaScript(it.FileName))
				{
					AfxMessageBox(xorstr_(L"卸载lua脚本成功"), MB_OK);
				}
				else
				{
					AfxMessageBox(xorstr_(L"卸载lua脚本失败"), MB_OK);
				}

				break;
			}
		}

	}
	else
	{
		//其他
	}



	*pResult = 0;
}


void CLuaScriptDlg::OnClickList2(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	*pResult = 0;
}

VOID CLuaScriptDlg::LuaInit()
{
	if (m_VecListScript.empty())
	{
		auto LuaList = g_pTool->GetLuaScriptInfo();

		auto pUtil = std::make_shared<CACEUtil>();

		if (LuaList)
		{
			m_VecListScript.assign((*LuaList).begin(), (*LuaList).end());

			/*std::copy((*LuaList).begin(), (*LuaList).end(), std::back_inserter(m_VecListScript));
			m_VecListScript.swap(*LuaList);*/

			ULONG uCount = 0;

			for (auto& it : m_VecListScript)
			{
				m_List_Lua.InsertItem(uCount, pUtil->UTF8_To_UniCode(it.Name).data());
				m_List_Lua.SetItemText(uCount, 1, pUtil->UTF8_To_UniCode(it.Auther).data());
				m_List_Lua.SetItemText(uCount, 2, pUtil->UTF8_To_UniCode(it.UpdateTime).data());
				m_List_Lua.SetItemText(uCount, 3, pUtil->UTF8_To_UniCode(it.Version).data());
				m_List_Lua.SetItemText(uCount, 4, pUtil->UTF8_To_UniCode(it.Method).data());

				int Pos = it.FileName.rfind("\\") + 1;

				auto FileName = it.FileName.substr(Pos, it.FileName.length() - Pos);

				m_List_Lua.SetItemText(uCount, 5, pUtil->StringToWstring(FileName).data());
				uCount++;


			}

		}

	}

	g_pTool->unAllLoadLuaScript();
}


void CLuaScriptDlg::OnBnClickedCheckLuaGod()
{
	int Status = m_Check_LuaGod.GetCheck();

	auto Var = g_pTool->GetVar(xorstr_("sk_script_god"));

	if (Status)
	{
		g_pTool->ShowMsgKey(xorstr_("#Lua_Notice_8530God"), m_HotKey_God.GetHotKeyName());
	}

	Var->value = Status;
}


BOOL CLuaScriptDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	switch (LOWORD(wParam))//wParam的低字是控件ID，lParam是发送这个WM_COMMAND消息的控件的句柄。
	{
	case IDC_HOTKEY_GOD:
		g_pTool->GetVar(xorstr_("sk_script_god"))->hotkey = m_HotKey_God.GetHotKey();
		break;
	case IDC_HOTKEY_GOD_8133:
		g_pTool->GetVar(xorstr_("sk_script_excalibur"))->hotkey = m_HotKey_God8133.GetHotKey();
		break;
	default:
		break;
	}

	return CDialogEx::OnCommand(wParam, lParam);
}


void CLuaScriptDlg::OnBnClickedCheckLuaGod8133()
{
	int Status = m_Check_God8133.GetCheck();

	auto Var = g_pTool->GetVar(xorstr_("sk_script_excalibur"));

	if (Status)
	{
		g_pTool->ShowMsgKey(xorstr_("#Lua_Notice_8133God"), m_HotKey_God8133.GetHotKeyName());
	}

	Var->value = Status;
}
