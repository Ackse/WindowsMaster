// DiaOptimize.cpp : 实现文件
//

#include "stdafx.h"
#include "windows安全卫士.h"
#include "DiaOptimize.h"
#include "afxdialogex.h"
#include <psapi.h>
#include "resource.h"

// CDiaOptimize 对话框

IMPLEMENT_DYNAMIC(CDiaOptimize, CDialogEx)

CDiaOptimize::CDiaOptimize(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDiaOptimize::IDD, pParent)
	, m_strEditCPU(_T(""))
	, m_strEditMem(_T(""))
{

}

CDiaOptimize::~CDiaOptimize()
{
}

void CDiaOptimize::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESSCPU, m_ctlProgressCPU);
	DDX_Control(pDX, IDC_PROGRESSMEM, m_ctlProgressMem);
	DDX_Text(pDX, IDC_EDITCPU, m_strEditCPU);
	DDX_Text(pDX, IDC_EDITMEM, m_strEditMem);
}


BEGIN_MESSAGE_MAP(CDiaOptimize, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTONOPTIMIZE, &CDiaOptimize::OnBnClickedButtonoptimize)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDiaOptimize 消息处理程序


BOOL CDiaOptimize::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//littrocket
	m_pLittleRocket = new CLittleRocket();
	m_pLittleRocket->Create(IDD_ROCKET);

	//cpu
	CRect cpuRect;
	GetDlgItem(IDC_STATICCPURATE)->GetWindowRect(cpuRect);
	ScreenToClient(cpuRect);
	m_ctrlHistogramCPU.Create(WS_VISIBLE | WS_CHILD | WS_TABSTOP, cpuRect, this, 1000);
	m_ctrlHistogramCPU.SetRange(0, 100);
	m_ctrlHistogramCPU.SetSpeed(CHistogramCtrl::HIGH_SPEED);

	//mem
	CRect memRect;
	GetDlgItem(IDC_STATICMEMRATE)->GetWindowRect(memRect);
	ScreenToClient(memRect);
	m_ctrlHistogramMem.Create(WS_VISIBLE | WS_CHILD | WS_TABSTOP, memRect, this, 1000);
	m_ctrlHistogramMem.SetRange(0, 100);
	m_ctrlHistogramMem.SetSpeed(CHistogramCtrl::HIGH_SPEED);

	SetTimer(0, 2000, NULL);
	m_ctlProgressCPU.SetRange(0, 100);
	m_ctlProgressMem.SetRange(0, 100);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


//响应点击优化按钮事件
void CDiaOptimize::OnBnClickedButtonoptimize()
{
	// TODO:  在此添加控件通知处理程序代码
	DWORD dwPIDList[1000] = { 0 };
	DWORD bufSize = sizeof(dwPIDList);
	DWORD dwNeedSize = 0;
	EnumProcesses(dwPIDList, bufSize, &dwNeedSize);
	for (DWORD i = 0; i < dwNeedSize / sizeof(DWORD); i++)
	{
		HANDLE hProcess = OpenProcess(PROCESS_SET_QUOTA,
			false, dwPIDList[i]);
		//设置进程使用的内存容量
		SetProcessWorkingSetSize(hProcess, -1, -1);
		//进程句柄 进程最小的内存容量	进程最大的内存容量
	}
}

//CPU当前使用率
int CDiaOptimize::GetCPURate()
{
	double idleTime1 = 0;//空闲时间1
	double idleTime2 = 0;//空闲时间2
	double kernelTime1 = 0;//内核时间1
	double kernelTime2 = 0;//内核时间2
	double userTime1 = 0;//用户时间1
	double userTime2 = 0;//用户时间2
	int useRate = 0;//使用率
	//1.第一次获取处理器时间
	FILETIME idleTime, kernelTime, userTime;
	GetSystemTimes(&idleTime, &kernelTime, &userTime);
	//2.创建内核对象,并且等待1000毫秒
	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	WaitForSingleObject(hEvent, 1000);//等待1秒
	//3.第二次获取处理器时间
	FILETIME preidleTime, prekernelTime, preuserTime;
	GetSystemTimes(&preidleTime, &prekernelTime, &preuserTime);
	//4.转换上面所有的时间,将获取到的时间变量都转换
	/*因为第五步需要用四则运算来计算,不转换没办法直接运算
	souble 转换后的值=(double)(时间变量高32位值*4.294967296E9)+
	(double)时间变量低32位值
	*/
	idleTime1 = (double)(idleTime.dwHighDateTime*4.294967296E9) +
		(double)idleTime.dwLowDateTime;
	idleTime2 = (double)(preidleTime.dwHighDateTime*4.294967296E9) +
		(double)preidleTime.dwLowDateTime;
	kernelTime1 = (double)(kernelTime.dwHighDateTime*4.294967296E9) +
		(double)kernelTime.dwLowDateTime;
	kernelTime2 = (double)(prekernelTime.dwHighDateTime*4.294967296E9) +
		(double)prekernelTime.dwLowDateTime;
	userTime1 = (double)(userTime.dwHighDateTime*4.294967296E9) +
		(double)userTime.dwLowDateTime;
	userTime2 = (double)(preuserTime.dwHighDateTime*4.294967296E9) +
		(double)preuserTime.dwLowDateTime;
	//5.计算CPU使用率
	useRate = (int)
		(100.0 - (idleTime2 - idleTime1) /
		(kernelTime2 - kernelTime1 + userTime2 - userTime1)*100.0);

	return useRate;

}

//内存当前使用率
double CDiaOptimize::GetMemRate()
{
	double memUseRate = 0.0;
	//1.获取当前内存状态
	MEMORYSTATUSEX stcMemStatusEx = { 0 };
	//该结构体长度
	stcMemStatusEx.dwLength = sizeof(stcMemStatusEx);
	//获取当前系统前物理内存和虚拟内存的使用
	GlobalMemoryStatusEx(&stcMemStatusEx);
	//当前使用内存=实际物理内存-当前可用的物理内存
	DWORDLONG preUsedMem =
		stcMemStatusEx.ullTotalPhys - stcMemStatusEx.ullAvailPhys;
	memUseRate = double(preUsedMem) / double(stcMemStatusEx.ullTotalPhys);

	return memUseRate;
}

void CDiaOptimize::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int cpuRate = GetCPURate();
	m_strEditCPU.Format(L"%d", cpuRate);
	m_strEditCPU += L"%";
	m_ctlProgressCPU.SetPos(cpuRate);
	m_ctrlHistogramCPU.SetPos(cpuRate);

	double MemRate = GetMemRate();
	m_strEditMem.Format(L"%d", int(MemRate * 100));
	m_strEditMem += L"%";
	m_ctlProgressMem.SetPos(MemRate * 100);
	m_ctrlHistogramMem.SetPos(MemRate * 100);

	UpdateData(FALSE);

	CDialogEx::OnTimer(nIDEvent);
}


