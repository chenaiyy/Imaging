#pragma once


// BianHua �Ի���

class BianHua : public CDialog
{
	DECLARE_DYNAMIC(BianHua)

public:
	BianHua(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~BianHua();

// �Ի�������
	enum { IDD = IDD_DIALOG7 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
