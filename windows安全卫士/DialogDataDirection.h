#pragma once


// CDialogDataDirection �Ի���

class CDialogDataDirection : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogDataDirection)

public:
	CDialogDataDirection(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDialogDataDirection();

// �Ի�������
	enum { IDD = IDD_DIALOGDATADIRECTORY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
// 	DWORD   RvaToOffset(const void* pFileData, DWORD dwRva);

	CString m_exportRVA;	
	CString m_exportSize;

	CString m_importRVA;
	CString m_importSize;

	CString m_resourceRVA;
	CString m_resourceSize;

	CString m_baseRelocRVA;
	CString m_baseRelocSize;

	CString m_TLSRVA;
	CString m_TLSSize;

	CString m_delayRVA;
	CString m_delaySize;
};
// extern IMAGE_NT_HEADERS* pNtHeader;//���α�����Ҫ
// extern BYTE  *pFileData;//Ŀ¼������Ҫ
