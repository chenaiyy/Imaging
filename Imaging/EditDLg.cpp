// EditDLg.cpp : 实现文件
//

#include "stdafx.h"
#include "Imaging.h"
#include "EditDLg.h"


// EditDLg 对话框

IMPLEMENT_DYNAMIC(EditDLg, CDialog)

EditDLg::EditDLg(CWnd* pParent /*=NULL*/)
	: CDialog(EditDLg::IDD, pParent)
	, m_temp(_T(""))
{

}

EditDLg::~EditDLg()
{
}

void EditDLg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_Com);
	DDX_Text(pDX, IDC_EDIT1, m_temp);
	DDX_Control(pDX, IDC_LIST3, m_List);
}


BEGIN_MESSAGE_MAP(EditDLg, CDialog)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// EditDLg 消息处理程序

BOOL EditDLg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetWindowText("分类结果");
	m_List.InsertColumn(0, "Class_CLR", LVCFMT_LEFT,92);
	m_List.InsertColumn(1, "Total_Count", LVCFMT_LEFT,90);
	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void EditDLg::LoadClassNum(int num)
{
	m_num = num;
}

void EditDLg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: 在此处添加消息处理程序代码
	CString temp;
	for (int i = 1; i <= m_num; i++)
	{
		temp.Format("%d",i);
		m_Com.InsertString(-1,temp);
	}
}
