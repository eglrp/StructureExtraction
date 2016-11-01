// MatchDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "StructureExtraction.h"
#include "MatchDlg.h"
#include "afxdialogex.h"


// CMatchDlg 对话框

IMPLEMENT_DYNAMIC(CMatchDlg, CDialog)

CMatchDlg::CMatchDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMatchDlg::IDD, pParent)
	, m_thetaT(0)
	, m_areaRatioT(0)
	, m_ratioT(0)
{

}

CMatchDlg::~CMatchDlg()
{
}

void CMatchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_THETA_THRESHOLD, m_thetaT);
	DDX_Text(pDX, IDC_EDIT_AREARATIO_THRESHOLD, m_areaRatioT);
	DDX_Text(pDX, IDC_EDIT_RATIO_THRESHOLD, m_ratioT);
}


BEGIN_MESSAGE_MAP(CMatchDlg, CDialog)
END_MESSAGE_MAP()


// CMatchDlg 消息处理程序
