#pragma once


// CRobertsDlg �Ի���

class CRobertsDlg : public CDialog
{
	DECLARE_DYNAMIC(CRobertsDlg)

public:
	CRobertsDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRobertsDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_ROBERTS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	// ����ֵ
	double m_low;
	// ����ֵ
//	double m_high;
	// ���ڳߴ�
	double m_wndSize;
};
