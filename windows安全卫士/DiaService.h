#pragma once
#include "afxcmn.h"
// #include <windows.h>


// CDiaService 对话框

class CDiaService : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaService)

public:
	CDiaService(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDiaService();

// 对话框数据
	enum { IDD = IDD_SERVICE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listService;
	virtual BOOL OnInitDialog();
	afx_msg void OnRclickListservice(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnPopupStartService();
//	afx_msg void OnPopupShutService();
	void ShowServicesStatus();
// 	void PromotePower ();
};
