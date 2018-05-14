// DialogDataDirection.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "windows��ȫ��ʿ.h"
#include "DialogDataDirection.h"
#include "afxdialogex.h"
#include "DiaPeAnaly.h"
#include "DialogExport.h"
#include "DialogImport.h"

// CDialogDataDirection �Ի���
//Ŀ¼��
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


// CDialogDataDirection ��Ϣ�������
// DWORD CDialogDataDirection::RvaToOffset(const void* pFileData, DWORD dwRva)
// {
// // 	IMAGE_DOS_HEADER *pDosHeader = (IMAGE_DOS_HEADER*)pFileData;
// 
// // 	IMAGE_NT_HEADERS *pNtHeader =   /*����*/
// // 		(IMAGE_NT_HEADERS*)((DWORD)pDosHeader + pDosHeader->e_lfanew);
// 
// 	// �õ����θ���
// 	DWORD   dwSectionNumber = CDiaPeAnaly::pNtHeader->FileHeader.NumberOfSections;
// 
// 	// �õ���һ������
// 	IMAGE_SECTION_HEADER* pSectionHeader = IMAGE_FIRST_SECTION(CDiaPeAnaly::pNtHeader);
// 
// 	// �������α��ҵ�RVA���ڵ�����
// 	/*
// 	* ÿ��ƫ�ƣ����������ļ��У��������ڴ��У����Ǿ������ο�ʼλ�õľ���
// 	* ������ȵġ�
// 	* ���ң����α��У�������������ʼƫ�ƣ�
// 	*  1. �ļ��еĿ�ʼƫ��
// 	*  2. �ڴ��еĿ�ʼƫ��
// 	* ������̣�
// 	*  �ҵ�RVA�������Σ� Ȼ���������RVA���������ڴ��еĿ�ʼλ�õľ��롣
// 	*  �������������������ļ��еĿ�ʼλ�þ͵õ��ļ�ƫ����
// 	*/
// 
// 	for (int i = 0; i < dwSectionNumber; ++i)
// 	{
// 
// 		// �ж�RVA�Ƿ��ڵ�ǰ��������
// 
// 		DWORD dwSectionEndRva =   /*����*/
// 			pSectionHeader[i].VirtualAddress + pSectionHeader[i].SizeOfRawData;
// 
// 		if (dwRva >= pSectionHeader[i].VirtualAddress
// 			&& dwRva <= dwSectionEndRva)
// 		{
// 
// 			// �����RVA��Ӧ���ļ�ƫ��
// 			// ��ʽ��
// 			// �ļ�ƫ��  =  RVA - ���ε���ʼRVA + ���ε���ʼ�ļ�ƫ��
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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	// ��ȡ��������С
	DWORD dwExportTableRva = CDiaPeAnaly::pDataDirectory[0].VirtualAddress;
	DWORD sizeOfExport = CDiaPeAnaly::pDataDirectory[0].Size;
	//1. ��ȡ�����
	// 1.1 ��ȡ������RVA����С
	DWORD   dwImportTableRva = CDiaPeAnaly::pDataDirectory[1].VirtualAddress;
	DWORD sizeOfImport = CDiaPeAnaly::pDataDirectory[1].Size;
	//��Դ��
	DWORD dwResourceTableRva = CDiaPeAnaly::pDataDirectory[2].VirtualAddress;
	DWORD sizeOfResource = CDiaPeAnaly::pDataDirectory[2].Size;
	//�ض�λ��
	DWORD dwBaseRelocTableRva = CDiaPeAnaly::pDataDirectory[5].VirtualAddress;
	DWORD sizeOfBaseReloc = CDiaPeAnaly::pDataDirectory[5].Size;
	//TLS��
	DWORD dwTLSTableRva = CDiaPeAnaly::pDataDirectory[9].VirtualAddress;
	DWORD sizeOfTLS = CDiaPeAnaly::pDataDirectory[9].Size;
	//�ӳټ��ر�
	DWORD dwDelayImportTableRva = CDiaPeAnaly::pDataDirectory[13].VirtualAddress;
	DWORD sizeOfDelayImport = CDiaPeAnaly::pDataDirectory[13].Size;

	//��ֵ���µ�����
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
	// �쳣:  OCX ����ҳӦ���� FALSE
}

