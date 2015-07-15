#pragma once


// EXTENTION 对话框

class EXTENTION : public CDialog
{
	DECLARE_DYNAMIC(EXTENTION)

public:
	EXTENTION(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~EXTENTION();

// 对话框数据
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_omin;
public:
	int m_omax;
public:
	int m_mode;
public:
	int m_fmin;
public:
	int m_fmax;
public:
	float m_stand;
};
