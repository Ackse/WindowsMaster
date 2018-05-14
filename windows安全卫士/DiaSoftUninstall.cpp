// DiaSoftUninstall.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "windows��ȫ��ʿ.h"
#include "DiaSoftUninstall.h"
#include "afxdialogex.h"
#include <vector>
using std::vector;

// CDiaSoftUninstall �Ի���

IMPLEMENT_DYNAMIC ( CDiaSoftUninstall, CDialogEx )

CDiaSoftUninstall::CDiaSoftUninstall ( CWnd* pParent /*=NULL*/ )
	: CDialogEx ( CDiaSoftUninstall::IDD, pParent )
{

}

CDiaSoftUninstall::~CDiaSoftUninstall ()
{}

void CDiaSoftUninstall::DoDataExchange ( CDataExchange* pDX )
{
	CDialogEx::DoDataExchange ( pDX );
	DDX_Control ( pDX, IDC_LISTSOFTUNINSTALL, m_listSoftUninstall );
}


BEGIN_MESSAGE_MAP ( CDiaSoftUninstall, CDialogEx )
	ON_COMMAND ( ID_POPUP_32775, &CDiaSoftUninstall::OnPopupOpenUninstallPath )
	ON_NOTIFY ( NM_RCLICK, IDC_LISTSOFTUNINSTALL, &CDiaSoftUninstall::OnRclickListsoftuninstall )
END_MESSAGE_MAP ()


// CDiaSoftUninstall ��Ϣ�������

void CDiaSoftUninstall::FreshUninstallList ()
{
	m_listSoftUninstall.DeleteAllItems ();					//���list�ؼ�����

													//1.������չ���
	m_listSoftUninstall.SetExtendedStyle ( LVS_EX_FULLROWSELECT );
	//2.����б���
	WCHAR* szCol[] = { L"�����", L"ж��·��", };
	for( int i = 0; i < 2; i++ )
	{
		m_listSoftUninstall.InsertColumn ( i, szCol[i], 0, 350 );
	}

	SOFTINFO m_SoftInfo = { 0 };
	HKEY RootKey = HKEY_LOCAL_MACHINE;				//����
													//�Ӽ�����(x86);
	LPCTSTR lpSubKey = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall";
	HKEY hkResult = 0;								//���ս�Ҫ�򿪵ļ��ľ��

													//1.��һ���Ѿ����ڵ�ע����
	LONG lReturn = RegOpenKeyEx (
		RootKey,									//�������
		lpSubKey,									//�ӽ�·��
		0,											//��������0
		KEY_ENUMERATE_SUB_KEYS | KEY_QUERY_VALUE,	//ָ���򿪼������Ȩ��
		&hkResult );									//�Ӽ����
	DWORD dwIndex = 0;
	DWORD dwType;
	//2.ѭ������UninstallĿ¼�µ��Ӽ�
	while( 1 )
	{
		DWORD dwKeyLen = 255;
		WCHAR szNewKeyName[MAX_PATH] = {};			//ע���������
		LONG lReturn = RegEnumKeyEx (
			hkResult,								//�ӽ����
			dwIndex,								//ָ����ö�ټ����ӽ�����������0��ʼ
			szNewKeyName,							//ָ��һ���ڴ汣���ӽ�������
			&dwKeyLen,								//ָ��szKeyName�ڴ�Ĵ�С
			0,										//��������Ϊ0
			NULL,									//���ڻ�ȡ�ӽ�ֵ������
			NULL,									//ָ��һ���ڴ棬���ڻ�ȡ�ӽ�ֵ����
			NULL									//��һ������������ڴ��С
			);
		if( ERROR_NO_MORE_ITEMS == lReturn )
		{
			break;
		}
		//2.1ͨ���õ����ӽ���������Ϻϳ��µ��ӽ�·��
		WCHAR strMidReg[MAX_PATH] = {};
		swprintf_s ( strMidReg, L"%s%s%s", lpSubKey, L"\\", szNewKeyName );
		//2.2���µ��Ӽ�����ȡ����
		HKEY hkValueKey = 0;						//�ӽ����
		RegOpenKeyEx ( RootKey, strMidReg, 0, KEY_QUERY_VALUE, &hkValueKey );
		//2.3��ȡ��ֵ
		DWORD dwNameLen = 255;						//ָ��szBuffer�ڴ�Ĵ�С
													//��ȡ�������
		RegQueryValueEx (
			hkValueKey,
			L"DisplayName",
			0,
			&dwType,
			(LPBYTE)m_SoftInfo.szSoftName,
			&dwNameLen
			);
		dwNameLen = 255;							//���û�����¸�ֵ��һ�ν���ȡ������Ϣ
													//ж��·��
		RegQueryValueEx (
			hkValueKey,
			L"UninstallString",
			0,
			&dwType,
			(LPBYTE)m_SoftInfo.strSoftUniPath,
			&dwNameLen
			);
		m_listSoftUninstall.InsertItem ( dwIndex, m_SoftInfo.szSoftName );
		m_listSoftUninstall.SetItemText ( dwIndex, 1, m_SoftInfo.strSoftUniPath );
		dwNameLen = 255;
		dwIndex++;			//�ӽ�����
 // 		m_vectSoftInfo.push_back ( m_SoftInfo );
	}

}


BOOL CDiaSoftUninstall::OnInitDialog ()
{
	CDialogEx::OnInitDialog ();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	FreshUninstallList ();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}
//����ȫ�ֱ�����־��ǰ����һ�����·��ֵ
CString str;
//�б���Ҽ���Ӧж�ز˵�
void CDiaSoftUninstall::OnRclickListsoftuninstall ( NMHDR *pNMHDR, LRESULT *pResult )
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	int nCol = pNMItemActivate->iSubItem;
	int nRow = pNMItemActivate->iItem;
	str = m_listSoftUninstall.GetItemText ( nRow, 1 );
	// 	CString x = "123";
	// 	printf("%d", atoi(x.GetBuffer(x.GetLength())));
	//��CStringת��DWORD
	// 	pNMItemActivate->iItem;
	// 	m_list.GetItemText();
	CMenu mu;
	// 	my-�������˵���Դ���ؽ���
	mu.LoadMenuW ( IDR_MENUOPENPATH );
	// ��ȡ"��̬"�˵��ľ��
	CMenu *pSub = mu.GetSubMenu ( 0 );
	CPoint pt;
	GetCursorPos ( &pt );
	// 	//my-��ȡ��ǰ����λ��
	TrackPopupMenu ( pSub->m_hMenu, TPM_LEFTALIGN, pt.x, pt.y, 0, m_hWnd, NULL );
	//my-			 menu���		���뷽ʽ		����λ��		�����ھ��

	*pResult = 0;
}

//ж�����
void CDiaSoftUninstall::OnPopupOpenUninstallPath ()
{
	// TODO:  �ڴ���������������
	//ж�ض�Ӧ·���µ����
// 	L"C:\\Program Files (x86)\\2345Soft\\2345Explorer\\Uninstall.exe"

	ShellExecute ( NULL, L"open", str, NULL, NULL, SW_SHOW );
	FreshUninstallList ();
}
