#pragma once
#include "afxwin.h"


// Show 对话框

class Show : public CDialog
{
	DECLARE_DYNAMIC(Show)

public:
	Show(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Show();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };
	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int sglslct; //选择的单选框
	int band;  //传进来的波段数
// 记录选择的波段
	int B_gray;
	int B_red;
	int B_green;
	int B_blue;
	int B_index;
public:
	virtual BOOL OnInitDialog();
public:
	void Loaddata(int );
public:
	CComboBox combo_gray;
public:
	CComboBox combo_index;
public:
	CComboBox combo_red;
public:
	CComboBox combo_green;
public:
	CComboBox combo_blue;
public:
	afx_msg void OnBnClickedRadio1();
public:
	afx_msg void OnBnClickedRadio3();
public:
	afx_msg void OnBnClickedRadio2();
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
public:
	afx_msg void OnDestroy();
};
