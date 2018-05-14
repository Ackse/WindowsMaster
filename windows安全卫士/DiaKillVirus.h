#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include <vector>
using std::vector;

// CDiaKillVirus �Ի���

class CDiaKillVirus : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaKillVirus)

public:
	CDiaKillVirus(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDiaKillVirus();

// �Ի�������
	enum { IDD = IDD_KILLVIRUS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	//���벡����
	afx_msg void OnBnClickedButtonaddvirus();
	//MD5��ɱ
	afx_msg void OnBnClickedButtonmd5kill();
	//ȫ·����ɱ
	afx_msg void OnBnClickedButtonallpathkill();

	afx_msg void OnRclickListsuspicious ( NMHDR *pNMHDR, LRESULT *pResult );
	//�رտ��ɽ���
	afx_msg void OnCloseSuspiciousProcess ();
	//��������ɱ
	afx_msg void OnBnClickedButtonwhitekill ();
	//�ؽ���
	BOOL KillProgram ( DWORD ProcessId );
	//�������ɽ���
	void ShowSuspiciousProcessList ();
	//�����ļ���
	void Travel(const TCHAR* pszPath, int nDeep);

	CListCtrl m_listVirus;
	CListBox m_ctlListVirus;
	CListCtrl m_listSuspicious;
	vector<CString> m_vecWhiteList;
	DWORD m_dwCurrentProcess;
	virtual BOOL OnInitDialog ();

	//my-���ӹ���
	afx_msg void OnBnClickedButtonshutdown ();
	afx_msg void OnBnClickedButtonreboot ();
	afx_msg void OnBnClickedButtonlogoff ();
	afx_msg void OnBnClickedButtondormant ();
	afx_msg void OnBnClickedButtonsleep ();
	afx_msg void OnBnClickedButtonlockscreen ();

};
