// DialogImport.cpp : 实现文件
//

#include "stdafx.h"
#include "windows安全卫士.h"
#include "DialogImport.h"
#include "afxdialogex.h"
#include "DiaPeAnaly.h"


// CDialogImport 对话框

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


// CDialogImport 消息处理程序


DWORD  CDialogImport::RvaToOffset(const void* pFileData, DWORD dwRva)
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

BOOL CDialogImport::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	// 获取扩展头
// 	IMAGE_OPTIONAL_HEADER* pOptionHeader = /*换行*/
// 		(IMAGE_OPTIONAL_HEADER*)&CDiaPeAnaly::pNtHeader->OptionalHeader;
// 
// 	// 获取到数据目录表
// 	IMAGE_DATA_DIRECTORY* pDataDirectory = pOptionHeader->DataDirectory;
// 

	//1. 获取导入表
	// 1.1 获取导入表的RVA
	DWORD   dwImportTableRva = CDiaPeAnaly::pDataDirectory[1].VirtualAddress;
	// 1.2 将导入表的RVA转换成文件偏移
	DWORD   dwImportTableOffset = RvaToOffset( CDiaPeAnaly::pFileData, dwImportTableRva);
	// 1.3 将导入表结构体指针指向缓冲区中的导入表
	IMAGE_IMPORT_DESCRIPTOR* pImportTable =
		(IMAGE_IMPORT_DESCRIPTOR*)((DWORD)CDiaPeAnaly::pFileData + dwImportTableOffset);

	//2. 遍历导入表块
	//  因为一个exe可能会导入多个DLL，而每一个Dll对应着一个导入表
	//  多个导入表就形成一个导入表块
	//  这个导入表块是以全0结尾(全0结尾指的是整个结构体都是0)

	//2.1判断是否遍历到了最后一个结构体。
	while (pImportTable->Name != 0)
	{
		//3. 解析出导入的Dll的模块名
		//  3.1 将Dll模块名的RVA转换成文件偏移
		DWORD dwNameOffset = RvaToOffset( CDiaPeAnaly::pFileData, pImportTable->Name);
		//  3.2 用一个char*指针指向dll名在内存中的位置
		char* pDllName = (char*)(dwNameOffset + (DWORD)CDiaPeAnaly::pFileData);
// 		printf("[%s]\n", pDllName);

		int nLen = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, pDllName, -1, NULL, 0);
		if (nLen == 0)
		{
			MessageBox(L"格式转换出错!");
		}
		wchar_t* pDllNameResult = new wchar_t[nLen];

		MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, pDllName, -1, pDllNameResult, nLen);

		HTREEITEM hTree = m_treeImport.InsertItem(pDllNameResult, TVI_ROOT, TVI_LAST);


		// 4. 解析当前dll的导入函数名称
		// 4.1 得到导入名称表的地址
		pImportTable->OriginalFirstThunk; // 导入名称表
		pImportTable->FirstThunk; // 导入地址表
		// 上面说的两个表，在文件中保存的内容是完全相同的。

		DWORD dwIATOffset = RvaToOffset( CDiaPeAnaly::pFileData, pImportTable->FirstThunk);
		IMAGE_THUNK_DATA *pIAT = (IMAGE_THUNK_DATA *)((DWORD)CDiaPeAnaly::pFileData + dwIATOffset);

		// 4.2 遍历IAT
		//  IAT是一个IMAGE_THUNK_DATA结构体数组，
		//  数组是一个全0结尾。
		while (pIAT->u1.AddressOfData != 0)
		{

			// 5. 判断导入的函数，是以什么方式导入的
			//    如果是以序号导入的，则这个结构体中任何一个字段最高就是1。
			//    如果是以名称方式导入的，则这个结构体中的任何一个字段保存
			//    的是一个RVA ，这个RVA是一个指向IMAGE_IMPORT_BY_NAME结构体。
			//    IMAGE_IMPORT_BY_NAME中，保存的是导入函数的序号和名称。

			// 5.1 判断最高位是否是1

			if ((pIAT->u1.Ordinal & 0x80000000) == 1)
			{
				// 函数是以序号导入的。
				// 序号都是WORD类型，所以，只取它低16的值
// 				printf("导入序号【%d】\n", (pIAT->u1.Ordinal & 0xFFFF));
				CString tempNumber;
				tempNumber.Format(L"%d", pIAT->u1.Ordinal & 0xFFFF);
				m_treeImport.InsertItem(
					tempNumber, // 标题
					hTree,// 父节点句柄
					TVI_LAST);// 要插入的节点的排序
			}
			else
			{
				// 函数是以名称方式导入的
				// 字段保存着一个指向IMAGE_IMPORT_BY_NAME结构体的RVA
				DWORD dwFunctionNameOffset = /*换行*/
					RvaToOffset( CDiaPeAnaly::pFileData, pIAT->u1.AddressOfData);

				IMAGE_IMPORT_BY_NAME* pImportName = /*换行*/
					(IMAGE_IMPORT_BY_NAME*)((DWORD)CDiaPeAnaly::pFileData + dwFunctionNameOffset);

// 				printf("   导入名称【%s】\n", pImportName->Name);

				int nLen1 = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, pImportName->Name, -1, NULL, 0);
				if (nLen1 == 0)
				{
					MessageBox(L"格式转换出错!");
				}
				wchar_t* pDllNameResult1 = new wchar_t[nLen1];

				MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, pImportName->Name, -1, pDllNameResult1, nLen1);

				m_treeImport.InsertItem(
					pDllNameResult1, // 标题
					hTree,// 父节点句柄
					TVI_LAST);// 要插入的节点的排序
			}

			// 递增到下一个结构体
			++pIAT;
		}
		++pImportTable;
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}
