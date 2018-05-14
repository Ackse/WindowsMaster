#pragma once
#include "afxcmn.h"


// CDiaApplist 对话框

class CDiaApplist : public CDialogEx
{
	DECLARE_DYNAMIC ( CDiaApplist )

public:
	CDiaApplist ( CWnd* pParent = NULL );   // 标准构造函数
	virtual ~CDiaApplist ();

	// 对话框数据
	enum
	{
		IDD = IDD_APPLIST
	};

protected:
	virtual void DoDataExchange ( CDataExchange* pDX );    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP ()
public:
	virtual BOOL OnInitDialog ();
	//EnumWindows的回调函数
	static BOOL CALLBACK WndEnumProc ( HWND hwnd,//遍历到的窗口句柄
									   LPARAM lParam//EnumWindows函数传来的参数
									   );
	static int countOfApp;
	CListCtrl m_listAppCtrl;

};
