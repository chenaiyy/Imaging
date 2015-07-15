// Show.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Imaging.h"
#include "Show.h"


// Show �Ի���

IMPLEMENT_DYNAMIC(Show, CDialog)

Show::Show(CWnd* pParent /*=NULL*/)
	: CDialog(Show::IDD, pParent)
	, sglslct(0)
{
	band = 0;
	B_blue = 1;
	B_gray = 1;
	B_red = 3;
	B_green = 2;
	B_index = 1;
}

Show::~Show()
{
}

void Show::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO2, combo_gray);
	DDX_Control(pDX, IDC_COMBO3, combo_index);
	DDX_Control(pDX, IDC_COMBO4, combo_red);
	DDX_Control(pDX, IDC_COMBO5, combo_green);
	DDX_Control(pDX, IDC_COMBO6, combo_blue);
}


BEGIN_MESSAGE_MAP(Show, CDialog)
	ON_BN_CLICKED(IDC_RADIO1, &Show::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO3, &Show::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO2, &Show::OnBnClickedRadio2)
	ON_WM_SHOWWINDOW()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// Show ��Ϣ�������

BOOL Show::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CWnd::CheckRadioButton(IDC_RADIO1,IDC_RADIO3,IDC_RADIO1);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void Show::Loaddata(int num)
{
	band = num;
}

void Show::OnBnClickedRadio1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	sglslct = 0;

}

void Show::OnBnClickedRadio3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	sglslct = 2;
}

void Show::OnBnClickedRadio2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	sglslct = 1;
}

void Show::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: �ڴ˴������Ϣ����������
	CString name = "band ";
	CString fullname;
	CString temp;
	for (int i = 1; i <= band; i++)
	{
		temp.Format("%d",i);
		fullname = name + temp;
		combo_gray.InsertString(-1,fullname);
		combo_index.InsertString(-1,fullname);
		combo_red.InsertString(-1,fullname);
		combo_green.InsertString(-1,fullname);
		combo_blue.InsertString(-1,fullname);
	}

}


void Show::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	B_blue = combo_blue.GetCurSel() + 1;
	B_red = combo_red.GetCurSel() + 1;
	B_green = combo_green.GetCurSel() + 1;
	B_gray = combo_gray.GetCurSel() + 1;
	B_index = combo_index.GetCurSel() + 1; 
}
