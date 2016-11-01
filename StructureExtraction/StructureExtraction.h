
// StructureExtraction.h : StructureExtraction 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号
#include <vector>
#include <string>

// CStructureExtractionApp:
// 有关此类的实现，请参阅 StructureExtraction.cpp
//

class CStructureExtractionApp : public CWinApp
{
public:
	CStructureExtractionApp();


// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 实现
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
public:
	// 影像列表
	std::vector<std::string> m_imageFileList;
	afx_msg void OnImgSel();
};

extern CStructureExtractionApp theApp;
