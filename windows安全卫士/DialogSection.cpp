// DialogSection.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "windows��ȫ��ʿ.h"
#include "DialogSection.h"
#include "afxdialogex.h"
#include "DiaPeAnaly.h"
// #define CHAR_TO_WCHAR(lpChar,lpW_Char)\
// 	MultiByteToWideChar(CP_ACP,NULL,lpChar,-1,lpW_Char,_countof(lpW_Char));


// CDialogSection �Ի���
//���α�

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


// CDialogSection ��Ϣ�������


//BOOL CDialogSection::DestroyWindow()
//{
//	// TODO:  �ڴ����ר�ô����/����û���
//
//	return CDialogEx::DestroyWindow();
//}


BOOL CDialogSection::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_listSection.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	m_listSection.InsertColumn(0, L"������", 0, 80);
	m_listSection.InsertColumn(1, L"�������ݵ��ļ�ƫ��", 0, 80);
	m_listSection.InsertColumn(2, L"�������ݵ�RVA��", 0, 80);
	m_listSection.InsertColumn(3, L"�������ݵĴ�С", 0, 80);

	// �õ�����ͷ��ĸ���
	DWORD dwNumberOfSection = CDiaPeAnaly::pNtHeader->FileHeader.NumberOfSections;
	//����numberOfSection�Ѿ�����
	// �õ�����ͷ����׵�ַ
	IMAGE_SECTION_HEADER* pSectionHeader = IMAGE_FIRST_SECTION( CDiaPeAnaly::pNtHeader );
	// 	PIMAGE_SECTION_HEADER pSectionHeader = IMAGE_FIRST_SECTION(pNtHeader);

	for (int i = 0; i < dwNumberOfSection; i++)
	{
		//BYTEתΪchar�ַ�
		char tempCharName[9] = {};
		memcpy_s(tempCharName, 9, pSectionHeader[i].Name, 8);
		//���ַ�ת���ַ�
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
	// �쳣:  OCX ����ҳӦ���� FALSE
}
