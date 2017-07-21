
// Teacher2Screen.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Teacher2Screen.h"
#include "Teacher2ScreenDlg.h"
#include "agoralWrapper/AgoralWrapper.h"
#include "agoralWrapper/AgoralUtils.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTeacher2ScreenApp

BEGIN_MESSAGE_MAP(CTeacher2ScreenApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CTeacher2ScreenApp construction

CTeacher2ScreenApp::CTeacher2ScreenApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CTeacher2ScreenApp object

CTeacher2ScreenApp theApp;


int  g_cameraIndex = 1;

// CTeacher2ScreenApp initialization

BOOL CTeacher2ScreenApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	LPWSTR *szArgList;
	int argCount;
	LPTSTR  str = GetCommandLineW();
	//wchar_t *wstr = cctryAnsiCharToWide(str);
	szArgList = CommandLineToArgvW(str, &argCount);

	char buf[256];
	std::wstring strwDevIndex, strwProfile,strShowDlg;
	if (argCount < 4) {
		strwDevIndex = L"No input";
		if (argCount == 3) {
			strwDevIndex = szArgList[2];
		}
		sprintf(buf, "%d", argCount);
	} else {
		strwDevIndex = szArgList[1]; 
		strwProfile = szArgList[2];
		strShowDlg = szArgList[3];
	}



	g_cameraIndex = atoi(CAgoralUtils::WStringToString(strwDevIndex).c_str());

	int profile = atoi(CAgoralUtils::WStringToString(strwProfile).c_str());
	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	CShellManager *pShellManager = new CShellManager;

	// Activate "Windows Native" visual manager for enabling themes in MFC controls
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	SetRegistryKey(_T("Local AppWizard-Generated Applications"));


	CAgoralWrapper::GlobalInit(APP_ID);

	CTeacher2ScreenDlg dlg;
	dlg.SetTartIndex(g_cameraIndex);
	dlg.SetTeache2Profile(profile);
	if (strShowDlg == L"1") {
		dlg.m_bShowDlg = TRUE;
	} else {
		dlg.m_bShowDlg = FALSE;
	}

	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();


	// Delete the shell manager created above.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	return FALSE;
}

