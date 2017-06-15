
// SimplistAgoral_WinSDK.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CSimplistAgoral_WinSDKApp: 
// 有关此类的实现，请参阅 SimplistAgoral_WinSDK.cpp
//

class CSimplistAgoral_WinSDKApp : public CWinApp
{
public:
	CSimplistAgoral_WinSDKApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CSimplistAgoral_WinSDKApp theApp;