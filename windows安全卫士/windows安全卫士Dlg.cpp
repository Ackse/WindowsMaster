
// windows��ȫ��ʿDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "windows��ȫ��ʿ.h"
#include "windows��ȫ��ʿDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cwindows��ȫ��ʿDlg �Ի���



Cwindows��ȫ��ʿDlg::Cwindows��ȫ��ʿDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Cwindows��ȫ��ʿDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cwindows��ȫ��ʿDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tab);
}

BEGIN_MESSAGE_MAP(Cwindows��ȫ��ʿDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// Cwindows��ȫ��ʿDlg ��Ϣ�������

BOOL Cwindows��ȫ��ʿDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
	

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	//ע��ȫ���ȼ�
	RegisterHotKey(GetSafeHwnd(), WM_MYHOTKEY, MOD_CONTROL | MOD_ALT, 'X');

	m_tab.InsertItem(0, L"�ں���Ϣ");
	m_tab.InsertItem(1, L"�����Ż�");
	m_tab.InsertItem(2, L"�ļ�����");
	m_tab.InsertItem(3, L"������");
	m_tab.InsertItem(4, L"ɱ������");
	m_tab.InsertItem(5, L"���洰��");
	m_tab.InsertItem(6, L"PE�ļ�����");
	m_tab.InsertItem(7, L"���ж��");

	m_tab.InitDlg();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void Cwindows��ȫ��ʿDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR Cwindows��ȫ��ʿDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


BOOL m_IsWindowHide = TRUE;
//����ȫ���ȼ��¼�
BOOL Cwindows��ȫ��ʿDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  �ڴ����ר�ô����/����û���
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
