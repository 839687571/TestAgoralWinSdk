
// SimplistAgoral_WinSDK.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSimplistAgoral_WinSDKApp: 
// �йش����ʵ�֣������ SimplistAgoral_WinSDK.cpp
//

class CSimplistAgoral_WinSDKApp : public CWinApp
{
public:
	CSimplistAgoral_WinSDKApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSimplistAgoral_WinSDKApp theApp;