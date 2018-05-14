// DialogReloc.cpp : 实现文件
//

#include "stdafx.h"
#include "windows安全卫士.h"
#include "DialogReloc.h"
#include "afxdialogex.h"
#include "DiaPeAnaly.h"


// CDialogReloc 对话框

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


// CDialogReloc 消息处理程序
DWORD  CDialogReloc::RvaToOffset ( const void* pFileData, DWORD dwRva )
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


BOOL CDialogReloc::OnInitDialog ()
{
	CDialogEx::OnInitDialog ();

	// TODO:  在此添加额外的初始化
	// 获取重定位表
	/*
	+-----------+
	|  重定位块  |
	+-----------+
	+-----------+
	|  重定位块  |
	+-----------+
	+-----------+
	|  重定位块  |
	+-----------+
	+-----------+
	|00000000000|
	+-----------+
	*/
	// 得到第一个重定位块的数组首地址(RVA)
	DWORD dwRelcationBlockOffset =
		RvaToOffset ( CDiaPeAnaly::pFileData, CDiaPeAnaly::pDataDirectory[5].VirtualAddress );

	// 得到第一个重定位块的地址
	IMAGE_BASE_RELOCATION* pRelcationBlock =
		(IMAGE_BASE_RELOCATION*)((DWORD)CDiaPeAnaly::pFileData + dwRelcationBlockOffset);

	while( 1 )
	{
		// 判断是否是最后一个重定位块
		if( pRelcationBlock->VirtualAddress == 0 && pRelcationBlock->SizeOfBlock == 0 )
		{
			break;
		}
		TCHAR buff[100];
		_stprintf_s ( buff,
					  _T ( "块开始的RVA[0x%X],字节数[%x]\r\n" ), 
					  pRelcationBlock->VirtualAddress, 
					  pRelcationBlock->SizeOfBlock );

		m_EditReloc += buff;
// 		printf ( "块开始的RVA[0x%X],字节数[%x]\n",
// 				 pRelcationBlock->VirtualAddress,
// 				 pRelcationBlock->SizeOfBlock );

//  遍历重定位类型和偏移的数据块
		TypeOffset* pTypeOffset =
			(TypeOffset*)((DWORD)pRelcationBlock + sizeof ( IMAGE_BASE_RELOCATION ));
		//my-因为TypeOffset并不真是这个结构体的成员,他紧随在结构体之后,是个不定长的数组.
		//my-自加区
		int number = sizeof ( TypeOffset );//内含两个word的结构体还是2个字节
		int number2 = sizeof ( WORD );//word是2个字节
									  //my-自加区
		DWORD   dwCount =
			(pRelcationBlock->SizeOfBlock - sizeof ( IMAGE_BASE_RELOCATION )) / sizeof ( TypeOffset );
		//my-需重定位个数n=(SizeOfBlock-8)/2;
		//my-(0x70-0x8)/2=0x34
		for( int i = 0; i < dwCount; ++i )
		{
			/*
			Type的值对应以下宏:
			IMAGE_REL_BASED_ABSOLUTE (0) 使块按照32位对齐，位置为0。
			IMAGE_REL_BASED_HIGH (1) 高16位必须应用于偏移量所指高字16位。
			IMAGE_REL_BASED_LOW (2) 低16位必须应用于偏移量所指低字16位。
			IMAGE_REL_BASED_HIGHLOW (3) 全部32位应用于所有32位。
			IMAGE_REL_BASED_HIGHADJ (4) 需要32位，高16位位于偏移量，低16位位于下一个偏移量数组元素，组合为一个带符号数，加上32位的一个数，然后加上8000然后把高16位保存在偏移量的16位域内。
			IMAGE_REL_BASED_MIPS_JMPADDR (5) Unknown
			IMAGE_REL_BASED_SECTION (6) Unknown
			IMAGE_REL_BASED_REL32 (7) Unknown
			*/
			if( pTypeOffset[i].Type == 3 )
			{
// 				printf ( " + 偏移[0x%X]\n", pTypeOffset[i].Offset );
				TCHAR buff1[100];
				_stprintf_s ( buff1, _T ( "+ 偏移[0x%X]\r\n" ), pTypeOffset[i].Offset );
				m_EditReloc += buff1;
			}

			// 得到需要重定位的数据
			DWORD dwRva = pRelcationBlock->VirtualAddress + pTypeOffset[i].Offset;
			DWORD dwData = RvaToOffset ( CDiaPeAnaly::pFileData, dwRva );
			DWORD* pRelData = (DWORD*)(dwData + CDiaPeAnaly::pFileData);
			TCHAR buff2[100];
			_stprintf_s ( buff2, _T ( " + 要修改的地址:[0x%08X]\n" ), *pRelData );
			m_EditReloc += buff2;
// 			printf ( " + 要修改的地址:[0x%08X]\n", *pRelData );
		}
		// 递增到下一个重定位块
		pRelcationBlock =
			(IMAGE_BASE_RELOCATION*)((DWORD)(pRelcationBlock)+pRelcationBlock->SizeOfBlock);
	}

	UpdateData ( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
