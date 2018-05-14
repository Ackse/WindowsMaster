#pragma once
#include "afxwin.h"


// CDiaPeAnaly �Ի���

class CDiaPeAnaly : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaPeAnaly)

public:
	CDiaPeAnaly(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDiaPeAnaly();

// �Ի�������
	enum { IDD = IDD_PEANALY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	//��PE
	afx_msg void OnBnClickedButtonopenpe();
	//��ȡ�ļ���Ϣ
	afx_msg void OnBnClickedButtongetfileinfo ();
	//���������Ӧ
	afx_msg void OnBnClickedButtonsections ();
	//����Ŀ¼
	afx_msg void OnBnClickedButtondatadirectory ();
	//������
	afx_msg void OnBnClickedButtonexport ();
	//�����
	afx_msg void OnBnClickedButtonimport ();
	//��Դ��
	afx_msg void OnBnClickedButtonresource ();
	//�ض�λ
	afx_msg void OnBnClickedButtonreloc ();
	//TLS��
	afx_msg void OnBnClickedButtontls ();
	//�ӳټ��ر�
	afx_msg void OnBnClickedButtondelay ();
	//��ȡPE��Ϣ
	void getAndShowPeInfo ();
	//�������Ƿ����
	void IsExist ();
	//�ж��Ƿ�ΪPE
	BOOL IsPeFile(const void* pFileData);
	static IMAGE_NT_HEADERS* pNtHeader;//���α�����Ҫ
	static BYTE  *pFileData;//Ŀ¼������Ҫ
	static IMAGE_DATA_DIRECTORY* pDataDirectory;
// 	friend class CDiaPEHeader;

	CString m_strFileInfo;

	CString m_addOfEntryPoint;
	CString m_ImageBase;
	CString m_sizeOfImage;
	CString m_baseOfCode;
	CString m_baseOfData;
	CString m_sectionAlignment;
	CString m_fileAlignment;
	CString m_magic;
	CString m_subSystem;
	CString m_numberOfSections;
	CString m_timeDate;
	CString m_sizeOfHeader;
	CString m_characteristics;
	CString m_checkSum;
	CString m_sizeOfOptionalHeader;
	CString m_numberOfRVAAndSize;
	CString m_sysBit;

	BOOL isExistExport=FALSE;
	BOOL isExistImport=FALSE;
	BOOL isExistResource=FALSE;
	BOOL isExistBaseReloc=FALSE;
	BOOL isExistTls=FALSE;
	BOOL isExistDelay = FALSE;
	CButton m_buttonSections;
	CButton m_buttonDataDirectory;
	CButton m_buttonExport;
	CButton m_buttonImport;
	CButton m_buttonResource;
	CButton m_buttonReloc;
	CButton m_buttonTls;
	CButton m_buttonDelayLoad;
};