#pragma once


// CMatchDlg �Ի���

class CMatchDlg : public CDialog
{
	DECLARE_DYNAMIC(CMatchDlg)

public:
	CMatchDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMatchDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_MATCH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	double m_thetaT;
	double m_areaRatioT;
	double m_ratioT;
};
