
// SimplistAgoral_WinSDKDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SimplistAgoral_WinSDK.h"
#include "SimplistAgoral_WinSDKDlg.h"
#include "afxdialogex.h"
#include "resource.h"
#include "agoralWrapper/DeviceManager.h"
#include <vector>
#include "agoralWrapper/AgoralUtils.h"

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

}
void CSimplistAgoral_WinSDKDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_SLIDER_AIN, m_sliderInVolume);
	DDX_Control(pDX, IDC_SLIDER_AOUT, m_sliderOutVolume);
}

BEGIN_MESSAGE_MAP(CSimplistAgoral_WinSDKDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CSimplistAgoral_WinSDKDlg::OnBnClickedJoin)
	ON_BN_CLICKED(IDC_BUTTON_LEAVE, &CSimplistAgoral_WinSDKDlg::OnBnClickedLeave)
	ON_BN_CLICKED(IDC_CHECK1, &CSimplistAgoral_WinSDKDlg::OnBnClickedCheck)
	ON_BN_CLICKED(ID_BUTTON_PREVIEW, &CSimplistAgoral_WinSDKDlg::OnBnClickedButtonPreview)
	ON_BN_CLICKED(ID_BUTTON_TEST_AIN, &CSimplistAgoral_WinSDKDlg::OnBnClickedButtonTestAin)
	ON_BN_CLICKED(ID_BUTTON_TEST_AOUT, &CSimplistAgoral_WinSDKDlg::OnBnClickedButtonTestAout)
	ON_CBN_SELCHANGE(IDC_COMBO_AINPUT, &CSimplistAgoral_WinSDKDlg::OnCbnSelchangeComboAinput)
	ON_CBN_SELCHANGE(IDC_COMBOX_VINPUT, &CSimplistAgoral_WinSDKDlg::OnCbnSelchangeComboxVinput)
	ON_CBN_SELCHANGE(IDC_COMBOX_AOUT, &CSimplistAgoral_WinSDKDlg::OnCbnSelchangeComboxAout)
	ON_WM_HSCROLL()

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

	m_cmbInputAudDev = (CComboBox*)GetDlgItem(IDC_COMBO_AINPUT);
	m_cmbOutputAudDev = (CComboBox*)GetDlgItem(IDC_COMBOX_AOUT);
	m_cmbVideoDev = (CComboBox*)GetDlgItem(IDC_COMBOX_VINPUT);
	m_silderVolIndicate = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS_VOL_IDC);
	m_silderVolIndicate->SetRange(0, 255);

	m_pAgroObject = new CAgoralWrapper;
	m_pAgroObject->SetLocalHwnd((HWND)localCtrl->GetSafeHwnd());
	m_pAgroObject->SetRemoetHwnd((HWND)remoteCtrl->GetSafeHwnd());
	m_pAgroObject->SetMainHWND(m_hWnd);

	std::string appPath = CAgoralUtils::getAppPath();
	appPath += "//agorl.log";
	m_pAgroObject->InitAgoral(appPath.c_str());

	m_deviceManager = new CDeviceManager;

	m_deviceManager->InitManager(appPath.c_str());
	m_deviceManager->SetCurrentObserver(this);
	m_deviceManager->SetMsgHandleWnd(m_hWnd);

	std::vector<DevicesInfo> &audioInput = m_deviceManager->GetAudioInputDeviceLists();
	for (std::vector<DevicesInfo>::iterator iter = audioInput.begin();
		iter != audioInput.end(); iter++) {
		DevicesInfo info = *iter;

		m_cmbInputAudDev->InsertString(m_cmbVideoDev->GetCount(), CString(info.deviceName.c_str()));
		m_cmbInputAudDev->SetCurSel(0);

	}

	std::vector<DevicesInfo> &audioOutput = m_deviceManager->GetAudioOutputDeviceLists();
	for (std::vector<DevicesInfo>::iterator iter = audioOutput.begin();
		iter != audioOutput.end(); iter++) {
		DevicesInfo info = *iter;

		m_cmbOutputAudDev->InsertString(m_cmbVideoDev->GetCount(), CString(info.deviceName.c_str()));
		m_cmbOutputAudDev->SetCurSel(0);
	}

	std::vector<DevicesInfo> &videoDevs = m_deviceManager->GetVideoDeviceLists();
	for (std::vector<DevicesInfo>::iterator iter = videoDevs.begin();
		iter != videoDevs.end(); iter++) {
		DevicesInfo info = *iter;

		m_cmbVideoDev->InsertString(m_cmbVideoDev->GetCount(), CString(info.deviceName.c_str()));
		m_cmbVideoDev->SetCurSel(0);
	}


	m_sliderInVolume.SetRange(0, 255);
	m_sliderOutVolume.SetRange(0, 255);


	m_sliderInVolume.SetPos(200);
	m_sliderOutVolume.SetPos(200);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE	
}


void CSimplistAgoral_WinSDKDlg::onAudioVolumIndication(const void *param)
{
	LPAGE_AUDIO_VOLUME_INDICATION lpData = (LPAGE_AUDIO_VOLUME_INDICATION)param;
	m_silderVolIndicate->SetPos(lpData->totalVolume);


	std::string devid = m_deviceManager->GetCurrentUseAudioInputDevId();

	printf("\n vol = %d number =%d devid  = %s\n", lpData->totalVolume, lpData->speakerNumber, devid.c_str());
	delete lpData;
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
	m_pAgroObject->JoinChannel("w");
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

	if (pMsg->message == WM_COMMAND) {
		int i = 0;
	}
	if (m_pAgroObject != NULL) {
		m_pAgroObject->MsgHandle(pMsg->message, pMsg->wParam);
	}
	if (m_deviceManager != NULL) {
		m_deviceManager->MsgHandle(pMsg->message, pMsg->wParam);
	}
	
	
	return CDialogEx::PreTranslateMessage(pMsg);
}



void CSimplistAgoral_WinSDKDlg::OnBnClickedCheck()
{
	// TODO: Add your control notification handler code here
}




void CSimplistAgoral_WinSDKDlg::OnBnClickedButtonPreview()
{
	CButton *btn = (CButton*)GetDlgItem(ID_BUTTON_PREVIEW);
	CStatic * remoteCtrl = (CStatic*)GetDlgItem(IDC_STATIC_REMOTE);

	if (m_deviceManager->GetIsTestVideoDev()) {
		btn->SetWindowText(_T("测试"));
	} else {
		btn->SetWindowText(_T("停止"));
		remoteCtrl->Invalidate(TRUE);
	}
	m_deviceManager->TestCurrentVideoDev(remoteCtrl->GetSafeHwnd());
	
}


void CSimplistAgoral_WinSDKDlg::OnBnClickedButtonTestAin()
{

	CButton *btn = (CButton*)GetDlgItem(ID_BUTTON_TEST_AIN);

	if (m_deviceManager->GetIsTestAudioInDev()) {
		btn->SetWindowText(_T("测试"));
	} else {
		btn->SetWindowText(_T("停止"));
	}
	m_silderVolIndicate->SetPos(0);
	m_deviceManager->TestCurrentAudioInputDev();
}


void CSimplistAgoral_WinSDKDlg::OnBnClickedButtonTestAout()
{
	TCHAR	szWavPath[MAX_PATH];

	::GetModuleFileName(NULL, szWavPath, MAX_PATH);
	LPTSTR lpLastSlash = (LPTSTR)_tcsrchr(szWavPath, _T('\\')) + 1;
	_tcscpy_s(lpLastSlash, 16, _T("test.wav"));

	std::string str = CStringA(szWavPath);
	
	CButton *btn = (CButton*)GetDlgItem(ID_BUTTON_TEST_AOUT);
	if (m_deviceManager->GetIsTestAudioOutDev()) {
		btn->SetWindowText(_T("测试"));
	} else {
		btn->SetWindowText(_T("停止"));
	}
	m_deviceManager->TestCurrentAudioOutDev(str.c_str());
}



void CSimplistAgoral_WinSDKDlg::OnCbnSelchangeComboAinput()
{
	int index = m_cmbInputAudDev->GetCurSel();
	std::vector<DevicesInfo> &audioDevs = m_deviceManager->GetAudioInputDeviceLists();

	DevicesInfo info = audioDevs.at(index);

	printf("set cur dev id  = %s\n", info.deviceId.c_str());
	m_deviceManager->SetCurrentAudioInputDev(info.deviceId.c_str());
}


void CSimplistAgoral_WinSDKDlg::OnCbnSelchangeComboxVinput()
{
	int index = m_cmbVideoDev->GetCurSel();
	std::vector<DevicesInfo> &videoDevs = m_deviceManager->GetVideoDeviceLists();

	DevicesInfo info = videoDevs.at(index);

	printf("set cur dev id  = %s\n", info.deviceId.c_str());
	m_deviceManager->SetCurrentVidoeInputDev(info.deviceId.c_str());
}


void CSimplistAgoral_WinSDKDlg::OnCbnSelchangeComboxAout()
{
	int index = m_cmbInputAudDev->GetCurSel();
	std::vector<DevicesInfo> &audioODevs = m_deviceManager->GetAudioOutputDeviceLists();

	DevicesInfo info = audioODevs.at(index);

	m_deviceManager->SetCurrentAudioOutputDev(info.deviceId.c_str());
}


afx_msg void CSimplistAgoral_WinSDKDlg::OnHScroll(
	UINT nSBCode,
	UINT nPos,
	CScrollBar* pScrollBar
	)
{

// 	CAGSliderCtrl  *slider = (CScrollBar*)pScrollBar;
// 	int pos = slider->GetPos();
// 	printf("set volum = %d\n", pos);
// 


	// Get the minimum and maximum scroll-bar positions.
	int minpos;
	int maxpos;
	pScrollBar->GetScrollRange(&minpos, &maxpos);
	maxpos = pScrollBar->GetScrollLimit();
	// Get the current position of scroll box.
	int curpos = pScrollBar->GetScrollPos();
	// Determine the new position of scroll box.
	switch (nSBCode) {
	case SB_LEFT:      // Scroll to far left.
		curpos = minpos;
		break;
	case SB_RIGHT:      // Scroll to far right.
		curpos = maxpos;
		break;
	case SB_ENDSCROLL:   // End scroll.
		break;
	case SB_LINELEFT:      // Scroll left.
		if (curpos > minpos)
			curpos--;
		break;
	case SB_LINERIGHT:   // Scroll right.
		if (curpos < maxpos)
			curpos++;
		break;
	case SB_PAGELEFT:    // Scroll one page left.
	{
		// Get the page size. 
		SCROLLINFO   info;
		pScrollBar->GetScrollInfo(&info, SIF_ALL);

		if (curpos > minpos)
			curpos = max(minpos, curpos - (int)info.nPage);
	}
	break;
	case SB_PAGERIGHT:      // Scroll one page right.
	{
		// Get the page size. 
		SCROLLINFO   info;
		pScrollBar->GetScrollInfo(&info, SIF_ALL);
		if (curpos < maxpos)
			curpos = min(maxpos, curpos + (int)info.nPage);
	}
	break;
	case SB_THUMBPOSITION: // Scroll to absolute position. nPos is the position
		curpos = nPos;      // of the scroll box at the end of the drag operation.
		break;
	case SB_THUMBTRACK:   // Drag scroll box to specified position. nPos is the
		curpos = nPos;     // position that the scroll box has been dragged to.
		break;
	}
	// Set the new position of the thumb (scroll box).
	pScrollBar->SetScrollPos(curpos);


	printf("set volum = %d\n", curpos);

	
	if (pScrollBar->GetDlgCtrlID() == IDC_SLIDER_AIN) {

		if (m_deviceManager != NULL) {
			if (curpos > 0)
			m_deviceManager->SetCurrentInputVolume(curpos);
		}
	} else if (pScrollBar->GetDlgCtrlID() == IDC_SLIDER_AOUT) {
		if (m_deviceManager != NULL) {
			if (curpos > 0)
			m_deviceManager->SetCurrentOutputVolume(curpos);
		} 
	}


	
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}
