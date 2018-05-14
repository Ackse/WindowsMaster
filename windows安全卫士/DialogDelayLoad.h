#pragma once


// CDialogDelayLoad 对话框

class CDialogDelayLoad : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogDelayLoad)

public:
	CDialogDelayLoad(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogDelayLoad();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOGDELAYLOAD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_editDelayLoad;
	virtual BOOL OnInitDialog ();
	DWORD  RvaToOffset ( const void* pFileData, DWORD dwRva );

};
