// ImageSelect.cpp : 实现文件
//

#include "stdafx.h"
#include "StructureExtraction.h"
#include "ImageSelectDlg.h"
#include "afxdialogex.h"


// CImageSelectDlg 对话框

IMPLEMENT_DYNAMIC(CImageSelectDlg, CDialog)

CImageSelectDlg::CImageSelectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CImageSelectDlg::IDD, pParent)
	, m_left(_T(""))
	, m_right(_T(""))
{

}

CImageSelectDlg::~CImageSelectDlg()
{
}

void CImageSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_LEFT, m_leftComboList);
	DDX_Control(pDX, IDC_COMBO_RIGHT, m_rightComboList);
	DDX_CBString(pDX, IDC_COMBO_LEFT, m_left);
	DDX_CBString(pDX, IDC_COMBO_RIGHT, m_right);
}


BEGIN_MESSAGE_MAP(CImageSelectDlg, CDialog)
END_MESSAGE_MAP()


// CImageSelectDlg 消息处理程序


BOOL CImageSelectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	for (int i = 0; i < m_fileList.size(); i++)
	{
		CString str = m_fileList[i].c_str();
		m_leftComboList.AddString(str);
		m_rightComboList.AddString(str);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}
