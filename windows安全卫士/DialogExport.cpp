// DialogExport.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "windows��ȫ��ʿ.h"
#include "DialogExport.h"
#include "afxdialogex.h"


// CDialogExport �Ի���

IMPLEMENT_DYNAMIC(CDialogExport, CDialogEx)

CDialogExport::CDialogExport(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogExport::IDD, pParent)
{

}

CDialogExport::~CDialogExport()
{
}

void CDialogExport::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTEXPORT, m_listExport);
}


BEGIN_MESSAGE_MAP(CDialogExport, CDialogEx)
END_MESSAGE_MAP()


// CDialogExport ��Ϣ�������

DWORD  CDialogExport::RvaToOffset(const void* pFileData, DWORD dwRva)
{
	IMAGE_DOS_HEADER *pDosHeader = (IMAGE_DOS_HEADER*)pFileData;

	IMAGE_NT_HEADERS *pNtHeader =   /*����*/
		(IMAGE_NT_HEADERS*)((DWORD)pDosHeader + pDosHeader->e_lfanew);

	// �õ����θ���
	DWORD   dwSectionNumber = pNtHeader->FileHeader.NumberOfSections;

	// �õ���һ������
	IMAGE_SECTION_HEADER* pSectionHeader = IMAGE_FIRST_SECTION(pNtHeader);

	// �������α��ҵ�RVA���ڵ�����
	/*
	* ÿ��ƫ�ƣ����������ļ��У��������ڴ��У����Ǿ������ο�ʼλ�õľ���
	* ������ȵġ�
	* ���ң����α��У�������������ʼƫ�ƣ�
	*  1. �ļ��еĿ�ʼƫ��
	*  2. �ڴ��еĿ�ʼƫ��
	* ������̣�
	*  �ҵ�RVA�������Σ� Ȼ���������RVA���������ڴ��еĿ�ʼλ�õľ��롣
	*  �������������������ļ��еĿ�ʼλ�þ͵õ��ļ�ƫ����
	*/

	for (int i = 0; i < dwSectionNumber; ++i) {

		// �ж�RVA�Ƿ��ڵ�ǰ��������

		DWORD dwSectionEndRva =   /*����*/
			pSectionHeader[i].VirtualAddress + pSectionHeader[i].SizeOfRawData;

		if (dwRva >= pSectionHeader[i].VirtualAddress
			&& dwRva <= dwSectionEndRva) {

			// �����RVA��Ӧ���ļ�ƫ��
			// ��ʽ��
			// �ļ�ƫ��  =  RVA - ���ε���ʼRVA + ���ε���ʼ�ļ�ƫ��
			DWORD dwTemp = dwRva - pSectionHeader[i].VirtualAddress;
			DWORD dwOffset = dwTemp + pSectionHeader[i].PointerToRawData;

			return dwOffset;
		}
	}

	return -1;
}

BOOL CDialogExport::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_listExport.InsertColumn(0, L"�����", 0, 80);
	m_listExport.InsertColumn(1, L"��ַRVA", 0, 80);
	m_listExport.InsertColumn(2, L"��������", 0, 80);
// 	m_listExport.InsertColumn(3, L"�����(��/��)", 0, 80);
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
// 	// ��ȡ��չͷ
// 	IMAGE_OPTIONAL_HEADER* pOptionHeader = /*����*/
// 		(IMAGE_OPTIONAL_HEADER*)&CDiaPeAnaly::pNtHeader->OptionalHeader;
// 
// 	// ��ȡ������Ŀ¼��
// 	IMAGE_DATA_DIRECTORY* pDataDirectory = pOptionHeader->DataDirectory;

	// ��ȡ������
	DWORD dwExportTableRva = CDiaPeAnaly::pDataDirectory[0].VirtualAddress;
	// ���������RVAת�����ļ�ƫ��
	DWORD dwExportTableOffset = RvaToOffset( CDiaPeAnaly::pFileData, dwExportTableRva);

	// ��ȡ������
	IMAGE_EXPORT_DIRECTORY* pExportTable = /*����*/
		(IMAGE_EXPORT_DIRECTORY*)(dwExportTableOffset + (DWORD)CDiaPeAnaly::pFileData);

	// �õ�����DLL��ģ����
	// Name�ֶα������һ���ַ�����RVA����Ҫת�����ļ�ƫ��
	DWORD dwNameOffset = RvaToOffset( CDiaPeAnaly::pFileData, pExportTable->Name);
	char* pDllName = (char*)(dwNameOffset + (DWORD)CDiaPeAnaly::pFileData);



// 	HTREEITEM hTree = m_treeExport.InsertItem(pDllNameResult, TVI_ROOT, TVI_LAST);

	// �����еĵ����ĺ�����ַ��ӡ������
	// ���ң�����������Ƶ����������������
	// ���������ŵ��������������š�

	// ����ַ���RVAת���ļ�ƫ��
	DWORD dwAddressTableOffset = RvaToOffset( CDiaPeAnaly::pFileData,
		pExportTable->AddressOfFunctions);
	// �õ�������ַ��
	DWORD* pAddressTable = /*����*/
		(DWORD*)(dwAddressTableOffset + (DWORD)CDiaPeAnaly::pFileData);


	// �õ���ű�ĵ�ַ
	WORD* pOrdinalTable =
		(WORD*)((DWORD)CDiaPeAnaly::pFileData + RvaToOffset( CDiaPeAnaly::pFileData, pExportTable->AddressOfNameOrdinals));

	//�������Ʊ�ĵ�ַ
	DWORD* pNameTable =
		(DWORD*)((DWORD)CDiaPeAnaly::pFileData + RvaToOffset( CDiaPeAnaly::pFileData, pExportTable->AddressOfNames));

	BOOL bIndexIsExist = FALSE; // 

	for (int i = 0; i < pExportTable->NumberOfFunctions; ++i)
	{
		// ��ӡ��ַ

		CString emptyNumber, tempAddress;
		emptyNumber.Format(L"%d", i);
		tempAddress.Format(L"%08x", pAddressTable[i]);
		m_listExport.InsertItem(i, emptyNumber);
		m_listExport.SetItemText(i, 1, tempAddress);

		// �жϵ�ǰ�������ַ�Ƿ��������Ʒ�ʽ������
		// �ж����ݣ�
		//   ��ű�����ǵ�ַ���һ���±꣬����±��¼��
		//   ��ַ�����ĸ���ַ�������Ʒ�ʽ�����ġ�
		//   �����ǰ������±걣������ű��У���˵�������ַ
		//   ��һ�����Ʒ�ʽ�������������±�����ű��в����ڣ�
		//   ��˵���������ַ����һ�����Ʒ�ʽ����
		bIndexIsExist = FALSE;
		int nNameIndex = 0;
		for (; nNameIndex < pExportTable->NumberOfNames; ++nNameIndex)
		{
			// �жϵ�ַ����±��Ƿ��������ű���
			if (i == pOrdinalTable[nNameIndex])
			{
				bIndexIsExist = TRUE;
				break;
			}
		}

		if (bIndexIsExist == TRUE)
		{
			// �õ����Ʊ��е�RVA
			DWORD dwNameRva = pNameTable[nNameIndex];

			// ������Rvaת�����ļ�ƫ��
			char* pFunName =
				(char*)((DWORD)CDiaPeAnaly::pFileData + RvaToOffset( CDiaPeAnaly::pFileData, dwNameRva));
			int nLen = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, pFunName, -1, NULL, 0);
			if (nLen == 0)
			{
				MessageBox(L"��ʽת������!");
			}
			wchar_t* pDllNameResult = new wchar_t[nLen];

			MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, pFunName, -1, pDllNameResult, nLen);

			m_listExport.SetItemText(i, 2, pDllNameResult);
// 			printf(" ������:��%s��", pFunName);
		}
		else
		{
			// �жϵ�ַ��ǰ��������Ԭ���Ƿ񱣴��ŵ�ַ
			if (pAddressTable[i] != 0)
			{
				// i �� �ǵ�ַ���е������ţ�Ҳ����һ�������
				// ��������� = ����� + ��Ż���
				CString tempTrueNumber;
				tempTrueNumber.Format(L"%d", i + pExportTable->Base);
// 				printf(" ���:��%d��", i + pExportTable->Base);
				m_listExport.SetItemText(i, 3, tempTrueNumber);
			}
		}

	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}
