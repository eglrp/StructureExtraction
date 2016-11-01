// FittingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "StructureExtraction.h"
#include "FittingDlg.h"
#include "afxdialogex.h"


// CFittingDlg 对话框

IMPLEMENT_DYNAMIC(CFittingDlg, CDialog)

CFittingDlg::CFittingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFittingDlg::IDD, pParent)
	, m_threshold(0)
	, m_proportion(0)
{

}

CFittingDlg::~CFittingDlg()
{
}

void CFittingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_THRESHOLD, m_threshold);
	//  DDX_Text(pDX, IDC_EDIT_PROPORTION, m_proportion);
	DDX_Text(pDX, IDC_EDIT_PROPORTION, m_proportion);
	DDV_MinMaxDouble(pDX, m_proportion, 0, 1);
}


BEGIN_MESSAGE_MAP(CFittingDlg, CDialog)
END_MESSAGE_MAP()


// CFittingDlg 消息处理程序
