#pragma once
#include "MyListBox.h"
#include <vector>
using std::vector;

// CDiaClean 对话框

class CDiaClean : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaClean)

public:
	CDiaClean(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDiaClean();

// 对话框数据
	enum { IDD = IDD_CLEAN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	HICON m_hIcon;

	DECLARE_MESSAGE_MAP()
public:
	vector<CString> m_vecFileDelete;
	void TraverseFile(TCHAR szPath[MAX_PATH], const CString& strFilter, vector<CString> &vecPath);


	CString m_strFolder;
	CString m_strSuffix;
	CMyListBox m_ctlListBoxFolder;
	CMyListBox m_ctrListBoxFile;

	//打开文件夹
	afx_msg void OnBnClickedButtonopen();
	//更新后缀
	afx_msg void OnBnClickedButtonupdate();
	//遍历
	afx_msg void OnBnClickedButtonergodic();
	//删除文件
	afx_msg void OnBnClickedButtondelete();
	
	afx_msg void OnPaint();
	//只清除系统垃圾
	afx_msg void OnBnClickedButtonsys();
	//只清除浏览器垃圾
	afx_msg void OnBnClickedButtonbrow();
	//只清除VS垃圾
	afx_msg void OnBnClickedButtonvs();
};
