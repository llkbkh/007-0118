#pragma once
#include "afxdialogex.h"


// CPayDlg 对话框

class CPayDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPayDlg)

public:
	CPayDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CPayDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_PAY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
//	CEdit m_Edit_PayUser;
//	CEdit m_Edit_PayCard;
	afx_msg void OnBnClickedButton1();
	CString m_Edit_PayUser;
	CString m_Edit_PayCard;
};
