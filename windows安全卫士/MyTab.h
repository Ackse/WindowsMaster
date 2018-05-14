#pragma once
#include "DiaKernel.h"
#include "DiaOptimize.h"
#include "DiaClean.h"
#include "DiaService.h"
#include "DiaPeAnaly.h"
#include "DiaApplist.h"
#include "DiaKillVirus.h"
#include "DiaSoftUninstall.h"


// CMyTab

class CMyTab : public CTabCtrl
{
	DECLARE_DYNAMIC(CMyTab)

public:
	CMyTab();
	virtual ~CMyTab();

protected:
	DECLARE_MESSAGE_MAP()
public:
	void InitDlg();

	CDiaKernel m_dlgKernel;
	CDiaOptimize m_dlgOptimize;
	CDiaClean m_dlgClean;
	CDiaService m_dlgService;
	CDiaKillVirus m_dlgKillVirus;
	CDiaApplist m_dlgCloudSecurity;
	CDiaPeAnaly m_dlgPeAnaly;
	CDiaSoftUninstall m_dlgSoftUninstall;
	afx_msg void OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult);
};


