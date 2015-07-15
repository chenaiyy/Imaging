// Rgst.cpp : 实现文件
//

#include "stdafx.h"
#include "Imaging.h"
#include "Rgst.h"


// Rgst 对话框

IMPLEMENT_DYNAMIC(Rgst, CDialog)

Rgst::Rgst(CWnd* pParent /*=NULL*/)
	: CDialog(Rgst::IDD, pParent)
	, m_basefile(_T(""))
	, m_warpfile(_T(""))
	, m_gcpfile(_T(""))
{

}

Rgst::~Rgst()
{
}

void Rgst::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_basefile);
	DDX_Text(pDX, IDC_EDIT2, m_warpfile);
	DDX_Text(pDX, IDC_EDIT3, m_gcpfile);
}


BEGIN_MESSAGE_MAP(Rgst, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &Rgst::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &Rgst::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &Rgst::OnBnClickedButton3)
END_MESSAGE_MAP()


// Rgst 消息处理程序

void Rgst::OnBnClickedButton1()  //获取参考影像文件路径
{
	// TODO: 在此添加控件通知处理程序代码
	char szFilter[] = "GeoTiff (*.tif)|*.tif|All Files (*.*)|*.*||";
	CString filePath("");
	CFileDialog fileOpenDlg(TRUE, "tif", NULL,OFN_HIDEREADONLY,szFilter);
	if (fileOpenDlg.DoModal() == IDOK) 
	{
		VERIFY(filePath = fileOpenDlg.GetPathName());
	}
	m_basefile = filePath;

	UpdateData(false);
}

void Rgst::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	char szFilter[] = "GeoTiff (*.tif)|*.tif|All Files (*.*)|*.*||";
	CString filePath("");
	CFileDialog fileOpenDlg(TRUE, "tif", NULL,OFN_HIDEREADONLY,szFilter);
	if (fileOpenDlg.DoModal() == IDOK) 
	{
		VERIFY(filePath = fileOpenDlg.GetPathName());
	}
	m_warpfile = filePath;

	UpdateData(false);
}

void Rgst::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	char szFilter[] = "文本文件 (*.txt)|*.txt|All Files (*.*)|*.*||";
	CString filePath("");
	CFileDialog fileOpenDlg(TRUE, "txt", NULL,OFN_HIDEREADONLY,szFilter);
	if (fileOpenDlg.DoModal() == IDOK) 
	{
		VERIFY(filePath = fileOpenDlg.GetPathName());
	}
	m_gcpfile = filePath;

	UpdateData(false);
}
