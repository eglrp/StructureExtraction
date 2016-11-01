#pragma once


// CRobertsDlg 对话框

class CRobertsDlg : public CDialog
{
	DECLARE_DYNAMIC(CRobertsDlg)

public:
	CRobertsDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRobertsDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_ROBERTS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 低阈值
	double m_low;
	// 高阈值
//	double m_high;
	// 窗口尺寸
	double m_wndSize;
};
