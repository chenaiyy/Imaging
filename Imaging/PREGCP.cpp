// PREGCP.cpp : 实现文件
//

#include "stdafx.h"
#include "Imaging.h"
#include "PREGCP.h"


// PREGCP 对话框

IMPLEMENT_DYNAMIC(PREGCP, CDialog)

PREGCP::PREGCP(CWnd* pParent /*=NULL*/)
	: CDialog(PREGCP::IDD, pParent)
	, m_outtif(_T(""))
{
	m_slct = 0;
	m_datafile = "";
	m_Gcpfile = "";
	m_outfile = "";

}

PREGCP::~PREGCP()
{
}

void PREGCP::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_Multi);
	DDX_Text(pDX, IDC_EDIT1, m_outtif);
}


BEGIN_MESSAGE_MAP(PREGCP, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &PREGCP::OnOpenfile)
	ON_BN_CLICKED(IDC_BUTTON2, &PREGCP::OnReadGcps)
	ON_BN_CLICKED(IDC_BUTTON3, &PREGCP::OnOutfile)
	ON_BN_CLICKED(IDC_RADIO1, &PREGCP::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &PREGCP::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &PREGCP::OnBnClickedRadio3)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// PREGCP 消息处理程序

void PREGCP::OnOpenfile()  //打开影像数据路径
{
	// TODO: 在此添加控件通知处理程序代码
	char szFilter[] = "GeoTiff (*.tif)|*.tif|All Files (*.*)|*.*||";
	CString filePath("");
	CFileDialog fileOpenDlg(TRUE, "tif", NULL,OFN_HIDEREADONLY,szFilter);
	if (fileOpenDlg.DoModal() == IDOK) 
	{
		VERIFY(filePath = fileOpenDlg.GetPathName());
	}
	m_datafile = filePath;
}

void PREGCP::OnReadGcps()  //读取控制点文件路径
{
	// TODO: 在此添加控件通知处理程序代码
	char szFilter[] = "文本文件 (*.txt)|*.txt|All Files (*.*)|*.*||";
	CString filePath("");
	CFileDialog fileOpenDlg(TRUE, "txt", NULL,OFN_HIDEREADONLY,szFilter);
	if (fileOpenDlg.DoModal() == IDOK) 
	{
		VERIFY(filePath = fileOpenDlg.GetPathName());
	}
	m_Gcpfile = filePath;
}

void PREGCP::OnOutfile()  //校正后的图像存储路径
{
	// TODO: 在此添加控件通知处理程序代码
	char szFilter[] = "GeoTiff (*.tif)|*.tif|All Files (*.*)|*.*||";
	CString filePath("");
	CFileDialog fileOpenDlg(TRUE, "tif", NULL,OFN_HIDEREADONLY,szFilter);
	if (fileOpenDlg.DoModal() == IDOK) 
	{
		VERIFY(filePath = fileOpenDlg.GetPathName());
	}
	m_outfile = filePath;
	m_outtif = m_outfile;

	UpdateData(false);
}

BOOL PREGCP::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CWnd::CheckRadioButton(IDC_RADIO1,IDC_RADIO3,IDC_RADIO1);  //单选框的初始化
 
	//Combobox的初始化
	CString temp;
	for (int i = 1; i < 4; i++)
	{
		temp.Format("%d",i);
		m_Multi.InsertString(-1,temp);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void PREGCP::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_slct = 0;
}

void PREGCP::OnBnClickedRadio2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_slct = 1;
}

void PREGCP::OnBnClickedRadio3()
{
	// TODO: 在此添加控件通知处理程序代码
	m_slct = 2;
}

void PREGCP::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	m_combo = m_Multi.GetCurSel()+1;
}
