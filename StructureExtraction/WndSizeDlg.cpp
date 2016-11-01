// WndSizeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "StructureExtraction.h"
#include "WndSizeDlg.h"
#include "afxdialogex.h"


// CWndSizeDlg 对话框

IMPLEMENT_DYNAMIC(CWndSizeDlg, CDialog)

CWndSizeDlg::CWndSizeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWndSizeDlg::IDD, pParent)
	, m_wndSize(0)
{

}

CWndSizeDlg::~CWndSizeDlg()
{
}

void CWndSizeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_WNDSIZE, m_wndSize);
}


BEGIN_MESSAGE_MAP(CWndSizeDlg, CDialog)
END_MESSAGE_MAP()


// CWndSizeDlg 消息处理程序
