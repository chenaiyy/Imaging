#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// EditDLg �Ի���

class EditDLg : public CDialog
{
	DECLARE_DYNAMIC(EditDLg)

public:
	EditDLg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~EditDLg();

	int m_num;

// �Ի�������
	enum { IDD = IDD_DIALOG6 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void LoadClassNum(int num);
public:
	CComboBox m_Com;
public:
	CString m_temp;
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
public:
	CListCtrl m_List;
};
