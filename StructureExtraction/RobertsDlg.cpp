// CRobertsDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "StructureExtraction.h"
#include "RobertsDlg.h"
#include "afxdialogex.h"


// CRobertsDlg 对话框

IMPLEMENT_DYNAMIC(CRobertsDlg, CDialog)

CRobertsDlg::CRobertsDlg(CWnd* pParent /*=NULL*/)
: CDialog(CRobertsDlg::IDD, pParent)
	, m_low(0)
	, m_wndSize(0)
{

}

CRobertsDlg::~CRobertsDlg()
{
}

void CRobertsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_LOW, m_low);
	DDX_Text(pDX, IDC_EDIT_WNDSIZE, m_wndSize);
}


BEGIN_MESSAGE_MAP(CRobertsDlg, CDialog)
END_MESSAGE_MAP()


// CRobertsDlg 消息处理程序
