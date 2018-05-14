// DialogResource.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "windows��ȫ��ʿ.h"
#include "DialogResource.h"
#include "afxdialogex.h"
#include "DiaPeAnaly.h"


// CDialogResource �Ի���

IMPLEMENT_DYNAMIC ( CDialogResource, CDialogEx )

CDialogResource::CDialogResource ( CWnd* pParent /*=NULL*/ )
	: CDialogEx ( IDD_DIALOGRESOURCE, pParent )
	, m_EditResource ( _T ( "" ) )
{

}

CDialogResource::~CDialogResource ()
{}

void CDialogResource::DoDataExchange ( CDataExchange* pDX )
{
	CDialogEx::DoDataExchange ( pDX );
	DDX_Text ( pDX, IDC_EDITRESOURCE, m_EditResource );
}


BEGIN_MESSAGE_MAP ( CDialogResource, CDialogEx )
END_MESSAGE_MAP ()


// CDialogResource ��Ϣ�������
DWORD  CDialogResource::RvaToOffset ( const void* pFileData, DWORD dwRva )
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


BOOL CDialogResource::OnInitDialog ()
{
	CDialogEx::OnInitDialog ();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	// ��ȡ��Դ��
	CString TmpCstr;
	// ��ȡ��Դ��
	m_EditResource.Empty ();
	DWORD resourcesTableOffset =
		RvaToOffset ( CDiaPeAnaly::pFileData, CDiaPeAnaly::pDataDirectory[2].VirtualAddress );

	IMAGE_RESOURCE_DIRECTORY* pRoot =
		(IMAGE_RESOURCE_DIRECTORY*)(((DWORD)CDiaPeAnaly::pFileData) + resourcesTableOffset);


	IMAGE_RESOURCE_DIRECTORY*    pDir2;// ��ԴĿ¼
	IMAGE_RESOURCE_DIRECTORY*    pDir3;// ��ԴĿ¼

	IMAGE_RESOURCE_DIRECTORY_ENTRY* pEntry1;//Ŀ¼���
	IMAGE_RESOURCE_DIRECTORY_ENTRY* pEntry2;//Ŀ¼���
	IMAGE_RESOURCE_DIRECTORY_ENTRY* pEntry3;//Ŀ¼���

	IMAGE_RESOURCE_DATA_ENTRY*      pDataEntry;// ��Դ�������
	IMAGE_RESOURCE_DIR_STRING_U*    pIdString; // ����Id���ַ���

											   /* �ѵ�һ�����е�Ŀ¼��ڶ��������� */
											   // �õ���һ��Ŀ¼��ڵĵ�ַ
	pEntry1 = (IMAGE_RESOURCE_DIRECTORY_ENTRY*)(pRoot + 1);
	for( int i = 0;
	i < pRoot->NumberOfIdEntries + pRoot->NumberOfNamedEntries;
		i++, pEntry1++ )
	{

		//��ȡ��һ��Ŀ¼��ڵ�ID��ID������Դ�����ͣ�
		if( pEntry1->NameIsString == 1 )
		{
			// NameOffset���ֶε�ֵ��һ��ƫ��
			// ���ƫ��������Դ���Ŀ¼�ĵ�ַ��Ϊ��ַ��
			pIdString =
				(IMAGE_RESOURCE_DIR_STRING_U*)(pEntry1->NameOffset + (DWORD)pRoot);

			WCHAR* psTest = new WCHAR[pIdString->Length + 1]{};
			//wcscpy_s(psTest, pIdString->Length, pIdString->NameString);
			memcpy_s ( psTest, sizeof ( WCHAR )*(pIdString->Length + 1),
					   pIdString->NameString,
					   sizeof ( WCHAR )*pIdString->Length );
			//	wprintf(L"��Դ����[%s]\n", psTest);
			TmpCstr.Format ( _T ( "��Դ����:%s\r\n\r\n" ), psTest );
			m_EditResource += TmpCstr;
			delete[] psTest;

		}
		else
		{
			TCHAR* pType[] =
			{
				_T ( "" ),          // 0
				_T ( "���ָ��" ),   // 1
				_T ( "λͼ" ),   // 2
				_T ( "ͼ��" ),       // 3
				_T ( "�˵�" ),       // 4
				_T ( "�Ի���" ),      //5
				_T ( "�ַ����б�" ),  //6
				_T ( "����Ŀ¼" ),  //7
				_T ( "����" ),      //8
				_T ( "��ݼ�" ),  //9
				_T ( "�Ǹ�ʽ����Դ" ),  //A
				_T ( "��Ϣ�б�" ),  //B
				_T ( "���ָ����" ),  //C
				_T ( "" ),             // D
				_T ( "ͼ����" ),  //E
				_T ( "" ),         // F
				_T ( "�汾��Ϣ" )//10
			};

			if( pEntry1->Id > 16 )
			{
				TmpCstr.Format ( _T ( "��Դ����:%d\r\n\r\n" ), pEntry1->Id );
				m_EditResource += TmpCstr;
			}
			else
			{
				TmpCstr.Format ( _T ( "��Դ����:%s\r\n\r\n" ), pType[pEntry1->Id] );
				m_EditResource += TmpCstr;
			}

		}

		// ��ȡ�ڶ���Ŀ¼
		if( pEntry1->DataIsDirectory == 1 )
		{

			pDir2 = /*�õ��ڶ�����ԴĿ¼*/
				(IMAGE_RESOURCE_DIRECTORY*)(pEntry1->OffsetToDirectory + (DWORD)pRoot);

			// �����ڶ�����ԴĿ¼������Ŀ¼���
			pEntry2 = (IMAGE_RESOURCE_DIRECTORY_ENTRY*)(pDir2 + 1);
			for( int j = 0;
			j < pDir2->NumberOfIdEntries + pDir2->NumberOfNamedEntries;
				++j, ++pEntry2 )
			{

				// �õ���Դ��ID
				if( pEntry2->NameIsString == 1 )
				{
					// NameOffset���ֶε�ֵ��һ��ƫ��
					// ���ƫ��������Դ���Ŀ¼�ĵ�ַ��Ϊ��ַ��
					pIdString =
						(IMAGE_RESOURCE_DIR_STRING_U*)(pEntry2->NameOffset + (DWORD)pRoot);

					WCHAR* psTest = new WCHAR[pIdString->Length + 1]{};
					//wcscpy_s(psTest, pIdString->Length, pIdString->NameString);
					memcpy_s ( psTest, sizeof ( WCHAR )*(pIdString->Length + 1),
							   pIdString->NameString,
							   sizeof ( WCHAR )*pIdString->Length );
					//	wprintf(L"��Դ����[%s]\n", psTest);
					TmpCstr.Format ( _T ( "+��ԴID:%s\r\n" ), psTest );
					m_EditResource += TmpCstr;
					delete[] psTest;

				}
				else
				{

					TmpCstr.Format ( _T ( " +��ԴID:%d\r\n" ), (DWORD)pEntry2->Id );
					m_EditResource += TmpCstr;
				}
				// �жϵ�������Ŀ¼��������
				if( pEntry2->DataIsDirectory == 1 )
				{

					// �õ�������Ŀ¼�ĵ�ַ
					pDir3 =
						(IMAGE_RESOURCE_DIRECTORY*)(pEntry2->OffsetToDirectory + (DWORD)pRoot);

					// �õ�������ĵ�һ��Ŀ¼���
					pEntry3 =
						(IMAGE_RESOURCE_DIRECTORY_ENTRY*)(pDir3 + 1);

					TmpCstr.Format ( _T ( "   +id:%d\r\n" ), pEntry3->Id );
					m_EditResource += TmpCstr;
					// �õ��������
					pDataEntry =
						(IMAGE_RESOURCE_DATA_ENTRY*)(pEntry3->OffsetToData + (DWORD)pRoot);

					TmpCstr.Format ( _T ( "     +��Դƫ��(RVA):%x\r\n" ), pDataEntry->OffsetToData );
					m_EditResource += TmpCstr;
					TmpCstr.Format ( _T ( "     +��Դ��С:%x\r\n\r\n" ), pDataEntry->Size );
					m_EditResource += TmpCstr;

				}
			}
		}
		m_EditResource += _T ( "\r\n\r\n" );
		UpdateData ( FALSE );
		// �쳣: OCX ����ҳӦ���� FALSE
	}
	return TRUE;  // return TRUE unless you set the focus to a control
}
