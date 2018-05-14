#pragma once
#include "afxcmn.h"


// CDialogSection 对话框

class CDialogSection : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogSection)

public:
	CDialogSection(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogSection();

// 对话框数据
	enum { IDD = IDD_DIALOGSECTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listSection;
//	virtual BOOL DestroyWindow();
	virtual BOOL OnInitDialog();
};
// extern IMAGE_NT_HEADERS* pNtHeader;//区段表中需要
// extern BYTE  *pFileData;//目录表中需要
