// EuclideanDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "StructureExtraction.h"
#include "EuclideanDlg.h"
#include "afxdialogex.h"


// CEuclideanDlg 对话框

IMPLEMENT_DYNAMIC(CEuclideanDlg, CDialog)

CEuclideanDlg::CEuclideanDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEuclideanDlg::IDD, pParent)
	, m_minDis(0)
	, m_minCluster(0)
	, m_maxCluster(0)
{

}

CEuclideanDlg::~CEuclideanDlg()
{
}

void CEuclideanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MINDIS, m_minDis);
	DDX_Text(pDX, IDC_MINCLUSTER, m_minCluster);
	DDX_Text(pDX, IDC_MAXCLUSTER, m_maxCluster);
}


BEGIN_MESSAGE_MAP(CEuclideanDlg, CDialog)
END_MESSAGE_MAP()


// CEuclideanDlg 消息处理程序
