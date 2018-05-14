#pragma once
#include "afxcmn.h"


// CDialogImport 对话框

class CDialogImport : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogImport)

public:
	CDialogImport(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogImport();

// 对话框数据
	enum { IDD = IDD_DIALOGIMPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CTreeCtrl m_treeImport;
	DWORD  RvaToOffset(const void* pFileData, DWORD dwRva);

};
// extern IMAGE_NT_HEADERS* pNtHeader;//区段表中需要
// extern BYTE  *pFileData;//目录表中需要
