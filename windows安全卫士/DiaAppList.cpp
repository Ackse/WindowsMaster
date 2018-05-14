// DiaApplist.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "windows��ȫ��ʿ.h"
#include "DiaApplist.h"
#include "afxdialogex.h"


// CDiaApplist �Ի���
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


// CDiaApplist ��Ϣ�������

BOOL CDiaApplist::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_listAppCtrl.SetExtendedStyle ( LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT );
	// ����б���
	m_listAppCtrl.InsertColumn(0, L"����", 0, 300);
	m_listAppCtrl.InsertColumn ( 1, L"����", 0, 300 );

	EnumWindows(WndEnumProc, (LPARAM)this);
	//my-----�ص����������Ǿ�̬����,���ǰ��������������cppҲ������.��??
	m_listAppCtrl.SetExtendedStyle ( LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT );

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

BOOL CALLBACK CDiaApplist::WndEnumProc(HWND hWnd, LPARAM lParam)
{
	// 	CHAR szWindowName[MAX_PATH];
	// 	CHAR szClassName[MAX_PATH];
	CDiaApplist *pThis = (CDiaApplist*)lParam;
	WCHAR szWindowName[MAX_PATH];
	WCHAR szClassName[MAX_PATH];

	//�жϱ������Ĵ����Ƿ�����
	BOOL IsWinVis =
		::IsWindowVisible(hWnd);//����Ϊ�ص�������һ������
/*
	if( ::GetWindowLong ( hWnd, GWL_STYLE )& WS_VISIBLE )
	{
		::GetWindowText ( hWnd, m_strTitle, MAX_PATH - 1 );
		::GetClassName ( hWnd, m_strClass, MAX_PATH - 1 );
	}
*/

	if (IsWinVis)
	{
		//��ȡ��������
		// 	GetWindowTextA(hWnd, szWindowName, MAXBYTE);
		::GetWindowText ( hWnd, szWindowName, MAXBYTE );
		if( _tcslen ( szWindowName ) == 0 )
		{
			return TRUE;
		}

		//��ȡ��������
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
	return TRUE;//����true�����������һ������,����FALSE����������
}