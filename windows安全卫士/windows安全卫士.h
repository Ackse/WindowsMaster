
// windows��ȫ��ʿ.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Cwindows��ȫ��ʿApp: 
// �йش����ʵ�֣������ windows��ȫ��ʿ.cpp
//

class Cwindows��ȫ��ʿApp : public CWinApp
{
public:
	Cwindows��ȫ��ʿApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern Cwindows��ȫ��ʿApp theApp;