
// ChildFrm.cpp : CChildFrame ���ʵ��
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

// CChildFrame ����/����

CChildFrame::CChildFrame()
{
	// TODO:  �ڴ���ӳ�Ա��ʼ������
}

CChildFrame::~CChildFrame()
{
}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  �ڴ˴�ͨ���޸� CREATESTRUCT cs ���޸Ĵ��������ʽ
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;
	cs.style |= WS_VISIBLE | WS_MAXIMIZE;
	return TRUE;
}

// CChildFrame ���

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

// CChildFrame ��Ϣ�������


void CChildFrame::OnClose()
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CString title = GetActiveDocument()->GetTitle();
	std::vector<std::string>& fileList = ((CStructureExtractionApp*)AfxGetApp())->m_imageFileList;
	std::vector<std::string>::iterator index;//�ر��ĵ�λ��
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
