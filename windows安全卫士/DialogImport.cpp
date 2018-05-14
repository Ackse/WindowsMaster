// DialogImport.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "windows��ȫ��ʿ.h"
#include "DialogImport.h"
#include "afxdialogex.h"
#include "DiaPeAnaly.h"


// CDialogImport �Ի���

IMPLEMENT_DYNAMIC(CDialogImport, CDialogEx)

CDialogImport::CDialogImport(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDialogImport::IDD, pParent)
{

}

CDialogImport::~CDialogImport()
{
}

void CDialogImport::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREEIMPORT, m_treeImport);
}


BEGIN_MESSAGE_MAP(CDialogImport, CDialogEx)
END_MESSAGE_MAP()


// CDialogImport ��Ϣ�������


DWORD  CDialogImport::RvaToOffset(const void* pFileData, DWORD dwRva)
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

BOOL CDialogImport::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	// ��ȡ��չͷ
// 	IMAGE_OPTIONAL_HEADER* pOptionHeader = /*����*/
// 		(IMAGE_OPTIONAL_HEADER*)&CDiaPeAnaly::pNtHeader->OptionalHeader;
// 
// 	// ��ȡ������Ŀ¼��
// 	IMAGE_DATA_DIRECTORY* pDataDirectory = pOptionHeader->DataDirectory;
// 

	//1. ��ȡ�����
	// 1.1 ��ȡ������RVA
	DWORD   dwImportTableRva = CDiaPeAnaly::pDataDirectory[1].VirtualAddress;
	// 1.2 ��������RVAת�����ļ�ƫ��
	DWORD   dwImportTableOffset = RvaToOffset( CDiaPeAnaly::pFileData, dwImportTableRva);
	// 1.3 �������ṹ��ָ��ָ�򻺳����еĵ����
	IMAGE_IMPORT_DESCRIPTOR* pImportTable =
		(IMAGE_IMPORT_DESCRIPTOR*)((DWORD)CDiaPeAnaly::pFileData + dwImportTableOffset);

	//2. ����������
	//  ��Ϊһ��exe���ܻᵼ����DLL����ÿһ��Dll��Ӧ��һ�������
	//  ����������γ�һ��������
	//  �������������ȫ0��β(ȫ0��βָ���������ṹ�嶼��0)

	//2.1�ж��Ƿ�����������һ���ṹ�塣
	while (pImportTable->Name != 0)
	{
		//3. �����������Dll��ģ����
		//  3.1 ��Dllģ������RVAת�����ļ�ƫ��
		DWORD dwNameOffset = RvaToOffset( CDiaPeAnaly::pFileData, pImportTable->Name);
		//  3.2 ��һ��char*ָ��ָ��dll�����ڴ��е�λ��
		char* pDllName = (char*)(dwNameOffset + (DWORD)CDiaPeAnaly::pFileData);
// 		printf("[%s]\n", pDllName);

		int nLen = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, pDllName, -1, NULL, 0);
		if (nLen == 0)
		{
			MessageBox(L"��ʽת������!");
		}
		wchar_t* pDllNameResult = new wchar_t[nLen];

		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, pDllName, -1, pDllNameResult, nLen);

		HTREEITEM hTree = m_treeImport.InsertItem(pDllNameResult, TVI_ROOT, TVI_LAST);


		// 4. ������ǰdll�ĵ��뺯������
		// 4.1 �õ��������Ʊ�ĵ�ַ
		pImportTable->OriginalFirstThunk; // �������Ʊ�
		pImportTable->FirstThunk; // �����ַ��
		// ����˵�����������ļ��б������������ȫ��ͬ�ġ�

		DWORD dwIATOffset = RvaToOffset( CDiaPeAnaly::pFileData, pImportTable->FirstThunk);
		IMAGE_THUNK_DATA *pIAT = (IMAGE_THUNK_DATA *)((DWORD)CDiaPeAnaly::pFileData + dwIATOffset);

		// 4.2 ����IAT
		//  IAT��һ��IMAGE_THUNK_DATA�ṹ�����飬
		//  ������һ��ȫ0��β��
		while (pIAT->u1.AddressOfData != 0)
		{

			// 5. �жϵ���ĺ���������ʲô��ʽ�����
			//    ���������ŵ���ģ�������ṹ�����κ�һ���ֶ���߾���1��
			//    ����������Ʒ�ʽ����ģ�������ṹ���е��κ�һ���ֶα���
			//    ����һ��RVA �����RVA��һ��ָ��IMAGE_IMPORT_BY_NAME�ṹ�塣
			//    IMAGE_IMPORT_BY_NAME�У�������ǵ��뺯������ź����ơ�

			// 5.1 �ж����λ�Ƿ���1

			if ((pIAT->u1.Ordinal & 0x80000000) == 1)
			{
				// ����������ŵ���ġ�
				// ��Ŷ���WORD���ͣ����ԣ�ֻȡ����16��ֵ
// 				printf("������š�%d��\n", (pIAT->u1.Ordinal & 0xFFFF));
				CString tempNumber;
				tempNumber.Format(L"%d", pIAT->u1.Ordinal & 0xFFFF);
				m_treeImport.InsertItem(
					tempNumber, // ����
					hTree,// ���ڵ���
					TVI_LAST);// Ҫ����Ľڵ������
			}
			else
			{
				// �����������Ʒ�ʽ�����
				// �ֶα�����һ��ָ��IMAGE_IMPORT_BY_NAME�ṹ���RVA
				DWORD dwFunctionNameOffset = /*����*/
					RvaToOffset( CDiaPeAnaly::pFileData, pIAT->u1.AddressOfData);

				IMAGE_IMPORT_BY_NAME* pImportName = /*����*/
					(IMAGE_IMPORT_BY_NAME*)((DWORD)CDiaPeAnaly::pFileData + dwFunctionNameOffset);

// 				printf("   �������ơ�%s��\n", pImportName->Name);

				int nLen1 = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, pImportName->Name, -1, NULL, 0);
				if (nLen1 == 0)
				{
					MessageBox(L"��ʽת������!");
				}
				wchar_t* pDllNameResult1 = new wchar_t[nLen1];

				MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, pImportName->Name, -1, pDllNameResult1, nLen1);

				m_treeImport.InsertItem(
					pDllNameResult1, // ����
					hTree,// ���ڵ���
					TVI_LAST);// Ҫ����Ľڵ������
			}

			// ��������һ���ṹ��
			++pIAT;
		}
		++pImportTable;
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}
