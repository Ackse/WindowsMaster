#pragma once


// CDialogReloc �Ի���

class CDialogReloc : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogReloc)

public:
	CDialogReloc(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDialogReloc();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOGRELOC };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	typedef struct TypeOffset
	{
		WORD Offset : 12;  // (1) ��СΪ12Bit���ض�λƫ��
		WORD Type : 4;    // (2) ��СΪ4Bit���ض�λ��Ϣ����ֵ
	}TypeOffset;	    // ����ṹ����A1Pass�ܽ��


	virtual BOOL OnInitDialog ();
	DWORD  RvaToOffset ( const void* pFileData, DWORD dwRva );
	CString m_EditReloc;
};
