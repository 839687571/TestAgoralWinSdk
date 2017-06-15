
// SimplistAgoral_WinSDKDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SimplistAgoral_WinSDK.h"
#include "SimplistAgoral_WinSDKDlg.h"
#include "afxdialogex.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSimplistAgoral_WinSDKDlg 对话框



CSimplistAgoral_WinSDKDlg::CSimplistAgoral_WinSDKDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSimplistAgoral_WinSDKDlg::IDD, pParent)
{
	m_pAgroObject = NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CSimplistAgoral_WinSDKDlg::~CSimplistAgoral_WinSDKDlg()
{
	CAgoraObject::CloseAgoraObject();
}
void CSimplistAgoral_WinSDKDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSimplistAgoral_WinSDKDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CSimplistAgoral_WinSDKDlg::OnBnClickedJoin)
	ON_BN_CLICKED(IDCANCEL, &CSimplistAgoral_WinSDKDlg::OnBnClickedLeave)
END_MESSAGE_MAP()


// CSimplistAgoral_WinSDKDlg 消息处理程序

BOOL CSimplistAgoral_WinSDKDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码

	CEdit  * chanelEdit = (CEdit *)GetDlgItem(IDC_EDIT_CHANNEL);
	chanelEdit->SetWindowTextW(_T("w"));

	CStatic * localCtrl = (CStatic*)GetDlgItem(IDC_STATIC_LOCAL);
	CStatic * remoteCtrl = (CStatic*)GetDlgItem(IDC_STATIC_REMOTE);

	m_pAgroObject = new CAgoralWrapper;
	m_pAgroObject->setLocalHwnd((HWND)localCtrl->GetSafeHwnd());
	m_pAgroObject->setRemoetHwnd((HWND)remoteCtrl->GetSafeHwnd());
	m_pAgroObject->setMainHWND(m_hWnd);
	m_pAgroObject->setMainHWND(m_hWnd);
	m_pAgroObject->initAgoral();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE	
}


void CSimplistAgoral_WinSDKDlg::OnPaint()
{
	CDialogEx::OnPaint();
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSimplistAgoral_WinSDKDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSimplistAgoral_WinSDKDlg::OnBnClickedJoin()
{
	
	m_pAgroObject->joinChannel("w");
	printf("join channel");
}


void CSimplistAgoral_WinSDKDlg::OnBnClickedLeave()
{
	CAgoraObject	*lpAgoraObject = CAgoraObject::GetAgoraObject();
	lpAgoraObject->LeaveCahnnel();
	printf("leave channel");
}



BOOL CSimplistAgoral_WinSDKDlg::PreTranslateMessage(MSG* pMsg)
{
	if (m_pAgroObject != NULL) {
		m_pAgroObject->MsgHandle(pMsg->message, pMsg->wParam);
	}
	
	return CDialogEx::PreTranslateMessage(pMsg);
}

