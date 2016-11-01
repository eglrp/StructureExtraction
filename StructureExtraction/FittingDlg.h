#pragma once


// CFittingDlg 对话框

class CFittingDlg : public CDialog
{
	DECLARE_DYNAMIC(CFittingDlg)

public:
	CFittingDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFittingDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_FITTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 阈值
	double m_threshold;
	// 局内点比例
	double m_proportion;
};
