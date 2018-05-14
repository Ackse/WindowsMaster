// DialogResource.cpp : 实现文件
//

#include "stdafx.h"
#include "windows安全卫士.h"
#include "DialogResource.h"
#include "afxdialogex.h"
#include "DiaPeAnaly.h"


// CDialogResource 对话框

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


// CDialogResource 消息处理程序
DWORD  CDialogResource::RvaToOffset ( const void* pFileData, DWORD dwRva )
{
	IMAGE_DOS_HEADER *pDosHeader = (IMAGE_DOS_HEADER*)pFileData;

	IMAGE_NT_HEADERS *pNtHeader =   /*换行*/
		(IMAGE_NT_HEADERS*)((DWORD)pDosHeader + pDosHeader->e_lfanew);

	// 得到区段个数
	DWORD   dwSectionNumber = pNtHeader->FileHeader.NumberOfSections;

	// 得到第一个区段
	IMAGE_SECTION_HEADER* pSectionHeader = IMAGE_FIRST_SECTION ( pNtHeader );

	// 遍历区段表，找到RVA所在的区段
	/*
	* 每个偏移，不管是在文件中，还是在内存中，它们距离区段开始位置的距离
	* 总是相等的。
	* 而且，区段表中，保存着两个开始偏移：
	*  1. 文件中的开始偏移
	*  2. 内存中的开始偏移
	* 具体过程：
	*  找到RVA所在区段， 然后计算出这个RVA到区段在内存中的开始位置的距离。
	*  用这个距离加上区段在文件中的开始位置就得到文件偏移了
	*/

	for( int i = 0; i < dwSectionNumber; ++i )
	{

		// 判断RVA是否在当前的区段中

		DWORD dwSectionEndRva =   /*换行*/
			pSectionHeader[i].VirtualAddress + pSectionHeader[i].SizeOfRawData;

		if( dwRva >= pSectionHeader[i].VirtualAddress
			&& dwRva <= dwSectionEndRva )
		{

			// 计算出RVA对应的文件偏移
			// 公式：
			// 文件偏移  =  RVA - 区段的起始RVA + 区段的起始文件偏移
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

	// TODO:  在此添加额外的初始化
	// 获取资源表
	CString TmpCstr;
	// 获取资源表
	m_EditResource.Empty ();
	DWORD resourcesTableOffset =
		RvaToOffset ( CDiaPeAnaly::pFileData, CDiaPeAnaly::pDataDirectory[2].VirtualAddress );

	IMAGE_RESOURCE_DIRECTORY* pRoot =
		(IMAGE_RESOURCE_DIRECTORY*)(((DWORD)CDiaPeAnaly::pFileData) + resourcesTableOffset);


	IMAGE_RESOURCE_DIRECTORY*    pDir2;// 资源目录
	IMAGE_RESOURCE_DIRECTORY*    pDir3;// 资源目录

	IMAGE_RESOURCE_DIRECTORY_ENTRY* pEntry1;//目录入口
	IMAGE_RESOURCE_DIRECTORY_ENTRY* pEntry2;//目录入口
	IMAGE_RESOURCE_DIRECTORY_ENTRY* pEntry3;//目录入口

	IMAGE_RESOURCE_DATA_ENTRY*      pDataEntry;// 资源数据入口
	IMAGE_RESOURCE_DIR_STRING_U*    pIdString; // 保存Id的字符串

											   /* 把第一层所有的目录入口都遍历出来 */
											   // 得到第一个目录入口的地址
	pEntry1 = (IMAGE_RESOURCE_DIRECTORY_ENTRY*)(pRoot + 1);
	for( int i = 0;
	i < pRoot->NumberOfIdEntries + pRoot->NumberOfNamedEntries;
		i++, pEntry1++ )
	{

		//获取第一层目录入口的ID（ID就是资源的类型）
		if( pEntry1->NameIsString == 1 )
		{
			// NameOffset该字段的值是一个偏移
			// 这个偏移是以资源表根目录的地址作为基址。
			pIdString =
				(IMAGE_RESOURCE_DIR_STRING_U*)(pEntry1->NameOffset + (DWORD)pRoot);

			WCHAR* psTest = new WCHAR[pIdString->Length + 1]{};
			//wcscpy_s(psTest, pIdString->Length, pIdString->NameString);
			memcpy_s ( psTest, sizeof ( WCHAR )*(pIdString->Length + 1),
					   pIdString->NameString,
					   sizeof ( WCHAR )*pIdString->Length );
			//	wprintf(L"资源类型[%s]\n", psTest);
			TmpCstr.Format ( _T ( "资源类型:%s\r\n\r\n" ), psTest );
			m_EditResource += TmpCstr;
			delete[] psTest;

		}
		else
		{
			TCHAR* pType[] =
			{
				_T ( "" ),          // 0
				_T ( "鼠标指针" ),   // 1
				_T ( "位图" ),   // 2
				_T ( "图标" ),       // 3
				_T ( "菜单" ),       // 4
				_T ( "对话框" ),      //5
				_T ( "字符串列表" ),  //6
				_T ( "字体目录" ),  //7
				_T ( "字体" ),      //8
				_T ( "快捷键" ),  //9
				_T ( "非格式化资源" ),  //A
				_T ( "消息列表" ),  //B
				_T ( "鼠标指针组" ),  //C
				_T ( "" ),             // D
				_T ( "图标组" ),  //E
				_T ( "" ),         // F
				_T ( "版本信息" )//10
			};

			if( pEntry1->Id > 16 )
			{
				TmpCstr.Format ( _T ( "资源类型:%d\r\n\r\n" ), pEntry1->Id );
				m_EditResource += TmpCstr;
			}
			else
			{
				TmpCstr.Format ( _T ( "资源类型:%s\r\n\r\n" ), pType[pEntry1->Id] );
				m_EditResource += TmpCstr;
			}

		}

		// 获取第二层目录
		if( pEntry1->DataIsDirectory == 1 )
		{

			pDir2 = /*得到第二层资源目录*/
				(IMAGE_RESOURCE_DIRECTORY*)(pEntry1->OffsetToDirectory + (DWORD)pRoot);

			// 遍历第二层资源目录的所有目录入口
			pEntry2 = (IMAGE_RESOURCE_DIRECTORY_ENTRY*)(pDir2 + 1);
			for( int j = 0;
			j < pDir2->NumberOfIdEntries + pDir2->NumberOfNamedEntries;
				++j, ++pEntry2 )
			{

				// 得到资源的ID
				if( pEntry2->NameIsString == 1 )
				{
					// NameOffset该字段的值是一个偏移
					// 这个偏移是以资源表根目录的地址作为基址。
					pIdString =
						(IMAGE_RESOURCE_DIR_STRING_U*)(pEntry2->NameOffset + (DWORD)pRoot);

					WCHAR* psTest = new WCHAR[pIdString->Length + 1]{};
					//wcscpy_s(psTest, pIdString->Length, pIdString->NameString);
					memcpy_s ( psTest, sizeof ( WCHAR )*(pIdString->Length + 1),
							   pIdString->NameString,
							   sizeof ( WCHAR )*pIdString->Length );
					//	wprintf(L"资源类型[%s]\n", psTest);
					TmpCstr.Format ( _T ( "+资源ID:%s\r\n" ), psTest );
					m_EditResource += TmpCstr;
					delete[] psTest;

				}
				else
				{

					TmpCstr.Format ( _T ( " +资源ID:%d\r\n" ), (DWORD)pEntry2->Id );
					m_EditResource += TmpCstr;
				}
				// 判断第三层是目录还是数据
				if( pEntry2->DataIsDirectory == 1 )
				{

					// 得到第三层目录的地址
					pDir3 =
						(IMAGE_RESOURCE_DIRECTORY*)(pEntry2->OffsetToDirectory + (DWORD)pRoot);

					// 得到第三层的第一个目录入库
					pEntry3 =
						(IMAGE_RESOURCE_DIRECTORY_ENTRY*)(pDir3 + 1);

					TmpCstr.Format ( _T ( "   +id:%d\r\n" ), pEntry3->Id );
					m_EditResource += TmpCstr;
					// 得到数据入口
					pDataEntry =
						(IMAGE_RESOURCE_DATA_ENTRY*)(pEntry3->OffsetToData + (DWORD)pRoot);

					TmpCstr.Format ( _T ( "     +资源偏移(RVA):%x\r\n" ), pDataEntry->OffsetToData );
					m_EditResource += TmpCstr;
					TmpCstr.Format ( _T ( "     +资源大小:%x\r\n\r\n" ), pDataEntry->Size );
					m_EditResource += TmpCstr;

				}
			}
		}
		m_EditResource += _T ( "\r\n\r\n" );
		UpdateData ( FALSE );
		// 异常: OCX 属性页应返回 FALSE
	}
	return TRUE;  // return TRUE unless you set the focus to a control
}
