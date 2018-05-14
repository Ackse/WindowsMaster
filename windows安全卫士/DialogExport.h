#pragma once
#include "DiaPeAnaly.h"
#include "afxcmn.h"

// CDialogExport 对话框

class CDialogExport : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogExport)

public:
	CDialogExport(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogExport();

// 对话框数据
	enum { IDD = IDD_DIALOGEXPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	DWORD  RvaToOffset(const void* pFileData, DWORD dwRva);
	CListCtrl m_listExport;
};
// extern IMAGE_NT_HEADERS* pNtHeader;//区段表中需要
// extern BYTE  *pFileData;//目录表中需要
