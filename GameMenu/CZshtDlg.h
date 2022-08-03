#pragma once
#include "afxdialogex.h"


// CZshtDlg 对话框

class CZshtDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CZshtDlg)

public:
	CZshtDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CZshtDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ZSHT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CButton m_Check_ZshtReSoures;
	CButton m_Check_ZshtTransfer;
	CButton m_Check_ZshtContribute;
	CComboBox m_Combo_ZshtBuild;
	afx_msg void OnBnClickedCheckZshtResoures();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheckZshtTaansfer();
	afx_msg void OnBnClickedCheckZshtContribute();

	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	CString m_Edit_ZshtCount;
	CString m_Edit_ZshtX;
	CString m_Edit_ZshtY;
	CString m_Edit_ZshtZ;
	afx_msg void OnCbnSelchangeComboZshtBuild();
	afx_msg void OnBnClickedCheckBuild();
	CButton m_Check_Build;
	afx_msg void OnEnChangeEditZshtCount();
	afx_msg void OnEnChangeEditZshtX();
	afx_msg void OnEnChangeEditZshtY();
	afx_msg void OnEnChangeEditZshtZ();
};
