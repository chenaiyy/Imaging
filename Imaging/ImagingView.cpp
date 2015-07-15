// ImagingView.cpp : CImagingView ���ʵ��
//

#include "stdafx.h"
#include "Imaging.h"
#include <cmath>

#include "ImagingDoc.h"
#include "ImagingView.h"
#include "Show.h"
#include "EXTENTION.h"

#include <fstream>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

ofstream outfile;

// CImagingView

IMPLEMENT_DYNCREATE(CImagingView, CView)

BEGIN_MESSAGE_MAP(CImagingView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_FILE_OPEN, &CImagingView::OnFileOpen)
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_32771, &CImagingView::On32771)
	ON_COMMAND(ID_32772, &CImagingView::On32772)
	ON_COMMAND(ID_32773, &CImagingView::Onselfadapt)
	ON_COMMAND(ID_32775, &CImagingView::On32775)
	ON_COMMAND(ID_32777, &CImagingView::OnPredict)
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_32778, &CImagingView::OutGcp)
	ON_WM_RBUTTONUP()
	ON_WM_MBUTTONUP()
	ON_COMMAND(ID_32780, &CImagingView::OnGmtryCrrct)
	ON_COMMAND(ID_32782, &CImagingView::OnIsoData)
	ON_COMMAND(ID_32784, &CImagingView::On32784)
END_MESSAGE_MAP()

// CImagingView ����/����

CImagingView::CImagingView()
{
	// TODO: �ڴ˴���ӹ������
	show = NULL;
	//show_clr = NULL;
	IS_Gray = false;
	ISindex = false;
	x = -1.0;
	y = -1.0;
	m_width = 0;
	m_height = 0;
	m_nbands = 0;

	for (int i = 0; i < 3; i++)
	{
		clrful[i] = 3-i;
	}

	IsAdd = false;
}

CImagingView::~CImagingView()
{
	//if(show)
	//	delete show;
//	show = NULL;
}

BOOL CImagingView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CImagingView ����

void CImagingView::OnDraw(CDC* pDC)
{
	CImagingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
	if (show)
	{
		if (!ISindex)
			Draw(show,IS_Gray);
		else
			IndexShow(show);
	}
	if(IsAdd)
	{
		if(GcpInfo.empty())
			return;

		int x,y;
		CRect rect;
		GetClientRect(&rect);

		int temp_W = (int)m_width*rect.Height()/m_height;
		int x_offset = (int)(rect.Width()-temp_W)/2;

		CPen pen(PS_SOLID,1,RGB(255,0,0));
		CPen *pOldPen = pDC->SelectObject(&pen);
		
		for(vector<CPoint>::size_type ix = 0; ix != GcpInfo.size(); ix++)
		{
			
			x = (int)(GcpInfo[ix].x*rect.Height()/m_height + x_offset);
			y = (int)(GcpInfo[ix].y*rect.Height()/m_height);
			
			pDC->MoveTo(x,y-10);
			pDC->LineTo(x,y+10);
			pDC->MoveTo(x-10,y);
			pDC->LineTo(x+10,y);
		}

		pDC->SelectObject(pOldPen);

	}
}


// CImagingView ��ӡ

BOOL CImagingView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CImagingView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CImagingView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// CImagingView ���

#ifdef _DEBUG
void CImagingView::AssertValid() const
{
	CView::AssertValid();
}

void CImagingView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CImagingDoc* CImagingView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImagingDoc)));
	return (CImagingDoc*)m_pDocument;
}
#endif //_DEBUG


// CImagingView ��Ϣ�������

void CImagingView::OnFileOpen()
{
	// TODO: �ڴ���������������
	// ��ȡ�ļ�·��
	char szFilter[] = "GeoTiff (*.tif)|*.tif|All Files (*.*)|*.*||";
	CString filePath("");
	CFileDialog fileOpenDlg(TRUE, "tif", NULL,OFN_HIDEREADONLY,szFilter);
	if (fileOpenDlg.DoModal() == IDOK) 
	{
		VERIFY(filePath = fileOpenDlg.GetPathName());
	}
//��ȡӰ�������Ϣ
	if(!Test.ReadFile(filePath))
		return;

	this->Invalidate(true); //
	m_width = Test.nXsize;
	m_height = Test.nYsize;
	m_nbands = Test.nBands;


	if (m_nbands < 3)
	{
		IS_Gray = false;
		show = (BYTE *) CPLMalloc(sizeof(BYTE)*(m_width)*(m_height));
		Test.read1band(show,1,m_width,m_height);
		Draw(show,IS_Gray);
	}

	else
	{
		IS_Gray = true;
		show = (BYTE *) CPLMalloc(sizeof(BYTE)*(m_width)*(m_height)*3);
		Test.read3band(show,clrful,m_width,m_height);
		Draw(show,IS_Gray);
	}
}

void CImagingView::Draw(BYTE * data,bool clr)
{
	LONG i, j; //ѭ������
	CDC *pDC = GetDC();
	
	//this->Invalidate(true);
	if(!clr)   //�Ҷ���ʾ
	{
		BITMAPINFO *pBitmapInfo = (BITMAPINFO*)new char[sizeof(BITMAPINFO) + 
			sizeof(RGBQUAD) * (256)]; 

		pBitmapInfo->bmiHeader.biClrUsed = 0;
		pBitmapInfo->bmiHeader.biBitCount = 8;
		pBitmapInfo->bmiHeader.biClrImportant = 0;
		pBitmapInfo->bmiHeader.biCompression = BI_RGB;
		pBitmapInfo->bmiHeader.biWidth = m_width;
		pBitmapInfo->bmiHeader.biHeight = m_height;
		pBitmapInfo->bmiHeader.biPlanes = 1;
		pBitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);//sizeof(BITMAPINFO);
		pBitmapInfo->bmiHeader.biSizeImage = (m_width*8+31)/32*4*m_height; 
		pBitmapInfo->bmiHeader.biXPelsPerMeter = 0;	
		pBitmapInfo->bmiHeader.biYPelsPerMeter = 0;

		//Ϊ��ɫ��ֵ
		for ( i=0; i<256; i++)
		{
			pBitmapInfo->bmiColors[i].rgbBlue = BYTE(i);
			pBitmapInfo->bmiColors[i].rgbGreen = BYTE(i);
			pBitmapInfo->bmiColors[i].rgbRed = BYTE(i);
			pBitmapInfo->bmiColors[i].rgbReserved = 0;
		}

		//����������
		LONG LineBytes = (m_width*8+31)/32*4;
		LPBYTE pData = (LPBYTE)new char[LineBytes*m_height];

		//Ϊ�����ظ�ֵ
		for ( i=0; i<m_height; i++)
			for ( j=0; j<m_width; j++)
				pData[(m_height-i-1)*LineBytes + j] = data[i*m_width + j];
//  
		CRect rect;
		GetClientRect(&rect);
		int temp_W = (int)m_width*rect.Height()/m_height;
		int x_offset = (int)(rect.Width()-temp_W)/2;
		SetStretchBltMode(pDC->m_hDC,BLACKONWHITE|WHITEONBLACK);
		StretchDIBits(pDC->m_hDC, x_offset, 0, temp_W, rect.Height(), 0, 0, m_width, m_height, 
			pData, pBitmapInfo, DIB_RGB_COLORS, SRCCOPY);

		delete pBitmapInfo;
		delete pData;

	}
	else    //��ɫ��ʾ
	{

		BITMAPINFO *pBitmapInfo = (BITMAPINFO*)new char[sizeof(BITMAPINFO) ]; 

		pBitmapInfo->bmiHeader.biClrUsed = 0;
		pBitmapInfo->bmiHeader.biBitCount = 24;
		pBitmapInfo->bmiHeader.biClrImportant = 0;
		pBitmapInfo->bmiHeader.biCompression = BI_RGB;
		pBitmapInfo->bmiHeader.biWidth = m_width;
		pBitmapInfo->bmiHeader.biHeight = m_height;
		pBitmapInfo->bmiHeader.biPlanes = 1;
		pBitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);//sizeof(BITMAPINFO);
		pBitmapInfo->bmiHeader.biSizeImage = (m_width*24+31)/32*4*m_height; 
		pBitmapInfo->bmiHeader.biXPelsPerMeter = 0;	
		pBitmapInfo->bmiHeader.biYPelsPerMeter = 0;

		//����������	
		LONG LineBytes = (m_width*24 + 31)/32*4;
		LPBYTE pData = (LPBYTE)new char[LineBytes*m_height*3];

		//Ϊ�����ظ�ֵ
		for ( i=0; i<m_height; i++)
			for ( j=0; j<m_width; j++)
			{
//				pData[(m_height-i-1)*LineBytes + j] = data[i*m_width*3 + j]; //��������BGR����λͼ��
				pData[(m_height-i-1)*LineBytes + 3*j] = data[i*m_width + j];
				pData[(m_height-i-1)*LineBytes + 3*j+1] = data[(m_width*m_height) + i*m_width + j];
				pData[(m_height-i-1)*LineBytes + 3*j+2] = data[2*(m_width*m_height) + i*m_width + j];
			}
		CRect rect;
		GetClientRect(&rect); 
		int temp_W = (int)m_width*rect.Height()/m_height;
		int x_offset = (int)(rect.Width()-temp_W)/2;
		SetStretchBltMode(pDC->m_hDC,BLACKONWHITE|WHITEONBLACK);
		StretchDIBits(pDC->m_hDC, x_offset, 0, temp_W, rect.Height(), 0, 0, m_width, m_height, 
			pData, pBitmapInfo, DIB_RGB_COLORS, SRCCOPY);


//		UpdateWindow();		
		delete pBitmapInfo;
		delete pData;

	}
	ReleaseDC(pDC);
}

BOOL CImagingView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return CView::OnEraseBkgnd(pDC);
}

void CImagingView::On32771()   //����ʾ��ʽ�Ի���
{
	// TODO: �ڴ���������������
	if(!show)
	{
		AfxMessageBox("û�д����ݣ��޷����в�������");
		return;
	}
	Show showdlg;
	showdlg.Loaddata(m_nbands);
	
	if (showdlg.DoModal() == IDOK)
	{
		switch(showdlg.sglslct)
		{
		case 0:	
			if(IS_Gray)
				IS_Gray = false;
			if(ISindex)
				ISindex = false;

			show = (BYTE *) CPLMalloc(sizeof(BYTE)*(m_width)*(m_height));
			Test.read1band(show,showdlg.B_gray,m_width,m_height);
			Draw(show,IS_Gray);
			break;
		case 1:
			if(!IS_Gray)
				IS_Gray = true;
			if(ISindex)
				ISindex = false;

			clrful[0] = showdlg.B_blue;
			clrful[1] = showdlg.B_green;
			clrful[2] = showdlg.B_red;
			show = (BYTE *) CPLMalloc(sizeof(BYTE)*(m_width)*(m_height)*3);
			Test.read3band(show,clrful,m_width,m_height);
			Draw(show,IS_Gray);
			break;
		case 2:
			if(!ISindex)
				ISindex = true;

			show = (BYTE *) CPLMalloc(sizeof(BYTE)*(m_width)*(m_height));
			Test.read1band(show,showdlg.B_index,m_width,m_height);
			IndexShow(show);
			break;
		default:
			AfxMessageBox("û����ȷ����ʾ��ʽ");
				break;
		}

	}

}


void CImagingView::IndexShow(BYTE *index)
{
	LONG i, j; //ѭ������
	CDC *pDC = GetDC();


	BITMAPINFO *pBitmapInfo = (BITMAPINFO*)new char[sizeof(BITMAPINFO) + 
		sizeof(RGBQUAD) * (256)]; 

	pBitmapInfo->bmiHeader.biClrUsed = 0;
	pBitmapInfo->bmiHeader.biBitCount = 8;
	pBitmapInfo->bmiHeader.biClrImportant = 0;
	pBitmapInfo->bmiHeader.biCompression = BI_RGB;
	pBitmapInfo->bmiHeader.biWidth = m_width;
	pBitmapInfo->bmiHeader.biHeight = m_height;
	pBitmapInfo->bmiHeader.biPlanes = 1;
	pBitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);//sizeof(BITMAPINFO);
	pBitmapInfo->bmiHeader.biSizeImage = (m_width*8+31)/32*4*m_height; 
	pBitmapInfo->bmiHeader.biXPelsPerMeter = 0;	
	pBitmapInfo->bmiHeader.biYPelsPerMeter = 0;

	//Ϊ��ɫ��������ֵ

	for ( i = 0; i < 64; i++) 
	{
		pBitmapInfo->bmiColors[i].rgbBlue = BYTE(255);
		pBitmapInfo->bmiColors[i].rgbGreen = BYTE(4*i);
		pBitmapInfo->bmiColors[i].rgbRed = 0;
		pBitmapInfo->bmiColors[i].rgbReserved = 0;
	}
	for ( i = 64; i < 128; i++) 
	{
		pBitmapInfo->bmiColors[i].rgbBlue = BYTE(i/4);
		pBitmapInfo->bmiColors[i].rgbGreen = BYTE(255);
		pBitmapInfo->bmiColors[i].rgbRed = 0;
		pBitmapInfo->bmiColors[i].rgbReserved = 0;
	}
	for ( i = 128; i < 192; i++) 
	{
		pBitmapInfo->bmiColors[i].rgbBlue = 0;
		pBitmapInfo->bmiColors[i].rgbGreen = BYTE(255);
		pBitmapInfo->bmiColors[i].rgbRed = BYTE(4*i-512);
		pBitmapInfo->bmiColors[i].rgbReserved = 0;
	}
	for ( i = 192; i < 256; i++) 
	{
		pBitmapInfo->bmiColors[i].rgbBlue = 0;
		pBitmapInfo->bmiColors[i].rgbGreen = BYTE(1024-4*i);
		pBitmapInfo->bmiColors[i].rgbRed = 0;
		pBitmapInfo->bmiColors[i].rgbReserved = 0;
	}

	//����������
	LONG LineBytes = (m_width*8+31)/32*4;
	LPBYTE pData = (LPBYTE)new char[LineBytes*m_height];

	//Ϊ�����ظ�ֵ
	for ( i=0; i<m_height; i++)
		for ( j=0; j<m_width; j++)
			pData[(m_height-i-1)*LineBytes + j] = index[i*m_width + j];

	CRect rect;
	GetClientRect(&rect);
	int temp_W = (int)m_width*rect.Height()/m_height;
	int x_offset = (int)(rect.Width()-temp_W)/2;
	SetStretchBltMode(pDC->m_hDC,BLACKONWHITE|WHITEONBLACK);
	StretchDIBits(pDC->m_hDC, x_offset, 0, temp_W, rect.Height(), 0, 0, m_width, m_height, 
		pData, pBitmapInfo, DIB_RGB_COLORS, SRCCOPY);


	delete pBitmapInfo;
	delete pData;
	ReleaseDC(pDC);
}
void CImagingView::On32772() //ֱ��ͼ���⻯��ʾ
{
	// TODO: �ڴ���������������
	if(!show)
	{
		AfxMessageBox("û�е���ͼ��");
		return;
	}
	if(ISindex)
		ISindex = false; 

	if(!IS_Gray)
	{
		//GetInfo Info(m_width,m_height);  //�����ȡͼ����Ϣ�Ķ���
		float hst[256];
		float tempt[256];
		int pxl[256];
		//ѭ������
		int i;
		int j;
		//��ʱ����
		int temp = 0;
		int rt[256];
		int trans;

		for(i = 0; i < 256;i++)
			rt[i] = 0;

		for (i = 0; i < m_width; i++)
		{
			for (j = 0; j < m_height; j++)
			{
				temp = (int)show[i*m_height + j];
				rt[temp] ++;
			}
		}
		for(i = 0;i < 256; i++)
			hst[i] = rt[i]/(m_width*m_height*1.0f);

		//���о��⻯����
		for (i = 0; i < 256; i++)
		{
			if(i == 0)
				tempt[i] = hst[i];
			else
				tempt[i] = tempt[i-1] +hst[i];
			pxl[i] = (int)(255.0f * tempt[i] + 0.5f);
		}

		for (i = 0; i < m_width*m_height; i++)
		{
			trans = (int)show[i];
			show[i] = (BYTE)pxl[trans];
		}
	}

	else
	{
		float hst[3][256];
		float tempt[3][256];
		int pxl[3][256];
		//ѭ������
		int i;
		int j;
		//��ʱ����
		int temp = 0;
		int rt[3][256];
		int trans;
		for(i = 0; i < 3; i++)
			for(j = 0; j < 256;j++)
				rt[i][j] = 0;

		for (i = 0; i <3 ; i++)
		{
			for (j = 0; j < m_width*m_height; j++)
			{
				temp = (int)show[i*(m_height*m_width) + j];
				rt[i][temp] ++;
			}
		}

		for(i = 0; i < 3; i++)
		{
			for(j = 0;j < 256; j++)
				hst[i][j] = rt[i][j]/(m_width*m_height*1.0f);
		}

		//���о��⻯����
		for (i = 0; i < 3; i++)
		{
			for (j = 0; j < 256; j++)
			{
				if(j == 0)
					tempt[i][j] = hst[i][j];
				else
					tempt[i][j] = tempt[i][j-1] +hst[i][j];
				pxl[i][j] = (int)(255.0f * tempt[i][j] + 0.5f);
			}
		}

		for(i = 0; i < 3; i++)
			for (j = 0; j < m_width*m_height; j++)
			{
				trans = (int)show[i*m_width*m_height + j];
				show[i*m_width*m_height+j] = (BYTE)pxl[i][trans];
			}
	}
	//��ͼ
	Draw(show,IS_Gray);

}

void CImagingView::Onselfadapt() //����������ʾ
{
	// TODO: �ڴ���������������
	if(!show)
	{
		AfxMessageBox("û�е���ͼ��");
		return;
	}
	if(ISindex)
		ISindex = false;

	//if(!IS_Gray)  //�Ҷ�ͼ��
	//{
		EXTENTION exdlg;  //�Ի������
		GetInfo Info(m_width,m_height);   //ͼ����Ϣ����
		float var;

		Info.Max(show,exdlg.m_omax);
		Info.Min(show,exdlg.m_omin);
		Info.Mod(show,exdlg.m_mode);
		Info.Variance(show,var);
		exdlg.m_fmin = 0;
		exdlg.m_fmax = 255;
		exdlg.m_stand = sqrt(var);

		int omax = exdlg.m_omax;
		int omin = exdlg.m_omin;
		int fmax ;
		int fmin ;

		if(exdlg.DoModal() == IDOK)
		{
			int i; //ѭ������
			int temp; //�м����
			fmax = exdlg.m_fmax;
			fmin = exdlg.m_fmin;

			float k = (float)(fmax - fmin)/(omax - omin);

			for(i = 0; i < m_width * m_height; i++)
			{
				temp = (int)show[i];
				show[i] = (BYTE)(fmin + k * (temp - omin));
			}

			Draw(show,IS_Gray);
		}
		else 
			return;
	//}
	//
	//else   //��ɫͼ��
	//{
	//}
}



//Ӱ�����
void CImagingView::On32775()  //Ӱ�����
{
	// TODO: �ڴ���������������
	if (!GcpInfo.empty())
		GcpInfo.clear();

	IsAdd = false;

	Rgst MyDlg;
	if (MyDlg.DoModal() == IDOK)
	{
		if(MyDlg.m_basefile == "")
		{
			AfxMessageBox("��û��ѡ��ο�Ӱ��");
			return;
		}

		if(MyDlg.m_warpfile == "")
		{
			AfxMessageBox("��û��ѡ���У��Ӱ��");
			return;
		}

		if(MyDlg.m_gcpfile == "")
		{
			AfxMessageBox("��û��������Ƶ��ļ��ı����ַ��");
			return;
		}

		Get_data Base;  //�ο�Ӱ��
		m_Gcpfile = MyDlg.m_gcpfile;

		//��ȡ�ο�Ӱ����Ϣ������ʾ�ڴ�����
		if(!Base.ReadFile(MyDlg.m_basefile))
			return;

		m_width = Base.nXsize;
		m_height = Base.nYsize;
		m_nbands = Base.nBands;

		//��ȡ��һ������Ϣ����Ӱ����ش���
		m_baseband = (BYTE *) CPLMalloc(sizeof(BYTE)*(m_width)*(m_height));
		Base.read1band(m_baseband,1,m_width,m_height);
		//��ʾ��Ϣ
		if (m_nbands < 3)
		{
			IS_Gray = false;
			show = (BYTE *) CPLMalloc(sizeof(BYTE)*(m_width)*(m_height));
			Base.read1band(show,1,m_width,m_height);
			Draw(show,IS_Gray);
		}

		else
		{
			IS_Gray = true;
			show = (BYTE *) CPLMalloc(sizeof(BYTE)*(m_width)*(m_height)*3);
			Base.read3band(show,clrful,m_width,m_height);
			Draw(show,IS_Gray);
		}


		//��У��ͼ�����Ϣ
		Get_data Warp;  //��У��Ӱ��
		if(!Warp.ReadFile(MyDlg.m_warpfile))
			return;
		m_warpxsize = Warp.nXsize;
		m_warpysize = Warp.nYsize;

		m_warpband = (BYTE *) CPLMalloc(sizeof(BYTE)*(m_warpxsize)*(m_warpysize));
		Warp.read1band(m_warpband,1,m_warpxsize,m_warpysize);
		
		this->Invalidate(true);
	}
	

}


void CImagingView::OnPredict()
{
	// TODO: �ڴ���������������
	IsAdd = true;
}

void CImagingView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (IsAdd)
	{
		int x,y;
		CRect rect;
		GetClientRect(&rect);
		
		int zoom = int(rect.Height()/m_height);
		int temp_W = (int)m_width*rect.Height()/m_height;
		int x_offset = (int)(rect.Width()-temp_W)/2;
//��ȡ��Ļ�ϵĵ���Ϣ
		if( point.x < x_offset || point.x > (rect.Width() -x_offset))
		{
			AfxMessageBox("��û�е����ͼ���ϣ�");
			return;
		}
//��Ӧ��ͼ����ȥ
		x = (int)((point.x - x_offset)*m_height/rect.Height());
		y = (int)(point.y*m_height/rect.Height());
//����Ϣ������Ƶ���Ϣ��
		CPoint temp;
		temp.x = x;
		temp.y = y;

		GcpInfo.push_back(temp);

//��ͼ���ϻ���ʮ��˿
		CDC *pdc=GetDC();
		CPen pen(PS_SOLID,1,RGB(255,0,0));
		CPen *pOldPen = pdc->SelectObject(&pen);
		
		pdc->MoveTo(point.x,point.y-10);
		pdc->LineTo(point.x,point.y+10);
		pdc->MoveTo(point.x-10,point.y);
		pdc->LineTo(point.x+10,point.y);

		pdc->SelectObject(pOldPen);
		ReleaseDC(pdc);

	}

	CView::OnLButtonUp(nFlags, point);
}

void CImagingView::OutGcp()
{
	// TODO: �ڴ���������������
	if(GcpInfo.empty())
	{
		AfxMessageBox("û��ѡ����Ƶ���Ϣ");
		return;
	}

	Registrat Reg;
	Reg.Getcoef();  //������ϵ��

	int warpx = 0;
	int warpy = 0;
	double coef;  //���ϵ��

	m_outfile.open(m_Gcpfile);
	for(vector<CPoint>::size_type ix = 0; ix != GcpInfo.size(); ix++)
	{
		/*m_outfile<<GcpInfo[ix].x<<" "<<GcpInfo[ix].y<<endl;*/
		coef = Reg.GetRelate(m_baseband,m_width,m_height,m_warpband,m_warpxsize,m_warpysize,
			GcpInfo[ix].x ,GcpInfo[ix].y,warpx,warpy);
		m_outfile<<"��"<<ix+1<<"�����Ƶ��: "<<"("<<GcpInfo[ix].x<<","<<GcpInfo[ix].y
			<<")"<<"  "<<"("<<warpx<<","<<warpy<<")"<<"  "<<"���ϵ��: "<<coef<<endl;
	}
	AfxMessageBox("��Ԥ��Ŀ��Ƶ��Ѿ����������");
	IsAdd = false;
}


void CImagingView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (GcpInfo.empty())
		return;

	GcpInfo.pop_back();
	this->Invalidate(true);

	CView::OnRButtonUp(nFlags, point);
}

void CImagingView::OnMButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	IsAdd = false;

	CView::OnMButtonUp(nFlags, point);
}


//����У��
void CImagingView::OnGmtryCrrct() //���ζ���ʽУ��
{
	// TODO: �ڴ���������������
	PREGCP CrrctDlg;
	if (CrrctDlg.DoModal() == IDOK)
	{
		//�ļ���ȡ���
		if (CrrctDlg.m_datafile == "")
		{
			AfxMessageBox("û�ж�ȡӰ��");
			return;
		}
		if (CrrctDlg.m_Gcpfile == "")
		{
			AfxMessageBox("û�ж�ȡ���Ƶ�");
			return;
		}
		if (CrrctDlg.m_outfile == "")
		{
			AfxMessageBox("û��ָ��У����Ӱ��洢λ�ã�");
			return;
		}
		//��ȡͼ����������ʾ
		if (!Test.ReadFile(CrrctDlg.m_datafile))
			return;

		m_nbands = Test.nBands;
		m_width = Test.nXsize;
		m_height = Test.nYsize;
		
//��ʾ����
		if (m_nbands < 3)
		{
			IS_Gray = false;
			show = (BYTE *) CPLMalloc(sizeof(BYTE)*(m_width)*(m_height));
			Test.read1band(show,1,m_width,m_height);
			Draw(show,IS_Gray);
		}
		else
		{
			IS_Gray = true;
			show = (BYTE *) CPLMalloc(sizeof(BYTE)*(m_width)*(m_height)*3);
			Test.read3band(show,clrful,m_width,m_height);
			Draw(show,IS_Gray);
		}
		this->Invalidate(true);
//===========================У��=============================================================//
		CGcpGeoCorrect GcpCorrect;
		//��ȡGCP��Ϣ		
		GcpCorrect.LoadGcpPoint(CrrctDlg.m_Gcpfile);
		//��ȡ����ʽϵ��
		GcpCorrect.computfacter(CrrctDlg.m_combo);

		//�����ز�����洢У�����Ӱ��
		GcpCorrect.Correct(CrrctDlg.m_datafile,CrrctDlg.m_outfile,CrrctDlg.m_slct+1);
		//��ʾУ�����Ӱ��

		if (!Test.ReadFile(CrrctDlg.m_outfile))
			return;
		m_nbands = Test.nBands;
		m_width = Test.nXsize;
		m_height = Test.nYsize;
		//��ʾ����
		if (m_nbands < 3)
		{
			IS_Gray = false;
			show = (BYTE *) CPLMalloc(sizeof(BYTE)*(m_width)*(m_height));
			Test.read1band(show,1,m_width,m_height);
			Draw(show,IS_Gray);
		}
		else
		{
			IS_Gray = true;
			show = (BYTE *) CPLMalloc(sizeof(BYTE)*(m_width)*(m_height)*3);
			Test.read3band(show,clrful,m_width,m_height);
			Draw(show,IS_Gray);
		}
		this->Invalidate(true);
	}
}


//����
void CImagingView::OnIsoData()
{
	// TODO: �ڴ���������������
	ISODlg MyDlg;
	CISODATA Iso;
	if(MyDlg.DoModal() == IDOK)
	{
		//�ļ���ȡ���
		if (MyDlg.m_infile == "")
		{
			AfxMessageBox("û�ж�ȡӰ��");
			return;
		}
		if (MyDlg.m_outfile == "")
		{
			AfxMessageBox("û�и������·����");
			return;
		}
		this->Invalidate(true);
		//ԭ������ʾ
		if (!Test.ReadFile(MyDlg.m_infile))
			return;

		m_nbands = Test.nBands;
		m_width = Test.nXsize;
		m_height = Test.nYsize;

		if (m_nbands < 3)
		{
			IS_Gray = false;
			show = (BYTE *) CPLMalloc(sizeof(BYTE)*(m_width)*(m_height));
			Test.read1band(show,1,m_width,m_height);
			Draw(show,IS_Gray);
		}
		else
		{
			IS_Gray = true;
			show = (BYTE *) CPLMalloc(sizeof(BYTE)*(m_width)*(m_height)*3);
			Test.read3band(show,clrful,m_width,m_height);
			Draw(show,IS_Gray);
		}
		//���ݴ���
		Iso.m_infile = MyDlg.m_infile;
		Iso.m_outfile = MyDlg.m_outfile;
		Iso.m_iClasses = MyDlg.m_iClasses;
		Iso.m_iIniClasses = MyDlg.m_iIniClasses;
		Iso.m_iLoopNum = MyDlg.m_iLoopNum;
		Iso.m_iMaxMergeClassNum = MyDlg.m_iMaxMergeClassNum;
		Iso.m_fMinPixelNum = MyDlg.m_fMinPixelNum;
		Iso.m_fMaxSD = MyDlg.m_fMaxSD;
		Iso.m_fMinClassDis = MyDlg.m_fMinClassDis;

		Iso.Classfy();
		//�����������ʾ
		if (!Test.ReadFile(MyDlg.m_outfile))
			return;

		m_nbands = Test.nBands;
		m_width = Test.nXsize;
		m_height = Test.nYsize;

		if (m_nbands < 3)
		{
			IS_Gray = false;
			show = (BYTE *) CPLMalloc(sizeof(BYTE)*(m_width)*(m_height));
			Test.read1band(show,1,m_width,m_height);
			Draw(show,IS_Gray);
		}
		else
		{
			IS_Gray = true;
			show = (BYTE *) CPLMalloc(sizeof(BYTE)*(m_width)*(m_height)*3);
			Test.read3band(show,clrful,m_width,m_height);
			Draw(show,IS_Gray);
		}
		this->Invalidate(true);
	}
}


//�仯���
void CImagingView::On32784()//�仯���
{
	// TODO: �ڴ���������������
	BianHua BDlg;
	UpDt TTT;

	//ֻ���ȡһ�����ν��б仯���
	if (BDlg.DoModal() == IDOK)
	{
		TTT.ReadFile(BDlg.m_firstfile,BDlg.m_sercondfile);
		TTT.Sub(BDlg.m_outfile);
	}
}
