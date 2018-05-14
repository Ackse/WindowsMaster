#pragma once
#include "MyListBox.h"
#include <vector>
using std::vector;

// CDiaClean �Ի���

class CDiaClean : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaClean)

public:
	CDiaClean(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDiaClean();

// �Ի�������
	enum { IDD = IDD_CLEAN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	HICON m_hIcon;

	DECLARE_MESSAGE_MAP()
public:
	vector<CString> m_vecFileDelete;
	void TraverseFile(TCHAR szPath[MAX_PATH], const CString& strFilter, vector<CString> &vecPath);


	CString m_strFolder;
	CString m_strSuffix;
	CMyListBox m_ctlListBoxFolder;
	CMyListBox m_ctrListBoxFile;

	//���ļ���
	afx_msg void OnBnClickedButtonopen();
	//���º�׺
	afx_msg void OnBnClickedButtonupdate();
	//����
	afx_msg void OnBnClickedButtonergodic();
	//ɾ���ļ�
	afx_msg void OnBnClickedButtondelete();
	
	afx_msg void OnPaint();
	//ֻ���ϵͳ����
	afx_msg void OnBnClickedButtonsys();
	//ֻ������������
	afx_msg void OnBnClickedButtonbrow();
	//ֻ���VS����
	afx_msg void OnBnClickedButtonvs();
};
