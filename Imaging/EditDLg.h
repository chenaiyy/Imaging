#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// EditDLg 对话框

class EditDLg : public CDialog
{
	DECLARE_DYNAMIC(EditDLg)

public:
	EditDLg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~EditDLg();

	int m_num;

// 对话框数据
	enum { IDD = IDD_DIALOG6 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
