// LightDirectionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "StructureExtraction.h"
#include "LightDirectionDlg.h"
#include "afxdialogex.h"


// CLightDirectionDlg 对话框

IMPLEMENT_DYNAMIC(CLightDirectionDlg, CDialog)

CLightDirectionDlg::CLightDirectionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLightDirectionDlg::IDD, pParent)
	, m_direction(0)
{

}

CLightDirectionDlg::~CLightDirectionDlg()
{
}

void CLightDirectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_LIGHT_DIRECTION, m_direction);
	DDV_MinMaxDouble(pDX, m_direction, 0, 360);
}


BEGIN_MESSAGE_MAP(CLightDirectionDlg, CDialog)
END_MESSAGE_MAP()


// CLightDirectionDlg 消息处理程序
