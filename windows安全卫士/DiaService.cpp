// DiaService.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "windows��ȫ��ʿ.h"
#include "DiaService.h"
#include "afxdialogex.h"
#include <winsvc.h>

// CDiaService �Ի���

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


// CDiaService ��Ϣ�������

// void CDiaService::PromotePower ()
// {
// 	// TODO:  �ڴ���ӿؼ�֪ͨ����������
// 	// �Թ���Ա������б�����
// 	// ��ȡ�����̵��ļ�·��.
// 	WCHAR szApplication[MAX_PATH] = { 0 };			// ��Ҫ��ʼ��
// 	DWORD cchLength = _countof ( szApplication );
// 	QueryFullProcessImageName (
// 		GetCurrentProcess (),
// 		0,
// 		szApplication,
// 		&cchLength
// 		);
// 
// 	//5 �Թ���ԱȨ�����´򿪽���
// 	SHELLEXECUTEINFO sei = { sizeof ( SHELLEXECUTEINFO ) };
// 	sei.lpVerb = L"runas";				//��������Ȩ��
// 	sei.lpFile = szApplication;			//��ִ���ļ�·��
// 	sei.lpParameters = NULL;			//����Ҫ����
// 	sei.nShow = SW_SHOWNORMAL;			//������ʾ����
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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_listService.InsertColumn ( 0, L"������", 0, 120 );
	m_listService.InsertColumn ( 1, L"����״̬", 0, 60 );
	m_listService.InsertColumn ( 2, L"��������", 0, 60 );
	m_listService.InsertColumn ( 3, L"��������", 0, 60 );
	m_listService.InsertColumn ( 4, L"·��", 0, 200 );
	m_listService.InsertColumn ( 5, L"currentSC_HANDLE", 0, 200 );
	ShowServicesStatus ();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}
SC_HANDLE hSCM;
void CDiaService::ShowServicesStatus ()
{
	//1.��Զ�̼����������ƹ�����
	/*
	SC_HANDLE
	WINAPI
	OpenSCManagerW(
	_In_opt_   LPCWSTR  lpMachineName,//������,������Զ������������,�����NULL��򿪵��Ǳ�������
	_In_opt_   LPCWSTR  lpDatabaseName,//���ݿ���,Ӧ����ΪSERVICES_ACTIVE_DATABASE,��NULL���Զ����ǰ��ĺ�
	_In_       DWORD    dwDesiredAccess//����Ȩ��,
	SC_MANAGER_ALL_ACCESS:����Ȩ
	);
	*/

	hSCM = OpenSCManagerW ( NULL,
							NULL,
							SC_MANAGER_ALL_ACCESS );
	//2.��һ�ε���,��ȡ��Ҫ���ڴ��С
	DWORD dwServiceNum = 0;
	DWORD dwSize = 0;
	EnumServicesStatusEx ( hSCM,
						   SC_ENUM_PROCESS_INFO,
						   SERVICE_WIN32,
						   SERVICE_STATE_ALL,//���з���״̬
						   NULL,//������
						   0,//��������С
						   &dwSize,//��Ҫ�Ĵ�С
						   &dwServiceNum,//�������еķ������
						   NULL,
						   NULL
						   );
	//3.������Ҫ���ڴ�,�ڶ��ε���
	LPENUM_SERVICE_STATUS_PROCESS pEnumSerice =
		(LPENUM_SERVICE_STATUS_PROCESS)new char[dwSize];
	//4.�ڶ���ö��
	bool bStatus = FALSE;
	bStatus = EnumServicesStatusEx ( hSCM,
									 SC_ENUM_PROCESS_INFO,
									 SERVICE_WIN32,
									 SERVICE_STATE_ALL,//���з���״̬
									 (PBYTE)pEnumSerice,//������
									 dwSize,//��������С
									 &dwSize,//��Ҫ�Ĵ�С
									 &dwServiceNum,//�������еķ������
									 NULL, NULL
									 );
	// 	5.������Ϣ
	for( DWORD i = 0; i < dwServiceNum; i++ )
	{
		//��ȡ������Ϣ
		//һ.������		
		m_listService.InsertItem ( i, pEnumSerice[i].lpServiceName );
		//��.����״̬ �� �Ѿ�ֹͣ,��������,������ͣ
		//(���ݵõ���ֵ�ֶ�����ַ���,����ֵ�ĺ�������ڸýṹ���ϰ�F1��MSDN)	
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
		//��.��������
		//���ļ�ϵͳ��������,��������,�������̷���
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

		//6.��ȡ�������ϸ��Ϣ ���ｫֻ��������
		//�򿪷���
		SC_HANDLE hService = OpenService ( hSCM,//������ƹ������ľ��
										   pEnumSerice[i].lpServiceName,//������
										   SERVICE_QUERY_CONFIG//�㿪Ȩ��
										   );
		CString tempHandle;
		tempHandle.Format ( L"%x", hService );
		m_listService.SetItemText ( i, 5, tempHandle );
		////��һ�ε��û�ȡ��Ҫ�Ļ�������С
		QueryServiceConfig ( hService, NULL, 0, &dwSize );
		////�����ڴ�
		LPQUERY_SERVICE_CONFIG pServiceConfig = \
			(LPQUERY_SERVICE_CONFIG)new char[dwSize];
		////�ڶ��ε���,��ȡ��Ϣ
		QueryServiceConfig ( hService, pServiceConfig, dwSize, &dwSize );
		//  
		////ͨ�������ȡ���Ľṹ����Ϣ����õ���Ҫ��ֵ

		//��.��ȡ��������
		////������:"������"."�ֶ�����""�ѽ���"
		DWORD m_dwStartType = pServiceConfig->dwStartType;
		CStringW m_strDwStartType = nullptr;
		switch( m_dwStartType )
		{
			case 0x00000000:
			m_strDwStartType = L"���غ�����";
			break;
			case 0x00000001:
			m_strDwStartType = L"��ϵͳ����";
			break;
			case 0x00000002:
			m_strDwStartType = L"������ ";
			break;
			case 0x00000003:
			m_strDwStartType = "�ֶ�����";
			break;
			case 0x00000004:
			m_strDwStartType = "����";
			break;
			default:
			break;
		}
		m_listService.SetItemText ( i, 3, m_strDwStartType );
		//��.��ȡ·����Ϣ
		m_listService.SetItemText ( i, 4, pServiceConfig->lpBinaryPathName );
	}
	// 		m_listService.InsertItem(i++, processInfo.szExeFile);

}

// CString strCurrentService;
//�б���Ҽ���Ӧ�¼�
void CDiaService::OnRclickListservice ( NMHDR *pNMHDR, LRESULT *pResult )
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	// 	int nCol = pNMItemActivate->iSubItem;
	int nRow = pNMItemActivate->iItem;
	// 	strCurrentService = m_listService.GetItemText(nRow,1);
	// 	currentHandle = (SC_HANDLE)_wtoi(m_listService.GetItemText(nRow,5));
	SC_HANDLE currentHandle =
		SC_HANDLE ( pNMItemActivate->hdr.hwndFrom );
	// 	m_listService.GetItemText(nRow, 1);
	// 	currentService = (DWORD)_wtoi(str);
		//��CStringת��DWORD
	// 	pNMItemActivate->iItem;
		// 	m_list.GetItemText();
	CMenu mu;
	// 	my-�������˵���Դ���ؽ���
	mu.LoadMenuW ( IDR_MENUSERVICE );
	// ��ȡ"��̬"�˵��ľ��
	CMenu *pSub = mu.GetSubMenu ( 0 );
	CPoint pt;
	GetCursorPos ( &pt );
	// 	//my-��ȡ��ǰ����λ��
	UINT unMenuId = 0;
	// 	unMenuId = TrackPopupMenu(pSub->m_hMenu, TPM_LEFTALIGN, pt.x, pt.y, 0, m_hWnd, NULL);
	// 	TPM_RETURNCMD����������������ֵ����ʾ��ǰ���λ����Ϣ
	unMenuId = TrackPopupMenu ( pSub->m_hMenu, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RETURNCMD, pt.x, pt.y, 0, m_hWnd, NULL );
	//my-			 menu���		���뷽ʽ		����λ��		�����ھ��
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
			// �򿪷�����ƹ������ݿ⣬�����ط�����ƹ������ݿ�ľ��
			if( schSCManager == NULL )
			{
				return;
			}
			// 			SC_HANDLE schService = OpenService(schSCManager, strPid,
			// 											   SERVICE_ALL_ACCESS | SERVICE_Delte );    // ��÷�����
			SC_HANDLE schService = OpenService ( schSCManager, 
												 strPid,
												 SERVICE_START );    // ��÷�����

			if( schService == NULL )
			{
				return;
			}
			QueryServiceStatus ( schService, &status );   // ��÷���ĵ�ǰ״̬
			if( status.dwCurrentState = SERVICE_STOPPED )   // ���������ֹͣ״̬������״̬����Ϊ����
														   //״̬
				StartService ( schService, 0, NULL );   //��������
// 			CloseServiceHandle(schSCManager);   // �رշ�����
// 			CloseServiceHandle(schService);

			ShowServicesStatus ();

			/*���������ֱ��ʾ
						Ҫ����ɨ���ַ���ָ��
						[out] endptr  �洢ɨ����޷�ת����ʣ����ַ���
						[base]  ��ʾ���ֵĻ���ȡֵΪ��2��8��10��16  ��Ӧ�Ľ���
			*/
			/*
						//	//��һ�����еķ���
						BOOL ret = StartService(dwPid, NULL, NULL);
						if (!ret)
						{
							WCHAR szFailInfo[100];
							_stprintf_s(szFailInfo, L"StartService serviceHandle=%p -> fail(%ld)\n", dwPid, GetLastError());
							//��1056��-�����ʵ�����������С�
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
								//����(�����߳�),��λ����
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

			SC_HANDLE schSCManager = OpenSCManager ( NULL, NULL, SC_MANAGER_ALL_ACCESS );   // �򿪷�����ƹ�
																						 //�����ݿ⣬�����ط�����ƹ������ݿ�ľ��
			if( schSCManager == NULL )
			{
				return;
			}
			SC_HANDLE schService = OpenService ( schSCManager, strPid,
												 SERVICE_ALL_ACCESS | DELETE );   // ��÷�����
			if( schService == NULL )
			{
				return;
			}
			QueryServiceStatus ( schService, &status );  // ��÷���ĵ�ǰ״̬
			if( status.dwCurrentState != SERVICE_STOPPED )   // ������񲻴���ֹͣ״̬������״̬����Ϊ
															//ֹͣ״̬
				ControlService ( schService, SERVICE_CONTROL_STOP, &status );
			DeleteService ( schService );    // ɾ������
			CloseServiceHandle ( schSCManager );  // �رշ�����
			CloseServiceHandle ( schService );

			ShowServicesStatus ();
			break;
		}
	}
	*pResult = 0;
}

//�Ҽ���������
//void CDiaService::OnPopupStartService()
//{
//	// TODO:  �ڴ���������������
//	//��һ�����еķ���
// 	SC_HANDLE hser = OpenService(
// 		currentHandle,								//������ƹ��������ݿ�Ĵ����������OpenSCManager�������ء�
// 		getServiceName,							//��������
// 		SC_MANAGER_ALL_ACCESS					//����Ȩ��
// 		);
//	BOOL ret = StartService(currentHandle, NULL, NULL);
//	if (!ret)
//	{
//		WCHAR szFailInfo[100];
//		_stprintf_s(szFailInfo, L"StartService serviceHandle=%p -> fail(%ld)\n", currentHandle, GetLastError());
//		//��1056��-�����ʵ�����������С�
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
//			//����(�����߳�),��λ����
//			Sleep(300);
//		} while (ret && serviceStatus.dwCurrentState != SERVICE_RUNNING);
//	}
//	ShowServicesStatus();
//
//}

//�Ҽ��رշ���
//void CDiaService::OnPopupShutService()
//{
//	// TODO:  �ڴ���������������
//	DWORD dwControl = SERVICE_CONTROL_STOP;
//	SERVICE_STATUS serviceStatus = { 0 };
//
//	bool ret = ControlService(currentHandle, dwControl, &serviceStatus);
//	//��1052��-����Ŀؼ��Դ˷�����Ч����1062��-����δ������
//	//The CloseServiceHandle function does not destroy the service control manager object referred to by the handle. A service control manager object cannot be destroyed. A service object can be destroyed by calling the DeleteService function.
//	CloseServiceHandle(currentHandle);
//	DeleteService(currentHandle);
//	ShowServicesStatus();
//
//}
