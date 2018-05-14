// DiaClean.cpp : 实现文件
//

#include "stdafx.h"
#include "windows安全卫士.h"
#include "DiaClean.h"
#include "afxdialogex.h"


// CDiaClean 对话框

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


// CDiaClean 消息处理程序

void CDiaClean::TraverseFile(TCHAR szPath[MAX_PATH],
	const CString& strFilter,
	vector<CString> &vecPath)
{
	// 向文件夹末尾添加\\*，遍历所有文件
	CString strPath = szPath;
	strPath += L"\\*";
	WIN32_FIND_DATA fi = {};
	// 找到的文件信息放到fi里面
	HANDLE hFile = FindFirstFile(strPath, &fi);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return;
	}
	do
	{
		// 判断是否当前目录或上级目录
		if (!lstrcmp(fi.cFileName, L".") || !lstrcmp(fi.cFileName, L".."))
		{
			continue;
		}
		// 判断是否是文件夹，若是，递归遍历
		if (fi.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			CString strDIr = szPath;
			strDIr += L"\\";
			strDIr += fi.cFileName;
			TraverseFile(strDIr.GetBuffer(), strFilter, vecPath);
			continue;
		}
		// 获取文件扩展名，用于判断是否垃圾文件后缀
		TCHAR *p = PathFindExtension(fi.cFileName);
		// Find返回查找的子串p在m_Delete中的索引，若找不到返回-1
		if (-1 == strFilter.Find(p))
		{
			continue;
		}
		// 经过重重筛选，终于找到要删除的文件名了，和之前的路径构造成完整路径
		CString strDIr = szPath;
		strDIr += L"\\";
		strDIr += fi.cFileName;
		// 保存进路径向量里
		vecPath.push_back(strDIr);
	} while (FindNextFile(hFile, &fi));

}


// 选择要删除的文件夹

void CDiaClean::OnBnClickedButtonopen()
{
	// TODO:  在此添加控件通知处理程序代码
	TCHAR szPath1[MAX_PATH] = {};
	BROWSEINFO bi = {};
	// 文件夹选择对话框所属窗口句柄
	bi.hwndOwner = m_hWnd;
	bi.pszDisplayName = szPath1;
	// 窗口说明信息
	bi.lpszTitle = L"打开要遍历的文件夹";
	// 对话框带编辑框
	bi.ulFlags = BIF_EDITBOX;
	LPITEMIDLIST pid;
	// 返回标识文件夹路径的“PID”，用它找到文件夹路径
	pid = SHBrowseForFolder(&bi);
	SHGetPathFromIDList(pid, szPath1);
	// 把路径更新到路径显示编辑框内
	m_strFolder = szPath1;
	// 把路径更新到路径显示ListBox内
	if (LB_ERR == m_ctlListBoxFolder.FindString(0, m_strFolder))
	{// 如果m_ListBox里面没有这个新的文件夹路径，再添加进m_ListBox
		m_ctlListBoxFolder.AddString(m_strFolder);
	}
	// 把变量内容更新到对话框
	UpdateData(FALSE);
	// 用完释放掉之前的“PID”
	CoTaskMemFree(pid);

}

//跟新后缀
void CDiaClean::OnBnClickedButtonupdate()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
}

//遍历文件夹
void CDiaClean::OnBnClickedButtonergodic()
{
	// TODO:  在此添加控件通知处理程序代码
	// 遍历之前先清空之前遍历的文件信息和列表框内容
	m_vecFileDelete.clear();
	m_ctrListBoxFile.ResetContent();
	// 获取要遍历的文件夹数目
	int nCount = m_ctlListBoxFolder.GetCount();
	for (int i = 0; i < nCount; i++)
	{
		CString test;
		// 获取ListBox控件内的文件夹路径
		m_ctlListBoxFolder.GetText(i, test);
		// 遍历文件夹内的所有文件和子文件夹
		TraverseFile(test.GetBuffer(), m_strSuffix, m_vecFileDelete);
	}
	// 将遍历到的要删除文件添加到ListBox内显示出来
	for (DWORD i = 0; i < m_vecFileDelete.size(); i++)
	{
		m_ctrListBoxFile.AddString(m_vecFileDelete[i]);
		m_ctrListBoxFile.SelectString(i, m_vecFileDelete[i]);
	}

}

//删除对应文件
void CDiaClean::OnBnClickedButtondelete()
{
	// TODO:  在此添加控件通知处理程序代码
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	UpdateData(FALSE);
	// 把垃圾文件后缀更新到窗口上

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE

}

void CDiaClean::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}

}

//系统垃圾文件
void CDiaClean::OnBnClickedButtonsys()
{
	// TODO:  在此添加控件通知处理程序代码
	m_strSuffix = L".tmp.log.chk.old.bak.gid";
	UpdateData(FALSE);
}

//浏览器垃圾
void CDiaClean::OnBnClickedButtonbrow()
{
	// TODO:  在此添加控件通知处理程序代码
	m_strSuffix = L".cache";
	UpdateData(FALSE);
}

//VS垃圾
void CDiaClean::OnBnClickedButtonvs()
{
	// TODO:  在此添加控件通知处理程序代码
	m_strSuffix = L".obj.tlog.lastbuildstate.idb.pdb.pch.res.ilk.exe.sdf.ipch.log";
	UpdateData(FALSE);
}
