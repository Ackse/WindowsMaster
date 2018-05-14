// DialogTls.cpp : 实现文件
//

#include "stdafx.h"
#include "windows安全卫士.h"
#include "DialogTls.h"
#include "afxdialogex.h"
#include "DiaPeAnaly.h"


// CDialogTls 对话框

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


// CDialogTls 消息处理程序
DWORD  CDialogTls::RvaToOffset ( const void* pFileData, DWORD dwRva )
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


BOOL CDialogTls::OnInitDialog ()
{
	CDialogEx::OnInitDialog ();

	// TODO:  在此添加额外的初始化
	CString TmpCstr;
	//1.获取Tls表RVA
	DWORD dwTlsTableRva = CDiaPeAnaly::pDataDirectory[9].VirtualAddress;

	//1.1将Tls表的RVA转换成文件偏移
	DWORD dwExportTableOffset = RvaToOffset ( CDiaPeAnaly::pFileData, dwTlsTableRva );

	IMAGE_TLS_DIRECTORY* pTlsTable = (IMAGE_TLS_DIRECTORY*)(dwExportTableOffset + (DWORD)CDiaPeAnaly::pFileData);


	TmpCstr.Format ( _T ( "源数据的起始位置:%X\r\n" ), pTlsTable->StartAddressOfRawData );
	m_editTls += TmpCstr;

	TmpCstr.Format ( _T ( "源数据的结束位置:%X\r\n" ), pTlsTable->EndAddressOfRawData );
	m_editTls += TmpCstr;

	TmpCstr.Format ( _T ( "保存TLS索引的位置:%X\r\n" ), pTlsTable->AddressOfIndex );
	m_editTls += TmpCstr;

	TmpCstr.Format ( _T ( "TLS回调函数的地址表的位置:%X\r\n" ), pTlsTable->AddressOfCallBacks );
	m_editTls += TmpCstr;

	TmpCstr.Format ( _T ( "TLS变量区域的大小:%X\r\n" ), pTlsTable->SizeOfZeroFill );
	m_editTls += TmpCstr;

	TmpCstr.Format ( _T ( "保留:%X\r\n" ), pTlsTable->Characteristics );
	m_editTls += TmpCstr;
	UpdateData ( FALSE );
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
