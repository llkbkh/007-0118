#pragma once
#include "afxdialogex.h"
#include <list>


// CWeapon 对话框

class CWeaponDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWeaponDlg)

public:
	CWeaponDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CWeaponDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_WEAPON };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();




private:
	std::map<int, PGAME_ITEMMAP>* m_Item;
	std::list<int> m_Weapon_Array[6];
	std::string m_WeaponList;
public:
	CListBox m_List01;
	CListBox m_List02;
	CListBox m_List03;
	CListBox m_List04;
	CListBox m_List05;
	CListBox m_List06;
	CButton m_Check_Weapon;
	afx_msg void OnBnClickedCheckWeapon();
	CHotKeyCtrl m_HotKey_Weapon;
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnLbnSelchangeList3();
	afx_msg void OnLbnSelchangeList4();
	afx_msg void OnLbnSelchangeList5();
	afx_msg void OnLbnSelchangeList6();
	afx_msg void OnLbnSelchangeList7();
	void WeaponSetF();

};
