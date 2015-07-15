// EditDLg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Imaging.h"
#include "EditDLg.h"


// EditDLg �Ի���

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


// EditDLg ��Ϣ�������

BOOL EditDLg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SetWindowText("������");
	m_List.InsertColumn(0, "Class_CLR", LVCFMT_LEFT,92);
	m_List.InsertColumn(1, "Total_Count", LVCFMT_LEFT,90);
	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void EditDLg::LoadClassNum(int num)
{
	m_num = num;
}

void EditDLg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: �ڴ˴������Ϣ����������
	CString temp;
	for (int i = 1; i <= m_num; i++)
	{
		temp.Format("%d",i);
		m_Com.InsertString(-1,temp);
	}
}
