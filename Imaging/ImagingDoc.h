// ImagingDoc.h : CImagingDoc ��Ľӿ�
//


#pragma once


class CImagingDoc : public CDocument
{
protected: // �������л�����
	CImagingDoc();
	DECLARE_DYNCREATE(CImagingDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CImagingDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


