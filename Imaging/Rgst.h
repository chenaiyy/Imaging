#pragma once


// Rgst 对话框

class Rgst : public CDialog
{
	DECLARE_DYNAMIC(Rgst)

public:
	Rgst(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Rgst();

// 对话框数据
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_basefile;
public:
	CString m_warpfile;
public:
	CString m_gcpfile;
public:
	afx_msg void OnBnClickedButton1();
public:
	afx_msg void OnBnClickedButton2();
public:
	afx_msg void OnBnClickedButton3();
};
