// DiaSoftUninstall.cpp : 实现文件
//

#include "stdafx.h"
#include "windows安全卫士.h"
#include "DiaSoftUninstall.h"
#include "afxdialogex.h"
#include <vector>
using std::vector;

// CDiaSoftUninstall 对话框

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


// CDiaSoftUninstall 消息处理程序

void CDiaSoftUninstall::FreshUninstallList ()
{
	m_listSoftUninstall.DeleteAllItems ();					//清空list控件内容

													//1.设置扩展风格
	m_listSoftUninstall.SetExtendedStyle ( LVS_EX_FULLROWSELECT );
	//2.添加列标题
	WCHAR* szCol[] = { L"软件名", L"卸载路径", };
	for( int i = 0; i < 2; i++ )
	{
		m_listSoftUninstall.InsertColumn ( i, szCol[i], 0, 350 );
	}

	SOFTINFO m_SoftInfo = { 0 };
	HKEY RootKey = HKEY_LOCAL_MACHINE;				//主键
													//子键名称(x86);
	LPCTSTR lpSubKey = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall";
	HKEY hkResult = 0;								//接收将要打开的键的句柄

													//1.打开一个已经存在的注册表键
	LONG lReturn = RegOpenKeyEx (
		RootKey,									//根键句柄
		lpSubKey,									//子建路径
		0,											//保留必须0
		KEY_ENUMERATE_SUB_KEYS | KEY_QUERY_VALUE,	//指定打开键句柄的权限
		&hkResult );									//子键句柄
	DWORD dwIndex = 0;
	DWORD dwType;
	//2.循环遍历Uninstall目录下的子键
	while( 1 )
	{
		DWORD dwKeyLen = 255;
		WCHAR szNewKeyName[MAX_PATH] = {};			//注册表项名称
		LONG lReturn = RegEnumKeyEx (
			hkResult,								//子建句柄
			dwIndex,								//指定被枚举键下子建的索引，从0开始
			szNewKeyName,							//指向一块内存保存子建的名称
			&dwKeyLen,								//指向szKeyName内存的大小
			0,										//保留必须为0
			NULL,									//用于获取子建值得类型
			NULL,									//指向一块内存，用于获取子建值数据
			NULL									//上一个参数所向的内存大小
			);
		if( ERROR_NO_MORE_ITEMS == lReturn )
		{
			break;
		}
		//2.1通过得到的子建名重新组合合成新的子建路径
		WCHAR strMidReg[MAX_PATH] = {};
		swprintf_s ( strMidReg, L"%s%s%s", lpSubKey, L"\\", szNewKeyName );
		//2.2打开新的子键，获取其句柄
		HKEY hkValueKey = 0;						//子建句柄
		RegOpenKeyEx ( RootKey, strMidReg, 0, KEY_QUERY_VALUE, &hkValueKey );
		//2.3获取键值
		DWORD dwNameLen = 255;						//指向szBuffer内存的大小
													//获取软件名称
		RegQueryValueEx (
			hkValueKey,
			L"DisplayName",
			0,
			&dwType,
			(LPBYTE)m_SoftInfo.szSoftName,
			&dwNameLen
			);
		dwNameLen = 255;							//如果没有重新赋值下一次将获取不到信息
													//卸载路径
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
		dwIndex++;			//子建索引
 // 		m_vectSoftInfo.push_back ( m_SoftInfo );
	}

}


BOOL CDiaSoftUninstall::OnInitDialog ()
{
	CDialogEx::OnInitDialog ();

	// TODO:  在此添加额外的初始化
	FreshUninstallList ();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}
//设置全局变量标志当前鼠标右击处的路径值
CString str;
//列表框右键响应卸载菜单
void CDiaSoftUninstall::OnRclickListsoftuninstall ( NMHDR *pNMHDR, LRESULT *pResult )
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	int nCol = pNMItemActivate->iSubItem;
	int nRow = pNMItemActivate->iItem;
	str = m_listSoftUninstall.GetItemText ( nRow, 1 );
	// 	CString x = "123";
	// 	printf("%d", atoi(x.GetBuffer(x.GetLength())));
	//将CString转换DWORD
	// 	pNMItemActivate->iItem;
	// 	m_list.GetItemText();
	CMenu mu;
	// 	my-把整个菜单资源加载进来
	mu.LoadMenuW ( IDR_MENUOPENPATH );
	// 获取"静态"菜单的句柄
	CMenu *pSub = mu.GetSubMenu ( 0 );
	CPoint pt;
	GetCursorPos ( &pt );
	// 	//my-获取当前坐标位置
	TrackPopupMenu ( pSub->m_hMenu, TPM_LEFTALIGN, pt.x, pt.y, 0, m_hWnd, NULL );
	//my-			 menu句柄		对齐方式		坐标位置		父窗口句柄

	*pResult = 0;
}

//卸载软件
void CDiaSoftUninstall::OnPopupOpenUninstallPath ()
{
	// TODO:  在此添加命令处理程序代码
	//卸载对应路径下的软件
// 	L"C:\\Program Files (x86)\\2345Soft\\2345Explorer\\Uninstall.exe"

	ShellExecute ( NULL, L"open", str, NULL, NULL, SW_SHOW );
	FreshUninstallList ();
}
