// ImagingDoc.cpp : CImagingDoc 类的实现
//

#include "stdafx.h"
#include "Imaging.h"

#include "ImagingDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CImagingDoc

IMPLEMENT_DYNCREATE(CImagingDoc, CDocument)

BEGIN_MESSAGE_MAP(CImagingDoc, CDocument)
END_MESSAGE_MAP()


// CImagingDoc 构造/析构

CImagingDoc::CImagingDoc()
{
	// TODO: 在此添加一次性构造代码

}

CImagingDoc::~CImagingDoc()
{
}

BOOL CImagingDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CImagingDoc 序列化

void CImagingDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CImagingDoc 诊断

#ifdef _DEBUG
void CImagingDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CImagingDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CImagingDoc 命令
