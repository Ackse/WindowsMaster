#pragma once
#include "DiaPeAnaly.h"
#include "afxcmn.h"

// CDialogExport �Ի���

class CDialogExport : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogExport)

public:
	CDialogExport(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDialogExport();

// �Ի�������
	enum { IDD = IDD_DIALOGEXPORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	DWORD  RvaToOffset(const void* pFileData, DWORD dwRva);
	CListCtrl m_listExport;
};
// extern IMAGE_NT_HEADERS* pNtHeader;//���α�����Ҫ
// extern BYTE  *pFileData;//Ŀ¼������Ҫ
