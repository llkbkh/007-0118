#pragma once
#include "afxdialogex.h"


// CRegDlg 对话框

class CRegDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRegDlg)

public:
	CRegDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CRegDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_REG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CButton m_Button_Reg;
	CString m_Edit_RegUser;
	CString m_Edit_RegPassWord;
	CString m_Edit_RegEmail;
	CString m_Edit_RegCode;
	afx_msg void OnBnClickedButton2();
};
