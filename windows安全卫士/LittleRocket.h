#pragma once


// CLittleRocket �Ի���

class CLittleRocket : public CDialogEx
{
	DECLARE_DYNAMIC(CLittleRocket)

public:
	CLittleRocket(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLittleRocket();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ROCKET };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
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
