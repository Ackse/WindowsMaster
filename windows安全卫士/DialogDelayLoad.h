#pragma once


// CDialogDelayLoad �Ի���

class CDialogDelayLoad : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogDelayLoad)

public:
	CDialogDelayLoad(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDialogDelayLoad();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOGDELAYLOAD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_editDelayLoad;
	virtual BOOL OnInitDialog ();
	DWORD  RvaToOffset ( const void* pFileData, DWORD dwRva );

};
