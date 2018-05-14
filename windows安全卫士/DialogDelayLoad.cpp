// DialogDelayLoad.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "windows��ȫ��ʿ.h"
#include "DialogDelayLoad.h"
#include "afxdialogex.h"
#include "DiaPeAnaly.h"


// CDialogDelayLoad �Ի���

IMPLEMENT_DYNAMIC(CDialogDelayLoad, CDialogEx)

CDialogDelayLoad::CDialogDelayLoad(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOGDELAYLOAD, pParent)
	, m_editDelayLoad ( _T ( "" ) )
{

}

CDialogDelayLoad::~CDialogDelayLoad()
{
}

void CDialogDelayLoad::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange ( pDX );
	DDX_Text ( pDX, IDC_EDITDELAYLOAD, m_editDelayLoad );
}


BEGIN_MESSAGE_MAP(CDialogDelayLoad, CDialogEx)
END_MESSAGE_MAP()


// CDialogDelayLoad ��Ϣ�������

DWORD  CDialogDelayLoad::RvaToOffset ( const void* pFileData, DWORD dwRva )
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


BOOL CDialogDelayLoad::OnInitDialog ()
{
	CDialogEx::OnInitDialog ();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CString TmpCstr=L"->Delay Import Directory\r\n\t1. DelayImportDescriptor:\r\n";
	m_editDelayLoad += TmpCstr;
	//1.��ȡ��ʱ���ر�RVA
	DWORD dwIATTableRVA = CDiaPeAnaly::pDataDirectory[13].VirtualAddress;

	//1.1���ӳټ��ر�RVAת���ļ�ƫ��
	DWORD dwIATTableOffset = RvaToOffset ( CDiaPeAnaly::pFileData, dwIATTableRVA );
	IMAGE_DELAYLOAD_DESCRIPTOR* pTlsTable = (IMAGE_DELAYLOAD_DESCRIPTOR*)(dwIATTableOffset + (DWORD)CDiaPeAnaly::pFileData);
	TmpCstr.Format ( _T ( "\tgrAttrs:\t\t%#x\r\n" ), pTlsTable->Attributes );
	m_editDelayLoad += TmpCstr;

	TmpCstr.Format ( _T ( "\tDLLName ( R )VA :\t%#x\t" ), pTlsTable->DllNameRVA );
	m_editDelayLoad += TmpCstr;

	DWORD dwDllNameOffset = RvaToOffset ( CDiaPeAnaly::pFileData, pTlsTable->DllNameRVA );
	CHAR *pDllName = (CHAR*)((DWORD)CDiaPeAnaly::pFileData + dwDllNameOffset);
	m_editDelayLoad += pDllName;

	TmpCstr.Format ( _T ( "\r\n\tHmod ( R )VA :\t%#x\r\n" ), pTlsTable->ModuleHandleRVA );
	m_editDelayLoad += TmpCstr;
	TmpCstr.Format ( _T ( "\tIAT ( R )VA :\t%#x\r\n" ), pTlsTable->ImportAddressTableRVA );
	m_editDelayLoad += TmpCstr;
	TmpCstr.Format ( _T ( "\tINT ( R )VA :\t%#x\r\n" ), pTlsTable->ImportNameTableRVA );
	m_editDelayLoad += TmpCstr;
	TmpCstr.Format ( _T ( "\tBoundIAT ( R )VA :\t%#x\r\n" ), pTlsTable->BoundImportAddressTableRVA );
	m_editDelayLoad += TmpCstr;
	TmpCstr.Format ( _T ( "\tUnloadIAT ( R )VA :\t%#x\r\n" ), pTlsTable->UnloadInformationTableRVA );
	m_editDelayLoad += TmpCstr;
	TmpCstr.Format ( _T ( "\tTimeDateStamp :\t%#x\r\n" ), pTlsTable->TimeDateStamp );
	m_editDelayLoad += TmpCstr;

/*
	->Delay Import Directory
		1. DelayImportDescriptor:
		grAttrs:          0x00000001  (dlattrRva)
		DLLName ( R )VA : 0x000184F0  ("testDelayDll.dll")
		Hmod ( R )VA : 0x0001A574
		IAT ( R )VA : 0x0001C070
		INT ( R )VA : 0x0001C040
		BoundIAT ( R )VA : 0x0001C1A8
		UnloadIAT ( R )VA : 0x00000000
		TimeDateStamp : 0x00000000  (GMT : Thu Jan 01 00 : 00 : 00 1970)

		Ordinal / Hint API name
		------------ -------------------------------------- -
		0x0000       "fun"
*/

	//DLLģ������RVA����Ҫת��
// 	DWORD* ModuleHandle = (DWORD*)((DWORD)CDiaPeAnaly::pFileData + RvaToOffset ( CDiaPeAnaly::pFileData, pTlsTable->ModuleHandleRVA ));
// 
// 	//�ӳ�����IAT��RVA,��Ҫת��
// 	DWORD* ImportAddressTable = (DWORD*)((DWORD)CDiaPeAnaly::pFileData + RvaToOffset ( CDiaPeAnaly::pFileData, pTlsTable->ImportAddressTableRVA ));
// 
// 	//�ӳ�����INT��RVA����Ҫת��
// 	DWORD* ImportNameTable = (DWORD*)((DWORD)CDiaPeAnaly::pFileData + RvaToOffset ( CDiaPeAnaly::pFileData, pTlsTable->ImportNameTableRVA ));
// 
// 	//���ӳټ��ص����ַ���RVA����Ҫת��
// 	DWORD* BoundImportAddressTable = (DWORD*)((DWORD)CDiaPeAnaly::pFileData + RvaToOffset ( CDiaPeAnaly::pFileData, pTlsTable->BoundImportAddressTableRVA ));
// 
// 	//ж���ӳټ��ص����ַ���RVA����Ҫת��
// 	DWORD* UnloadInformationTable = (DWORD*)((DWORD)CDiaPeAnaly::pFileData + RvaToOffset ( CDiaPeAnaly::pFileData, pTlsTable->UnloadInformationTableRVA ));
// 
// 	//DLLģ������RVA
// 	TmpCstr.Format ( _T ( "DLLģ������RVA:%X\r\n" ), ModuleHandle );
// 	m_editDelayLoad += TmpCstr;
// 
// 	//�ӳ�����IAT��RVA
// 	TmpCstr.Format ( _T ( "�ӳ�����IAT��RVA:%X\r\n" ), ImportAddressTable );
// 	m_editDelayLoad += TmpCstr;
// 
// 	//�ӳ�����INT��RVA
// 	TmpCstr.Format ( _T ( "�ӳ�����INT��RVA:%X\r\n" ), ImportNameTable );
// 	m_editDelayLoad += TmpCstr;
// 
// 	//���ӳټ��ص����ַ���RVA����Ҫת��
// 	TmpCstr.Format ( _T ( "���ӳټ��ص����ַ���RVA:%X\r\n" ), BoundImportAddressTable );
// 	m_editDelayLoad += TmpCstr;
// 	//ж���ӳټ��ص����ַ���RVA����Ҫת��
// 	TmpCstr.Format ( _T ( "ж���ӳټ��ص����ַ���RVA:%X\r\n" ), UnloadInformationTable );
// 	m_editDelayLoad += TmpCstr;

	UpdateData ( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
