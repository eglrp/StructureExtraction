// ResizeDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "StructureExtraction.h"
#include "ResizeDlg.h"
#include "afxdialogex.h"


// CResizeDlg �Ի���

IMPLEMENT_DYNAMIC(CResizeDlg, CDialog)

CResizeDlg::CResizeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CResizeDlg::IDD, pParent)
	, m_resize(0)
{

}

CResizeDlg::~CResizeDlg()
{
}

void CResizeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_RESIZE, m_resize);
}


BEGIN_MESSAGE_MAP(CResizeDlg, CDialog)
END_MESSAGE_MAP()


// CResizeDlg ��Ϣ�������
