// DialogDelayLoad.cpp : 实现文件
//

#include "stdafx.h"
#include "windows安全卫士.h"
#include "DialogDelayLoad.h"
#include "afxdialogex.h"
#include "DiaPeAnaly.h"


// CDialogDelayLoad 对话框

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


// CDialogDelayLoad 消息处理程序

DWORD  CDialogDelayLoad::RvaToOffset ( const void* pFileData, DWORD dwRva )
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


BOOL CDialogDelayLoad::OnInitDialog ()
{
	CDialogEx::OnInitDialog ();

	// TODO:  在此添加额外的初始化
	CString TmpCstr=L"->Delay Import Directory\r\n\t1. DelayImportDescriptor:\r\n";
	m_editDelayLoad += TmpCstr;
	//1.获取延时加载表RVA
	DWORD dwIATTableRVA = CDiaPeAnaly::pDataDirectory[13].VirtualAddress;

	//1.1将延迟加载表RVA转换文件偏移
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

	//DLL模块句柄的RVA，需要转换
// 	DWORD* ModuleHandle = (DWORD*)((DWORD)CDiaPeAnaly::pFileData + RvaToOffset ( CDiaPeAnaly::pFileData, pTlsTable->ModuleHandleRVA ));
// 
// 	//延迟载入IAT的RVA,需要转换
// 	DWORD* ImportAddressTable = (DWORD*)((DWORD)CDiaPeAnaly::pFileData + RvaToOffset ( CDiaPeAnaly::pFileData, pTlsTable->ImportAddressTableRVA ));
// 
// 	//延迟载入INT的RVA，需要转换
// 	DWORD* ImportNameTable = (DWORD*)((DWORD)CDiaPeAnaly::pFileData + RvaToOffset ( CDiaPeAnaly::pFileData, pTlsTable->ImportNameTableRVA ));
// 
// 	//绑定延迟加载导入地址表的RVA，需要转换
// 	DWORD* BoundImportAddressTable = (DWORD*)((DWORD)CDiaPeAnaly::pFileData + RvaToOffset ( CDiaPeAnaly::pFileData, pTlsTable->BoundImportAddressTableRVA ));
// 
// 	//卸载延迟加载导入地址表的RVA，需要转换
// 	DWORD* UnloadInformationTable = (DWORD*)((DWORD)CDiaPeAnaly::pFileData + RvaToOffset ( CDiaPeAnaly::pFileData, pTlsTable->UnloadInformationTableRVA ));
// 
// 	//DLL模块句柄的RVA
// 	TmpCstr.Format ( _T ( "DLL模块句柄的RVA:%X\r\n" ), ModuleHandle );
// 	m_editDelayLoad += TmpCstr;
// 
// 	//延迟载入IAT的RVA
// 	TmpCstr.Format ( _T ( "延迟载入IAT的RVA:%X\r\n" ), ImportAddressTable );
// 	m_editDelayLoad += TmpCstr;
// 
// 	//延迟载入INT的RVA
// 	TmpCstr.Format ( _T ( "延迟载入INT的RVA:%X\r\n" ), ImportNameTable );
// 	m_editDelayLoad += TmpCstr;
// 
// 	//绑定延迟加载导入地址表的RVA，需要转换
// 	TmpCstr.Format ( _T ( "绑定延迟加载导入地址表的RVA:%X\r\n" ), BoundImportAddressTable );
// 	m_editDelayLoad += TmpCstr;
// 	//卸载延迟加载导入地址表的RVA，需要转换
// 	TmpCstr.Format ( _T ( "卸载延迟加载导入地址表的RVA:%X\r\n" ), UnloadInformationTable );
// 	m_editDelayLoad += TmpCstr;

	UpdateData ( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
