#pragma once


// CWndSizeDlg 对话框

class CWndSizeDlg : public CDialog
{
	DECLARE_DYNAMIC(CWndSizeDlg)

public:
	CWndSizeDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWndSizeDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_WNDSIZE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 滤波窗口尺寸大小
	double m_wndSize;
};
