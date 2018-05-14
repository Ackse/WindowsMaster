#pragma once


// CDialogReloc 对话框

class CDialogReloc : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogReloc)

public:
	CDialogReloc(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogReloc();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOGRELOC };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	typedef struct TypeOffset
	{
		WORD Offset : 12;  // (1) 大小为12Bit的重定位偏移
		WORD Type : 4;    // (2) 大小为4Bit的重定位信息类型值
	}TypeOffset;	    // 这个结构体是A1Pass总结的


	virtual BOOL OnInitDialog ();
	DWORD  RvaToOffset ( const void* pFileData, DWORD dwRva );
	CString m_EditReloc;
};
