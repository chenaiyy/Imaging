// Imaging.h : Imaging Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CImagingApp:
// �йش����ʵ�֣������ Imaging.cpp
//

class CImagingApp : public CWinApp
{
public:
	CImagingApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CImagingApp theApp;