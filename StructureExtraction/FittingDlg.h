#pragma once


// CFittingDlg �Ի���

class CFittingDlg : public CDialog
{
	DECLARE_DYNAMIC(CFittingDlg)

public:
	CFittingDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFittingDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_FITTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	// ��ֵ
	double m_threshold;
	// ���ڵ����
	double m_proportion;
};
