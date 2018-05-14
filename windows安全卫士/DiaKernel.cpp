// DiaKernel.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "windows��ȫ��ʿ.h"
#include "DiaKernel.h"
#include "afxdialogex.h"
#include <locale.h>
#include <tlhelp32.h>
// #include "MyTab.h"
// CDiaKernel �Ի���

IMPLEMENT_DYNAMIC(CDiaKernel, CDialogEx)


vector<CString> CDiaKernel::m_vecProcessName;


CDiaKernel::CDiaKernel(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDiaKernel::IDD, pParent)
{

}

CDiaKernel::~CDiaKernel()
{
}

void CDiaKernel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Control(pDX, IDC_LISTTHREAD, m_listThread);
	DDX_Control(pDX, IDC_LISTMODUL, m_listModul);
}


BEGIN_MESSAGE_MAP(CDiaKernel, CDialogEx)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CDiaKernel::OnNMRClickList1)
	ON_COMMAND(ID_32772, &CDiaKernel::On32772)
	ON_COMMAND(ID_32771, &CDiaKernel::On32771)
	ON_COMMAND ( ID_ADDWHITELIST, &CDiaKernel::OnAddwhitelist )
	ON_COMMAND ( ID_CLOSEPROCESS, &CDiaKernel::OnCloseprocess )
	ON_COMMAND ( ID_REFRESH, &CDiaKernel::OnRefresh )
END_MESSAGE_MAP()


// CDiaKernel ��Ϣ�������

void CDiaKernel::Refresh ()
{
	// 1.1 �������̿���
	HANDLE hProcSnap = CreateToolhelp32Snapshot ( TH32CS_SNAPPROCESS, 0 );

	if( INVALID_HANDLE_VALUE == hProcSnap )
	{
		MessageBox ( L"�������̿���ʧ��",0,0 );
		return;
	}
	PROCESSENTRY32 processInfo;

	// ��ʼ���ṹ��Ľṹ���ֽ��ֶ�
	processInfo.dwSize = sizeof ( processInfo );

	// my-�������д��������ôд��
	//	PROCESSENTRY32 processInfo = { sizeof(processInfo) };
	// ��ȡ��һ������
	Process32First ( hProcSnap, &processInfo );

	int i = 0;
	do
	{
		// ���õ��Ľ�����Ϣ���
		m_list.InsertItem ( i, processInfo.szExeFile );
		//DWORDתCString
		CString tempProcessID, tempPcPriClassBase;
		tempProcessID.Format ( L"%d", processInfo.th32ProcessID );
		tempPcPriClassBase.Format ( L"%d", processInfo.pcPriClassBase );
		m_list.SetItemText ( i, 1, tempProcessID );
		m_list.SetItemText ( i++, 2, tempPcPriClassBase );

		// ��ȡ��һ������
	}
	while( Process32Next ( hProcSnap, &processInfo ) );

}

BOOL CDiaKernel::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_list.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	// ����б���
	m_list.InsertColumn(0, L"������", 0, 100);
	m_list.InsertColumn(1, L"����ID", 0, 100);
	m_list.InsertColumn(2, L"�������ȼ�", 0, 100);
	m_listThread.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	// ����б���
	m_listThread.InsertColumn(0, L"PID", 0, 100);
	m_listThread.InsertColumn(1, L"�߳�ID", 0, 100);
	m_listThread.InsertColumn(2, L"�߳����ȼ�", 0, 100);
	m_listModul.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	// ����б���
	m_listModul.InsertColumn(0, L"���ػ���", 0, 80);
	m_listModul.InsertColumn(1, L"ģ���С", 0, 80);
	m_listModul.InsertColumn(2, L"ģ����", 0, 80);
	m_listModul.InsertColumn(3, L"ģ��·��", 0, 80);
	// 	char *p = ".txt";
	// 	CString temp(p);
	// 	DWORD dw = 123456;
	// 	temp.Format(L"%d", dw);
	// ���ñ����ַ���
	setlocale(LC_ALL, "chs");
	Refresh ();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}




DWORD currentProcess;

//�б������Ҽ���Ӧ�¼�
void CDiaKernel::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nCol = pNMItemActivate->iSubItem;
	int nRow = pNMItemActivate->iItem;
// 	CString str = m_list.GetItemText ( nRow, nCol );
	//��ȡ����ID
	CString str = m_list.GetItemText(nRow, 1);
	//��ȡ������
	m_processName = m_list.GetItemText ( nRow, 0 );
// 	CString x = "123";
// 	printf("%d", atoi(x.GetBuffer(x.GetLength())));
	currentProcess = (DWORD)_wtoi(str);
	//��CStringת��DWORD
// 	pNMItemActivate->iItem;
// 	m_list.GetItemText();
	CMenu mu;
	// 	my-�������˵���Դ���ؽ���
	mu.LoadMenuW(IDR_MENU1);
	// ��ȡ"��̬"�˵��ľ��
	CMenu *pSub = mu.GetSubMenu(0);
	CPoint pt;
	GetCursorPos(&pt);
	// 	//my-��ȡ��ǰ����λ��
	TrackPopupMenu(pSub->m_hMenu, TPM_LEFTALIGN, pt.x, pt.y, 0, m_hWnd, NULL);
	//my-			 menu���		���뷽ʽ		����λ��		�����ھ��

	*pResult = 0;
}

//�鿴�߳�
void CDiaKernel::On32771()
{
	// TODO:  �ڴ���������������
	EnumThreadList(currentProcess);
	ShowWindow(SW_SHOW);
}

//�鿴ģ��
void CDiaKernel::On32772()
{
	// TODO:  �ڴ���������������
	getProcessModule(currentProcess);
	ShowWindow(SW_SHOW);
}

//ö���߳�
void CDiaKernel::EnumThreadList(DWORD dwPid)
{
	// �����߳̿���
	HANDLE hSnThread = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (hSnThread == INVALID_HANDLE_VALUE)
	{
		// 		printf("�����߳̿���ʧ��,������:%d\n", GetLastError());
		MessageBox(L"�����߳̿���ʧ��");
		return;
	}

	// ��ʼ�����߳�
	THREADENTRY32 threadEntry = { sizeof(THREADENTRY32) };

	// ��ȡ�����еĵ�һ���̵߳���Ϣ
	Thread32First(hSnThread, &threadEntry);

	DWORD dwSuspendCount = 0;
	int i = 0;
	do {

		CString tempOwnerProcessID, tempThreadID, tempBasePri;
		tempOwnerProcessID.Format(L"%d", threadEntry.th32OwnerProcessID);
		tempThreadID.Format(L"%d", threadEntry.th32ThreadID);
		tempBasePri.Format(L"%d", threadEntry.tpBasePri);

		// �жϱ��������߳��Ƿ�����������̵�.
		if (threadEntry.th32OwnerProcessID == dwPid)
		{
			m_listThread.InsertItem(i, tempOwnerProcessID);
			m_listThread.SetItemText(i, 1, tempThreadID);
			m_listThread.SetItemText(i++, 2, tempBasePri);
		}
		// ��ȡ�����е���һ���߳���Ϣ
	} while (Thread32Next(hSnThread, &threadEntry));
	ShowWindow(TRUE);
	return;
}

//ö��ģ��
void CDiaKernel::getProcessModule(DWORD dwPid)
{
	// ReleaseSemaphore;
	// ����ģ�����
	HANDLE hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,
		dwPid);

	if (hModuleSnap == INVALID_HANDLE_VALUE)
		return;

	// ��ʼ��ģ����Ϣ�ṹ��
	MODULEENTRY32 moduleInfo = { sizeof(MODULEENTRY32) };

	//typedef struct tagMODULEENTRY32W
	//{
	//    DWORD   dwSize;             // �ṹ����ֽ���
	//    DWORD   th32ModuleID;       // ģ��ID
	//    DWORD   th32ProcessID;      // �������̵�ID
	//    DWORD   GlblcntUsage;       // Global usage count on the module
	//    DWORD   ProccntUsage;       // Module usage count in th32ProcessID's context
	//    BYTE  * modBaseAddr;        // ģ����ػ�ַ
	//    DWORD   modBaseSize;        // ģ����ֽ���
	//    HMODULE hModule;            // ģ����
	//    WCHAR   szModule[ MAX_MODULE_NAME32 + 1 ]; // ģ����
	//    WCHAR   szExePath[ MAX_PATH ];   // ģ��·��
	//} MODULEENTRY32W;

// 	printf("\t���ػ�ַ | ģ���С |        ģ����         |   ģ��·��\n");
	Module32First(hModuleSnap, &moduleInfo);
	int i = 0;
	do
	{
		CString tempBaseAddr, BaseSize;
		tempBaseAddr.Format(L"%x", moduleInfo.modBaseAddr);
		BaseSize.Format(L"%x", moduleInfo.modBaseSize);

		m_listModul.InsertItem(i, tempBaseAddr);
		m_listModul.SetItemText(i, 1, BaseSize);
		m_listModul.SetItemText(i, 2, moduleInfo.szModule);
		m_listModul.SetItemText(i, 3, moduleInfo.szExePath);
	} while (Module32Next(hModuleSnap, &moduleInfo));
	ShowWindow(TRUE);

}

void CDiaKernel::OnAddwhitelist ()
{
	// TODO: �ڴ���������������
	//������������������
	m_vecProcessName.push_back ( m_processName );
// 	//�����������浽�ļ���
// 	HANDLE hFile = INVALID_HANDLE_VALUE;
// 	hFile = CreateFile (
// 		L"WhiteList.txt",
// 		GENERIC_WRITE | GENERIC_READ,
// 		FILE_SHARE_READ,
// 		0,
// 		OPEN_ALWAYS,
// 		FILE_ATTRIBUTE_NORMAL,
// 		NULL
// 		);
// 	if( hFile == INVALID_HANDLE_VALUE )
// 	{
// 		MessageBox ( L"������򿪰������ļ�ʧ��" );
// 		return;
// 	}
// 	DWORD dwFileSize = GetFileSize ( hFile, NULL );
// 
// 	// 	LPBYTE pFileData = new BYTE[dwFileSize];
// 	// 	DWORD  dwRead = 0;
// 	// 	ReadFile(hFile, // �ļ����
// 	// 		pFileData, // �����ļ����ݵĻ�����
// 	// 		dwFileSize,// Ҫ��ȡ���ֽ���
// 	// 		&dwRead, // ����ʵ�ʶ�ȡ�����ֽ���
// 	// 		NULL
// 	// 		);
// 
// 	// ���ļ���дλ�ö�λ���ļ���ʼ�ĵ�10���ַ�
// 	// 	SetFilePointer(hFile ,10,0,FILE_BEGIN); // ����fseek()
// 	SetFilePointer ( hFile, 0, 0, FILE_END ); // ����fseek()
// 
// 
// 	DWORD dwWrite = 0;
// 	BOOL rt = WriteFile (
// 		hFile,  // �ļ����
// 		m_processName, // д������
// 		64,  // д�����ݵ��ֽ���
// 		&dwWrite, // ����ʵ��д�뵽�ļ��е��ֽ���
// 		0 );
// 	if( dwWrite < 64 )
// 	{
// 		MessageBox ( L"д��ʧ��" );
// 	}
// 
// 	// ���ļ���дλ�ö�λ���ļ�����ǰ�ĵ�10���ַ�
// 	// 	SetFilePointer(hFile, 59, 0, FILE_END); // ����fseek()
// 
// 	// 	printf("%s", pFileData);
// 	// 	delete[] pFileData;
// 	CloseHandle ( hFile ); // �ر��ļ����

}

BOOL CDiaKernel::KillProgram ( DWORD ProcessId )
{
	HANDLE hProcess = OpenProcess ( PROCESS_TERMINATE, FALSE, ProcessId );
	if( hProcess == NULL )
		return FALSE;
	if( !TerminateProcess ( hProcess, 0 ) )
		return FALSE;
	return TRUE;
}


void CDiaKernel::OnCloseprocess ()
{
	// TODO: �ڴ���������������
	KillProgram ( currentProcess );
	Refresh ();
}


void CDiaKernel::OnRefresh ()
{
	// TODO: �ڴ���������������
	Refresh ();
}
