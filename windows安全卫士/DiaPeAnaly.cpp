// DiaPeAnaly.cpp : 实现文件
//

#include "stdafx.h"
#include "windows安全卫士.h"
#include "DiaPeAnaly.h"
#include "afxdialogex.h"
#include "MD5.h"
#include "DialogDataDirection.h"
#include "DialogSection.h"
#include "DialogImport.h"
#include "DialogExport.h"
#include "DialogResource.h"
#include "DialogReloc.h"
#include "DialogTls.h"
#include "DialogDelayLoad.h"

// CDiaPeAnaly 对话框
IMAGE_NT_HEADERS* CDiaPeAnaly::pNtHeader = nullptr;
BYTE* CDiaPeAnaly::pFileData = nullptr;
IMAGE_DATA_DIRECTORY* CDiaPeAnaly::pDataDirectory=nullptr;

//静态成员要先初始化
IMPLEMENT_DYNAMIC(CDiaPeAnaly, CDialogEx)

CDiaPeAnaly::CDiaPeAnaly(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDiaPeAnaly::IDD, pParent)
	, m_strFileInfo(_T(""))
	, m_addOfEntryPoint ( _T ( "" ) )
	, m_ImageBase ( _T ( "" ) )
	, m_sizeOfImage ( _T ( "" ) )
	, m_baseOfCode ( _T ( "" ) )
	, m_baseOfData ( _T ( "" ) )
	, m_sectionAlignment ( _T ( "" ) )
	, m_fileAlignment ( _T ( "" ) )
	, m_magic ( _T ( "" ) )
	, m_subSystem ( _T ( "" ) )
	, m_numberOfSections ( _T ( "" ) )
	, m_timeDate ( _T ( "" ) )
	, m_sizeOfHeader ( _T ( "" ) )
	, m_characteristics ( _T ( "" ) )
	, m_checkSum ( _T ( "" ) )
	, m_sizeOfOptionalHeader ( _T ( "" ) )
	, m_numberOfRVAAndSize ( _T ( "" ) )
	, m_sysBit ( _T ( "" ) )
{

}

CDiaPeAnaly::~CDiaPeAnaly()
{
}

void CDiaPeAnaly::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange ( pDX );
	DDX_Text ( pDX, IDC_EDITFILEINFO, m_strFileInfo );
	DDX_Text ( pDX, IDC_EDITADDOFENTRYPOINT, m_addOfEntryPoint );
	DDX_Text ( pDX, IDC_EDITIMAGEBASE, m_ImageBase );
	DDX_Text ( pDX, IDC_EDITSIZEOFIMAGE2, m_sizeOfImage );
	DDX_Text ( pDX, IDC_EDITBASEOFCODE2, m_baseOfCode );
	DDX_Text ( pDX, IDC_EDITBASEOFDATA, m_baseOfData );
	DDX_Text ( pDX, IDC_EDITSECTIONALIGNMENT, m_sectionAlignment );
	DDX_Text ( pDX, IDC_EDITFILEALIGNMENT, m_fileAlignment );
	DDX_Text ( pDX, IDC_EDITMAGIC, m_magic );
	DDX_Text ( pDX, IDC_EDITSUBSYSTEM, m_subSystem );
	DDX_Text ( pDX, IDC_EDITNUMBEROFSECTIONS, m_numberOfSections );
	DDX_Text ( pDX, IDC_EDITTIMEDATE, m_timeDate );
	DDX_Text ( pDX, IDC_EDITSIZEOFHEADER, m_sizeOfHeader );
	DDX_Text ( pDX, IDC_EDITCHARACTERISTICS, m_characteristics );
	DDX_Text ( pDX, IDC_EDITCHECKSUM, m_checkSum );
	DDX_Text ( pDX, IDC_EDITSIZEOFOPTIONALHEADER, m_sizeOfOptionalHeader );
	DDX_Text ( pDX, IDC_EDITNUMOFRVAANDSIZE, m_numberOfRVAAndSize );
	DDX_Text ( pDX, IDC_EDITSYSBIT, m_sysBit );
	DDX_Control ( pDX, IDC_BUTTONSECTIONS, m_buttonSections );
	DDX_Control ( pDX, IDC_BUTTONDATADIRECTORY, m_buttonDataDirectory );
	DDX_Control ( pDX, IDC_BUTTONEXPORT, m_buttonExport );
	DDX_Control ( pDX, IDC_BUTTONIMPORT, m_buttonImport );
	DDX_Control ( pDX, IDC_BUTTONRESOURCE, m_buttonResource );
	DDX_Control ( pDX, IDC_BUTTONRELOC, m_buttonReloc );
	DDX_Control ( pDX, IDC_BUTTONTLS, m_buttonTls );
	DDX_Control ( pDX, IDC_BUTTONDELAY, m_buttonDelayLoad );
}


BEGIN_MESSAGE_MAP(CDiaPeAnaly, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTONOPENPE, &CDiaPeAnaly::OnBnClickedButtonopenpe)
	ON_BN_CLICKED(IDC_BUTTONGETFILEINFO, &CDiaPeAnaly::OnBnClickedButtongetfileinfo)
	ON_BN_CLICKED ( IDC_BUTTONSECTIONS, &CDiaPeAnaly::OnBnClickedButtonsections )
	ON_BN_CLICKED ( IDC_BUTTONDATADIRECTORY, &CDiaPeAnaly::OnBnClickedButtondatadirectory )
	ON_BN_CLICKED ( IDC_BUTTONEXPORT, &CDiaPeAnaly::OnBnClickedButtonexport )
	ON_BN_CLICKED ( IDC_BUTTONIMPORT, &CDiaPeAnaly::OnBnClickedButtonimport )

	ON_BN_CLICKED ( IDC_BUTTONRESOURCE, &CDiaPeAnaly::OnBnClickedButtonresource )
	ON_BN_CLICKED ( IDC_BUTTONRELOC, &CDiaPeAnaly::OnBnClickedButtonreloc )
	ON_BN_CLICKED ( IDC_BUTTONTLS, &CDiaPeAnaly::OnBnClickedButtontls )
	ON_BN_CLICKED ( IDC_BUTTONDELAY, &CDiaPeAnaly::OnBnClickedButtondelay )
END_MESSAGE_MAP()


// CDiaPeAnaly 消息处理程序


//检测是否是PE文件
BOOL CDiaPeAnaly::IsPeFile(const void* pFileData)
{
	// PE文件所有的结构体都是以 IMAGE_ 开头
	//PIMAGE_DOS_HEADER => IMAGE_DOS_HEADER*
	IMAGE_DOS_HEADER* pDosHeader = (IMAGE_DOS_HEADER*)pFileData;

	// 判断第一个字段是否MZ
	if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE) {
		return FALSE;
	}

	// 判断是否是有效的NT头
	IMAGE_NT_HEADERS* pNtHeader =
		(IMAGE_NT_HEADERS*)(pDosHeader->e_lfanew + (DWORD)pDosHeader);

	if (pNtHeader->Signature != IMAGE_NT_SIGNATURE) {
		return FALSE;
	}

	return TRUE;
}

//打开PE文件
void CDiaPeAnaly::OnBnClickedButtonopenpe()
{
	// TODO:  在此添加控件通知处理程序代码
	BOOL isExistExport = FALSE;
	BOOL isExistImport = FALSE;
	BOOL isExistResource = FALSE;
	BOOL isExistBaseReloc = FALSE;
	BOOL isExistTls = FALSE;
	BOOL isExistDelay = FALSE;

	CStringA FilePathName;

	CFileDialog dlg(TRUE);
	dlg.m_ofn.lpstrTitle = L"PE";
// 	dlg.m_ofn.lpstrFilter = L"PEFile(*.exe)\0*.exe\0(.dll)\0*.dll\0AllFile(*.*)\0*.*\0\0";
	dlg.m_ofn.lpstrFilter = L"AllFile(*.exe)\0*.*\0(.dll)\0*.dll\0exe(*.exe)\0*.*\0\0";

	if (dlg.DoModal() == IDOK)
		FilePathName = dlg.GetPathName();
	// 	CFile file(FilePathName, CFile::modeRead);//只读方式打开 

	HANDLE hFile = INVALID_HANDLE_VALUE;
	hFile = CreateFileA(FilePathName,
		GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
		);
	if (hFile == INVALID_HANDLE_VALUE) 
	{
		MessageBox(L"文件不存在,或者被占用");
		return;
	}

	// 获取文件的字节数
	DWORD dwFileSize = GetFileSize(hFile, NULL);

	// 申请对应大小的缓存区来保存文件内容
	pFileData = new BYTE[dwFileSize];

	// 将文件的全部内容读取到缓冲区
	DWORD  dwReadSize = 0;
	ReadFile(hFile, pFileData, dwFileSize, &dwReadSize, NULL);
	if (dwReadSize != dwFileSize) {
		MessageBox(L"文件读取失败");
		// CloseHandle( hFile );
		// delete[ ] pFileData;
		return;
	}

	if (IsPeFile(pFileData) == FALSE) {
		MessageBox(L"不是有效的PE文件");
		return;
	}

	getAndShowPeInfo ();

	IsExist ();

// 	dlgPeHeader.DoModal();

}


void CDiaPeAnaly::getAndShowPeInfo ()
{
	// 得到DOS头
	IMAGE_DOS_HEADER* pDosHeader = (IMAGE_DOS_HEADER*)pFileData;

	// 得到Nt头（为了得到扩展头得先找到Nt头）
	// 	IMAGE_NT_HEADERS* pNtHeader =   /* 换行 */
	// 		(IMAGE_NT_HEADERS*)(pDosHeader->e_lfanew + (DWORD)(pDosHeader));
	pNtHeader = (IMAGE_NT_HEADERS*)(pDosHeader->e_lfanew + (DWORD)(pDosHeader));

	// 获取扩展头
	IMAGE_OPTIONAL_HEADER* pOptionalHeader = &pNtHeader->OptionalHeader;
	if( pOptionalHeader->Magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC )
	{
		// 		MessageBox(L"这是一个32位的PE文件");
		m_sysBit = "32位";
	}
	else if( pOptionalHeader->Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC )
	{
		// 		MessageBox(L"这是一个64位的PE文件");
		m_sysBit = "64位";
		IMAGE_OPTIONAL_HEADER64* pOptionHeader64 =  /* 换行 */
			(IMAGE_OPTIONAL_HEADER64*)pOptionalHeader;
	}

// 	IMAGE_OPTIONAL_HEADER* pOptionHeader = /*换行*/
// 		(IMAGE_OPTIONAL_HEADER*)&CDiaPeAnaly::pNtHeader->OptionalHeader;

	// 获取到数据目录表
	pDataDirectory = pOptionalHeader->DataDirectory;


	//计算PE头信息并显示出来
	// 	UpdateData(TRUE);
	DWORD addressOfEntryPoint = pOptionalHeader->AddressOfEntryPoint;
	DWORD imageBase = pOptionalHeader->ImageBase;
	DWORD sizeOfImage = pOptionalHeader->SizeOfImage;
	DWORD baseOfCode = pOptionalHeader->BaseOfCode;
	DWORD baseOfData = pOptionalHeader->BaseOfData;
	DWORD sectionAlignment = pOptionalHeader->SectionAlignment;
	DWORD fileAlignment = pOptionalHeader->FileAlignment;
	WORD magic = pOptionalHeader->Magic;

	WORD subSystem = pOptionalHeader->Subsystem;
	WORD numberOfSections = pNtHeader->FileHeader.NumberOfSections;
	DWORD timeDateStamp = pNtHeader->FileHeader.TimeDateStamp;
	DWORD sizeOfHeaders = pOptionalHeader->SizeOfHeaders;
	WORD characteristics = pNtHeader->FileHeader.Characteristics;
	DWORD checkSum = pOptionalHeader->CheckSum;
	WORD sizeOfOptionalHeader = pNtHeader->FileHeader.SizeOfOptionalHeader;
	DWORD numberOfRVAAndSizes = pOptionalHeader->NumberOfRvaAndSizes;
	//将头信息更新到对应编辑框中
	// 	CDiaPEHeader dlgPeHeader;
	m_addOfEntryPoint.Format ( L"%x", addressOfEntryPoint );
	m_ImageBase.Format ( L"%x", imageBase );
	m_sizeOfImage.Format ( L"%x", sizeOfImage );
	m_baseOfCode.Format ( L"%x", baseOfCode );
	m_baseOfData.Format ( L"%x", baseOfData );
	m_sectionAlignment.Format ( L"%x", sectionAlignment );
	m_fileAlignment.Format ( L"%x", fileAlignment );
	m_magic.Format ( L"%x", magic );

	m_subSystem.Format ( L"%x", subSystem );
	m_numberOfSections.Format ( L"%x", numberOfSections );
	m_timeDate.Format ( L"%x", timeDateStamp );
	m_sizeOfHeader.Format ( L"%x", sizeOfHeaders );
	m_characteristics.Format ( L"%x", characteristics );
	m_checkSum.Format ( L"%x", checkSum );
	m_sizeOfOptionalHeader.Format ( L"%x", sizeOfOptionalHeader );
	m_numberOfRVAAndSize.Format ( L"%x", numberOfRVAAndSizes );

	UpdateData ( FALSE );
	m_buttonDataDirectory.EnableWindow ( TRUE );
	m_buttonSections.EnableWindow ( TRUE );
}

void CDiaPeAnaly::IsExist ()
{
	//用来判断导出表是否存在
	if( pDataDirectory[0].VirtualAddress != 0 )
	{
		isExistExport = TRUE;
		m_buttonExport.EnableWindow ( TRUE );
	}
	else
	{
		m_buttonExport.EnableWindow ( FALSE );

	}
	//判断导入表
	if( pDataDirectory[1].VirtualAddress != 0 )
	{
		isExistImport = TRUE;
		m_buttonImport.EnableWindow ( TRUE );

	}
	else
	{
		m_buttonImport.EnableWindow ( FALSE );

	}
	//判断资源表
	if( pDataDirectory[2].VirtualAddress != 0 )
	{
		isExistResource = TRUE;
		m_buttonResource.EnableWindow ( TRUE );

	}
	else
	{
		m_buttonResource.EnableWindow ( FALSE );

	}
	//判断重定位表
	if( pDataDirectory[5].VirtualAddress != 0 )
	{
		isExistBaseReloc = TRUE;
		m_buttonReloc.EnableWindow ( TRUE );

	}
	else
	{
		m_buttonReloc.EnableWindow ( FALSE );

	}
	//判断TLS表
	if( pDataDirectory[9].VirtualAddress != 0 )
	{
		isExistTls = TRUE;
		m_buttonTls.EnableWindow ( TRUE );

	}
	else
	{
		m_buttonTls.EnableWindow ( FALSE );

	}
	//判断延迟加载表
	if( pDataDirectory[13].VirtualAddress != 0 )
	{
		isExistDelay = TRUE;
		m_buttonDelayLoad.EnableWindow ( TRUE );
	}
	else
	{
		m_buttonDelayLoad.EnableWindow ( FALSE );

	}
}
//获取文件信息
void CDiaPeAnaly::OnBnClickedButtongetfileinfo()
{
	// TODO:  在此添加控件通知处理程序代码
	CStringA FilePathName;
// 	char* FilePathName=nullptr;
	CFileDialog dlg(TRUE);
	dlg.m_ofn.lpstrTitle = L"选择文件";
	dlg.m_ofn.lpstrFilter = L"AllFile(*.*)\0*.*\0\0";

	if (dlg.DoModal() == IDOK)
		FilePathName = dlg.GetPathName();


	WIN32_FIND_DATAA stcFData = { 0 };

	HANDLE hFind = FindFirstFileA(FilePathName, &stcFData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		return;
	}

	SYSTEMTIME ftCreationTime = { 0 };
	SYSTEMTIME ftLastWriteTime = { 0 };

	char buf[1024] = { 0 };
	//转换为系统时间
	FileTimeToSystemTime(&stcFData.ftCreationTime, &ftCreationTime);

	FileTimeToSystemTime(&stcFData.ftCreationTime, &ftLastWriteTime);
	strcpy_s(buf, FilePathName);
	CString strMD5 = nullptr;
	strMD5 = md5FileValue(buf);

// 	printf("文件创建时间: (YYYY-MM-DD HH:MM:SS): %d-%d-%d %d:%d:%d\n", ftCreationTime.wYear, ftCreationTime.wMonth,
// 		ftCreationTime.wDay, ftCreationTime.wHour, ftCreationTime.wMinute, ftCreationTime.wSecond);
// 	m_strFileInfo = L"文件创建时间:\r\n (YYYY-MM-DD HH:MM:SS):";
// 	m_strFileInfo += ftCreationTime.wYear;
	m_strFileInfo.Format(L"文件创建时间:%d-%d-%d %d:%d:%d\r\n文件修改时间:%d-%d-%d %d:%d:%d\r\nMD5:", 
		ftCreationTime.wYear, ftCreationTime.wMonth,ftCreationTime.wDay,
		ftCreationTime.wHour, ftCreationTime.wMinute, ftCreationTime.wSecond,
		ftLastWriteTime.wYear, ftLastWriteTime.wMonth, ftLastWriteTime.wDay,
		ftLastWriteTime.wHour, ftLastWriteTime.wMinute, ftLastWriteTime.wSecond);
	m_strFileInfo += strMD5;
// 	printf("%s", strMD5);
	UpdateData(FALSE);
}



//点击打开区段表对话框及信息
void CDiaPeAnaly::OnBnClickedButtonsections ()
{
	// TODO:  在此添加控件通知处理程序代码
	CDialogSection dlgSection;
	// 	ShowWindow(SW_SHOW);
	dlgSection.DoModal ();
	ShowWindow ( SW_SHOW );
}

//点击响应目录表信息
void CDiaPeAnaly::OnBnClickedButtondatadirectory ()
{
	// TODO:  在此添加控件通知处理程序代码
	CDialogDataDirection dlgDataDirection;
	dlgDataDirection.DoModal ();
	// 	ShowWindow(SW_SHOW);
}


//解析导出表
void CDiaPeAnaly::OnBnClickedButtonexport ()
{
	// TODO:  在此添加控件通知处理程序代码
	CDialogExport dlgExport;
	dlgExport.DoModal ();
}

//导入表
void CDiaPeAnaly::OnBnClickedButtonimport ()
{
	// TODO:  在此添加控件通知处理程序代码
	CDialogImport dlgImport;
	dlgImport.DoModal ();
}

//分析资源表
void CDiaPeAnaly::OnBnClickedButtonresource ()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogResource dlgResource;
	dlgResource.DoModal ();
}

//解析重定位
void CDiaPeAnaly::OnBnClickedButtonreloc ()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogReloc dlgReloc;
	dlgReloc.DoModal ();
}

//TLS
void CDiaPeAnaly::OnBnClickedButtontls ()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogTls dlgTls;
	dlgTls.DoModal ();
}


void CDiaPeAnaly::OnBnClickedButtondelay ()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogDelayLoad dlgDelayLoad;
	dlgDelayLoad.DoModal ();
}
