#pragma once


// ISODlg �Ի���

class ISODlg : public CDialog
{
	DECLARE_DYNAMIC(ISODlg)

public:
	ISODlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~ISODlg();

// �Ի�������
	enum { IDD = IDD_DIALOG5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_outfile;
public:
	CString m_infile;
public:
	int m_iClasses;
public:
	int m_iIniClasses;
public:
	int m_iLoopNum;
public:
	int m_iMaxMergeClassNum;
public:
	float m_fMinPixelNum;
public:
	float m_fMaxSD;
public:
	float m_fMinClassDis;
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedButton1();
public:
	afx_msg void OnBnClickedButton2();
};
