#pragma once
#include "afxwin.h"


// PREGCP �Ի���

class PREGCP : public CDialog
{
	DECLARE_DYNAMIC(PREGCP)

public:
	PREGCP(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~PREGCP();

	int m_slct;   //��ѡ����
	CString m_datafile;  //����·��
	CString m_Gcpfile;  //���Ƶ���Ϣ·��
	CString m_outfile;  //У��Ӱ��Ĵ洢·��
	int m_combo;

// �Ի�������
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
