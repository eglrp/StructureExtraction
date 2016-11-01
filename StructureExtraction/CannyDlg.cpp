// CannyDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "StructureExtraction.h"
#include "CannyDlg.h"
#include "afxdialogex.h"


// CCannyDlg 对话框

IMPLEMENT_DYNAMIC(CCannyDlg, CDialogEx)

CCannyDlg::CCannyDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCannyDlg::IDD, pParent)
	, m_low(0)
	, m_high(0)
{

}

CCannyDlg::~CCannyDlg()
{
}

void CCannyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_LOWER, m_low);
	DDX_Text(pDX, IDC_HIGHER, m_high);
}


BEGIN_MESSAGE_MAP(CCannyDlg, CDialogEx)
END_MESSAGE_MAP()


// CCannyDlg 消息处理程序
