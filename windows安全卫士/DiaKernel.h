#pragma once
#include "afxcmn.h"
#include <vector>
using std::vector;


// CDiaKernel 对话框

class CDiaKernel : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaKernel)

public:
	CDiaKernel(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDiaKernel();

// 对话框数据
	enum { IDD = IDD_KERNEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_list;
// 	void InitListCtrl();
	void EnumThreadList(DWORD dwPid);
	void getProcessModule(DWORD dwPid);

	afx_msg void OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	//响应查看模块函数
	//查看线程
	afx_msg void On32772();
	//响应查看线程函数
	//查看模块
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
