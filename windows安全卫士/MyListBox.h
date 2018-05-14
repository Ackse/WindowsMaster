#pragma once


// CMyListBox

class CMyListBox : public CListBox
{
	DECLARE_DYNAMIC(CMyListBox)

public:
	CMyListBox();
	virtual ~CMyListBox();

protected:
	DECLARE_MESSAGE_MAP()
public:
	void Scroll();
	afx_msg void OnPaint();
	afx_msg void OnDropFiles(HDROP hDropInfo);
};


