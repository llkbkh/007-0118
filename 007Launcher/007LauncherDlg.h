
// 007LauncherDlg.h: 头文件
//

#pragma once
#include "TabSheet.h"
#include "CLoginDlg.h"
#include "CRegDlg.h"
#include "CPayDlg.h"


// CMy007LauncherDlg 对话框
class CMy007LauncherDlg : public CDialogEx
{
// 构造
public:
	CMy007LauncherDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MY007LAUNCHER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持



	void InitCfg();

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
//	CComboBox   m_Combo_NetLine;
	CTabSheet   m_Tab;
	CString		m_Page_Title[20];
	CLoginDlg   m_LoginPage;
	CRegDlg     m_RegPage;
	CPayDlg		m_PayPage;

	std::shared_ptr<CACEUtil> m_pACEUtil;
};
