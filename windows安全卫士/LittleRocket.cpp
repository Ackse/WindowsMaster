// LittleRocket.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "windows��ȫ��ʿ.h"
#include "LittleRocket.h"
#include "afxdialogex.h"
#include <psapi.h>

// CLittleRocket �Ի���

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

// CLittleRocket ��Ϣ�������

BOOL CLittleRocket::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	// ��ȡͼƬ��С��Ϣ
	m_bitmap.LoadBitmapW(IDB_BITMAP1);
	m_bitmap.GetBitmap(&m_bitinfo);
	// ���ô��ں�ͼƬһ����
	hDesktopWnd = ::GetDesktopWindow();
	CRect rt;
	::GetWindowRect(hDesktopWnd, &rt);
	rt.left = rt.right*0.85;
	rt.right = rt.left + m_bitinfo.bmWidth;
	rt.top = rt.bottom*0.65;
	rt.bottom = rt.top + m_bitinfo.bmHeight;
	MoveWindow(&rt);
	ShowWindow(SW_SHOWNORMAL);
	// ���ô��ڷ��Ϊ��͸��
	LONG lStyle = GetWindowLong(m_hWnd, GWL_EXSTYLE);
	SetWindowLong(m_hWnd, GWL_EXSTYLE, lStyle | WS_EX_LAYERED);
	// ����͸����
	SetLayeredWindowAttributes(RGB(27, 30, 40), 0, LWA_COLORKEY);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


void CLittleRocket::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: �ڴ˴������Ϣ����������
					   // ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);
	dcMem.SelectObject(m_bitmap);
	dc.BitBlt(0, 0, m_bitinfo.bmWidth, m_bitinfo.bmHeight, &dcMem, 0, 0, SRCCOPY);

}


void CLittleRocket::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	SendMessage(WM_SYSCOMMAND, SC_MOVE | HTCAPTION);

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CLittleRocket::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	DWORD dwPIDList[1000] = { 0 };
	DWORD bufSize = sizeof(dwPIDList);
	DWORD dwNeedSize = 0;
	EnumProcesses(dwPIDList, bufSize, &dwNeedSize);
	for (DWORD i = 0; i < dwNeedSize / sizeof(DWORD); i++)
	{
		HANDLE hProcess = OpenProcess(PROCESS_SET_QUOTA,
									  false, dwPIDList[i]);
		//���ý���ʹ�õ��ڴ�����
		SetProcessWorkingSetSize(hProcess, -1, -1);
		//���̾�� ������С���ڴ�����	���������ڴ�����
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
