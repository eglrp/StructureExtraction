
// StructureExtraction.h : StructureExtraction Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������
#include <vector>
#include <string>

// CStructureExtractionApp:
// �йش����ʵ�֣������ StructureExtraction.cpp
//

class CStructureExtractionApp : public CWinApp
{
public:
	CStructureExtractionApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
public:
	// Ӱ���б�
	std::vector<std::string> m_imageFileList;
	afx_msg void OnImgSel();
};

extern CStructureExtractionApp theApp;
