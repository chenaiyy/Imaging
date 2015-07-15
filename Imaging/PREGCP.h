#pragma once
#include "afxwin.h"


// PREGCP 对话框

class PREGCP : public CDialog
{
	DECLARE_DYNAMIC(PREGCP)

public:
	PREGCP(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~PREGCP();

	int m_slct;   //单选框标记
	CString m_datafile;  //数据路径
	CString m_Gcpfile;  //控制点信息路劲
	CString m_outfile;  //校正影像的存储路劲
	int m_combo;

// 对话框数据
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnOpenfile();
public:
	afx_msg void OnReadGcps();
public:
	afx_msg void OnOutfile();
public:
	CComboBox m_Multi;
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedRadio1();
public:
	afx_msg void OnBnClickedRadio2();
public:
	afx_msg void OnBnClickedRadio3();
public:
	afx_msg void OnDestroy();
public:
	CString m_outtif;
};
