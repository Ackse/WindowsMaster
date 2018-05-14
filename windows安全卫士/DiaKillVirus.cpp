// DiaKillVirus.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "windows��ȫ��ʿ.h"
#include "DiaKillVirus.h"
#include "afxdialogex.h"
#include "MD5.h"
#include <locale.h>
#include <PowrProf.h>
#include <tlhelp32.h>
#include "DiaKernel.h"
#pragma comment(lib,"PowrProf.lib")
/////////�ػ���������ͷ�ļ�
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "advapi32.lib")
/////////////


// CDiaKillVirus �Ի���


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
//�ر�ϵͳ,û�����Ȩ���ǹز��˻���Ŷ...
//��һ����Ϥ��
//SE_DEBUG_NAME = "SeDebugPrivilege "
//����������н���.
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


// CDiaKillVirus ��Ϣ�������

//�������ļ����벡����
void CDiaKillVirus::OnBnClickedButtonaddvirus ()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CStringA FilePathName;
	// 	char* FilePathName=nullptr;
	CFileDialog dlg ( TRUE );
	dlg.m_ofn.lpstrTitle = L"ѡ���ļ�";
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
		OutputDebugString ( L"��������ļ�ʧ��\n" );
		return;
	}
	DWORD dwFileSize = GetFileSize ( hFile, NULL );

	// 	LPBYTE pFileData = new BYTE[dwFileSize];
	// 	DWORD  dwRead = 0;
	// 	ReadFile(hFile, // �ļ����
	// 		pFileData, // �����ļ����ݵĻ�����
	// 		dwFileSize,// Ҫ��ȡ���ֽ���
	// 		&dwRead, // ����ʵ�ʶ�ȡ�����ֽ���
	// 		NULL
	// 		);

	// ���ļ���дλ�ö�λ���ļ���ʼ�ĵ�10���ַ�
	// 	SetFilePointer(hFile ,10,0,FILE_BEGIN); // ����fseek()
	SetFilePointer ( hFile, 0, 0, FILE_END ); // ����fseek()


	DWORD dwWrite = 0;
	BOOL rt = WriteFile (
		hFile,  // �ļ����
		strMD5, // д������
		64,  // д�����ݵ��ֽ���
		&dwWrite, // ����ʵ��д�뵽�ļ��е��ֽ���
		0 );
	if( dwWrite < 64 )
	{
		MessageBox ( L"д��ʧ��" );
	}

	// ���ļ���дλ�ö�λ���ļ�����ǰ�ĵ�10���ַ�
	// 	SetFilePointer(hFile, 59, 0, FILE_END); // ����fseek()

	// 	printf("%s", pFileData);
	// 	delete[] pFileData;
	CloseHandle ( hFile ); // �ر��ļ����

}

//MD5�鶾
void CDiaKillVirus::OnBnClickedButtonmd5kill ()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CStringA FilePathName;
	// 	char* FilePathName=nullptr;
	CFileDialog dlg ( TRUE );
	dlg.m_ofn.lpstrTitle = L"ѡ���ļ�";
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
		MessageBox ( L"��������ļ�ʧ��" );
		return;
	}
	DWORD dwFileSize = GetFileSize ( hFile, NULL );

	LPBYTE pFileData = new BYTE[dwFileSize + 2]{};

	DWORD  dwRead = 0;
	// 		LPVOID strLib=nullptr;
	ReadFile ( hFile, // �ļ����
			   pFileData, // �����ļ����ݵĻ�����
			   dwFileSize,// Ҫ��ȡ���ֽ���
			   &dwRead, // ����ʵ�ʶ�ȡ�����ֽ���
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
			MessageBox ( L"���ļ��ǲ���!" );
			// 			return;������return,����֮��,���в���,ֱ���Ƴ�,���Ǿ����û�й�,�´β�ɱ�ᵼ�´����ļ�ʧ��
			isSafe = FALSE;
			break;
		}
	}
	if( isSafe )
	{
		MessageBox ( L"���ļ��ǰ�ȫ��" );
	}
	delete[] pFileData;
	CloseHandle ( hFile ); // �ر��ļ����

}


//����ָ���ļ������ļ�
void CDiaKillVirus::Travel ( const TCHAR* pszPath, int nDeep )
{
	if( nDeep == 0 )
		return;

	HANDLE hFind = NULL;
	TCHAR buff[MAX_PATH];

	// �ϳ�һ�����ڲ��ҵ��ַ���
	_stprintf_s ( buff, MAX_PATH, _T ( "%s\\*" ), pszPath );

	WIN32_FIND_DATA findData = { 0 };
	// ����ָ��Ŀ¼�µĵ�һ���ļ�, ���Ұ��ļ���Ϣ���浽�ṹ����,
	// ������һ�����Ҿ��
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


		//���ַ�ת���ַ�
		int nLen = WideCharToMultiByte ( CP_ACP, 0, fileNameBuff, -1, NULL, 0, NULL, NULL );
		if( nLen == 0 )
		{
			MessageBox ( L"�ַ�ת������!" );
		}
		char* pResult = new char[nLen];
		WideCharToMultiByte ( CP_ACP, 0, fileNameBuff, -1, pResult, nLen, NULL, NULL );
		//���ַ�����MD5ת��
		// 		char buf[1024] = { 0 };
		// 		strcpy_s(buf, pResult);
		CString strMD5 = nullptr;
		strMD5 = md5FileValue ( pResult );
		//���ļ�
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
			MessageBox ( L"��������ļ�ʧ��" );

		}
		DWORD dwFileSize = GetFileSize ( hFile, NULL );

		LPBYTE pFileData = new BYTE[dwFileSize + 2]{};

		DWORD  dwRead = 0;
		// 		LPVOID strLib=nullptr;
		ReadFile ( hFile, // �ļ����
				   pFileData, // �����ļ����ݵĻ�����
				   dwFileSize,// Ҫ��ȡ���ֽ���
				   &dwRead, // ����ʵ�ʶ�ȡ�����ֽ���
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
				// 				m_listVirus.InsertColumn(0, L"����·��");
				// 				m_listVirus.InsertItem(0,)
				m_ctlListVirus.AddString ( fileNameBuff );
				// 				MessageBox(L"���ļ��ǲ���!");
				break;
			}
		}
		// �ѱ������ݸ��µ��Ի���
		UpdateData ( FALSE );
		// 		MessageBox(L"���ļ��ǰ�ȫ��");
		delete[] pFileData;
		CloseHandle ( hFile ); // �ر��ļ����


		// 
		// ���˵���ǰĿ¼���ϲ�Ŀ¼
		if( _tcscmp ( findData.cFileName, _T ( "." ) ) == 0 //�ж��Ƿ��ǵ�ǰĿ¼
			|| _tcscmp ( findData.cFileName, _T ( ".." ) ) == 0 )// �ж��Ƿ����ϲ�Ŀ¼
		{
			continue;
		}

		// �жϵ�ǰ�ļ��Ƿ���һ��Ŀ¼
		if( findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{
			// ����ǰ�ļ���·����Ŀ¼���ϳ�һ���µ��ļ���·��
			_stprintf_s ( buff, MAX_PATH, _T ( "%s\\%s" ), pszPath, findData.cFileName );
			Travel ( buff, nDeep - 1 );
		}


	}
	while( FindNextFile ( hFind, &findData ) );// ������һ���ļ�,���Ұ��ļ���Ϣ���浽�ṹ����

}

//ȫ·���鶾
void CDiaKillVirus::OnBnClickedButtonallpathkill ()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_ctlListVirus.ResetContent ();
	TCHAR szKillVirusPath[MAX_PATH] = {};
	BROWSEINFO bi = {};
	// �ļ���ѡ��Ի����������ھ��
	bi.hwndOwner = m_hWnd;
	bi.pszDisplayName = szKillVirusPath;
	// ����˵����Ϣ
	bi.lpszTitle = L"��Ҫ�������ļ���";
	// �Ի�����༭��
	bi.ulFlags = BIF_EDITBOX;
	LPITEMIDLIST pid;
	// ���ر�ʶ�ļ���·���ġ�PID���������ҵ��ļ���·��
	pid = SHBrowseForFolder ( &bi );
	SHGetPathFromIDList ( pid, szKillVirusPath );

	setlocale ( LC_ALL, "chs" );
	Travel ( szKillVirusPath, 2 );
}

//��������ɱ
void CDiaKillVirus::OnBnClickedButtonwhitekill ()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ShowSuspiciousProcessList ();
}

void CDiaKillVirus::ShowSuspiciousProcessList ()
{
	// 1. ʹ�ÿ�������������
	// 1.1 �������̿���
	HANDLE hProcSnap = CreateToolhelp32Snapshot ( TH32CS_SNAPPROCESS, 0 );

	if( INVALID_HANDLE_VALUE == hProcSnap )
	{
		MessageBox ( 0, L"�������̿���ʧ��", 0 );
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
		//vec�д���valueֵ
		}
		else
		{
		//vec�в�����valueֵ
		}
		*/


		// ��ȡ��һ������
	}
	while( Process32Next ( hProcSnap, &processInfo ) );

}

//�ػ�
void CDiaKillVirus::OnBnClickedButtonshutdown ()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	/*
	ExitWindowsEx(EWX_POWEROFF | EWX_FORCE,NULL);
	system("shutdown /p");
	*/
	MySystemShutdown ();

}

//����
void CDiaKillVirus::OnBnClickedButtonreboot ()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	/*
	ExitWindowsEx(EWX_REBOOT| EWX_FORCE, NULL);
	system("shutdown /r");
	*/
	MySystemReboot ();
}

//ע��
void CDiaKillVirus::OnBnClickedButtonlogoff ()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	ExitWindowsEx ( EWX_LOGOFF | EWX_FORCE, NULL );

}

//����
void CDiaKillVirus::OnBnClickedButtondormant ()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	SetSuspendState ( TRUE, FALSE, FALSE );
	//��˯�߹������Ǳ��벻��ȥ,��δ����lib�ļ�,����

}

//˯��
void CDiaKillVirus::OnBnClickedButtonsleep ()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	SetSuspendState ( FALSE, FALSE, FALSE );

}

//����
void CDiaKillVirus::OnBnClickedButtonlockscreen ()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	LockWorkStation ();
}



BOOL CDiaKillVirus::OnInitDialog ()
{
	CDialogEx::OnInitDialog ();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	setlocale ( LC_ALL, "chs" );

	m_listSuspicious.SetExtendedStyle ( LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT );

	m_listSuspicious.InsertColumn ( 0, L"���ɽ���",0,100 );
	m_listSuspicious.InsertColumn ( 2, L"����ID", 0, 100 );

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CDiaKillVirus::OnRclickListsuspicious ( NMHDR *pNMHDR, LRESULT *pResult )
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nRow = pNMItemActivate->iItem;
	CString strTemp= m_listSuspicious.GetItemText ( nRow, 1 );
	m_dwCurrentProcess =  (DWORD)_wtoi ( strTemp );
	CMenu mu;
	// 	my-�������˵���Դ���ؽ���
	mu.LoadMenuW ( IDR_MENU2 );
	// ��ȡ"��̬"�˵��ľ��
	CMenu *pSub = mu.GetSubMenu ( 0 );
	CPoint pt;
	GetCursorPos ( &pt );
	// 	//my-��ȡ��ǰ����λ��
	TrackPopupMenu ( pSub->m_hMenu, TPM_LEFTALIGN, pt.x, pt.y, 0, m_hWnd, NULL );
	//my-			 menu���		���뷽ʽ		����λ��		�����ھ��

	*pResult = 0;
}

void CDiaKillVirus::OnCloseSuspiciousProcess ()
{
	// TODO: �ڴ���������������
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
