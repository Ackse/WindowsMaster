// DialogDataDirection.cpp : 实现文件
//

#include "stdafx.h"
#include "windows安全卫士.h"
#include "DialogDataDirection.h"
#include "afxdialogex.h"
#include "DiaPeAnaly.h"
#include "DialogExport.h"
#include "DialogImport.h"

// CDialogDataDirection 对话框
//目录表
IMPLEMENT_DYNAMIC(CDialogDataDirection, CDialogEx)

CDialogDataDirection::CDialogDataDirection(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogDataDirection::IDD, pParent)
	, m_exportRVA(_T(""))
	, m_importRVA(_T(""))
	, m_resourceRVA(_T(""))
	, m_baseRelocRVA(_T(""))
	, m_TLSRVA(_T(""))
	, m_delayRVA(_T(""))
	, m_exportSize(_T(""))
	, m_importSize(_T(""))
	, m_resourceSize(_T(""))
	, m_baseRelocSize(_T(""))
	, m_TLSSize(_T(""))
	, m_delaySize(_T(""))
{

}

CDialogDataDirection::~CDialogDataDirection()
{
}

void CDialogDataDirection::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITEXPORTRVA, m_exportRVA);
	DDX_Text(pDX, IDC_EDITIMPORTRVA, m_importRVA);
	DDX_Text(pDX, IDC_EDITRESOURCERVA, m_resourceRVA);
	DDX_Text(pDX, IDC_EDITBASERELOCRVA, m_baseRelocRVA);
	DDX_Text(pDX, IDC_EDITTLSRVA, m_TLSRVA);
	DDX_Text(pDX, IDC_EDITDELAYRVA, m_delayRVA);
	DDX_Text(pDX, IDC_EDITEXPORTSIZE, m_exportSize);
	DDX_Text(pDX, IDC_EDITIMPORTSIZE, m_importSize);
	DDX_Text(pDX, IDC_EDITRESOURCESIZE, m_resourceSize);
	DDX_Text(pDX, IDC_EDITBASERELOCSIZE, m_baseRelocSize);
	DDX_Text(pDX, IDC_EDITTLSSIZE, m_TLSSize);
	DDX_Text(pDX, IDC_EDITDELAYSIZE, m_delaySize);
}


BEGIN_MESSAGE_MAP(CDialogDataDirection, CDialogEx)
END_MESSAGE_MAP()


// CDialogDataDirection 消息处理程序
// DWORD CDialogDataDirection::RvaToOffset(const void* pFileData, DWORD dwRva)
// {
// // 	IMAGE_DOS_HEADER *pDosHeader = (IMAGE_DOS_HEADER*)pFileData;
// 
// // 	IMAGE_NT_HEADERS *pNtHeader =   /*换行*/
// // 		(IMAGE_NT_HEADERS*)((DWORD)pDosHeader + pDosHeader->e_lfanew);
// 
// 	// 得到区段个数
// 	DWORD   dwSectionNumber = CDiaPeAnaly::pNtHeader->FileHeader.NumberOfSections;
// 
// 	// 得到第一个区段
// 	IMAGE_SECTION_HEADER* pSectionHeader = IMAGE_FIRST_SECTION(CDiaPeAnaly::pNtHeader);
// 
// 	// 遍历区段表，找到RVA所在的区段
// 	/*
// 	* 每个偏移，不管是在文件中，还是在内存中，它们距离区段开始位置的距离
// 	* 总是相等的。
// 	* 而且，区段表中，保存着两个开始偏移：
// 	*  1. 文件中的开始偏移
// 	*  2. 内存中的开始偏移
// 	* 具体过程：
// 	*  找到RVA所在区段， 然后计算出这个RVA到区段在内存中的开始位置的距离。
// 	*  用这个距离加上区段在文件中的开始位置就得到文件偏移了
// 	*/
// 
// 	for (int i = 0; i < dwSectionNumber; ++i)
// 	{
// 
// 		// 判断RVA是否在当前的区段中
// 
// 		DWORD dwSectionEndRva =   /*换行*/
// 			pSectionHeader[i].VirtualAddress + pSectionHeader[i].SizeOfRawData;
// 
// 		if (dwRva >= pSectionHeader[i].VirtualAddress
// 			&& dwRva <= dwSectionEndRva)
// 		{
// 
// 			// 计算出RVA对应的文件偏移
// 			// 公式：
// 			// 文件偏移  =  RVA - 区段的起始RVA + 区段的起始文件偏移
// 			DWORD dwTemp = dwRva - pSectionHeader[i].VirtualAddress;
// 			DWORD dwOffset = dwTemp + pSectionHeader[i].PointerToRawData;
// 
// 			return dwOffset;
// 		}
// 	}
// 
// 	return -1;
// }


BOOL CDialogDataDirection::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	// 获取导出表及大小
	DWORD dwExportTableRva = CDiaPeAnaly::pDataDirectory[0].VirtualAddress;
	DWORD sizeOfExport = CDiaPeAnaly::pDataDirectory[0].Size;
	//1. 获取导入表
	// 1.1 获取导入表的RVA及大小
	DWORD   dwImportTableRva = CDiaPeAnaly::pDataDirectory[1].VirtualAddress;
	DWORD sizeOfImport = CDiaPeAnaly::pDataDirectory[1].Size;
	//资源表
	DWORD dwResourceTableRva = CDiaPeAnaly::pDataDirectory[2].VirtualAddress;
	DWORD sizeOfResource = CDiaPeAnaly::pDataDirectory[2].Size;
	//重定位表
	DWORD dwBaseRelocTableRva = CDiaPeAnaly::pDataDirectory[5].VirtualAddress;
	DWORD sizeOfBaseReloc = CDiaPeAnaly::pDataDirectory[5].Size;
	//TLS表
	DWORD dwTLSTableRva = CDiaPeAnaly::pDataDirectory[9].VirtualAddress;
	DWORD sizeOfTLS = CDiaPeAnaly::pDataDirectory[9].Size;
	//延迟加载表
	DWORD dwDelayImportTableRva = CDiaPeAnaly::pDataDirectory[13].VirtualAddress;
	DWORD sizeOfDelayImport = CDiaPeAnaly::pDataDirectory[13].Size;

	//将值更新到框中
	m_exportRVA.Format(L"%x", dwExportTableRva);
	m_exportSize.Format(L"%x", sizeOfExport);

	m_importRVA.Format(L"%x", dwImportTableRva);
	m_importSize.Format(L"%x", sizeOfImport);

	m_resourceRVA.Format(L"%x", dwResourceTableRva);
	m_resourceSize.Format(L"%x", sizeOfResource);

	m_baseRelocRVA.Format(L"%x", dwBaseRelocTableRva);
	m_baseRelocSize.Format(L"%x", sizeOfBaseReloc);

	m_TLSRVA.Format(L"%x", dwTLSTableRva);
	m_TLSSize.Format(L"%x", sizeOfTLS);

	m_delayRVA.Format(L"%x", dwDelayImportTableRva);
	m_delaySize.Format(L"%x", sizeOfDelayImport);

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

