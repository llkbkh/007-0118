#pragma once
#include "afxdialogex.h"
#include "RCFServer.h"


// CMainDlg 对话框

class CMainDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMainDlg)

public:
	CMainDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CMainDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MAIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_Edit_BinPath;
	CButton m_Button_BinPath;
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnBnClickedButton1();
	CButton m_Button_StartCheat;
	afx_msg void OnBnClickedButton2();

	void ThreadHeat();
	void ThreadCheckGameModule();

	void ClearRegedit();
	void ClearTempFile();
private:
	CRCFServerManage	m_RCFManage;

};
