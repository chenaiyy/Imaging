#pragma once
#include "afxwin.h"


// Show �Ի���

class Show : public CDialog
{
	DECLARE_DYNAMIC(Show)

public:
	Show(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~Show();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };
	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int sglslct; //ѡ��ĵ�ѡ��
	int band;  //�������Ĳ�����
// ��¼ѡ��Ĳ���
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
