
// ChildFrm.cpp : CChildFrame 类的实现
//

#include "stdafx.h"
#include "StructureExtraction.h"

#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

// CChildFrame 构造/析构

CChildFrame::CChildFrame()
{
	// TODO:  在此添加成员初始化代码
}

CChildFrame::~CChildFrame()
{
}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  在此处通过修改 CREATESTRUCT cs 来修改窗口类或样式
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;
	cs.style |= WS_VISIBLE | WS_MAXIMIZE;
	return TRUE;
}

// CChildFrame 诊断

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}
#endif //_DEBUG

// CChildFrame 消息处理程序


void CChildFrame::OnClose()
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	CString title = GetActiveDocument()->GetTitle();
	std::vector<std::string>& fileList = ((CStructureExtractionApp*)AfxGetApp())->m_imageFileList;
	std::vector<std::string>::iterator index;//关闭文档位置
	std::vector<std::string>::iterator it;
	std::string stringTitle = (LPTSTR)(LPCTSTR)title;
	for (it = fileList.begin(); it != fileList.end();++it)
	{
		if (*it == stringTitle)
		{
			it = fileList.erase(it);
		}
		if (it == fileList.end())
			break;
	}
	CMDIChildWnd::OnClose();
}
