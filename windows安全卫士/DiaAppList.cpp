// DiaApplist.cpp : 实现文件
//

#include "stdafx.h"
#include "windows安全卫士.h"
#include "DiaApplist.h"
#include "afxdialogex.h"


// CDiaApplist 对话框
int CDiaApplist::countOfApp = 0;

IMPLEMENT_DYNAMIC(CDiaApplist, CDialogEx)

CDiaApplist::CDiaApplist(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDiaApplist::IDD, pParent)
{

}

CDiaApplist::~CDiaApplist()
{
}

void CDiaApplist::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listAppCtrl);
}


BEGIN_MESSAGE_MAP(CDiaApplist, CDialogEx)
END_MESSAGE_MAP()


// CDiaApplist 消息处理程序

BOOL CDiaApplist::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_listAppCtrl.SetExtendedStyle ( LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT );
	// 添加列标题
	m_listAppCtrl.InsertColumn(0, L"任务", 0, 300);
	m_listAppCtrl.InsertColumn ( 1, L"类名", 0, 300 );

	EnumWindows(WndEnumProc, (LPARAM)this);
	//my-----回调函数必须是静态函数,若是把这个函数定义在cpp也不报错.是??
	m_listAppCtrl.SetExtendedStyle ( LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT );

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

BOOL CALLBACK CDiaApplist::WndEnumProc(HWND hWnd, LPARAM lParam)
{
	// 	CHAR szWindowName[MAX_PATH];
	// 	CHAR szClassName[MAX_PATH];
	CDiaApplist *pThis = (CDiaApplist*)lParam;
	WCHAR szWindowName[MAX_PATH];
	WCHAR szClassName[MAX_PATH];

	//判断遍历到的窗口是否被隐藏
	BOOL IsWinVis =
		::IsWindowVisible(hWnd);//参数为回调函数第一个参数
/*
	if( ::GetWindowLong ( hWnd, GWL_STYLE )& WS_VISIBLE )
	{
		::GetWindowText ( hWnd, m_strTitle, MAX_PATH - 1 );
		::GetClassName ( hWnd, m_strClass, MAX_PATH - 1 );
	}
*/

	if (IsWinVis)
	{
		//获取窗口名称
		// 	GetWindowTextA(hWnd, szWindowName, MAXBYTE);
		::GetWindowText ( hWnd, szWindowName, MAXBYTE );
		if( _tcslen ( szWindowName ) == 0 )
		{
			return TRUE;
		}

		//获取窗口类名
		// 	GetClassNameA(hWnd, szClassName, MAXBYTE);
		GetClassName ( hWnd, szClassName, MAXBYTE );
// 		wcscpy_s ( m_strDisplay, szWindowName );
// 		wcscat_s ( m_strDisplay, L":[" );
// 		wcscat_s ( m_strDisplay, szClassName );
// 		wcscat_s ( m_strDisplay, L"\n" );
// 		OutputDebugString ( m_strDisplay );
		pThis->m_listAppCtrl.InsertItem ( CDiaApplist::countOfApp, szWindowName );
		pThis->m_listAppCtrl.SetItemText ( CDiaApplist::countOfApp,1, szClassName );

		CDiaApplist::countOfApp++;
	}
	return TRUE;//返回true会继续遍历下一个窗口,返回FALSE将结束遍历
}