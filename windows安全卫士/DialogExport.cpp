// DialogExport.cpp : 实现文件
//

#include "stdafx.h"
#include "windows安全卫士.h"
#include "DialogExport.h"
#include "afxdialogex.h"


// CDialogExport 对话框

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


// CDialogExport 消息处理程序

DWORD  CDialogExport::RvaToOffset(const void* pFileData, DWORD dwRva)
{
	IMAGE_DOS_HEADER *pDosHeader = (IMAGE_DOS_HEADER*)pFileData;

	IMAGE_NT_HEADERS *pNtHeader =   /*换行*/
		(IMAGE_NT_HEADERS*)((DWORD)pDosHeader + pDosHeader->e_lfanew);

	// 得到区段个数
	DWORD   dwSectionNumber = pNtHeader->FileHeader.NumberOfSections;

	// 得到第一个区段
	IMAGE_SECTION_HEADER* pSectionHeader = IMAGE_FIRST_SECTION(pNtHeader);

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

	for (int i = 0; i < dwSectionNumber; ++i) {

		// 判断RVA是否在当前的区段中

		DWORD dwSectionEndRva =   /*换行*/
			pSectionHeader[i].VirtualAddress + pSectionHeader[i].SizeOfRawData;

		if (dwRva >= pSectionHeader[i].VirtualAddress
			&& dwRva <= dwSectionEndRva) {

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

BOOL CDialogExport::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_listExport.InsertColumn(0, L"虚序号", 0, 80);
	m_listExport.InsertColumn(1, L"地址RVA", 0, 80);
	m_listExport.InsertColumn(2, L"函数名：", 0, 80);
// 	m_listExport.InsertColumn(3, L"真序号(有/无)", 0, 80);
	// TODO:  在此添加额外的初始化
// 	// 获取扩展头
// 	IMAGE_OPTIONAL_HEADER* pOptionHeader = /*换行*/
// 		(IMAGE_OPTIONAL_HEADER*)&CDiaPeAnaly::pNtHeader->OptionalHeader;
// 
// 	// 获取到数据目录表
// 	IMAGE_DATA_DIRECTORY* pDataDirectory = pOptionHeader->DataDirectory;

	// 获取导出表
	DWORD dwExportTableRva = CDiaPeAnaly::pDataDirectory[0].VirtualAddress;
	// 将导出表的RVA转换成文件偏移
	DWORD dwExportTableOffset = RvaToOffset( CDiaPeAnaly::pFileData, dwExportTableRva);

	// 获取导出表
	IMAGE_EXPORT_DIRECTORY* pExportTable = /*换行*/
		(IMAGE_EXPORT_DIRECTORY*)(dwExportTableOffset + (DWORD)CDiaPeAnaly::pFileData);

	// 得到导出DLL的模块名
	// Name字段保存的是一个字符串的RVA，需要转换成文件偏移
	DWORD dwNameOffset = RvaToOffset( CDiaPeAnaly::pFileData, pExportTable->Name);
	char* pDllName = (char*)(dwNameOffset + (DWORD)CDiaPeAnaly::pFileData);



// 	HTREEITEM hTree = m_treeExport.InsertItem(pDllNameResult, TVI_ROOT, TVI_LAST);

	// 把所有的导出的函数地址打印出来。
	// 并且，如果是以名称导出，则输出该名称
	// 如果是以序号导出，则输出该序号。

	// 将地址表的RVA转换文件偏移
	DWORD dwAddressTableOffset = RvaToOffset( CDiaPeAnaly::pFileData,
		pExportTable->AddressOfFunctions);
	// 得到导出地址表
	DWORD* pAddressTable = /*换行*/
		(DWORD*)(dwAddressTableOffset + (DWORD)CDiaPeAnaly::pFileData);


	// 得到序号表的地址
	WORD* pOrdinalTable =
		(WORD*)((DWORD)CDiaPeAnaly::pFileData + RvaToOffset( CDiaPeAnaly::pFileData, pExportTable->AddressOfNameOrdinals));

	//函数名称表的地址
	DWORD* pNameTable =
		(DWORD*)((DWORD)CDiaPeAnaly::pFileData + RvaToOffset( CDiaPeAnaly::pFileData, pExportTable->AddressOfNames));

	BOOL bIndexIsExist = FALSE; // 

	for (int i = 0; i < pExportTable->NumberOfFunctions; ++i)
	{
		// 打印地址

		CString emptyNumber, tempAddress;
		emptyNumber.Format(L"%d", i);
		tempAddress.Format(L"%08x", pAddressTable[i]);
		m_listExport.InsertItem(i, emptyNumber);
		m_listExport.SetItemText(i, 1, tempAddress);

		// 判断当前的这个地址是否是以名称方式导出的
		// 判断依据：
		//   序号表保存的是地址表的一个下标，这个下标记录着
		//   地址表中哪个地址是以名称方式导出的。
		//   如果当前的这个下标保存在序号表中，则说明这个地址
		//   是一个名称方式导出，如果这个下标在序号表中不存在，
		//   则说明，这个地址不是一个名称方式导出
		bIndexIsExist = FALSE;
		int nNameIndex = 0;
		for (; nNameIndex < pExportTable->NumberOfNames; ++nNameIndex)
		{
			// 判断地址表的下标是否存在于序号表中
			if (i == pOrdinalTable[nNameIndex])
			{
				bIndexIsExist = TRUE;
				break;
			}
		}

		if (bIndexIsExist == TRUE)
		{
			// 得到名称表中的RVA
			DWORD dwNameRva = pNameTable[nNameIndex];

			// 将名称Rva转换成文件偏移
			char* pFunName =
				(char*)((DWORD)CDiaPeAnaly::pFileData + RvaToOffset( CDiaPeAnaly::pFileData, dwNameRva));
			int nLen = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, pFunName, -1, NULL, 0);
			if (nLen == 0)
			{
				MessageBox(L"格式转换出错!");
			}
			wchar_t* pDllNameResult = new wchar_t[nLen];

			MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, pFunName, -1, pDllNameResult, nLen);

			m_listExport.SetItemText(i, 2, pDllNameResult);
// 			printf(" 函数名:【%s】", pFunName);
		}
		else
		{
			// 判断地址表当前索引到的袁术是否保存着地址
			if (pAddressTable[i] != 0)
			{
				// i ： 是地址表中的索引号，也就是一个虚序号
				// 真正的序号 = 虚序号 + 序号基数
				CString tempTrueNumber;
				tempTrueNumber.Format(L"%d", i + pExportTable->Base);
// 				printf(" 序号:【%d】", i + pExportTable->Base);
				m_listExport.SetItemText(i, 3, tempTrueNumber);
			}
		}

	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}
