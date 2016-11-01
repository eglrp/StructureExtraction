#pragma once


// CCannyDlg 对话框

class CCannyDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCannyDlg)

public:
	CCannyDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCannyDlg();

// 对话框数据
	enum { IDD = IDD_CANNY_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 低阈值
	double m_low;
	// 高阈值
	double m_high;
};
