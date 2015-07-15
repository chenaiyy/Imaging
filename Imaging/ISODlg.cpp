// ISODlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Imaging.h"
#include "ISODlg.h"


// ISODlg 对话框

IMPLEMENT_DYNAMIC(ISODlg, CDialog)

ISODlg::ISODlg(CWnd* pParent /*=NULL*/)
	: CDialog(ISODlg::IDD, pParent)
	, m_outfile(_T(""))
	, m_infile(_T(""))
	, m_iClasses(0)
	, m_iIniClasses(0)
	, m_iLoopNum(0)
	, m_iMaxMergeClassNum(0)
	, m_fMinPixelNum(0)
	, m_fMaxSD(0)
	, m_fMinClassDis(0)
{

}

ISODlg::~ISODlg()
{
}

void ISODlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_outfile);
	DDX_Text(pDX, IDC_EDIT3, m_infile);
	DDX_Text(pDX, IDC_EDIT4, m_iClasses);
	DDX_Text(pDX, IDC_EDIT5, m_iIniClasses);
	DDX_Text(pDX, IDC_EDIT6, m_iLoopNum);
	DDX_Text(pDX, IDC_EDIT7, m_iMaxMergeClassNum);
	DDX_Text(pDX, IDC_EDIT8, m_fMinPixelNum);
	DDX_Text(pDX, IDC_EDIT9, m_fMaxSD);
	DDX_Text(pDX, IDC_EDIT10, m_fMinClassDis);
}


BEGIN_MESSAGE_MAP(ISODlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &ISODlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &ISODlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// ISODlg 消息处理程序

BOOL ISODlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_iClasses = 7;
	m_iIniClasses = 5;
	m_iLoopNum= 4;
	m_iMaxMergeClassNum= 2;
	m_fMinPixelNum= 0.01;
	m_fMaxSD= 6.0;
	m_fMinClassDis= 3.0;
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void ISODlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	char szFilter[] = "GeoTiff (*.tif)|*.tif|All Files (*.*)|*.*||";
	CString filePath("");
	CFileDialog fileOpenDlg(TRUE, "tif", NULL,OFN_HIDEREADONLY,szFilter);
	if (fileOpenDlg.DoModal() == IDOK) 
	{
		VERIFY(filePath = fileOpenDlg.GetPathName());
	}
	m_infile = filePath;
	UpdateData(false);
}

void ISODlg::OnBnClickedButton2()
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
	UpdateData(false);
}
