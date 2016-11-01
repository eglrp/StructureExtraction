#pragma once


// CMatchDlg 对话框

class CMatchDlg : public CDialog
{
	DECLARE_DYNAMIC(CMatchDlg)

public:
	CMatchDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMatchDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_MATCH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	double m_thetaT;
	double m_areaRatioT;
	double m_ratioT;
};
