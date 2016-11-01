#pragma once
#include "afxwin.h"
#include <vector>
#include <string>

// CImageSelectDlg �Ի���

class CImageSelectDlg : public CDialog
{
	DECLARE_DYNAMIC(CImageSelectDlg)

public:
	CImageSelectDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CImageSelectDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_IMAGE_SEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_leftComboList;
	CComboBox m_rightComboList;
	std::vector<std::string> m_fileList;
	virtual BOOL OnInitDialog();
	CString m_left;
	CString m_right;
};
