#pragma once
#include "afxdialogex.h"


// CLuaScript 对话框

class CLuaScriptDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLuaScriptDlg)

public:
	CLuaScriptDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CLuaScriptDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_LUA };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
//	CListBox m_List;
	virtual BOOL OnInitDialog();
	afx_msg void OnLbnSelchangeList1();
	CListCtrl m_List_Lua;
	afx_msg void OnLvnItemchangedList2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickList2(NMHDR* pNMHDR, LRESULT* pResult);


	VOID LuaInit();

private:
	std::vector <TAG_LUA_SCRIPT_TABLE> m_VecListScript;
public:
	CButton m_Check_LuaGod;
	afx_msg void OnBnClickedCheckLuaGod();
	CHotKeyCtrl m_HotKey_God;
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedCheckLuaGod8133();
	CButton m_Check_God8133;
	CHotKeyCtrl m_HotKey_God8133;
};
