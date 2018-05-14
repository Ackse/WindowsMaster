#pragma once
#include "afxcmn.h"
#include <vector>
using std::vector;


// CDiaKernel �Ի���

class CDiaKernel : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaKernel)

public:
	CDiaKernel(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDiaKernel();

// �Ի�������
	enum { IDD = IDD_KERNEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list;
// 	void InitListCtrl();
	void EnumThreadList(DWORD dwPid);
	void getProcessModule(DWORD dwPid);

	afx_msg void OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	//��Ӧ�鿴ģ�麯��
	//�鿴�߳�
	afx_msg void On32772();
	//��Ӧ�鿴�̺߳���
	//�鿴ģ��
	afx_msg void On32771();
	virtual BOOL OnInitDialog();
	CListCtrl m_listThread;
	CListCtrl m_listModul;
	afx_msg void OnAddwhitelist ();
	CString m_processName;
	static vector<CString> m_vecProcessName;
	BOOL KillProgram ( DWORD ProcessId );
	void Refresh ();
	afx_msg void OnCloseprocess ();
	afx_msg void OnRefresh ();
};
