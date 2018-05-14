// DialogReloc.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "windows��ȫ��ʿ.h"
#include "DialogReloc.h"
#include "afxdialogex.h"
#include "DiaPeAnaly.h"


// CDialogReloc �Ի���

IMPLEMENT_DYNAMIC(CDialogReloc, CDialogEx)

CDialogReloc::CDialogReloc(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOGRELOC, pParent)
	, m_EditReloc ( _T ( "" ) )
{

}

CDialogReloc::~CDialogReloc()
{
}

void CDialogReloc::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange ( pDX );
	DDX_Text ( pDX, IDC_EDITRELOC, m_EditReloc );
}


BEGIN_MESSAGE_MAP(CDialogReloc, CDialogEx)
END_MESSAGE_MAP()


// CDialogReloc ��Ϣ�������
DWORD  CDialogReloc::RvaToOffset ( const void* pFileData, DWORD dwRva )
{
	IMAGE_DOS_HEADER *pDosHeader = (IMAGE_DOS_HEADER*)pFileData;

	IMAGE_NT_HEADERS *pNtHeader =   /*����*/
		(IMAGE_NT_HEADERS*)((DWORD)pDosHeader + pDosHeader->e_lfanew);

	// �õ����θ���
	DWORD   dwSectionNumber = pNtHeader->FileHeader.NumberOfSections;

	// �õ���һ������
	IMAGE_SECTION_HEADER* pSectionHeader = IMAGE_FIRST_SECTION ( pNtHeader );

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

	for( int i = 0; i < dwSectionNumber; ++i )
	{

		// �ж�RVA�Ƿ��ڵ�ǰ��������

		DWORD dwSectionEndRva =   /*����*/
			pSectionHeader[i].VirtualAddress + pSectionHeader[i].SizeOfRawData;

		if( dwRva >= pSectionHeader[i].VirtualAddress
			&& dwRva <= dwSectionEndRva )
		{

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


BOOL CDialogReloc::OnInitDialog ()
{
	CDialogEx::OnInitDialog ();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	// ��ȡ�ض�λ��
	/*
	+-----------+
	|  �ض�λ��  |
	+-----------+
	+-----------+
	|  �ض�λ��  |
	+-----------+
	+-----------+
	|  �ض�λ��  |
	+-----------+
	+-----------+
	|00000000000|
	+-----------+
	*/
	// �õ���һ���ض�λ��������׵�ַ(RVA)
	DWORD dwRelcationBlockOffset =
		RvaToOffset ( CDiaPeAnaly::pFileData, CDiaPeAnaly::pDataDirectory[5].VirtualAddress );

	// �õ���һ���ض�λ��ĵ�ַ
	IMAGE_BASE_RELOCATION* pRelcationBlock =
		(IMAGE_BASE_RELOCATION*)((DWORD)CDiaPeAnaly::pFileData + dwRelcationBlockOffset);

	while( 1 )
	{
		// �ж��Ƿ������һ���ض�λ��
		if( pRelcationBlock->VirtualAddress == 0 && pRelcationBlock->SizeOfBlock == 0 )
		{
			break;
		}
		TCHAR buff[100];
		_stprintf_s ( buff,
					  _T ( "�鿪ʼ��RVA[0x%X],�ֽ���[%x]\r\n" ), 
					  pRelcationBlock->VirtualAddress, 
					  pRelcationBlock->SizeOfBlock );

		m_EditReloc += buff;
// 		printf ( "�鿪ʼ��RVA[0x%X],�ֽ���[%x]\n",
// 				 pRelcationBlock->VirtualAddress,
// 				 pRelcationBlock->SizeOfBlock );

//  �����ض�λ���ͺ�ƫ�Ƶ����ݿ�
		TypeOffset* pTypeOffset =
			(TypeOffset*)((DWORD)pRelcationBlock + sizeof ( IMAGE_BASE_RELOCATION ));
		//my-��ΪTypeOffset������������ṹ��ĳ�Ա,�������ڽṹ��֮��,�Ǹ�������������.
		//my-�Լ���
		int number = sizeof ( TypeOffset );//�ں�����word�Ľṹ�廹��2���ֽ�
		int number2 = sizeof ( WORD );//word��2���ֽ�
									  //my-�Լ���
		DWORD   dwCount =
			(pRelcationBlock->SizeOfBlock - sizeof ( IMAGE_BASE_RELOCATION )) / sizeof ( TypeOffset );
		//my-���ض�λ����n=(SizeOfBlock-8)/2;
		//my-(0x70-0x8)/2=0x34
		for( int i = 0; i < dwCount; ++i )
		{
			/*
			Type��ֵ��Ӧ���º�:
			IMAGE_REL_BASED_ABSOLUTE (0) ʹ�鰴��32λ���룬λ��Ϊ0��
			IMAGE_REL_BASED_HIGH (1) ��16λ����Ӧ����ƫ������ָ����16λ��
			IMAGE_REL_BASED_LOW (2) ��16λ����Ӧ����ƫ������ָ����16λ��
			IMAGE_REL_BASED_HIGHLOW (3) ȫ��32λӦ��������32λ��
			IMAGE_REL_BASED_HIGHADJ (4) ��Ҫ32λ����16λλ��ƫ��������16λλ����һ��ƫ��������Ԫ�أ����Ϊһ����������������32λ��һ������Ȼ�����8000Ȼ��Ѹ�16λ������ƫ������16λ���ڡ�
			IMAGE_REL_BASED_MIPS_JMPADDR (5) Unknown
			IMAGE_REL_BASED_SECTION (6) Unknown
			IMAGE_REL_BASED_REL32 (7) Unknown
			*/
			if( pTypeOffset[i].Type == 3 )
			{
// 				printf ( " + ƫ��[0x%X]\n", pTypeOffset[i].Offset );
				TCHAR buff1[100];
				_stprintf_s ( buff1, _T ( "+ ƫ��[0x%X]\r\n" ), pTypeOffset[i].Offset );
				m_EditReloc += buff1;
			}

			// �õ���Ҫ�ض�λ������
			DWORD dwRva = pRelcationBlock->VirtualAddress + pTypeOffset[i].Offset;
			DWORD dwData = RvaToOffset ( CDiaPeAnaly::pFileData, dwRva );
			DWORD* pRelData = (DWORD*)(dwData + CDiaPeAnaly::pFileData);
			TCHAR buff2[100];
			_stprintf_s ( buff2, _T ( " + Ҫ�޸ĵĵ�ַ:[0x%08X]\n" ), *pRelData );
			m_EditReloc += buff2;
// 			printf ( " + Ҫ�޸ĵĵ�ַ:[0x%08X]\n", *pRelData );
		}
		// ��������һ���ض�λ��
		pRelcationBlock =
			(IMAGE_BASE_RELOCATION*)((DWORD)(pRelcationBlock)+pRelcationBlock->SizeOfBlock);
	}

	UpdateData ( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
