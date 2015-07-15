// EXTENTION.cpp : 实现文件
//

#include "stdafx.h"
#include "Imaging.h"
#include "EXTENTION.h"


// EXTENTION 对话框

IMPLEMENT_DYNAMIC(EXTENTION, CDialog)

EXTENTION::EXTENTION(CWnd* pParent /*=NULL*/)
	: CDialog(EXTENTION::IDD, pParent)
	, m_omin(0)
	, m_omax(0)
	, m_mode(0)
	, m_fmin(0)
	, m_fmax(0)
	, m_stand(0)
{

}

EXTENTION::~EXTENTION()
{
}

void EXTENTION::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_omin);
	DDV_MinMaxInt(pDX, m_omin, 0, 255);
	DDX_Text(pDX, IDC_EDIT2, m_omax);
	DDV_MinMaxInt(pDX, m_omax, 0, 255);
	DDX_Text(pDX, IDC_EDIT3, m_mode);
	DDV_MinMaxInt(pDX, m_mode, 0, 255);
	DDX_Text(pDX, IDC_EDIT5, m_fmin);
	DDV_MinMaxInt(pDX, m_fmin, 0, 255);
	DDX_Text(pDX, IDC_EDIT6, m_fmax);
	DDV_MinMaxInt(pDX, m_fmax, 0, 255);
	DDX_Text(pDX, IDC_EDIT4, m_stand);
}


BEGIN_MESSAGE_MAP(EXTENTION, CDialog)
END_MESSAGE_MAP()


// EXTENTION 消息处理程序
