// LittleRocket.cpp : 实现文件
//

#include "stdafx.h"
#include "windows安全卫士.h"
#include "LittleRocket.h"
#include "afxdialogex.h"
#include <psapi.h>

// CLittleRocket 对话框

IMPLEMENT_DYNAMIC(CLittleRocket, CDialogEx)

CLittleRocket::CLittleRocket(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ROCKET, pParent)
{
}

CLittleRocket::~CLittleRocket()
{
}

void CLittleRocket::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CLittleRocket, CDialogEx)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// CLittleRocket 消息处理程序

BOOL CLittleRocket::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	// 获取图片大小信息
	m_bitmap.LoadBitmapW(IDB_BITMAP1);
	m_bitmap.GetBitmap(&m_bitinfo);
	// 设置窗口和图片一样大
	hDesktopWnd = ::GetDesktopWindow();
	CRect rt;
	::GetWindowRect(hDesktopWnd, &rt);
	rt.left = rt.right*0.85;
	rt.right = rt.left + m_bitinfo.bmWidth;
	rt.top = rt.bottom*0.65;
	rt.bottom = rt.top + m_bitinfo.bmHeight;
	MoveWindow(&rt);
	ShowWindow(SW_SHOWNORMAL);
	// 设置窗口风格为可透明
	LONG lStyle = GetWindowLong(m_hWnd, GWL_EXSTYLE);
	SetWindowLong(m_hWnd, GWL_EXSTYLE, lStyle | WS_EX_LAYERED);
	// 设置透明度
	SetLayeredWindowAttributes(RGB(27, 30, 40), 0, LWA_COLORKEY);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CLittleRocket::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CDialogEx::OnPaint()
	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);
	dcMem.SelectObject(m_bitmap);
	dc.BitBlt(0, 0, m_bitinfo.bmWidth, m_bitinfo.bmHeight, &dcMem, 0, 0, SRCCOPY);

}


void CLittleRocket::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	SendMessage(WM_SYSCOMMAND, SC_MOVE | HTCAPTION);

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CLittleRocket::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	DWORD dwPIDList[1000] = { 0 };
	DWORD bufSize = sizeof(dwPIDList);
	DWORD dwNeedSize = 0;
	EnumProcesses(dwPIDList, bufSize, &dwNeedSize);
	for (DWORD i = 0; i < dwNeedSize / sizeof(DWORD); i++)
	{
		HANDLE hProcess = OpenProcess(PROCESS_SET_QUOTA,
									  false, dwPIDList[i]);
		//设置进程使用的内存容量
		SetProcessWorkingSetSize(hProcess, -1, -1);
		//进程句柄 进程最小的内存容量	进程最大的内存容量
	}

	CRect tempRect,oldRect;
	GetWindowRect(&oldRect);
	tempRect = oldRect;
	for (int i = tempRect.bottom; i > (tempRect.bottom - tempRect.top); i--)
	{
		MoveWindow(&tempRect);
		tempRect.top -= 0.00000001;
		tempRect.bottom -= 0.00000001;
// 		Sleep(1);
	}
	MoveWindow(&oldRect);

	CDialogEx::OnLButtonUp(nFlags, point);
}
