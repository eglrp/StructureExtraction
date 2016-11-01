#pragma once
#include "afxwin.h"
#include <vector>
#include <string>

// CImageSelectDlg 对话框

class CImageSelectDlg : public CDialog
{
	DECLARE_DYNAMIC(CImageSelectDlg)

public:
	CImageSelectDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CImageSelectDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_IMAGE_SEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_leftComboList;
	CComboBox m_rightComboList;
	std::vector<std::string> m_fileList;
	virtual BOOL OnInitDialog();
	CString m_left;
	CString m_right;
};
