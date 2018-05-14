#pragma once


// CDialogTls 对话框

class CDialogTls : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogTls)

public:
	CDialogTls(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogTls();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOGTLS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_editTls;
	virtual BOOL OnInitDialog ();
	DWORD  RvaToOffset ( const void* pFileData, DWORD dwRva );

};
