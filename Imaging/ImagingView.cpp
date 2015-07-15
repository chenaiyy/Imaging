// ImagingView.cpp : CImagingView 类的实现
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
	// 标准打印命令
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

// CImagingView 构造/析构

CImagingView::CImagingView()
{
	// TODO: 在此处添加构造代码
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
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CImagingView 绘制

void CImagingView::OnDraw(CDC* pDC)
{
	CImagingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	// TODO: 在此处为本机数据添加绘制代码
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


// CImagingView 打印

BOOL CImagingView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CImagingView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CImagingView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清除过程
}


// CImagingView 诊断

#ifdef _DEBUG
void CImagingView::AssertValid() const
{
	CView::AssertValid();
}

void CImagingView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CImagingDoc* CImagingView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImagingDoc)));
	return (CImagingDoc*)m_pDocument;
}
#endif //_DEBUG


// CImagingView 消息处理程序

void CImagingView::OnFileOpen()
{
	// TODO: 在此添加命令处理程序代码
	// 获取文件路径
	char szFilter[] = "GeoTiff (*.tif)|*.tif|All Files (*.*)|*.*||";
	CString filePath("");
	CFileDialog fileOpenDlg(TRUE, "tif", NULL,OFN_HIDEREADONLY,szFilter);
	if (fileOpenDlg.DoModal() == IDOK) 
	{
		VERIFY(filePath = fileOpenDlg.GetPathName());
	}
//读取影像基本信息
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
	LONG i, j; //循环变量
	CDC *pDC = GetDC();
	
	//this->Invalidate(true);
	if(!clr)   //灰度显示
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

		//为颜色表赋值
		for ( i=0; i<256; i++)
		{
			pBitmapInfo->bmiColors[i].rgbBlue = BYTE(i);
			pBitmapInfo->bmiColors[i].rgbGreen = BYTE(i);
			pBitmapInfo->bmiColors[i].rgbRed = BYTE(i);
			pBitmapInfo->bmiColors[i].rgbReserved = 0;
		}

		//建立数据区
		LONG LineBytes = (m_width*8+31)/32*4;
		LPBYTE pData = (LPBYTE)new char[LineBytes*m_height];

		//为各像素赋值
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
	else    //彩色显示
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

		//建立数据区	
		LONG LineBytes = (m_width*24 + 31)/32*4;
		LPBYTE pData = (LPBYTE)new char[LineBytes*m_height*3];

		//为各像素赋值
		for ( i=0; i<m_height; i++)
			for ( j=0; j<m_width; j++)
			{
//				pData[(m_height-i-1)*LineBytes + j] = data[i*m_width*3 + j]; //像素是以BGR存入位图的
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return CView::OnEraseBkgnd(pDC);
}

void CImagingView::On32771()   //打开显示方式对话框
{
	// TODO: 在此添加命令处理程序代码
	if(!show)
	{
		AfxMessageBox("没有打开数据，无法进行操作！！");
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
			AfxMessageBox("没有正确的显示方式");
				break;
		}

	}

}


void CImagingView::IndexShow(BYTE *index)
{
	LONG i, j; //循环变量
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

	//为颜色表索引表赋值

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

	//建立数据区
	LONG LineBytes = (m_width*8+31)/32*4;
	LPBYTE pData = (LPBYTE)new char[LineBytes*m_height];

	//为各像素赋值
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
void CImagingView::On32772() //直方图均衡化显示
{
	// TODO: 在此添加命令处理程序代码
	if(!show)
	{
		AfxMessageBox("没有导入图像！");
		return;
	}
	if(ISindex)
		ISindex = false; 

	if(!IS_Gray)
	{
		//GetInfo Info(m_width,m_height);  //定义获取图像信息的对象
		float hst[256];
		float tempt[256];
		int pxl[256];
		//循环变量
		int i;
		int j;
		//临时变量
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

		//进行均衡化处理
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
		//循环变量
		int i;
		int j;
		//临时变量
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

		//进行均衡化处理
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
	//画图
	Draw(show,IS_Gray);

}

void CImagingView::Onselfadapt() //线性拉伸显示
{
	// TODO: 在此添加命令处理程序代码
	if(!show)
	{
		AfxMessageBox("没有导入图像！");
		return;
	}
	if(ISindex)
		ISindex = false;

	//if(!IS_Gray)  //灰度图像
	//{
		EXTENTION exdlg;  //对话框变量
		GetInfo Info(m_width,m_height);   //图像信息变量
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
			int i; //循环变量
			int temp; //中间变量
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
	//else   //彩色图像
	//{
	//}
}



//影像相关
void CImagingView::On32775()  //影像相关
{
	// TODO: 在此添加命令处理程序代码
	if (!GcpInfo.empty())
		GcpInfo.clear();

	IsAdd = false;

	Rgst MyDlg;
	if (MyDlg.DoModal() == IDOK)
	{
		if(MyDlg.m_basefile == "")
		{
			AfxMessageBox("您没有选择参考影像！");
			return;
		}

		if(MyDlg.m_warpfile == "")
		{
			AfxMessageBox("您没有选择待校正影像！");
			return;
		}

		if(MyDlg.m_gcpfile == "")
		{
			AfxMessageBox("您没有填入控制点文件的保存地址！");
			return;
		}

		Get_data Base;  //参考影像
		m_Gcpfile = MyDlg.m_gcpfile;

		//读取参考影像信息，并显示在窗口中
		if(!Base.ReadFile(MyDlg.m_basefile))
			return;

		m_width = Base.nXsize;
		m_height = Base.nYsize;
		m_nbands = Base.nBands;

		//读取第一波段信息进行影像相关处理
		m_baseband = (BYTE *) CPLMalloc(sizeof(BYTE)*(m_width)*(m_height));
		Base.read1band(m_baseband,1,m_width,m_height);
		//显示信息
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


		//待校正图像的信息
		Get_data Warp;  //待校正影像
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
	// TODO: 在此添加命令处理程序代码
	IsAdd = true;
}

void CImagingView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (IsAdd)
	{
		int x,y;
		CRect rect;
		GetClientRect(&rect);
		
		int zoom = int(rect.Height()/m_height);
		int temp_W = (int)m_width*rect.Height()/m_height;
		int x_offset = (int)(rect.Width()-temp_W)/2;
//获取屏幕上的电信息
		if( point.x < x_offset || point.x > (rect.Width() -x_offset))
		{
			AfxMessageBox("您没有点击在图像上！");
			return;
		}
//对应到图像上去
		x = (int)((point.x - x_offset)*m_height/rect.Height());
		y = (int)(point.y*m_height/rect.Height());
//将信息读入控制点信息中
		CPoint temp;
		temp.x = x;
		temp.y = y;

		GcpInfo.push_back(temp);

//在图像上画出十字丝
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
	// TODO: 在此添加命令处理程序代码
	if(GcpInfo.empty())
	{
		AfxMessageBox("没有选择控制点信息");
		return;
	}

	Registrat Reg;
	Reg.Getcoef();  //计算了系数

	int warpx = 0;
	int warpy = 0;
	double coef;  //相关系数

	m_outfile.open(m_Gcpfile);
	for(vector<CPoint>::size_type ix = 0; ix != GcpInfo.size(); ix++)
	{
		/*m_outfile<<GcpInfo[ix].x<<" "<<GcpInfo[ix].y<<endl;*/
		coef = Reg.GetRelate(m_baseband,m_width,m_height,m_warpband,m_warpxsize,m_warpysize,
			GcpInfo[ix].x ,GcpInfo[ix].y,warpx,warpy);
		m_outfile<<"第"<<ix+1<<"个控制点对: "<<"("<<GcpInfo[ix].x<<","<<GcpInfo[ix].y
			<<")"<<"  "<<"("<<warpx<<","<<warpy<<")"<<"  "<<"相关系数: "<<coef<<endl;
	}
	AfxMessageBox("您预测的控制点已经计算出来！");
	IsAdd = false;
}


void CImagingView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (GcpInfo.empty())
		return;

	GcpInfo.pop_back();
	this->Invalidate(true);

	CView::OnRButtonUp(nFlags, point);
}

void CImagingView::OnMButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	IsAdd = false;

	CView::OnMButtonUp(nFlags, point);
}


//几何校正
void CImagingView::OnGmtryCrrct() //几何多项式校正
{
	// TODO: 在此添加命令处理程序代码
	PREGCP CrrctDlg;
	if (CrrctDlg.DoModal() == IDOK)
	{
		//文件读取检查
		if (CrrctDlg.m_datafile == "")
		{
			AfxMessageBox("没有读取影像！");
			return;
		}
		if (CrrctDlg.m_Gcpfile == "")
		{
			AfxMessageBox("没有读取控制点");
			return;
		}
		if (CrrctDlg.m_outfile == "")
		{
			AfxMessageBox("没有指定校正后影像存储位置！");
			return;
		}
		//读取图像数据以显示
		if (!Test.ReadFile(CrrctDlg.m_datafile))
			return;

		m_nbands = Test.nBands;
		m_width = Test.nXsize;
		m_height = Test.nYsize;
		
//显示数据
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
//===========================校正=============================================================//
		CGcpGeoCorrect GcpCorrect;
		//读取GCP信息		
		GcpCorrect.LoadGcpPoint(CrrctDlg.m_Gcpfile);
		//求取多项式系数
		GcpCorrect.computfacter(CrrctDlg.m_combo);

		//进行重采样后存储校正后的影像
		GcpCorrect.Correct(CrrctDlg.m_datafile,CrrctDlg.m_outfile,CrrctDlg.m_slct+1);
		//显示校正后的影像

		if (!Test.ReadFile(CrrctDlg.m_outfile))
			return;
		m_nbands = Test.nBands;
		m_width = Test.nXsize;
		m_height = Test.nYsize;
		//显示数据
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


//分类
void CImagingView::OnIsoData()
{
	// TODO: 在此添加命令处理程序代码
	ISODlg MyDlg;
	CISODATA Iso;
	if(MyDlg.DoModal() == IDOK)
	{
		//文件读取检查
		if (MyDlg.m_infile == "")
		{
			AfxMessageBox("没有读取影像！");
			return;
		}
		if (MyDlg.m_outfile == "")
		{
			AfxMessageBox("没有给定输出路径！");
			return;
		}
		this->Invalidate(true);
		//原数据显示
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
		//数据传入
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
		//分类后数据显示
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


//变化监测
void CImagingView::On32784()//变化监测
{
	// TODO: 在此添加命令处理程序代码
	BianHua BDlg;
	UpDt TTT;

	//只需读取一个波段进行变化监测
	if (BDlg.DoModal() == IDOK)
	{
		TTT.ReadFile(BDlg.m_firstfile,BDlg.m_sercondfile);
		TTT.Sub(BDlg.m_outfile);
	}
}
