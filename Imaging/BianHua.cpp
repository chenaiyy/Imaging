// BianHua.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Imaging.h"
#include "BianHua.h"


// BianHua �Ի���

IMPLEMENT_DYNAMIC(BianHua, CDialog)

BianHua::BianHua(CWnd* pParent /*=NULL*/)
	: CDialog(BianHua::IDD, pParent)
	, m_firstfile(_T(""))
	, m_sercondfile(_T(""))
	, m_outfile(_T(""))
{

}

BianHua::~BianHua()
{
}

void BianHua::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_firstfile);
	DDX_Text(pDX, IDC_EDIT2, m_sercondfile);
	DDX_Text(pDX, IDC_EDIT3, m_outfile);
}


BEGIN_MESSAGE_MAP(BianHua, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &BianHua::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &BianHua::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &BianHua::OnBnClickedButton3)
END_MESSAGE_MAP()


// BianHua ��Ϣ�������

void BianHua::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	char szFilter[] = "GeoTiff (*.tif)|*.tif|All Files (*.*)|*.*||";
	CString filePath("");
	CFileDialog fileOpenDlg(TRUE, "tif", NULL,OFN_HIDEREADONLY,szFilter);
	if (fileOpenDlg.DoModal() == IDOK) 
	{
		VERIFY(filePath = fileOpenDlg.GetPathName());
	}
	m_firstfile = filePath;

	UpdateData(false);
}

void BianHua::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	char szFilter[] = "GeoTiff (*.tif)|*.tif|All Files (*.*)|*.*||";
	CString filePath("");
	CFileDialog fileOpenDlg(TRUE, "tif", NULL,OFN_HIDEREADONLY,szFilter);
	if (fileOpenDlg.DoModal() == IDOK) 
	{
		VERIFY(filePath = fileOpenDlg.GetPathName());
	}
	m_sercondfile = filePath;

	UpdateData(false);
}

void BianHua::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	char szFilter[] = "�ı��ļ� (*.txt)|*.txt|All Files (*.*)|*.*||";
	CString filePath("");
	CFileDialog fileOpenDlg(TRUE, "txt", NULL,OFN_HIDEREADONLY,szFilter);
	if (fileOpenDlg.DoModal() == IDOK) 
	{
		VERIFY(filePath = fileOpenDlg.GetPathName());
	}
	m_outfile = filePath;

	UpdateData(false);
}
