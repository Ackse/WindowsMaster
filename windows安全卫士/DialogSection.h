#pragma once
#include "afxcmn.h"


// CDialogSection �Ի���

class CDialogSection : public CDialogEx
{
	DECLARE_DYNAMIC(CDialogSection)

public:
	CDialogSection(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDialogSection();

// �Ի�������
	enum { IDD = IDD_DIALOGSECTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listSection;
//	virtual BOOL DestroyWindow();
	virtual BOOL OnInitDialog();
};
// extern IMAGE_NT_HEADERS* pNtHeader;//���α�����Ҫ
// extern BYTE  *pFileData;//Ŀ¼������Ҫ
