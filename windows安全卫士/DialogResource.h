#pragma once


// CDialogResource 对话框

class CDialogResource : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogResource)

public:
	CDialogResource(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogResource();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOGRESOURCE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog ();
	DWORD  RvaToOffset ( const void* pFileData, DWORD dwRva );

	CString m_EditResource;
};
