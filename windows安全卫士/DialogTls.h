#pragma once


// CDialogTls �Ի���

class CDialogTls : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogTls)

public:
	CDialogTls(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDialogTls();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOGTLS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_editTls;
	virtual BOOL OnInitDialog ();
	DWORD  RvaToOffset ( const void* pFileData, DWORD dwRva );

};
