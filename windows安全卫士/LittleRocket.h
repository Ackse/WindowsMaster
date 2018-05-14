#pragma once


// CLittleRocket 对话框

class CLittleRocket : public CDialogEx
{
	DECLARE_DYNAMIC(CLittleRocket)

public:
	CLittleRocket(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLittleRocket();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ROCKET };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	CBitmap m_bitmap;
	BITMAP m_bitinfo;
	HWND hDesktopWnd;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};
