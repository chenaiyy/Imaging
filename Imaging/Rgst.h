#pragma once


// Rgst �Ի���

class Rgst : public CDialog
{
	DECLARE_DYNAMIC(Rgst)

public:
	Rgst(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~Rgst();

// �Ի�������
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
