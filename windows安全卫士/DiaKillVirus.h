#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include <vector>
using std::vector;

// CDiaKillVirus 对话框

class CDiaKillVirus : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaKillVirus)

public:
	CDiaKillVirus(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDiaKillVirus();

// 对话框数据
	enum { IDD = IDD_KILLVIRUS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	//加入病毒库
	afx_msg void OnBnClickedButtonaddvirus();
	//MD5查杀
	afx_msg void OnBnClickedButtonmd5kill();
	//全路径查杀
	afx_msg void OnBnClickedButtonallpathkill();

	afx_msg void OnRclickListsuspicious ( NMHDR *pNMHDR, LRESULT *pResult );
	//关闭可疑进程
	afx_msg void OnCloseSuspiciousProcess ();
	//白名单查杀
	afx_msg void OnBnClickedButtonwhitekill ();
	//关进程
	BOOL KillProgram ( DWORD ProcessId );
	//遍历可疑进程
	void ShowSuspiciousProcessList ();
	//遍历文件夹
	void Travel(const TCHAR* pszPath, int nDeep);

	CListCtrl m_listVirus;
	CListBox m_ctlListVirus;
	CListCtrl m_listSuspicious;
	vector<CString> m_vecWhiteList;
	DWORD m_dwCurrentProcess;
	virtual BOOL OnInitDialog ();

	//my-附加功能
	afx_msg void OnBnClickedButtonshutdown ();
	afx_msg void OnBnClickedButtonreboot ();
	afx_msg void OnBnClickedButtonlogoff ();
	afx_msg void OnBnClickedButtondormant ();
	afx_msg void OnBnClickedButtonsleep ();
	afx_msg void OnBnClickedButtonlockscreen ();

};
