// DiaService.cpp : 实现文件
//

#include "stdafx.h"
#include "windows安全卫士.h"
#include "DiaService.h"
#include "afxdialogex.h"
#include <winsvc.h>

// CDiaService 对话框

IMPLEMENT_DYNAMIC ( CDiaService, CDialogEx )

CDiaService::CDiaService ( CWnd* pParent /*=NULL*/ )
	: CDialogEx ( CDiaService::IDD, pParent )
{

}

CDiaService::~CDiaService ()
{}

void CDiaService::DoDataExchange ( CDataExchange* pDX )
{
	CDialogEx::DoDataExchange ( pDX );
	DDX_Control ( pDX, IDC_LISTSERVICE, m_listService );
}


BEGIN_MESSAGE_MAP ( CDiaService, CDialogEx )
	ON_NOTIFY ( NM_RCLICK, IDC_LISTSERVICE, &CDiaService::OnRclickListservice )
	//	ON_COMMAND(ID_POPUP_32773, &CDiaService::OnPopupStartService)
	//	ON_COMMAND(ID_POPUP_32774, &CDiaService::OnPopupShutService)
END_MESSAGE_MAP ()


// CDiaService 消息处理程序

// void CDiaService::PromotePower ()
// {
// 	// TODO:  在此添加控件通知处理程序代码
// 	// 以管理员身份运行本进程
// 	// 获取本进程的文件路径.
// 	WCHAR szApplication[MAX_PATH] = { 0 };			// 需要初始化
// 	DWORD cchLength = _countof ( szApplication );
// 	QueryFullProcessImageName (
// 		GetCurrentProcess (),
// 		0,
// 		szApplication,
// 		&cchLength
// 		);
// 
// 	//5 以管理员权限重新打开进程
// 	SHELLEXECUTEINFO sei = { sizeof ( SHELLEXECUTEINFO ) };
// 	sei.lpVerb = L"runas";				//请求提升权限
// 	sei.lpFile = szApplication;			//可执行文件路径
// 	sei.lpParameters = NULL;			//不需要参数
// 	sei.nShow = SW_SHOWNORMAL;			//正常显示窗口
// 
// 	if( ShellExecuteEx ( &sei ) )
// 	{
// 		PostQuitMessage ( 0 );
// 	}
// 
// }

BOOL CDiaService::OnInitDialog ()
{
	CDialogEx::OnInitDialog ();
	m_listService.SetExtendedStyle ( LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT );

	// TODO:  在此添加额外的初始化
	m_listService.InsertColumn ( 0, L"服务名", 0, 120 );
	m_listService.InsertColumn ( 1, L"服务状态", 0, 60 );
	m_listService.InsertColumn ( 2, L"服务类型", 0, 60 );
	m_listService.InsertColumn ( 3, L"启动类型", 0, 60 );
	m_listService.InsertColumn ( 4, L"路径", 0, 200 );
	m_listService.InsertColumn ( 5, L"currentSC_HANDLE", 0, 200 );
	ShowServicesStatus ();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}
SC_HANDLE hSCM;
void CDiaService::ShowServicesStatus ()
{
	//1.打开远程计算机服务控制管理器
	/*
	SC_HANDLE
	WINAPI
	OpenSCManagerW(
	_In_opt_   LPCWSTR  lpMachineName,//主机名,可以是远程主机的名称,如果是NULL则打开的是本地主机
	_In_opt_   LPCWSTR  lpDatabaseName,//数据库名,应该设为SERVICES_ACTIVE_DATABASE,填NULL则自动设成前面的宏
	_In_       DWORD    dwDesiredAccess//访问权限,
	SC_MANAGER_ALL_ACCESS:所有权
	);
	*/

	hSCM = OpenSCManagerW ( NULL,
							NULL,
							SC_MANAGER_ALL_ACCESS );
	//2.第一次调用,获取需要的内存大小
	DWORD dwServiceNum = 0;
	DWORD dwSize = 0;
	EnumServicesStatusEx ( hSCM,
						   SC_ENUM_PROCESS_INFO,
						   SERVICE_WIN32,
						   SERVICE_STATE_ALL,//所有服务状态
						   NULL,//缓冲区
						   0,//缓冲区大小
						   &dwSize,//需要的大小
						   &dwServiceNum,//缓冲区中的服务个数
						   NULL,
						   NULL
						   );
	//3.申请需要的内存,第二次调用
	LPENUM_SERVICE_STATUS_PROCESS pEnumSerice =
		(LPENUM_SERVICE_STATUS_PROCESS)new char[dwSize];
	//4.第二次枚举
	bool bStatus = FALSE;
	bStatus = EnumServicesStatusEx ( hSCM,
									 SC_ENUM_PROCESS_INFO,
									 SERVICE_WIN32,
									 SERVICE_STATE_ALL,//所有服务状态
									 (PBYTE)pEnumSerice,//缓冲区
									 dwSize,//缓冲区大小
									 &dwSize,//需要的大小
									 &dwServiceNum,//缓冲区中的服务个数
									 NULL, NULL
									 );
	// 	5.遍历信息
	for( DWORD i = 0; i < dwServiceNum; i++ )
	{
		//获取基础信息
		//一.服务名		
		m_listService.InsertItem ( i, pEnumSerice[i].lpServiceName );
		//二.服务状态 有 已经停止,正在运行,正在暂停
		//(根据得到的值手动输出字符串,具体值的含义可以在该结构体上按F1差MSDN)	
		DWORD m_dwCurrentState = pEnumSerice[i].ServiceStatusProcess.dwCurrentState;
		CStringW m_strDwCurrentState = nullptr;
		switch( m_dwCurrentState )
		{
			case 1:
			m_strDwCurrentState = "stopped";
			break;
			case 2:
			m_strDwCurrentState = "starting";
			break;
			case 3:
			m_strDwCurrentState = "stopping";
			break;
			case 4:
			m_strDwCurrentState = "running";
			break;
			case 5:
			m_strDwCurrentState = "continue";
			break;
			case 6:
			m_strDwCurrentState = "pausing";
			break;
			case 7:
			m_strDwCurrentState = "paused";
			break;
		}
		m_listService.SetItemText ( i, 1, m_strDwCurrentState );
		//三.服务类型
		//有文件系统驱动服务,驱动服务,独立进程服务
		DWORD m_dwServiceType = pEnumSerice[i].ServiceStatusProcess.dwServiceType;
		CStringW m_strDwServiceType = nullptr;
		switch( m_dwServiceType )
		{
			case 0x00000001:
			m_strDwServiceType = "device driver";
			break;
			case 0x00000002:
			m_strDwServiceType = "file system driver";
			break;
			case 0x00000010:
			m_strDwServiceType = "runs in its own process";
			break;
			case 0x00000020:
			m_strDwServiceType = "The service shares a process with other services";
			break;
			default:
			break;
		}
		m_listService.SetItemText ( i, 2, m_strDwServiceType );

		//6.获取服务的详细信息 这里将只给出函数
		//打开服务
		SC_HANDLE hService = OpenService ( hSCM,//服务控制管理器的句柄
										   pEnumSerice[i].lpServiceName,//服务名
										   SERVICE_QUERY_CONFIG//点开权限
										   );
		CString tempHandle;
		tempHandle.Format ( L"%x", hService );
		m_listService.SetItemText ( i, 5, tempHandle );
		////第一次调用获取需要的缓冲区大小
		QueryServiceConfig ( hService, NULL, 0, &dwSize );
		////分配内存
		LPQUERY_SERVICE_CONFIG pServiceConfig = \
			(LPQUERY_SERVICE_CONFIG)new char[dwSize];
		////第二次调用,获取信息
		QueryServiceConfig ( hService, pServiceConfig, dwSize, &dwSize );
		//  
		////通过上面获取到的结构体信息具体得到需要的值

		//四.获取启动类型
		////类型有:"自启动"."手动启动""已禁用"
		DWORD m_dwStartType = pServiceConfig->dwStartType;
		CStringW m_strDwStartType = nullptr;
		switch( m_dwStartType )
		{
			case 0x00000000:
			m_strDwStartType = L"加载后启动";
			break;
			case 0x00000001:
			m_strDwStartType = L"随系统启动";
			break;
			case 0x00000002:
			m_strDwStartType = L"自启动 ";
			break;
			case 0x00000003:
			m_strDwStartType = "手动启动";
			break;
			case 0x00000004:
			m_strDwStartType = "禁用";
			break;
			default:
			break;
		}
		m_listService.SetItemText ( i, 3, m_strDwStartType );
		//五.获取路径信息
		m_listService.SetItemText ( i, 4, pServiceConfig->lpBinaryPathName );
	}
	// 		m_listService.InsertItem(i++, processInfo.szExeFile);

}

// CString strCurrentService;
//列表框右键响应事件
void CDiaService::OnRclickListservice ( NMHDR *pNMHDR, LRESULT *pResult )
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
	// 	int nCol = pNMItemActivate->iSubItem;
	int nRow = pNMItemActivate->iItem;
	// 	strCurrentService = m_listService.GetItemText(nRow,1);
	// 	currentHandle = (SC_HANDLE)_wtoi(m_listService.GetItemText(nRow,5));
	SC_HANDLE currentHandle =
		SC_HANDLE ( pNMItemActivate->hdr.hwndFrom );
	// 	m_listService.GetItemText(nRow, 1);
	// 	currentService = (DWORD)_wtoi(str);
		//将CString转换DWORD
	// 	pNMItemActivate->iItem;
		// 	m_list.GetItemText();
	CMenu mu;
	// 	my-把整个菜单资源加载进来
	mu.LoadMenuW ( IDR_MENUSERVICE );
	// 获取"静态"菜单的句柄
	CMenu *pSub = mu.GetSubMenu ( 0 );
	CPoint pt;
	GetCursorPos ( &pt );
	// 	//my-获取当前坐标位置
	UINT unMenuId = 0;
	// 	unMenuId = TrackPopupMenu(pSub->m_hMenu, TPM_LEFTALIGN, pt.x, pt.y, 0, m_hWnd, NULL);
	// 	TPM_RETURNCMD如果有这个参数返回值会显示当前点击位置信息
	unMenuId = TrackPopupMenu ( pSub->m_hMenu, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RETURNCMD, pt.x, pt.y, 0, m_hWnd, NULL );
	//my-			 menu句柄		对齐方式		坐标位置		父窗口句柄
	switch( unMenuId )
	{
		case ID_POPUP_32773:
		{
// 			PromotePower ();

			SERVICE_STATUS status;
			CString strPid = m_listService.GetItemText ( pNMItemActivate->iItem, 0 );
			// 			WCHAR *pEnd = L"";
			// 			SC_HANDLE dwPid = (SC_HANDLE)wcstol(strPid.GetBuffer(), &pEnd, 16);
						///////////////////////
			SC_HANDLE schSCManager = OpenSCManager ( NULL, NULL, SC_MANAGER_ALL_ACCESS );
			// 打开服务控制管理数据库，并返回服务控制管理数据库的句柄
			if( schSCManager == NULL )
			{
				return;
			}
			// 			SC_HANDLE schService = OpenService(schSCManager, strPid,
			// 											   SERVICE_ALL_ACCESS | SERVICE_Delte );    // 获得服务句柄
			SC_HANDLE schService = OpenService ( schSCManager, 
												 strPid,
												 SERVICE_START );    // 获得服务句柄

			if( schService == NULL )
			{
				return;
			}
			QueryServiceStatus ( schService, &status );   // 获得服务的当前状态
			if( status.dwCurrentState = SERVICE_STOPPED )   // 如果服务处于停止状态，则将其状态设置为启动
														   //状态
				StartService ( schService, 0, NULL );   //启动服务
// 			CloseServiceHandle(schSCManager);   // 关闭服务句柄
// 			CloseServiceHandle(schService);

			ShowServicesStatus ();

			/*三个参数分别表示
						要进行扫描字符串指针
						[out] endptr  存储扫描后无法转化的剩余的字符串
						[base]  表示数字的基，取值为：2，8，10，16  对应的进制
			*/
			/*
						//	//打开一个现有的服务
						BOOL ret = StartService(dwPid, NULL, NULL);
						if (!ret)
						{
							WCHAR szFailInfo[100];
							_stprintf_s(szFailInfo, L"StartService serviceHandle=%p -> fail(%ld)\n", dwPid, GetLastError());
							//〖1056〗-服务的实例已在运行中。
							OutputDebugString(szFailInfo);
						}
						else
						{
							WCHAR szSuccInfo[100];
							_stprintf_s(szSuccInfo, L"StartService serviceHandle=%p -> succ\n", dwPid);
							OutputDebugString(szSuccInfo);

							SERVICE_STATUS serviceStatus = { 0 };
							do
							{
								ret = QueryServiceStatus(dwPid, &serviceStatus);
								//休眠(挂起线程),单位毫秒
								Sleep(300);
							} while (ret && serviceStatus.dwCurrentState != SERVICE_RUNNING);
			*/
			break;
		}
		case ID_POPUP_32774:
		{
// 			PromotePower ();

			SERVICE_STATUS status;
			CString strPid = m_listService.GetItemText ( pNMItemActivate->iItem, 0 );

			SC_HANDLE schSCManager = OpenSCManager ( NULL, NULL, SC_MANAGER_ALL_ACCESS );   // 打开服务控制管
																						 //理数据库，并返回服务控制管理数据库的句柄
			if( schSCManager == NULL )
			{
				return;
			}
			SC_HANDLE schService = OpenService ( schSCManager, strPid,
												 SERVICE_ALL_ACCESS | DELETE );   // 获得服务句柄
			if( schService == NULL )
			{
				return;
			}
			QueryServiceStatus ( schService, &status );  // 获得服务的当前状态
			if( status.dwCurrentState != SERVICE_STOPPED )   // 如果服务不处于停止状态，则将其状态设置为
															//停止状态
				ControlService ( schService, SERVICE_CONTROL_STOP, &status );
			DeleteService ( schService );    // 删除服务
			CloseServiceHandle ( schSCManager );  // 关闭服务句柄
			CloseServiceHandle ( schService );

			ShowServicesStatus ();
			break;
		}
	}
	*pResult = 0;
}

//右键开启服务
//void CDiaService::OnPopupStartService()
//{
//	// TODO:  在此添加命令处理程序代码
//	//打开一个现有的服务
// 	SC_HANDLE hser = OpenService(
// 		currentHandle,								//服务控制管理器数据库的处理。这个处理OpenSCManager函数返回。
// 		getServiceName,							//服务名称
// 		SC_MANAGER_ALL_ACCESS					//访问权限
// 		);
//	BOOL ret = StartService(currentHandle, NULL, NULL);
//	if (!ret)
//	{
//		WCHAR szFailInfo[100];
//		_stprintf_s(szFailInfo, L"StartService serviceHandle=%p -> fail(%ld)\n", currentHandle, GetLastError());
//		//〖1056〗-服务的实例已在运行中。
//		OutputDebugString(szFailInfo);
//	}
//	else
//	{
//		WCHAR szSuccInfo[100];
//		_stprintf_s(szSuccInfo, L"StartService serviceHandle=%p -> succ\n", currentHandle);
//		OutputDebugString(szSuccInfo);
//
//		SERVICE_STATUS serviceStatus = { 0 };
//		do
//		{
//			ret = QueryServiceStatus(currentHandle, &serviceStatus);
//			//休眠(挂起线程),单位毫秒
//			Sleep(300);
//		} while (ret && serviceStatus.dwCurrentState != SERVICE_RUNNING);
//	}
//	ShowServicesStatus();
//
//}

//右键关闭服务
//void CDiaService::OnPopupShutService()
//{
//	// TODO:  在此添加命令处理程序代码
//	DWORD dwControl = SERVICE_CONTROL_STOP;
//	SERVICE_STATUS serviceStatus = { 0 };
//
//	bool ret = ControlService(currentHandle, dwControl, &serviceStatus);
//	//〖1052〗-请求的控件对此服务无效。〖1062〗-服务未启动。
//	//The CloseServiceHandle function does not destroy the service control manager object referred to by the handle. A service control manager object cannot be destroyed. A service object can be destroyed by calling the DeleteService function.
//	CloseServiceHandle(currentHandle);
//	DeleteService(currentHandle);
//	ShowServicesStatus();
//
//}
