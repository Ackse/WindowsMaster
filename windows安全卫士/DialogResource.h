#pragma once


// CDialogResource �Ի���

class CDialogResource : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogResource)

public:
	CDialogResource(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDialogResource();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOGRESOURCE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog ();
	DWORD  RvaToOffset ( const void* pFileData, DWORD dwRva );

	CString m_EditResource;
};
