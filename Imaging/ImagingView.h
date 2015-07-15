// ImagingView.h : CImagingView 类的接口
//


#pragma once
#include "Get_Data.h"
#include "GetInfo.h"
#include "REGISTRAT.h"
#include "Rgst.h"
#include "Sample.h"
#include "PREGCP.h"
#include "ISODlg.h"
#include "CISODATA.h"
#include "EditDLg.h"
#include "BianHua.h"
#include "UpDt.h"
#include <vector>
#include <fstream>
using namespace std;


class CImagingView : public CView
{
protected: // 仅从序列化创建
	CImagingView();
	DECLARE_DYNCREATE(CImagingView)

// 属性
public:
	CImagingDoc* GetDocument() const;

// 操作
public:
	Get_data Test;  //显示用的读取数据对象

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CImagingView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	BYTE *show; //显示数据的存储
	BYTE *m_baseband; //读取参考影像数据进行处理
	BYTE *m_warpband; //读取待校正影像数据
	BYTE *allband;
	//BYTE *show_clr; //显示数据的存储	
	bool IS_Gray;  //判断是否灰度显示
	bool ISindex;  //判断是否索引显示
	int clrful[3]; //彩色显示的三个波段
//画图时图的长宽
	int m_width;
	int m_height;
	int m_nbands;

	double x;   //获取屏幕上的点信息
	double y;

	int m_warpxsize;
	int m_warpysize;

	vector <CPoint> GcpInfo;  //预测控制点信息
	bool IsAdd;    //是否添加控制点
	CString m_Gcpfile; //控制点文件信息
	ofstream m_outfile; 

	void Draw(BYTE *,bool);
	void IndexShow(BYTE *);

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileOpen();
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
public:
	afx_msg void On32771();
public:
	afx_msg void On32772();
public:
	afx_msg void Onselfadapt();
public:
	afx_msg void On32775();
public:
	afx_msg void OnPredict();
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
public:
	afx_msg void OutGcp();
public:
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
public:
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
public:
	afx_msg void OnGmtryCrrct();
public:
	afx_msg void OnIsoData();
public:
	afx_msg void On32784();
};

#ifndef _DEBUG  // ImagingView.cpp 中的调试版本
inline CImagingDoc* CImagingView::GetDocument() const
   { return reinterpret_cast<CImagingDoc*>(m_pDocument); }
#endif

