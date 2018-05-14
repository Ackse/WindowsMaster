// DiaOptimize.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "windows��ȫ��ʿ.h"
#include "DiaOptimize.h"
#include "afxdialogex.h"
#include <psapi.h>
#include "resource.h"

// CDiaOptimize �Ի���

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


// CDiaOptimize ��Ϣ�������


BOOL CDiaOptimize::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
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
				  // �쳣: OCX ����ҳӦ���� FALSE
}


//��Ӧ����Ż���ť�¼�
void CDiaOptimize::OnBnClickedButtonoptimize()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	DWORD dwPIDList[1000] = { 0 };
	DWORD bufSize = sizeof(dwPIDList);
	DWORD dwNeedSize = 0;
	EnumProcesses(dwPIDList, bufSize, &dwNeedSize);
	for (DWORD i = 0; i < dwNeedSize / sizeof(DWORD); i++)
	{
		HANDLE hProcess = OpenProcess(PROCESS_SET_QUOTA,
			false, dwPIDList[i]);
		//���ý���ʹ�õ��ڴ�����
		SetProcessWorkingSetSize(hProcess, -1, -1);
		//���̾�� ������С���ڴ�����	���������ڴ�����
	}
}

//CPU��ǰʹ����
int CDiaOptimize::GetCPURate()
{
	double idleTime1 = 0;//����ʱ��1
	double idleTime2 = 0;//����ʱ��2
	double kernelTime1 = 0;//�ں�ʱ��1
	double kernelTime2 = 0;//�ں�ʱ��2
	double userTime1 = 0;//�û�ʱ��1
	double userTime2 = 0;//�û�ʱ��2
	int useRate = 0;//ʹ����
	//1.��һ�λ�ȡ������ʱ��
	FILETIME idleTime, kernelTime, userTime;
	GetSystemTimes(&idleTime, &kernelTime, &userTime);
	//2.�����ں˶���,���ҵȴ�1000����
	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	WaitForSingleObject(hEvent, 1000);//�ȴ�1��
	//3.�ڶ��λ�ȡ������ʱ��
	FILETIME preidleTime, prekernelTime, preuserTime;
	GetSystemTimes(&preidleTime, &prekernelTime, &preuserTime);
	//4.ת���������е�ʱ��,����ȡ����ʱ�������ת��
	/*��Ϊ���岽��Ҫ����������������,��ת��û�취ֱ������
	souble ת�����ֵ=(double)(ʱ�������32λֵ*4.294967296E9)+
	(double)ʱ�������32λֵ
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
	//5.����CPUʹ����
	useRate = (int)
		(100.0 - (idleTime2 - idleTime1) /
		(kernelTime2 - kernelTime1 + userTime2 - userTime1)*100.0);

	return useRate;

}

//�ڴ浱ǰʹ����
double CDiaOptimize::GetMemRate()
{
	double memUseRate = 0.0;
	//1.��ȡ��ǰ�ڴ�״̬
	MEMORYSTATUSEX stcMemStatusEx = { 0 };
	//�ýṹ�峤��
	stcMemStatusEx.dwLength = sizeof(stcMemStatusEx);
	//��ȡ��ǰϵͳǰ�����ڴ�������ڴ��ʹ��
	GlobalMemoryStatusEx(&stcMemStatusEx);
	//��ǰʹ���ڴ�=ʵ�������ڴ�-��ǰ���õ������ڴ�
	DWORDLONG preUsedMem =
		stcMemStatusEx.ullTotalPhys - stcMemStatusEx.ullAvailPhys;
	memUseRate = double(preUsedMem) / double(stcMemStatusEx.ullTotalPhys);

	return memUseRate;
}

void CDiaOptimize::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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


