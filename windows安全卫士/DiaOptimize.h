#pragma once
#include "afxcmn.h"
#include "HistogramCtrl.h"
#include "LittleRocket.h"


// CDiaOptimize �Ի���

class CDiaOptimize : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaOptimize)

public:
	CDiaOptimize(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDiaOptimize();

// �Ի�������
	enum { IDD = IDD_OPTIMIZE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	CHistogramCtrl	m_ctrlHistogramCPU;
	CHistogramCtrl  m_ctrlHistogramMem;
	CLittleRocket *m_pLittleRocket;
	DECLARE_MESSAGE_MAP()
public:
	CProgressCtrl m_ctlProgressCPU;
	CProgressCtrl m_ctlProgressMem;
	afx_msg void OnBnClickedButtonoptimize();
	CString m_strEditCPU;
	CString m_strEditMem;
	int GetCPURate();
	double GetMemRate();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
};
