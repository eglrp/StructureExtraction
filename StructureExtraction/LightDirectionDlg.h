#pragma once


// CLightDirectionDlg �Ի���

class CLightDirectionDlg : public CDialog
{
	DECLARE_DYNAMIC(CLightDirectionDlg)

public:
	CLightDirectionDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLightDirectionDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_LIGHT_DIRECTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	double m_direction;
};
