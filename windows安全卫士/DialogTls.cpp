// DialogTls.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "windows��ȫ��ʿ.h"
#include "DialogTls.h"
#include "afxdialogex.h"
#include "DiaPeAnaly.h"


// CDialogTls �Ի���

IMPLEMENT_DYNAMIC(CDialogTls, CDialogEx)

CDialogTls::CDialogTls(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOGTLS, pParent)
	, m_editTls ( _T ( "" ) )
{

}

CDialogTls::~CDialogTls()
{
}

void CDialogTls::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange ( pDX );
	DDX_Text ( pDX, IDC_EDITTLS, m_editTls );
}


BEGIN_MESSAGE_MAP(CDialogTls, CDialogEx)
END_MESSAGE_MAP()


// CDialogTls ��Ϣ�������
DWORD  CDialogTls::RvaToOffset ( const void* pFileData, DWORD dwRva )
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


BOOL CDialogTls::OnInitDialog ()
{
	CDialogEx::OnInitDialog ();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CString TmpCstr;
	//1.��ȡTls��RVA
	DWORD dwTlsTableRva = CDiaPeAnaly::pDataDirectory[9].VirtualAddress;

	//1.1��Tls���RVAת�����ļ�ƫ��
	DWORD dwExportTableOffset = RvaToOffset ( CDiaPeAnaly::pFileData, dwTlsTableRva );

	IMAGE_TLS_DIRECTORY* pTlsTable = (IMAGE_TLS_DIRECTORY*)(dwExportTableOffset + (DWORD)CDiaPeAnaly::pFileData);


	TmpCstr.Format ( _T ( "Դ���ݵ���ʼλ��:%X\r\n" ), pTlsTable->StartAddressOfRawData );
	m_editTls += TmpCstr;

	TmpCstr.Format ( _T ( "Դ���ݵĽ���λ��:%X\r\n" ), pTlsTable->EndAddressOfRawData );
	m_editTls += TmpCstr;

	TmpCstr.Format ( _T ( "����TLS������λ��:%X\r\n" ), pTlsTable->AddressOfIndex );
	m_editTls += TmpCstr;

	TmpCstr.Format ( _T ( "TLS�ص������ĵ�ַ���λ��:%X\r\n" ), pTlsTable->AddressOfCallBacks );
	m_editTls += TmpCstr;

	TmpCstr.Format ( _T ( "TLS��������Ĵ�С:%X\r\n" ), pTlsTable->SizeOfZeroFill );
	m_editTls += TmpCstr;

	TmpCstr.Format ( _T ( "����:%X\r\n" ), pTlsTable->Characteristics );
	m_editTls += TmpCstr;
	UpdateData ( FALSE );
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
