// ImagingView.h : CImagingView ��Ľӿ�
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
protected: // �������л�����
	CImagingView();
	DECLARE_DYNCREATE(CImagingView)

// ����
public:
	CImagingDoc* GetDocument() const;

// ����
public:
	Get_data Test;  //��ʾ�õĶ�ȡ���ݶ���

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CImagingView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	BYTE *show; //��ʾ���ݵĴ洢
	BYTE *m_baseband; //��ȡ�ο�Ӱ�����ݽ��д���
	BYTE *m_warpband; //��ȡ��У��Ӱ������
	BYTE *allband;
	//BYTE *show_clr; //��ʾ���ݵĴ洢	
	bool IS_Gray;  //�ж��Ƿ�Ҷ���ʾ
	bool ISindex;  //�ж��Ƿ�������ʾ
	int clrful[3]; //��ɫ��ʾ����������
//��ͼʱͼ�ĳ���
	int m_width;
	int m_height;
	int m_nbands;

	double x;   //��ȡ��Ļ�ϵĵ���Ϣ
	double y;

	int m_warpxsize;
	int m_warpysize;

	vector <CPoint> GcpInfo;  //Ԥ����Ƶ���Ϣ
	bool IsAdd;    //�Ƿ���ӿ��Ƶ�
	CString m_Gcpfile; //���Ƶ��ļ���Ϣ
	ofstream m_outfile; 

	void Draw(BYTE *,bool);
	void IndexShow(BYTE *);

// ���ɵ���Ϣӳ�亯��
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

#ifndef _DEBUG  // ImagingView.cpp �еĵ��԰汾
inline CImagingDoc* CImagingView::GetDocument() const
   { return reinterpret_cast<CImagingDoc*>(m_pDocument); }
#endif

