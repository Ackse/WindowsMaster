#pragma once
#include "afxcmn.h"


// CDialogImport �Ի���

class CDialogImport : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogImport)

public:
	CDialogImport(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDialogImport();

// �Ի�������
	enum { IDD = IDD_DIALOGIMPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CTreeCtrl m_treeImport;
	DWORD  RvaToOffset(const void* pFileData, DWORD dwRva);

};
// extern IMAGE_NT_HEADERS* pNtHeader;//���α�����Ҫ
// extern BYTE  *pFileData;//Ŀ¼������Ҫ
