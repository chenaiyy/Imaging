#pragma once


// BianHua 对话框

class BianHua : public CDialog
{
	DECLARE_DYNAMIC(BianHua)

public:
	BianHua(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~BianHua();

// 对话框数据
	enum { IDD = IDD_DIALOG7 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_firstfile;
public:
	CString m_sercondfile;
public:
	CString m_outfile;
public:
	afx_msg void OnBnClickedButton1();
public:
	afx_msg void OnBnClickedButton2();
public:
	afx_msg void OnBnClickedButton3();
};
