#pragma once
// #include<vector>
// using std::vector;

// CDiaSoftUninstall �Ի���

class CDiaSoftUninstall : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaSoftUninstall)

public:
	CDiaSoftUninstall(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDiaSoftUninstall();

// �Ի�������
	enum { IDD = IDD_SOFTUNINSTALL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	typedef struct _SOFTINFO
	{
		WCHAR szSoftName[50];//�������
		WCHAR szSoftVer[50];//����汾��
		WCHAR szSoftData[20];//�����װ����
		WCHAR szSoftSize[MAX_PATH];//�����С
		WCHAR strSoftInsPath[MAX_PATH];//�����װ·��
		WCHAR strSoftUniPath[MAX_PATH];//���ж��·��
		WCHAR strSoftVenRel[50];//�����������
		WCHAR strSoftIco[MAX_PATH];//���ͼ��·��
	}SOFTINFO, *PSOFTINFO;
	
// 	vector<SOFTINFO>m_vectSoftInfo;				//������������Ϣ

	CListCtrl m_listSoftUninstall;
	virtual BOOL OnInitDialog();
	//�Ҽ�ж��
	afx_msg void OnPopupOpenUninstallPath();
	//�Ҽ���Ӧ
	afx_msg void OnRclickListsoftuninstall(NMHDR *pNMHDR, LRESULT *pResult);
// 	void TestUninstall ( vector<SOFTINFO>&m_vectSoftInfo );
// 	void TestUninstall (  );
	//ˢ��
	void FreshUninstallList ();

// 	void ListMessageBox ();

};
