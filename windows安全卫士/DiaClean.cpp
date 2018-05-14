// DiaClean.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "windows��ȫ��ʿ.h"
#include "DiaClean.h"
#include "afxdialogex.h"


// CDiaClean �Ի���

IMPLEMENT_DYNAMIC(CDiaClean, CDialogEx)

CDiaClean::CDiaClean(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDiaClean::IDD, pParent)
	, m_strFolder(_T(""))
	, m_strSuffix(_T(".obj.tlog.lastbuildstate.idb.pdb.pch.res.ilk.exe.sdf.ipch.log"))
{

}

CDiaClean::~CDiaClean()
{
}

void CDiaClean::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITFOLDER, m_strFolder);
	DDX_Text(pDX, IDC_EDITSUFFIX, m_strSuffix);
	DDX_Control(pDX, IDC_LISTFOLDER, m_ctlListBoxFolder);
	DDX_Control(pDX, IDC_LISTFILE, m_ctrListBoxFile);
}


BEGIN_MESSAGE_MAP(CDiaClean, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTONOPEN, &CDiaClean::OnBnClickedButtonopen)
	ON_BN_CLICKED(IDC_BUTTONUPDATE, &CDiaClean::OnBnClickedButtonupdate)
	ON_BN_CLICKED(IDC_BUTTONERGODIC, &CDiaClean::OnBnClickedButtonergodic)
	ON_BN_CLICKED(IDC_BUTTONDELETE, &CDiaClean::OnBnClickedButtondelete)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTONSYS, &CDiaClean::OnBnClickedButtonsys)
	ON_BN_CLICKED(IDC_BUTTONBROW, &CDiaClean::OnBnClickedButtonbrow)
	ON_BN_CLICKED(IDC_BUTTONVS, &CDiaClean::OnBnClickedButtonvs)
END_MESSAGE_MAP()


// CDiaClean ��Ϣ�������

void CDiaClean::TraverseFile(TCHAR szPath[MAX_PATH],
	const CString& strFilter,
	vector<CString> &vecPath)
{
	// ���ļ���ĩβ���\\*�����������ļ�
	CString strPath = szPath;
	strPath += L"\\*";
	WIN32_FIND_DATA fi = {};
	// �ҵ����ļ���Ϣ�ŵ�fi����
	HANDLE hFile = FindFirstFile(strPath, &fi);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return;
	}
	do
	{
		// �ж��Ƿ�ǰĿ¼���ϼ�Ŀ¼
		if (!lstrcmp(fi.cFileName, L".") || !lstrcmp(fi.cFileName, L".."))
		{
			continue;
		}
		// �ж��Ƿ����ļ��У����ǣ��ݹ����
		if (fi.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			CString strDIr = szPath;
			strDIr += L"\\";
			strDIr += fi.cFileName;
			TraverseFile(strDIr.GetBuffer(), strFilter, vecPath);
			continue;
		}
		// ��ȡ�ļ���չ���������ж��Ƿ������ļ���׺
		TCHAR *p = PathFindExtension(fi.cFileName);
		// Find���ز��ҵ��Ӵ�p��m_Delete�е����������Ҳ�������-1
		if (-1 == strFilter.Find(p))
		{
			continue;
		}
		// ��������ɸѡ�������ҵ�Ҫɾ�����ļ����ˣ���֮ǰ��·�����������·��
		CString strDIr = szPath;
		strDIr += L"\\";
		strDIr += fi.cFileName;
		// �����·��������
		vecPath.push_back(strDIr);
	} while (FindNextFile(hFile, &fi));

}


// ѡ��Ҫɾ�����ļ���

void CDiaClean::OnBnClickedButtonopen()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	TCHAR szPath1[MAX_PATH] = {};
	BROWSEINFO bi = {};
	// �ļ���ѡ��Ի����������ھ��
	bi.hwndOwner = m_hWnd;
	bi.pszDisplayName = szPath1;
	// ����˵����Ϣ
	bi.lpszTitle = L"��Ҫ�������ļ���";
	// �Ի�����༭��
	bi.ulFlags = BIF_EDITBOX;
	LPITEMIDLIST pid;
	// ���ر�ʶ�ļ���·���ġ�PID���������ҵ��ļ���·��
	pid = SHBrowseForFolder(&bi);
	SHGetPathFromIDList(pid, szPath1);
	// ��·�����µ�·����ʾ�༭����
	m_strFolder = szPath1;
	// ��·�����µ�·����ʾListBox��
	if (LB_ERR == m_ctlListBoxFolder.FindString(0, m_strFolder))
	{// ���m_ListBox����û������µ��ļ���·��������ӽ�m_ListBox
		m_ctlListBoxFolder.AddString(m_strFolder);
	}
	// �ѱ������ݸ��µ��Ի���
	UpdateData(FALSE);
	// �����ͷŵ�֮ǰ�ġ�PID��
	CoTaskMemFree(pid);

}

//���º�׺
void CDiaClean::OnBnClickedButtonupdate()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
}

//�����ļ���
void CDiaClean::OnBnClickedButtonergodic()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	// ����֮ǰ�����֮ǰ�������ļ���Ϣ���б������
	m_vecFileDelete.clear();
	m_ctrListBoxFile.ResetContent();
	// ��ȡҪ�������ļ�����Ŀ
	int nCount = m_ctlListBoxFolder.GetCount();
	for (int i = 0; i < nCount; i++)
	{
		CString test;
		// ��ȡListBox�ؼ��ڵ��ļ���·��
		m_ctlListBoxFolder.GetText(i, test);
		// �����ļ����ڵ������ļ������ļ���
		TraverseFile(test.GetBuffer(), m_strSuffix, m_vecFileDelete);
	}
	// ����������Ҫɾ���ļ���ӵ�ListBox����ʾ����
	for (DWORD i = 0; i < m_vecFileDelete.size(); i++)
	{
		m_ctrListBoxFile.AddString(m_vecFileDelete[i]);
		m_ctrListBoxFile.SelectString(i, m_vecFileDelete[i]);
	}

}

//ɾ����Ӧ�ļ�
void CDiaClean::OnBnClickedButtondelete()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	for (DWORD i = 0; i < m_vecFileDelete.size(); i++)
	{
		DeleteFile(m_vecFileDelete[i].GetBuffer());
		m_ctrListBoxFile.DeleteString(0);
	}
	m_vecFileDelete.swap(vector<CString>());

}

BOOL CDiaClean::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	UpdateData(FALSE);
	// �������ļ���׺���µ�������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE

}

void CDiaClean::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}

}

//ϵͳ�����ļ�
void CDiaClean::OnBnClickedButtonsys()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_strSuffix = L".tmp.log.chk.old.bak.gid";
	UpdateData(FALSE);
}

//���������
void CDiaClean::OnBnClickedButtonbrow()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_strSuffix = L".cache";
	UpdateData(FALSE);
}

//VS����
void CDiaClean::OnBnClickedButtonvs()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	m_strSuffix = L".obj.tlog.lastbuildstate.idb.pdb.pch.res.ilk.exe.sdf.ipch.log";
	UpdateData(FALSE);
}
