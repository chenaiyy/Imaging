// ImagingDoc.cpp : CImagingDoc ���ʵ��
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


// CImagingDoc ����/����

CImagingDoc::CImagingDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CImagingDoc::~CImagingDoc()
{
}

BOOL CImagingDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CImagingDoc ���л�

void CImagingDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// CImagingDoc ���

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


// CImagingDoc ����
