#pragma once
#include "afxwin.h"


// CDiaPeAnaly 对话框

class CDiaPeAnaly : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaPeAnaly)

public:
	CDiaPeAnaly(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDiaPeAnaly();

// 对话框数据
	enum { IDD = IDD_PEANALY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	//打开PE
	afx_msg void OnBnClickedButtonopenpe();
	//获取文件信息
	afx_msg void OnBnClickedButtongetfileinfo ();
	//点击区段响应
	afx_msg void OnBnClickedButtonsections ();
	//数据目录
	afx_msg void OnBnClickedButtondatadirectory ();
	//导出表
	afx_msg void OnBnClickedButtonexport ();
	//导入表
	afx_msg void OnBnClickedButtonimport ();
	//资源表
	afx_msg void OnBnClickedButtonresource ();
	//重定位
	afx_msg void OnBnClickedButtonreloc ();
	//TLS表
	afx_msg void OnBnClickedButtontls ();
	//延迟加载表
	afx_msg void OnBnClickedButtondelay ();
	//获取PE信息
	void getAndShowPeInfo ();
	//各区段是否存在
	void IsExist ();
	//判断是否为PE
	BOOL IsPeFile(const void* pFileData);
	static IMAGE_NT_HEADERS* pNtHeader;//区段表中需要
	static BYTE  *pFileData;//目录表中需要
	static IMAGE_DATA_DIRECTORY* pDataDirectory;
// 	friend class CDiaPEHeader;

	CString m_strFileInfo;

	CString m_addOfEntryPoint;
	CString m_ImageBase;
	CString m_sizeOfImage;
	CString m_baseOfCode;
	CString m_baseOfData;
	CString m_sectionAlignment;
	CString m_fileAlignment;
	CString m_magic;
	CString m_subSystem;
	CString m_numberOfSections;
	CString m_timeDate;
	CString m_sizeOfHeader;
	CString m_characteristics;
	CString m_checkSum;
	CString m_sizeOfOptionalHeader;
	CString m_numberOfRVAAndSize;
	CString m_sysBit;

	BOOL isExistExport=FALSE;
	BOOL isExistImport=FALSE;
	BOOL isExistResource=FALSE;
	BOOL isExistBaseReloc=FALSE;
	BOOL isExistTls=FALSE;
	BOOL isExistDelay = FALSE;
	CButton m_buttonSections;
	CButton m_buttonDataDirectory;
	CButton m_buttonExport;
	CButton m_buttonImport;
	CButton m_buttonResource;
	CButton m_buttonReloc;
	CButton m_buttonTls;
	CButton m_buttonDelayLoad;
};