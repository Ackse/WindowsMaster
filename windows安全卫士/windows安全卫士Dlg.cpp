
// windows安全卫士Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "windows安全卫士.h"
#include "windows安全卫士Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cwindows安全卫士Dlg 对话框



Cwindows安全卫士Dlg::Cwindows安全卫士Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Cwindows安全卫士Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cwindows安全卫士Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tab);
}

BEGIN_MESSAGE_MAP(Cwindows安全卫士Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// Cwindows安全卫士Dlg 消息处理程序

BOOL Cwindows安全卫士Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	

	// TODO:  在此添加额外的初始化代码
	//注册全局热键
	RegisterHotKey(GetSafeHwnd(), WM_MYHOTKEY, MOD_CONTROL | MOD_ALT, 'X');

	m_tab.InsertItem(0, L"内核信息");
	m_tab.InsertItem(1, L"性能优化");
	m_tab.InsertItem(2, L"文件清理");
	m_tab.InsertItem(3, L"服务项");
	m_tab.InsertItem(4, L"杀毒功能");
	m_tab.InsertItem(5, L"桌面窗口");
	m_tab.InsertItem(6, L"PE文件解析");
	m_tab.InsertItem(7, L"软件卸载");

	m_tab.InitDlg();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void Cwindows安全卫士Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR Cwindows安全卫士Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


BOOL m_IsWindowHide = TRUE;
//接受全局热键事件
BOOL Cwindows安全卫士Dlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  在此添加专用代码和/或调用基类
	if ((pMsg->message == WM_HOTKEY) && (pMsg->wParam == WM_MYHOTKEY))
	{
		if (m_IsWindowHide == TRUE)
		{
			ShowWindow(SW_HIDE);
			m_IsWindowHide = FALSE;
		}
		else
		{
			ShowWindow(SW_SHOW);
			m_IsWindowHide = TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
