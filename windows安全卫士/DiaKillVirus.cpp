// DiaKillVirus.cpp : 实现文件
//

#include "stdafx.h"
#include "windows安全卫士.h"
#include "DiaKillVirus.h"
#include "afxdialogex.h"
#include "MD5.h"
#include <locale.h>
#include <PowrProf.h>
#include <tlhelp32.h>
#include "DiaKernel.h"
#pragma comment(lib,"PowrProf.lib")
/////////关机重启部分头文件
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "advapi32.lib")
/////////////


// CDiaKillVirus 对话框


BOOL MySystemShutdown ()
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;

	// Get a token for this process. 

	if( !OpenProcessToken ( GetCurrentProcess (),
							TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken ) )
		return(FALSE);

	// Get the LUID for the shutdown privilege. 

	LookupPrivilegeValue ( NULL, SE_SHUTDOWN_NAME,
						   &tkp.Privileges[0].Luid );
//SE_SHUTDOWN_NAME = "SeShutdownPrivilege "
//关闭系统,没有这个权限是关不了机的哦...
//另一个熟悉的
//SE_DEBUG_NAME = "SeDebugPrivilege "
//允许访问所有进程.
	tkp.PrivilegeCount = 1;  // one privilege to set    
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	// Get the shutdown privilege for this process. 

	AdjustTokenPrivileges ( hToken, FALSE, &tkp, 0,
							(PTOKEN_PRIVILEGES)NULL, 0 );

	if( GetLastError () != ERROR_SUCCESS )
		return FALSE;

	// Shut down the system and force all applications to close. 

	if( !ExitWindowsEx ( EWX_SHUTDOWN | EWX_FORCE,
						 SHTDN_REASON_MAJOR_OPERATINGSYSTEM |
						 SHTDN_REASON_MINOR_UPGRADE |
						 SHTDN_REASON_FLAG_PLANNED ) )
		return FALSE;

	//shutdown was successful
	return TRUE;
}

BOOL MySystemReboot ()
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;

	// Get a token for this process. 

	if( !OpenProcessToken ( GetCurrentProcess (),
							TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken ) )
		return(FALSE);

	// Get the LUID for the shutdown privilege. 

	LookupPrivilegeValue ( NULL, SE_SHUTDOWN_NAME,
						   &tkp.Privileges[0].Luid );

	tkp.PrivilegeCount = 1;  // one privilege to set    
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	// Get the shutdown privilege for this process. 

	AdjustTokenPrivileges ( hToken, FALSE, &tkp, 0,
							(PTOKEN_PRIVILEGES)NULL, 0 );

	if( GetLastError () != ERROR_SUCCESS )
		return FALSE;

	// Shut down the system and force all applications to close. 

	if( !ExitWindowsEx ( EWX_REBOOT | EWX_FORCE,
						 SHTDN_REASON_MAJOR_OPERATINGSYSTEM |
						 SHTDN_REASON_MINOR_UPGRADE |
						 SHTDN_REASON_FLAG_PLANNED ) )
		return FALSE;

	//shutdown was successful
	return TRUE;
}


IMPLEMENT_DYNAMIC ( CDiaKillVirus, CDialogEx )

CDiaKillVirus::CDiaKillVirus ( CWnd* pParent /*=NULL*/ )
	: CDialogEx ( CDiaKillVirus::IDD, pParent )
{

}

CDiaKillVirus::~CDiaKillVirus ()
{}

void CDiaKillVirus::DoDataExchange ( CDataExchange* pDX )
{
	CDialogEx::DoDataExchange ( pDX );
	DDX_Control ( pDX, IDC_LISTVIRUS, m_ctlListVirus );
	DDX_Control ( pDX, IDC_LISTsuspicious, m_listSuspicious );
}


BEGIN_MESSAGE_MAP ( CDiaKillVirus, CDialogEx )
	ON_BN_CLICKED ( IDC_BUTTONADDVIRUS, &CDiaKillVirus::OnBnClickedButtonaddvirus )
	ON_BN_CLICKED ( IDC_BUTTONMD5KILL, &CDiaKillVirus::OnBnClickedButtonmd5kill )
	ON_BN_CLICKED ( IDC_BUTTONALLPATHKILL, &CDiaKillVirus::OnBnClickedButtonallpathkill )
	ON_BN_CLICKED ( IDC_BUTTONSHUTDOWN, &CDiaKillVirus::OnBnClickedButtonshutdown )
	ON_BN_CLICKED ( IDC_BUTTONREBOOT, &CDiaKillVirus::OnBnClickedButtonreboot )
	ON_BN_CLICKED ( IDC_BUTTONLOGOFF, &CDiaKillVirus::OnBnClickedButtonlogoff )
	ON_BN_CLICKED ( IDC_BUTTONDORMANT, &CDiaKillVirus::OnBnClickedButtondormant )
	ON_BN_CLICKED ( IDC_BUTTONSLEEP, &CDiaKillVirus::OnBnClickedButtonsleep )
	ON_BN_CLICKED ( IDC_BUTTONLOCKSCREEN, &CDiaKillVirus::OnBnClickedButtonlockscreen )

	ON_BN_CLICKED ( IDC_BUTTONWHITEKILL, &CDiaKillVirus::OnBnClickedButtonwhitekill )
	ON_NOTIFY ( NM_RCLICK, IDC_LISTsuspicious, &CDiaKillVirus::OnRclickListsuspicious )
	ON_COMMAND ( ID_SUSPICIOUS_ClOSE, &CDiaKillVirus::OnCloseSuspiciousProcess )
END_MESSAGE_MAP ()


// CDiaKillVirus 消息处理程序

//将病毒文件加入病毒库
void CDiaKillVirus::OnBnClickedButtonaddvirus ()
{
	// TODO:  在此添加控件通知处理程序代码
	CStringA FilePathName;
	// 	char* FilePathName=nullptr;
	CFileDialog dlg ( TRUE );
	dlg.m_ofn.lpstrTitle = L"选择文件";
	dlg.m_ofn.lpstrFilter = L"AllFile(*.*)\0*.*\0\0";

	if( dlg.DoModal () == IDOK )
		FilePathName = dlg.GetPathName ();

	char buf[1024] = { 0 };
	strcpy_s ( buf, FilePathName );
	CString strMD5 = nullptr;
	strMD5 = md5FileValue ( buf );
	// 	printf("%s", strMD5);


	HANDLE hFile = INVALID_HANDLE_VALUE;
	hFile = CreateFile (
		L"VirusLib.txt",
		GENERIC_WRITE | GENERIC_READ,
		FILE_SHARE_READ,
		0,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
		);
	if( hFile == INVALID_HANDLE_VALUE )
	{
		OutputDebugString ( L"创建或打开文件失败\n" );
		return;
	}
	DWORD dwFileSize = GetFileSize ( hFile, NULL );

	// 	LPBYTE pFileData = new BYTE[dwFileSize];
	// 	DWORD  dwRead = 0;
	// 	ReadFile(hFile, // 文件句柄
	// 		pFileData, // 保存文件内容的缓冲区
	// 		dwFileSize,// 要读取的字节数
	// 		&dwRead, // 函数实际读取到的字节数
	// 		NULL
	// 		);

	// 将文件读写位置定位到文件开始的第10个字符
	// 	SetFilePointer(hFile ,10,0,FILE_BEGIN); // 类似fseek()
	SetFilePointer ( hFile, 0, 0, FILE_END ); // 类似fseek()


	DWORD dwWrite = 0;
	BOOL rt = WriteFile (
		hFile,  // 文件句柄
		strMD5, // 写入内容
		64,  // 写入内容的字节数
		&dwWrite, // 函数实际写入到文件中的字节数
		0 );
	if( dwWrite < 64 )
	{
		MessageBox ( L"写入失败" );
	}

	// 将文件读写位置定位到文件结束前的第10个字符
	// 	SetFilePointer(hFile, 59, 0, FILE_END); // 类似fseek()

	// 	printf("%s", pFileData);
	// 	delete[] pFileData;
	CloseHandle ( hFile ); // 关闭文件句柄

}

//MD5查毒
void CDiaKillVirus::OnBnClickedButtonmd5kill ()
{
	// TODO:  在此添加控件通知处理程序代码
	CStringA FilePathName;
	// 	char* FilePathName=nullptr;
	CFileDialog dlg ( TRUE );
	dlg.m_ofn.lpstrTitle = L"选择文件";
	dlg.m_ofn.lpstrFilter = L"AllFile(*.*)\0*.*\0\0";

	if( dlg.DoModal () == IDOK )
		FilePathName = dlg.GetPathName ();

	char buf[1024] = { 0 };
	strcpy_s ( buf, FilePathName );
	CString strMD5 = nullptr;
	strMD5 = md5FileValue ( buf );

	HANDLE hFile = INVALID_HANDLE_VALUE;
	hFile = CreateFile (
		L"VirusLib.txt",
		GENERIC_WRITE | GENERIC_READ,
		FILE_SHARE_READ,
		0,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
		);
	if( hFile == INVALID_HANDLE_VALUE )
	{
		MessageBox ( L"创建或打开文件失败" );
		return;
	}
	DWORD dwFileSize = GetFileSize ( hFile, NULL );

	LPBYTE pFileData = new BYTE[dwFileSize + 2]{};

	DWORD  dwRead = 0;
	// 		LPVOID strLib=nullptr;
	ReadFile ( hFile, // 文件句柄
			   pFileData, // 保存文件内容的缓冲区
			   dwFileSize,// 要读取的字节数
			   &dwRead, // 函数实际读取到的字节数
			   NULL
			   );
	//CString strLib(pFileData);
	TCHAR szTemp[33] = {};
	CString strOneOfMD5Lib;
	int nCount = dwRead / (32 * 2);
	bool isSafe = TRUE;
	for( int i = 0; i < nCount; i++ )
	{
		memcpy_s ( szTemp, 66, pFileData + (i * 64), 64 );
		strOneOfMD5Lib = szTemp;
		if( strMD5 == strOneOfMD5Lib )
		{
			MessageBox ( L"该文件是病毒!" );
			// 			return;不能用return,用了之后,若有病毒,直接推出,但是句柄还没有关,下次查杀会导致创建文件失败
			isSafe = FALSE;
			break;
		}
	}
	if( isSafe )
	{
		MessageBox ( L"该文件是安全的" );
	}
	delete[] pFileData;
	CloseHandle ( hFile ); // 关闭文件句柄

}


//遍历指定文件夹下文件
void CDiaKillVirus::Travel ( const TCHAR* pszPath, int nDeep )
{
	if( nDeep == 0 )
		return;

	HANDLE hFind = NULL;
	TCHAR buff[MAX_PATH];

	// 合成一个用于查找的字符串
	_stprintf_s ( buff, MAX_PATH, _T ( "%s\\*" ), pszPath );

	WIN32_FIND_DATA findData = { 0 };
	// 查找指定目录下的第一个文件, 并且把文件信息保存到结构体中,
	// 并返回一个查找句柄
	hFind = FindFirstFile ( buff, &findData );
	if( hFind == INVALID_HANDLE_VALUE )
		return;

	do
	{
		// 		wprintf(L"%s\n", findData.cFileName);
		if( _tcscmp ( findData.cFileName, L"." ) == 0
			|| _tcscmp ( findData.cFileName, L".." ) == 0 )
		{
			continue;
		}

		TCHAR fileNameBuff[MAX_PATH];
		_stprintf_s ( fileNameBuff, MAX_PATH, _T ( "%s\\%s" ), pszPath, findData.cFileName );


		//宽字符转单字符
		int nLen = WideCharToMultiByte ( CP_ACP, 0, fileNameBuff, -1, NULL, 0, NULL, NULL );
		if( nLen == 0 )
		{
			MessageBox ( L"字符转换出错!" );
		}
		char* pResult = new char[nLen];
		WideCharToMultiByte ( CP_ACP, 0, fileNameBuff, -1, pResult, nLen, NULL, NULL );
		//单字符进行MD5转换
		// 		char buf[1024] = { 0 };
		// 		strcpy_s(buf, pResult);
		CString strMD5 = nullptr;
		strMD5 = md5FileValue ( pResult );
		//打开文件
		HANDLE hFile = INVALID_HANDLE_VALUE;
		hFile = CreateFile (
			L"VirusLib.txt",
			GENERIC_WRITE | GENERIC_READ,
			FILE_SHARE_READ,
			0,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL
			);
		if( hFile == INVALID_HANDLE_VALUE )
		{
			MessageBox ( L"创建或打开文件失败" );

		}
		DWORD dwFileSize = GetFileSize ( hFile, NULL );

		LPBYTE pFileData = new BYTE[dwFileSize + 2]{};

		DWORD  dwRead = 0;
		// 		LPVOID strLib=nullptr;
		ReadFile ( hFile, // 文件句柄
				   pFileData, // 保存文件内容的缓冲区
				   dwFileSize,// 要读取的字节数
				   &dwRead, // 函数实际读取到的字节数
				   NULL
				   );
		//CString strLib(pFileData);
		TCHAR szTemp[33] = {};
		CString strOneOfMD5Lib;
		int nCount = dwRead / (32 * 2);
		for( int i = 0; i < nCount; i++ )
		{
			memcpy_s ( szTemp, 66, pFileData + (i * 64), 64 );
			strOneOfMD5Lib = szTemp;
			if( strMD5 == strOneOfMD5Lib )
			{
				// 				m_listVirus.InsertColumn(0, L"病毒路径");
				// 				m_listVirus.InsertItem(0,)
				m_ctlListVirus.AddString ( fileNameBuff );
				// 				MessageBox(L"该文件是病毒!");
				break;
			}
		}
		// 把变量内容更新到对话框
		UpdateData ( FALSE );
		// 		MessageBox(L"该文件是安全的");
		delete[] pFileData;
		CloseHandle ( hFile ); // 关闭文件句柄


		// 
		// 过滤掉当前目录和上层目录
		if( _tcscmp ( findData.cFileName, _T ( "." ) ) == 0 //判断是否是当前目录
			|| _tcscmp ( findData.cFileName, _T ( ".." ) ) == 0 )// 判断是否是上层目录
		{
			continue;
		}

		// 判断当前文件是否是一个目录
		if( findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{
			// 将当前文件夹路径和目录名合成一个新的文件夹路径
			_stprintf_s ( buff, MAX_PATH, _T ( "%s\\%s" ), pszPath, findData.cFileName );
			Travel ( buff, nDeep - 1 );
		}


	}
	while( FindNextFile ( hFind, &findData ) );// 查找下一个文件,并且把文件信息保存到结构体中

}

//全路径查毒
void CDiaKillVirus::OnBnClickedButtonallpathkill ()
{
	// TODO:  在此添加控件通知处理程序代码
	m_ctlListVirus.ResetContent ();
	TCHAR szKillVirusPath[MAX_PATH] = {};
	BROWSEINFO bi = {};
	// 文件夹选择对话框所属窗口句柄
	bi.hwndOwner = m_hWnd;
	bi.pszDisplayName = szKillVirusPath;
	// 窗口说明信息
	bi.lpszTitle = L"打开要遍历的文件夹";
	// 对话框带编辑框
	bi.ulFlags = BIF_EDITBOX;
	LPITEMIDLIST pid;
	// 返回标识文件夹路径的“PID”，用它找到文件夹路径
	pid = SHBrowseForFolder ( &bi );
	SHGetPathFromIDList ( pid, szKillVirusPath );

	setlocale ( LC_ALL, "chs" );
	Travel ( szKillVirusPath, 2 );
}

//白名单查杀
void CDiaKillVirus::OnBnClickedButtonwhitekill ()
{
	// TODO: 在此添加控件通知处理程序代码
	ShowSuspiciousProcessList ();
}

void CDiaKillVirus::ShowSuspiciousProcessList ()
{
	// 1. 使用快照来遍历进程
	// 1.1 创建进程快照
	HANDLE hProcSnap = CreateToolhelp32Snapshot ( TH32CS_SNAPPROCESS, 0 );

	if( INVALID_HANDLE_VALUE == hProcSnap )
	{
		MessageBox ( 0, L"创建进程快照失败", 0 );
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
		WCHAR   szExeFile[MAX_PATH];    // Path
		CString m_strExeFileName = processInfo.szExeFile;
		CString tempProcessID;
		tempProcessID.Format ( L"%d", processInfo.th32ProcessID );

		m_vecWhiteList = CDiaKernel::m_vecProcessName;
		vector<CString>::iterator it;
		it = find ( m_vecWhiteList.begin (), m_vecWhiteList.end (), m_strExeFileName );
		if( it == m_vecWhiteList.end () )
		{
			m_listSuspicious.InsertItem ( i, m_strExeFileName );
			m_listSuspicious.SetItemText ( i++, 1, tempProcessID );
		}

		/*
		vector<int> vec;
		vector<int>::iterator it;
		int value = 24;

		it = find ( vec.begin (), vec.end (), value );

		if( it != vec.end () )
		{
		//vec中存在value值
		}
		else
		{
		//vec中不存在value值
		}
		*/


		// 获取下一个进程
	}
	while( Process32Next ( hProcSnap, &processInfo ) );

}

//关机
void CDiaKillVirus::OnBnClickedButtonshutdown ()
{
	// TODO:  在此添加控件通知处理程序代码
	/*
	ExitWindowsEx(EWX_POWEROFF | EWX_FORCE,NULL);
	system("shutdown /p");
	*/
	MySystemShutdown ();

}

//重启
void CDiaKillVirus::OnBnClickedButtonreboot ()
{
	// TODO:  在此添加控件通知处理程序代码
	/*
	ExitWindowsEx(EWX_REBOOT| EWX_FORCE, NULL);
	system("shutdown /r");
	*/
	MySystemReboot ();
}

//注销
void CDiaKillVirus::OnBnClickedButtonlogoff ()
{
	// TODO:  在此添加控件通知处理程序代码
	ExitWindowsEx ( EWX_LOGOFF | EWX_FORCE, NULL );

}

//休眠
void CDiaKillVirus::OnBnClickedButtondormant ()
{
	// TODO:  在此添加控件通知处理程序代码
	SetSuspendState ( TRUE, FALSE, FALSE );
	//和睡眠功能总是编译不过去,是未包含lib文件,设置

}

//睡眠
void CDiaKillVirus::OnBnClickedButtonsleep ()
{
	// TODO:  在此添加控件通知处理程序代码
	SetSuspendState ( FALSE, FALSE, FALSE );

}

//锁屏
void CDiaKillVirus::OnBnClickedButtonlockscreen ()
{
	// TODO:  在此添加控件通知处理程序代码
	LockWorkStation ();
}



BOOL CDiaKillVirus::OnInitDialog ()
{
	CDialogEx::OnInitDialog ();

	// TODO:  在此添加额外的初始化
	setlocale ( LC_ALL, "chs" );

	m_listSuspicious.SetExtendedStyle ( LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT );

	m_listSuspicious.InsertColumn ( 0, L"可疑进程",0,100 );
	m_listSuspicious.InsertColumn ( 2, L"进程ID", 0, 100 );

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CDiaKillVirus::OnRclickListsuspicious ( NMHDR *pNMHDR, LRESULT *pResult )
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	int nRow = pNMItemActivate->iItem;
	CString strTemp= m_listSuspicious.GetItemText ( nRow, 1 );
	m_dwCurrentProcess =  (DWORD)_wtoi ( strTemp );
	CMenu mu;
	// 	my-把整个菜单资源加载进来
	mu.LoadMenuW ( IDR_MENU2 );
	// 获取"静态"菜单的句柄
	CMenu *pSub = mu.GetSubMenu ( 0 );
	CPoint pt;
	GetCursorPos ( &pt );
	// 	//my-获取当前坐标位置
	TrackPopupMenu ( pSub->m_hMenu, TPM_LEFTALIGN, pt.x, pt.y, 0, m_hWnd, NULL );
	//my-			 menu句柄		对齐方式		坐标位置		父窗口句柄

	*pResult = 0;
}

void CDiaKillVirus::OnCloseSuspiciousProcess ()
{
	// TODO: 在此添加命令处理程序代码
	KillProgram ( m_dwCurrentProcess );
	ShowSuspiciousProcessList ();
}

BOOL CDiaKillVirus::KillProgram ( DWORD ProcessId )
{
	HANDLE hProcess = OpenProcess ( PROCESS_TERMINATE, FALSE, ProcessId );
	if( hProcess == NULL )
		return FALSE;
	if( !TerminateProcess ( hProcess, 0 ) )
		return FALSE;
	return TRUE;
}
