#pragma once


// EXTENTION �Ի���

class EXTENTION : public CDialog
{
	DECLARE_DYNAMIC(EXTENTION)

public:
	EXTENTION(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~EXTENTION();

// �Ի�������
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
