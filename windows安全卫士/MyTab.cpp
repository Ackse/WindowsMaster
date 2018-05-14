// MyTab.cpp : 实现文件
//

#include "stdafx.h"
#include "windows安全卫士.h"
#include "MyTab.h"


// CMyTab

IMPLEMENT_DYNAMIC(CMyTab, CTabCtrl)

CMyTab::CMyTab()
{

}

CMyTab::~CMyTab()
{
}

void CMyTab::InitDlg()
{
	m_dlgKernel.Create(IDD_KERNEL);
	m_dlgOptimize.Create(IDD_OPTIMIZE);
	m_dlgClean.Create(IDD_CLEAN);
	m_dlgService.Create(IDD_SERVICE);
	m_dlgKillVirus.Create(IDD_KILLVIRUS);
	m_dlgCloudSecurity.Create(IDD_APPLIST);
	m_dlgPeAnaly.Create(IDD_PEANALY);
	m_dlgSoftUninstall.Create(IDD_SOFTUNINSTALL);

	CRect rt;
	GetClientRect(&rt);
	rt.DeflateRect(0, 40, 0, 0);//my-左.上.右.下
	m_dlgKernel.MoveWindow(&rt);//my-调整对话框大小  move到rt
// 	m_dlgKernel.InitListCtrl();
	m_dlgKernel.ShowWindow(SW_SHOW);

}

BEGIN_MESSAGE_MAP(CMyTab, CTabCtrl)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, &CMyTab::OnTcnSelchange)
END_MESSAGE_MAP()


// CMyTab 消息处理程序

void CMyTab::OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  在此添加控件通知处理程序代码
	int nSel = GetCurSel();//my-获取你当前点击是哪个标签
	CRect rt;
	GetClientRect(&rt);
	rt.DeflateRect(0, 40, 0, 0);
	switch (nSel)//my-根据不同的标签显示不同的对话框
	{
	case 0:
	{
		m_dlgKernel.MoveWindow(&rt);
		m_dlgKernel.ShowWindow(SW_SHOW);
		m_dlgOptimize.ShowWindow(SW_HIDE);
		m_dlgClean.ShowWindow(SW_HIDE);
		m_dlgService.ShowWindow(SW_HIDE);
		m_dlgKillVirus.ShowWindow(SW_HIDE);
		m_dlgCloudSecurity.ShowWindow(SW_HIDE);
		m_dlgPeAnaly.ShowWindow(SW_HIDE);
		m_dlgSoftUninstall.ShowWindow(SW_HIDE);
	}break;
	case 1:
	{
		m_dlgOptimize.MoveWindow(&rt);
		m_dlgKernel.ShowWindow(SW_HIDE);
		m_dlgOptimize.ShowWindow(SW_SHOW);
		m_dlgClean.ShowWindow(SW_HIDE);
		m_dlgService.ShowWindow(SW_HIDE);
		m_dlgKillVirus.ShowWindow(SW_HIDE);
		m_dlgCloudSecurity.ShowWindow(SW_HIDE);
		m_dlgPeAnaly.ShowWindow(SW_HIDE);
		m_dlgSoftUninstall.ShowWindow(SW_HIDE);


	}break;
	case 2:
	{
		m_dlgClean.MoveWindow(&rt);
		m_dlgKernel.ShowWindow(SW_HIDE);
		m_dlgOptimize.ShowWindow(SW_HIDE);
		m_dlgClean.ShowWindow(SW_SHOW);
		m_dlgService.ShowWindow(SW_HIDE);
		m_dlgKillVirus.ShowWindow(SW_HIDE);
		m_dlgCloudSecurity.ShowWindow(SW_HIDE);
		m_dlgPeAnaly.ShowWindow(SW_HIDE);
		m_dlgSoftUninstall.ShowWindow(SW_HIDE);
	}break;
	//自加的
	case 3:
	{
		m_dlgService.MoveWindow(&rt);
		m_dlgKernel.ShowWindow(SW_HIDE);
		m_dlgOptimize.ShowWindow(SW_HIDE);
		m_dlgClean.ShowWindow(SW_HIDE);
		m_dlgService.ShowWindow(SW_SHOW);
		m_dlgKillVirus.ShowWindow(SW_HIDE);
		m_dlgCloudSecurity.ShowWindow(SW_HIDE);
		m_dlgPeAnaly.ShowWindow(SW_HIDE);
		m_dlgSoftUninstall.ShowWindow(SW_HIDE);
	}
	break;
	case 4:
	{
		m_dlgKillVirus.MoveWindow(&rt);
		m_dlgKernel.ShowWindow(SW_HIDE);
		m_dlgOptimize.ShowWindow(SW_HIDE);
		m_dlgClean.ShowWindow(SW_HIDE);
		m_dlgService.ShowWindow(SW_HIDE);
		m_dlgKillVirus.ShowWindow(SW_SHOW);
		m_dlgCloudSecurity.ShowWindow(SW_HIDE);
		m_dlgPeAnaly.ShowWindow(SW_HIDE);
		m_dlgSoftUninstall.ShowWindow(SW_HIDE);

	}
	break;
	case 5:
	{
		m_dlgCloudSecurity.MoveWindow(&rt);
		m_dlgKernel.ShowWindow(SW_HIDE);
		m_dlgOptimize.ShowWindow(SW_HIDE);
		m_dlgClean.ShowWindow(SW_HIDE);
		m_dlgService.ShowWindow(SW_HIDE);
		m_dlgKillVirus.ShowWindow(SW_HIDE);
		m_dlgCloudSecurity.ShowWindow(SW_SHOW);
		m_dlgPeAnaly.ShowWindow(SW_HIDE);
		m_dlgSoftUninstall.ShowWindow(SW_HIDE);
	}
	break;
	case 6:
	{
		m_dlgPeAnaly.MoveWindow(&rt);
		m_dlgKernel.ShowWindow(SW_HIDE);
		m_dlgOptimize.ShowWindow(SW_HIDE);
		m_dlgClean.ShowWindow(SW_HIDE);
		m_dlgService.ShowWindow(SW_HIDE);
		m_dlgKillVirus.ShowWindow(SW_HIDE);
		m_dlgCloudSecurity.ShowWindow(SW_HIDE);
		m_dlgPeAnaly.ShowWindow(SW_SHOW);
		m_dlgSoftUninstall.ShowWindow(SW_HIDE);

	}
	break;
	case 7:
	{
		m_dlgSoftUninstall.MoveWindow(&rt);
		m_dlgKernel.ShowWindow(SW_HIDE);
		m_dlgOptimize.ShowWindow(SW_HIDE);
		m_dlgClean.ShowWindow(SW_HIDE);
		m_dlgService.ShowWindow(SW_HIDE);
		m_dlgKillVirus.ShowWindow(SW_HIDE);
		m_dlgCloudSecurity.ShowWindow(SW_HIDE);
		m_dlgPeAnaly.ShowWindow(SW_HIDE);
		m_dlgSoftUninstall.ShowWindow(SW_SHOW);
	}
	break;
	default:
		break;
	}

	*pResult = 0;
}
