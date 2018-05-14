// DiaKernel.cpp : 实现文件
//

#include "stdafx.h"
#include "windows安全卫士.h"
#include "DiaKernel.h"
#include "afxdialogex.h"
#include <locale.h>
#include <tlhelp32.h>
// #include "MyTab.h"
// CDiaKernel 对话框

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


// CDiaKernel 消息处理程序

void CDiaKernel::Refresh ()
{
	// 1.1 创建进程快照
	HANDLE hProcSnap = CreateToolhelp32Snapshot ( TH32CS_SNAPPROCESS, 0 );

	if( INVALID_HANDLE_VALUE == hProcSnap )
	{
		MessageBox ( L"创建进程快照失败",0,0 );
		return;
	}
	PROCESSENTRY32 processInfo;

	// 初始化结构体的结构体字节字段
	processInfo.dwSize = sizeof ( processInfo );

	// my-上面两行代码可以这么写↓
	//	PROCESSENTRY32 processInfo = { sizeof(processInfo) };
	// 获取第一个进程
	Process32First ( hProcSnap, &processInfo );

	int i = 0;
	do
	{
		// 将得到的进程信息输出
		m_list.InsertItem ( i, processInfo.szExeFile );
		//DWORD转CString
		CString tempProcessID, tempPcPriClassBase;
		tempProcessID.Format ( L"%d", processInfo.th32ProcessID );
		tempPcPriClassBase.Format ( L"%d", processInfo.pcPriClassBase );
		m_list.SetItemText ( i, 1, tempProcessID );
		m_list.SetItemText ( i++, 2, tempPcPriClassBase );

		// 获取下一个进程
	}
	while( Process32Next ( hProcSnap, &processInfo ) );

}

BOOL CDiaKernel::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_list.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	// 添加列标题
	m_list.InsertColumn(0, L"进程名", 0, 100);
	m_list.InsertColumn(1, L"进程ID", 0, 100);
	m_list.InsertColumn(2, L"进程优先级", 0, 100);
	m_listThread.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	// 添加列标题
	m_listThread.InsertColumn(0, L"PID", 0, 100);
	m_listThread.InsertColumn(1, L"线程ID", 0, 100);
	m_listThread.InsertColumn(2, L"线程优先级", 0, 100);
	m_listModul.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	// 添加列标题
	m_listModul.InsertColumn(0, L"加载机制", 0, 80);
	m_listModul.InsertColumn(1, L"模块大小", 0, 80);
	m_listModul.InsertColumn(2, L"模块名", 0, 80);
	m_listModul.InsertColumn(3, L"模块路径", 0, 80);
	// 	char *p = ".txt";
	// 	CString temp(p);
	// 	DWORD dw = 123456;
	// 	temp.Format(L"%d", dw);
	// 设置本地字符集
	setlocale(LC_ALL, "chs");
	Refresh ();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}




DWORD currentProcess;

//列表框鼠标右键响应事件
void CDiaKernel::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	int nCol = pNMItemActivate->iSubItem;
	int nRow = pNMItemActivate->iItem;
// 	CString str = m_list.GetItemText ( nRow, nCol );
	//获取进程ID
	CString str = m_list.GetItemText(nRow, 1);
	//获取进程名
	m_processName = m_list.GetItemText ( nRow, 0 );
// 	CString x = "123";
// 	printf("%d", atoi(x.GetBuffer(x.GetLength())));
	currentProcess = (DWORD)_wtoi(str);
	//将CString转换DWORD
// 	pNMItemActivate->iItem;
// 	m_list.GetItemText();
	CMenu mu;
	// 	my-把整个菜单资源加载进来
	mu.LoadMenuW(IDR_MENU1);
	// 获取"静态"菜单的句柄
	CMenu *pSub = mu.GetSubMenu(0);
	CPoint pt;
	GetCursorPos(&pt);
	// 	//my-获取当前坐标位置
	TrackPopupMenu(pSub->m_hMenu, TPM_LEFTALIGN, pt.x, pt.y, 0, m_hWnd, NULL);
	//my-			 menu句柄		对齐方式		坐标位置		父窗口句柄

	*pResult = 0;
}

//查看线程
void CDiaKernel::On32771()
{
	// TODO:  在此添加命令处理程序代码
	EnumThreadList(currentProcess);
	ShowWindow(SW_SHOW);
}

//查看模块
void CDiaKernel::On32772()
{
	// TODO:  在此添加命令处理程序代码
	getProcessModule(currentProcess);
	ShowWindow(SW_SHOW);
}

//枚举线程
void CDiaKernel::EnumThreadList(DWORD dwPid)
{
	// 创建线程快照
	HANDLE hSnThread = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (hSnThread == INVALID_HANDLE_VALUE)
	{
		// 		printf("创建线程快照失败,错误码:%d\n", GetLastError());
		MessageBox(L"创建线程快照失败");
		return;
	}

	// 开始遍历线程
	THREADENTRY32 threadEntry = { sizeof(THREADENTRY32) };

	// 获取快照中的第一个线程的信息
	Thread32First(hSnThread, &threadEntry);

	DWORD dwSuspendCount = 0;
	int i = 0;
	do {

		CString tempOwnerProcessID, tempThreadID, tempBasePri;
		tempOwnerProcessID.Format(L"%d", threadEntry.th32OwnerProcessID);
		tempThreadID.Format(L"%d", threadEntry.th32ThreadID);
		tempBasePri.Format(L"%d", threadEntry.tpBasePri);

		// 判断遍历到的线程是否属于这个进程的.
		if (threadEntry.th32OwnerProcessID == dwPid)
		{
			m_listThread.InsertItem(i, tempOwnerProcessID);
			m_listThread.SetItemText(i, 1, tempThreadID);
			m_listThread.SetItemText(i++, 2, tempBasePri);
		}
		// 获取快照中的下一个线程信息
	} while (Thread32Next(hSnThread, &threadEntry));
	ShowWindow(TRUE);
	return;
}

//枚举模块
void CDiaKernel::getProcessModule(DWORD dwPid)
{
	// ReleaseSemaphore;
	// 创建模块快照
	HANDLE hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,
		dwPid);

	if (hModuleSnap == INVALID_HANDLE_VALUE)
		return;

	// 初始化模块信息结构体
	MODULEENTRY32 moduleInfo = { sizeof(MODULEENTRY32) };

	//typedef struct tagMODULEENTRY32W
	//{
	//    DWORD   dwSize;             // 结构体的字节数
	//    DWORD   th32ModuleID;       // 模块ID
	//    DWORD   th32ProcessID;      // 所属进程的ID
	//    DWORD   GlblcntUsage;       // Global usage count on the module
	//    DWORD   ProccntUsage;       // Module usage count in th32ProcessID's context
	//    BYTE  * modBaseAddr;        // 模块加载基址
	//    DWORD   modBaseSize;        // 模块的字节数
	//    HMODULE hModule;            // 模块句柄
	//    WCHAR   szModule[ MAX_MODULE_NAME32 + 1 ]; // 模块名
	//    WCHAR   szExePath[ MAX_PATH ];   // 模块路径
	//} MODULEENTRY32W;

// 	printf("\t加载基址 | 模块大小 |        模块名         |   模块路径\n");
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
	// TODO: 在此添加命令处理程序代码
	//进程名加入向量表中
	m_vecProcessName.push_back ( m_processName );
// 	//将进程名保存到文件中
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
// 		MessageBox ( L"创建或打开白名单文件失败" );
// 		return;
// 	}
// 	DWORD dwFileSize = GetFileSize ( hFile, NULL );
// 
// 	// 	LPBYTE pFileData = new BYTE[dwFileSize];
// 	// 	DWORD  dwRead = 0;
// 	// 	ReadFile(hFile, // 文件句柄
// 	// 		pFileData, // 保存文件内容的缓冲区
// 	// 		dwFileSize,// 要读取的字节数
// 	// 		&dwRead, // 函数实际读取到的字节数
// 	// 		NULL
// 	// 		);
// 
// 	// 将文件读写位置定位到文件开始的第10个字符
// 	// 	SetFilePointer(hFile ,10,0,FILE_BEGIN); // 类似fseek()
// 	SetFilePointer ( hFile, 0, 0, FILE_END ); // 类似fseek()
// 
// 
// 	DWORD dwWrite = 0;
// 	BOOL rt = WriteFile (
// 		hFile,  // 文件句柄
// 		m_processName, // 写入内容
// 		64,  // 写入内容的字节数
// 		&dwWrite, // 函数实际写入到文件中的字节数
// 		0 );
// 	if( dwWrite < 64 )
// 	{
// 		MessageBox ( L"写入失败" );
// 	}
// 
// 	// 将文件读写位置定位到文件结束前的第10个字符
// 	// 	SetFilePointer(hFile, 59, 0, FILE_END); // 类似fseek()
// 
// 	// 	printf("%s", pFileData);
// 	// 	delete[] pFileData;
// 	CloseHandle ( hFile ); // 关闭文件句柄

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
	// TODO: 在此添加命令处理程序代码
	KillProgram ( currentProcess );
	Refresh ();
}


void CDiaKernel::OnRefresh ()
{
	// TODO: 在此添加命令处理程序代码
	Refresh ();
}
