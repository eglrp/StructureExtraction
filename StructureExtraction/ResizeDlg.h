#pragma once


// CResizeDlg 对话框

class CResizeDlg : public CDialog
{
	DECLARE_DYNAMIC(CResizeDlg)

public:
	CResizeDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CResizeDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_RESIZE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 重采样级别
	double m_resize;
};
