
// windows��ȫ��ʿDlg.h : ͷ�ļ�
//

#pragma once
#include "MyTab.h"
#define WM_MYHOTKEY WM_USER+1000 

// Cwindows��ȫ��ʿDlg �Ի���
class Cwindows��ȫ��ʿDlg : public CDialogEx
{
// ����
public:
	Cwindows��ȫ��ʿDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_WINDOWS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP();
public:
	CMyTab m_tab;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
