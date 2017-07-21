
// Teacher2Screen.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CTeacher2ScreenApp:
// See Teacher2Screen.cpp for the implementation of this class
//

class CTeacher2ScreenApp : public CWinApp
{
public:
	CTeacher2ScreenApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CTeacher2ScreenApp theApp;