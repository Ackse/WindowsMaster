#pragma once
#include "afxcmn.h"
// #include <windows.h>


// CDiaService �Ի���

class CDiaService : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaService)

public:
	CDiaService(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDiaService();

// �Ի�������
	enum { IDD = IDD_SERVICE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
