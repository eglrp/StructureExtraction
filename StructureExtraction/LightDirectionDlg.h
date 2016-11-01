#pragma once


// CLightDirectionDlg 对话框

class CLightDirectionDlg : public CDialog
{
	DECLARE_DYNAMIC(CLightDirectionDlg)

public:
	CLightDirectionDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLightDirectionDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_LIGHT_DIRECTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	double m_direction;
};
