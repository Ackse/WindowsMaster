// DialogSection.cpp : 实现文件
//

#include "stdafx.h"
#include "windows安全卫士.h"
#include "DialogSection.h"
#include "afxdialogex.h"
#include "DiaPeAnaly.h"
// #define CHAR_TO_WCHAR(lpChar,lpW_Char)\
// 	MultiByteToWideChar(CP_ACP,NULL,lpChar,-1,lpW_Char,_countof(lpW_Char));


// CDialogSection 对话框
//区段表

IMPLEMENT_DYNAMIC(CDialogSection, CDialogEx)

CDialogSection::CDialogSection(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogSection::IDD, pParent)
{

}

CDialogSection::~CDialogSection()
{
}

void CDialogSection::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listSection);
}


BEGIN_MESSAGE_MAP(CDialogSection, CDialogEx)
END_MESSAGE_MAP()


// CDialogSection 消息处理程序


//BOOL CDialogSection::DestroyWindow()
//{
//	// TODO:  在此添加专用代码和/或调用基类
//
//	return CDialogEx::DestroyWindow();
//}


BOOL CDialogSection::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_listSection.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	m_listSection.InsertColumn(0, L"区段名", 0, 80);
	m_listSection.InsertColumn(1, L"区段数据的文件偏移", 0, 80);
	m_listSection.InsertColumn(2, L"区段数据的RVA：", 0, 80);
	m_listSection.InsertColumn(3, L"区段数据的大小", 0, 80);

	// 得到区段头表的个数
	DWORD dwNumberOfSection = CDiaPeAnaly::pNtHeader->FileHeader.NumberOfSections;
	//上面numberOfSection已经给出
	// 得到区段头表的首地址
	IMAGE_SECTION_HEADER* pSectionHeader = IMAGE_FIRST_SECTION( CDiaPeAnaly::pNtHeader );
	// 	PIMAGE_SECTION_HEADER pSectionHeader = IMAGE_FIRST_SECTION(pNtHeader);

	for (int i = 0; i < dwNumberOfSection; i++)
	{
		//BYTE转为char字符
		char tempCharName[9] = {};
		memcpy_s(tempCharName, 9, pSectionHeader[i].Name, 8);
		//单字符转宽字符
		int nLen = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, tempCharName, -1, NULL, 0);
		if (nLen==0)
		{
			break;
		}
		wchar_t *pResult = new wchar_t[nLen];
		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, tempCharName, -1, pResult, nLen);
 
		CString tempPointerToRawData, tempVirtualAddress, tempSizeOfRawData;
		tempPointerToRawData.Format(L"%x", pSectionHeader[i].PointerToRawData);
		tempVirtualAddress.Format(L"%x", pSectionHeader[i].VirtualAddress);
		tempSizeOfRawData.Format(L"%x", pSectionHeader[i].SizeOfRawData);
// 		m_listSection.InsertItem(i, pSectionHeader[i].Name);//unsigned char[]

		m_listSection.InsertItem(i, pResult);//unsigned char[]
		m_listSection.SetItemText(i, 1, tempPointerToRawData);
		m_listSection.SetItemText(i, 2, tempVirtualAddress);
		m_listSection.SetItemText(i, 3, tempSizeOfRawData);

	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}
