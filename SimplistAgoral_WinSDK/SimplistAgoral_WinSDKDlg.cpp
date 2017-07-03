
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
#include <fstream>
#include <string>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


static int g_clientType = 0;

#define  CLIENT_STUDENT  0
#define  CLIENT_TACHER   1


#define  SET_DEVICE      1
#define  CLOSE_CLIENT    0
#define  WM_SEND_TEACHER2  WM_USER + 888
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

	ON_BN_CLICKED(IDC_BUTTON_NETWORK, &CSimplistAgoral_WinSDKDlg::OnBnClickedButtonNetwork)
	ON_BN_CLICKED(IDC_BUTTON_JOIN2, &CSimplistAgoral_WinSDKDlg::OnBnClickedButtonJoin2)
	ON_BN_CLICKED(IDC_BUTTON_LEAVE2, &CSimplistAgoral_WinSDKDlg::OnBnClickedButtonLeave2)
	ON_BN_CLICKED(ID_BUTTON_PREVIEW2, &CSimplistAgoral_WinSDKDlg::OnBnClickedButtonPreview2)
	ON_CBN_SELCHANGE(IDC_COMBOX_VINPUT2, &CSimplistAgoral_WinSDKDlg::OnCbnSelchangeComboxVinput2)
	ON_BN_CLICKED(IDCANCEL, &CSimplistAgoral_WinSDKDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CSimplistAgoral_WinSDKDlg::OnBnClickedButtonClose)
	ON_STN_DBLCLK(IDC_STATIC_LOCAL2, &CSimplistAgoral_WinSDKDlg::OnStnClickedStaticLocal2)
END_MESSAGE_MAP()

void getClientType(){

	printf("start get  getClientType\n");
	std::string appPath = CAgoralUtils::getAppPath();
	std::string configfile = appPath + "//config.ini";
	std::fstream file(configfile.c_str(), std::ios::in | std::ios::out);
	std::streamoff pos = file.tellg();

#define  MAX_ITEM_LEM  256
	char buf[MAX_ITEM_LEM];

	while (!file.eof()) {
		file.getline(buf, MAX_ITEM_LEM);
		if (file.fail()) {}

	}
	file.close();

	std::string type = buf;
	if (type == "1") {
		g_clientType = 1;  // 当前是 学生端.
		printf("student client\n");
	} else {
		printf("teacher client\n");
	}

}
// CSimplistAgoral_WinSDKDlg 消息处理程序

BOOL CSimplistAgoral_WinSDKDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	getClientType();

	CEdit  * chanelEdit = (CEdit *)GetDlgItem(IDC_EDIT_CHANNEL);
	chanelEdit->SetWindowTextW(_T("w"));

	CStatic * localCtrl = (CStatic*)GetDlgItem(IDC_STATIC_LOCAL);
	CStatic *local2Ctrl = (CStatic*)GetDlgItem(IDC_STATIC_LOCAL2);
	CStatic * remoteCtrl = (CStatic*)GetDlgItem(IDC_STATIC_REMOTE);

	m_cmbInputAudDev = (CComboBox*)GetDlgItem(IDC_COMBO_AINPUT);
	m_cmbOutputAudDev = (CComboBox*)GetDlgItem(IDC_COMBOX_AOUT);
	m_cmbVideoDev = (CComboBox*)GetDlgItem(IDC_COMBOX_VINPUT);
	m_cmbVideoDev2 = (CComboBox*)GetDlgItem(IDC_COMBOX_VINPUT2);

	m_silderVolIndicate = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS_VOL_IDC);
	m_silderVolIndicate->SetRange(0, 255);

	printf("new CAgoralWrapper; \n");
	m_pAgroObject = new CAgoralWrapper;
	m_pAgroObject->SetLocalHwnd((HWND)localCtrl->GetSafeHwnd());
	m_pAgroObject->SetLocalCamera2Hwnd(local2Ctrl->GetSafeHwnd());
	m_pAgroObject->SetRemoetHwnd((HWND)remoteCtrl->GetSafeHwnd());
	m_pAgroObject->SetMainHWND(m_hWnd);
	m_pAgroObject->SetClientType(g_clientType);
	m_pAgroObject->SetMsgObserver(this);

	std::string appPath = CAgoralUtils::getAppPath();
	appPath += "//agorl.log";
	m_pAgroObject->InitAgoral(appPath.c_str());

	m_deviceManager = new CDeviceManager;
	printf("new CDeviceManager; \n");
	m_deviceManager->InitManager(appPath.c_str());
	m_deviceManager->SetCurrentObserver(this);
	m_deviceManager->SetMsgHandleWnd(m_hWnd);

	std::vector<DevicesInfo> &audioInput = m_deviceManager->GetAudioInputDeviceLists();
	for (std::vector<DevicesInfo>::iterator iter = audioInput.begin();
		iter != audioInput.end(); iter++) {
		DevicesInfo info = *iter;

		m_cmbInputAudDev->InsertString(m_cmbInputAudDev->GetCount(), CString(info.deviceName.c_str()));
		m_cmbInputAudDev->SetCurSel(0);
		m_strInputAudDevId = info.deviceId;

	}

	std::vector<DevicesInfo> &audioOutput = m_deviceManager->GetAudioOutputDeviceLists();
	for (std::vector<DevicesInfo>::iterator iter = audioOutput.begin();
		iter != audioOutput.end(); iter++) {
		DevicesInfo info = *iter;

		m_cmbOutputAudDev->InsertString(m_cmbOutputAudDev->GetCount(), CString(info.deviceName.c_str()));
		m_cmbOutputAudDev->SetCurSel(0);
		m_strOutputAudDevId = info.deviceId;
	}

	std::vector<DevicesInfo> &videoDevs = m_deviceManager->GetVideoDeviceLists();
	for (std::vector<DevicesInfo>::iterator iter = videoDevs.begin();
		iter != videoDevs.end(); iter++) {
		DevicesInfo info = *iter;

		m_cmbVideoDev->InsertString(m_cmbVideoDev->GetCount(), CString(info.deviceName.c_str()));
		
		m_strVideoDevId = info.deviceId;
		m_cmbVideoDev2->InsertString(m_cmbVideoDev2->GetCount(), CString(info.deviceName.c_str()));
	}

	m_nCamera1Index = 0;
	m_nCamera2Index = 1;
	m_cmbVideoDev->SetCurSel(m_nCamera1Index);
	m_cmbVideoDev2->SetCurSel(m_nCamera2Index);

	m_sliderInVolume.SetRange(0, 255);
	m_sliderOutVolume.SetRange(0, 255);


	m_sliderInVolume.SetPos(m_deviceManager->GetCurrentInputVolume());
	m_sliderOutVolume.SetPos(m_deviceManager->GetCurrentInputVolume());
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
void  CSimplistAgoral_WinSDKDlg::onLastmileQuality(const void *wParam)
{
	LPAGE_LASTMILE_QUALITY lpData = (LPAGE_LASTMILE_QUALITY)wParam;
	printf("\n on quality ret  = %d\n", lpData->quality);
	///m_deviceManager->StopTestNetWork();
	delete lpData;
}

void  CSimplistAgoral_WinSDKDlg::onAudioDeviceChange(const void *wParam)
{
	LPAGE_VIDEO_DEVICE_STATE_CHANGED lpData = (LPAGE_VIDEO_DEVICE_STATE_CHANGED)wParam;

	//lpData->deviceType = deviceType;
	//lpData->deviceState = deviceState;
	m_deviceManager->UpdateDeviceList();
	printf("device state= %d type  = %d\n", lpData->deviceState, lpData->deviceType);
	/*
	UNKNOWN_AUDIO_DEVICE(-1)
	PLAYOUT_DEVICE(0)
	RECORDING_DEVICE(1)
	*/
	if (lpData->deviceType == 0) {
		UpdateAoutDev(lpData->deviceState);
	} else if(lpData->deviceType == 1)
	{
		UpdateAinDev(lpData->deviceState);
	}


	delete lpData;
}


void CSimplistAgoral_WinSDKDlg::UpdateAoutDev(int devState)
{
	/*
	AUDIO_DEVICE_STATE_ACTIVE(1)
	AUDIO_DEVICE_STATE_DISABLED(2)
	AUDIO_DEVICE_STATE_NOT_PRESENT(4)
	AUDIO_DEVICE_STATE_UNPLUGGED(8)
	AUDIO_DEVICE_STATE_ACTIVE
	*/

	m_cmbOutputAudDev->ResetContent();

	std::vector<DevicesInfo> &audioOutput = m_deviceManager->GetAudioOutputDeviceLists();
	for (std::vector<DevicesInfo>::iterator iter = audioOutput.begin();
		iter != audioOutput.end(); iter++) {
		DevicesInfo info = *iter;

		m_cmbOutputAudDev->InsertString(m_cmbOutputAudDev->GetCount(), CString(info.deviceName.c_str()));
		///m_cmbOutputAudDev->SetCurSel(0);
	}


	if (devState == 2
		|| devState == 4
		|| devState == 8) { //remove 

		BOOL aoutUpdate = TRUE;
		
		for (std::vector<DevicesInfo>::iterator iter = audioOutput.begin();
			iter != audioOutput.end(); iter++) {
			DevicesInfo info = *iter;
			if (info.deviceId == m_strOutputAudDevId) {
				aoutUpdate = FALSE;
			}
		}

		if (aoutUpdate) {
			if (audioOutput.size() > 0) {
				DevicesInfo info = audioOutput[0];
				m_deviceManager->SetCurrentAudioOutputDev(info.deviceId.c_str());
				m_strOutputAudDevId = info.deviceId;
			} else {
				m_strOutputAudDevId.clear();
			}
		}

	} else { // add 
		if (m_strOutputAudDevId.empty()) {
			if (audioOutput.size() > 0) {
				DevicesInfo info = audioOutput[0];
				m_deviceManager->SetCurrentAudioOutputDev(info.deviceId.c_str());
				m_strOutputAudDevId = info.deviceId;
			}
		}

	}
}
void CSimplistAgoral_WinSDKDlg::UpdateAinDev(int devState)
{

	m_cmbInputAudDev->ResetContent();

	std::vector<DevicesInfo> &audioInput = m_deviceManager->GetAudioInputDeviceLists();
	for (std::vector<DevicesInfo>::iterator iter = audioInput.begin();
		iter != audioInput.end(); iter++) {
		DevicesInfo info = *iter;

		m_cmbInputAudDev->InsertString(m_cmbInputAudDev->GetCount(), CString(info.deviceName.c_str()));
	}

	if (devState == 2
		|| devState == 4
		|| devState == 8) { //remove 

		BOOL aoutUpdate = TRUE;

		for (std::vector<DevicesInfo>::iterator iter = audioInput.begin();
			iter != audioInput.end(); iter++) {
			DevicesInfo info = *iter;
			if (info.deviceId == m_strInputAudDevId) {
				aoutUpdate = FALSE;
			}
		}

		if (aoutUpdate) {
			if (audioInput.size() > 0) {
				DevicesInfo info = audioInput[0];
				m_deviceManager->SetCurrentAudioOutputDev(info.deviceId.c_str());
				m_strInputAudDevId = info.deviceId;
			} else {
				m_strInputAudDevId.clear();
			}
		}

	} else { // add 
		if (m_strInputAudDevId.empty()) {
			if (audioInput.size() > 0) {
				DevicesInfo info = audioInput[0];
				m_deviceManager->SetCurrentAudioOutputDev(info.deviceId.c_str());
				m_strInputAudDevId = info.deviceId;
			}
		}

	}
}

void CSimplistAgoral_WinSDKDlg::UpdateVideoDev(int devState)
{

}

void  CSimplistAgoral_WinSDKDlg::onVideoDeviceChange(const void *wParam)
{
	m_deviceManager->UpdateDeviceList();
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
	UserType userType;
	
	userType.index = 0;

	if (g_clientType == 1) {
		userType.role = ROLE_STUDENT;
	} else {
		userType.role = ROLE_TEACHER_MAIN;
	}

	m_pAgroObject->JoinChannel("w", userType.userId, BASE_120);

	if (g_clientType != 1) {// 老师端
		std::string appPath = CAgoralUtils::getAppPath();
		appPath += "//Teacher2Screen.exe";

		int i = m_cmbVideoDev2->GetCurSel();
		char buf[10];
		sprintf_s(buf, 10,"%d", i);

		std::string title = buf;
		::ShellExecute(NULL, L"open", CAgoralUtils::StringToWString(appPath).c_str(), CAgoralUtils::StringToWString(title).c_str(), NULL, SW_SHOWNORMAL);
	}

	printf("join channel");
}


void CSimplistAgoral_WinSDKDlg::OnBnClickedLeave()
{
	CAgoraObject	*lpAgoraObject = CAgoraObject::GetAgoraObject();
	lpAgoraObject->LeaveCahnnel();
	//#32770 (Dialog)
	//Teacher2Screen
	HWND hTeacher2Wnd = ::FindWindow(NULL, L"Teacher2Screen");//

	printf("cur handle = %08X\n", hTeacher2Wnd);
	::PostMessage(hTeacher2Wnd, WM_SEND_TEACHER2, CLOSE_CLIENT, 0);

	printf("leave channel");

}



BOOL CSimplistAgoral_WinSDKDlg::PreTranslateMessage(MSG* pMsg)
{


	if (pMsg->message == WM_CLOSE) {
		//
		printf("wmclosse\n");
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
	setDevWithCursel(m_cmbVideoDev->GetCurSel());
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
	m_strInputAudDevId = info.deviceId;
}


void CSimplistAgoral_WinSDKDlg::OnCbnSelchangeComboxVinput()
{
	int index = m_cmbVideoDev2->GetCurSel();
	int  index1 = m_cmbVideoDev->GetCurSel();

	if (index1 == index) {
		m_cmbVideoDev->SetCurSel(m_nCamera1Index);
		return;
	}
	m_nCamera1Index = index1;

	setDevWithCursel(m_nCamera1Index);
}

void CSimplistAgoral_WinSDKDlg::OnCbnSelchangeComboxVinput2()
{

	int index2 = m_cmbVideoDev2->GetCurSel();
	int index1 = m_cmbVideoDev->GetCurSel();

	if (index2 == index1) {
		m_cmbVideoDev2->SetCurSel(m_nCamera2Index);
		return;
	}
	m_nCamera2Index = index2;
	PostTeacher2Msg(WM_SEND_TEACHER2, SET_DEVICE, m_nCamera2Index);

}


void CSimplistAgoral_WinSDKDlg::setDevWithCursel(int index)
{
	
	std::vector<DevicesInfo> &videoDevs = m_deviceManager->GetVideoDeviceLists();

	DevicesInfo info = videoDevs.at(index);

	printf("set cur dev id  = %s\n", info.deviceId.c_str());
	m_deviceManager->SetCurrentVidoeInputDev(info.deviceId.c_str());
}


void CSimplistAgoral_WinSDKDlg::PostTeacher2Msg(DWORD message, WPARAM wParam, LPARAM lParam)
{
	HWND hTeacher2Wnd = ::FindWindow(NULL, L"Teacher2Screen");//

	printf("cur handle = %08X\n", hTeacher2Wnd);
	::PostMessage(hTeacher2Wnd, message, wParam, lParam);
}



void CSimplistAgoral_WinSDKDlg::OnCbnSelchangeComboxAout()
{
	int index = m_cmbInputAudDev->GetCurSel();
	std::vector<DevicesInfo> &audioODevs = m_deviceManager->GetAudioOutputDeviceLists();

	DevicesInfo info = audioODevs.at(index);

	m_deviceManager->SetCurrentAudioOutputDev(info.deviceId.c_str());
	m_strOutputAudDevId = info.deviceId;
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


void CSimplistAgoral_WinSDKDlg::OnBnClickedButtonNetwork()
{
	m_deviceManager->StartTestNetWork(m_hWnd);
}


void CSimplistAgoral_WinSDKDlg::OnBnClickedButtonJoin2()
{
	
}


void CSimplistAgoral_WinSDKDlg::OnBnClickedButtonLeave2()
{
	
}



void CSimplistAgoral_WinSDKDlg::OnBnClickedButtonPreview2()
{

	setDevWithCursel(m_cmbVideoDev2->GetCurSel());


	CButton *btn = (CButton*)GetDlgItem(ID_BUTTON_PREVIEW2);
	CStatic * remoteCtrl = (CStatic*)GetDlgItem(IDC_STATIC_REMOTE);

	if (m_deviceManager->GetIsTestVideoDev()) {
		btn->SetWindowText(_T("测试"));
	} else {
		btn->SetWindowText(_T("停止"));
		remoteCtrl->Invalidate(TRUE);
	}
	m_deviceManager->TestCurrentVideoDev(remoteCtrl->GetSafeHwnd());
}


void CSimplistAgoral_WinSDKDlg::OnBnClickedCancel()
{
	INT I = 0;
	CDialogEx::OnCancel();
}


void CSimplistAgoral_WinSDKDlg::OnBnClickedButtonClose()
{
	//m_pAgroObject->LeaveChanel();
	//CDialogEx::OnCancel();

	CStatic *local2Ctrl = (CStatic*)GetDlgItem(IDC_STATIC_LOCAL2);
	local2Ctrl->Invalidate(TRUE);
}


void CSimplistAgoral_WinSDKDlg::OnStnClickedStaticLocal2()
{

	CStatic *local2Ctrl = (CStatic*)GetDlgItem(IDC_STATIC_LOCAL2);

	CRect rectParent,rectSelf;
	::GetWindowRect(m_hWnd, rectParent);
	::GetWindowRect(local2Ctrl->GetSafeHwnd(), rectSelf);

	if (rectTeacher2.left == rectTeacher2.right && rectTeacher2.left == 0) {
		CStatic *local2Ctrl = (CStatic*)GetDlgItem(IDC_STATIC_LOCAL2);
		::GetWindowRect(local2Ctrl->GetSafeHwnd(), rectTeacher2);

		CPoint pt;
		pt.x = rectTeacher2.left;
		pt.y = rectTeacher2.top;

		int w = rectTeacher2.Width();
		int h = rectTeacher2.Height();

		::ScreenToClient(m_hWnd, &pt);
		rectTeacher2.left = pt.x;
		rectTeacher2.top = pt.y;

		rectTeacher2.right = rectTeacher2.left + w;
		rectTeacher2.bottom = rectTeacher2.top + h;


	}

	if (rectSelf.Width() != rectParent.Width()) {
		///
		local2Ctrl->SetWindowPos(this, 0, 0, rectParent.Width(), rectParent.Height(), SWP_NOZORDER);
	} else {
		int nHeight = GetSystemMetrics(SM_CYCAPTION);
		//local2Ctrl->SetWindowPos(NULL, rectTeacher2.left, rectTeacher2.top - nHeight, rectTeacher2.Width(), rectTeacher2.Height(), 0);
		::SetWindowPos(local2Ctrl->GetSafeHwnd(), m_hWnd, rectTeacher2.left, rectTeacher2.top, rectTeacher2.Width(), rectTeacher2.Height(), SWP_NOZORDER);
	
	}

	printf("onclicked \n");
}


LRESULT CSimplistAgoral_WinSDKDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT ret = __super::WindowProc(message, wParam, lParam);


	return ret;
}

void CSimplistAgoral_WinSDKDlg::OnUserOffline(unsigned int userId)
{
//	throw std::logic_error("The method or operation is not implemented.");

	UserType userType;
	userType.userId = userId;
	if (g_clientType == CLIENT_TYPE_STUDENT) {//
		if (userType.role == ROLE_STUDENT) {
			CStatic * ctrl = (CStatic*)GetDlgItem(IDC_STATIC_REMOTE);
			ctrl->Invalidate(TRUE);
		} else if(userType.role == ROLE_TEACHER_MAIN){
			CStatic * ctrl = (CStatic*)GetDlgItem(IDC_STATIC_LOCAL);
			ctrl->Invalidate(TRUE);
		}

	} else if (g_clientType == CLIENT_TYPE_TEACHER) {
		if (userType.role == ROLE_TEACHER_MAIN) {
			CStatic * ctrl = (CStatic*)GetDlgItem(IDC_STATIC_LOCAL);
			ctrl->Invalidate(TRUE);
		} else if (userType.role == ROLE_STUDENT) {
			CStatic * ctrl = (CStatic*)GetDlgItem(IDC_STATIC_REMOTE);
			ctrl->Invalidate(TRUE);
		} else if (userType.role == ROLE_TEACHER_CAMERA2) {
			CStatic * ctrl = (CStatic*)GetDlgItem(IDC_STATIC_LOCAL2);
			ctrl->Invalidate(TRUE);
		}

	}
}
