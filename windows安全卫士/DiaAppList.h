#pragma once
#include "afxcmn.h"


// CDiaApplist �Ի���

class CDiaApplist : public CDialogEx
{
	DECLARE_DYNAMIC ( CDiaApplist )

public:
	CDiaApplist ( CWnd* pParent = NULL );   // ��׼���캯��
	virtual ~CDiaApplist ();

	// �Ի�������
	enum
	{
		IDD = IDD_APPLIST
	};

protected:
	virtual void DoDataExchange ( CDataExchange* pDX );    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP ()
public:
	virtual BOOL OnInitDialog ();
	//EnumWindows�Ļص�����
	static BOOL CALLBACK WndEnumProc ( HWND hwnd,//�������Ĵ��ھ��
									   LPARAM lParam//EnumWindows���������Ĳ���
									   );
	static int countOfApp;
	CListCtrl m_listAppCtrl;

};
