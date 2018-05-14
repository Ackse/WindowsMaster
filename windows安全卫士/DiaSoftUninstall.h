#pragma once
// #include<vector>
// using std::vector;

// CDiaSoftUninstall 对话框

class CDiaSoftUninstall : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaSoftUninstall)

public:
	CDiaSoftUninstall(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDiaSoftUninstall();

// 对话框数据
	enum { IDD = IDD_SOFTUNINSTALL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	typedef struct _SOFTINFO
	{
		WCHAR szSoftName[50];//软件名称
		WCHAR szSoftVer[50];//软件版本号
		WCHAR szSoftData[20];//软件安装日期
		WCHAR szSoftSize[MAX_PATH];//软件大小
		WCHAR strSoftInsPath[MAX_PATH];//软件安装路径
		WCHAR strSoftUniPath[MAX_PATH];//软件卸载路径
		WCHAR strSoftVenRel[50];//软件发布厂商
		WCHAR strSoftIco[MAX_PATH];//软件图标路径
	}SOFTINFO, *PSOFTINFO;
	
// 	vector<SOFTINFO>m_vectSoftInfo;				//保存软件相关信息

	CListCtrl m_listSoftUninstall;
	virtual BOOL OnInitDialog();
	//右键卸载
	afx_msg void OnPopupOpenUninstallPath();
	//右键响应
	afx_msg void OnRclickListsoftuninstall(NMHDR *pNMHDR, LRESULT *pResult);
// 	void TestUninstall ( vector<SOFTINFO>&m_vectSoftInfo );
// 	void TestUninstall (  );
	//刷新
	void FreshUninstallList ();

// 	void ListMessageBox ();

};
