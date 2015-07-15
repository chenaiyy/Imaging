// PREGCP.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Imaging.h"
#include "PREGCP.h"


// PREGCP �Ի���

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


// PREGCP ��Ϣ�������

void PREGCP::OnOpenfile()  //��Ӱ������·��
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	char szFilter[] = "GeoTiff (*.tif)|*.tif|All Files (*.*)|*.*||";
	CString filePath("");
	CFileDialog fileOpenDlg(TRUE, "tif", NULL,OFN_HIDEREADONLY,szFilter);
	if (fileOpenDlg.DoModal() == IDOK) 
	{
		VERIFY(filePath = fileOpenDlg.GetPathName());
	}
	m_datafile = filePath;
}

void PREGCP::OnReadGcps()  //��ȡ���Ƶ��ļ�·��
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	char szFilter[] = "�ı��ļ� (*.txt)|*.txt|All Files (*.*)|*.*||";
	CString filePath("");
	CFileDialog fileOpenDlg(TRUE, "txt", NULL,OFN_HIDEREADONLY,szFilter);
	if (fileOpenDlg.DoModal() == IDOK) 
	{
		VERIFY(filePath = fileOpenDlg.GetPathName());
	}
	m_Gcpfile = filePath;
}

void PREGCP::OnOutfile()  //У�����ͼ��洢·��
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CWnd::CheckRadioButton(IDC_RADIO1,IDC_RADIO3,IDC_RADIO1);  //��ѡ��ĳ�ʼ��
 
	//Combobox�ĳ�ʼ��
	CString temp;
	for (int i = 1; i < 4; i++)
	{
		temp.Format("%d",i);
		m_Multi.InsertString(-1,temp);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void PREGCP::OnBnClickedRadio1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_slct = 0;
}

void PREGCP::OnBnClickedRadio2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_slct = 1;
}

void PREGCP::OnBnClickedRadio3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_slct = 2;
}

void PREGCP::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	m_combo = m_Multi.GetCurSel()+1;
}
