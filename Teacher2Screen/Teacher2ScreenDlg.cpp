
// Teacher2ScreenDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Teacher2Screen.h"
#include "Teacher2ScreenDlg.h"
#include "afxdialogex.h"
#include "resource.h"
#include <fstream>
#include <string>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define  SET_DEVICE      1
#define  CLOSE_CLIENT    0
#define  WM_SEND_TEACHER2  WM_USER + 888


#include "agoralWrapper/DeviceManager.h"
#include <vector>
#include "agoralWrapper/AgoralUtils.h"

// CTeacher2ScreenDlg dialog



CTeacher2ScreenDlg::CTeacher2ScreenDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTeacher2ScreenDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTeacher2ScreenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTeacher2ScreenDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CTeacher2ScreenDlg::OnBnClickedJoin)
	ON_BN_CLICKED(IDCANCEL, &CTeacher2ScreenDlg::OnBnClickedCancel)
	ON_BN_CLICKED(ID_SENDTEXT, &CTeacher2ScreenDlg::OnBnClickedSendtext)
END_MESSAGE_MAP()


int g_g_clientShow = 0;
void getShowWnd()
{
	std::string appPath = CAgoralUtils::getAppPath();
	std::string configfile = appPath + "//config_t2.ini";
	std::fstream file(configfile.c_str(), std::ios::in | std::ios::out);
	std::streamoff pos = file.tellg();

#define  MAX_ITEM_LEM  256
	char buf[MAX_ITEM_LEM];

	if (file.badbit || file.failbit) {
		return;
	}
	while (!file.eof()) {
		file.getline(buf, MAX_ITEM_LEM);
		if (file.fail()) {
		
		}

	}
	file.close();

	std::string type = buf;
	if (type == "1") {
		//g_g_clientShow = 1; //show
	}

}

BOOL CTeacher2ScreenDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	///getShowWnd();

	g_g_clientShow = m_bShowDlg;
	CStatic  *s_Display = (CStatic *)GetDlgItem(IDC_STATIC_TEACHER2);

	m_pAgroObject = new CAgoralWrapper;
	m_pAgroObject->SetLocalCamera2Hwnd(s_Display->GetSafeHwnd());
	m_pAgroObject->SetBottomWnd(NULL);
	m_pAgroObject->SetMainHWND(m_hWnd);
	m_pAgroObject->SetAudioEnabled(FALSE);
	m_pAgroObject->SetClientType(3);

	std::string appPath = CAgoralUtils::getAppPath();
	appPath += "//agorl2.log";
	m_pAgroObject->InitAgoral(appPath.c_str());

	m_deviceManager = CDeviceManager::GetInstance();
	m_deviceManager->InitManager(appPath.c_str());
	m_deviceManager->SetMsgHandleWnd(m_hWnd);

	std::vector<DevicesInfo> &videoDevs = m_deviceManager->GetVideoDeviceLists();
	for (std::vector<DevicesInfo>::iterator iter = videoDevs.begin();
		iter != videoDevs.end(); iter++) {
		DevicesInfo info = *iter;

		//m_cmbVideoDev->InsertString(m_cmbVideoDev->GetCount(), CString(info.deviceName.c_str()));
		
		//m_strVideoDevId = info.deviceId;
		//m_cmbVideoDev2->InsertString(m_cmbVideoDev2->GetCount(), CString(info.deviceName.c_str()));
	}

	///::SetWindowPos(m_hWnd, NULL, 0, 0, 1, 1, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);

	OnBnClickedJoin();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTeacher2ScreenDlg::OnPaint()
{
	CDialogEx::OnPaint();
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTeacher2ScreenDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


LRESULT CTeacher2ScreenDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_NCPAINT) {
		if (g_g_clientShow == 0) {
			ShowWindow(SW_HIDE);
			return 0;
			//::SetWindowPos(m_hWnd, NULL, 0, 0, 1, 1, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
		}
	
	} 
	return CDialogEx::WindowProc(message, wParam, lParam);
}


void CTeacher2ScreenDlg::OnBnClickedJoin()
{
	setDevAtIndex(m_nStartIndex);

	printf("start device id = %d",m_nStartIndex);
	printf("start cur profile = %d\n", m_nStartProfile);
	UserInfo userType;
	userType.role = ROLE_TEACHER_CAMERA2;
	userType.index = 0;

	m_pAgroObject->JoinChannel("w", userType.userId, m_nStartProfile);
}


void CTeacher2ScreenDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here

	m_pAgroObject->LeaveChanel();
	CDialogEx::OnCancel();
}


void CTeacher2ScreenDlg::OnBnClickedSendtext()
{
	m_pAgroObject->SendChatMessage("i am teacher 2");
}


BOOL CTeacher2ScreenDlg::PreTranslateMessage(MSG* pMsg)
{

	if (pMsg->message == WM_SEND_TEACHER2) {
		if (pMsg->wParam == CLOSE_CLIENT) {
			OnBnClickedCancel();
		} else if(pMsg->wParam == SET_DEVICE){
			int devIndex = (int)pMsg->lParam;
			setDevAtIndex(devIndex);
		}
		return TRUE;
	} 

	if (m_pAgroObject != NULL) {
		m_pAgroObject->MsgHandle(pMsg->message, pMsg->wParam);
	}
	if (m_deviceManager != NULL) {
		AgoralMsgHandle(pMsg->message, pMsg->wParam);
	}


	return CDialogEx::PreTranslateMessage(pMsg);
}

void CTeacher2ScreenDlg::AgoralMsgHandle(DWORD msgId, WPARAM wParam)
{
	switch (msgId) {
	case WM_MSGID(EID_AUDIO_VOLUME_INDICATION):
		onAudioVolumIndication((void*)wParam);
		break;
	case WM_MSGID(EID_LASTMILE_QUALITY):
		onLastmileQuality((void*)wParam);
		break;
	case WM_MSGID(EID_AUDIO_DEVICE_STATE_CHANGED):
		//onAudioDeviceChange((void*)wParam);
		break;
	case WM_MSGID(EID_VIDEO_DEVICE_STATE_CHANGED):
		///onVideoDeviceChange((void*)wParam);
		break;
	default:
		break;
	}

}

void CTeacher2ScreenDlg::onAudioVolumIndication(const void *param)
{
	LPAGE_AUDIO_VOLUME_INDICATION lpData = (LPAGE_AUDIO_VOLUME_INDICATION)param;
	//m_silderVolIndicate->SetPos(lpData->totalVolume);


	std::string devid = m_deviceManager->GetCurrentUseMicDevId();

	printf("\n vol = %d number =%d devid  = %s\n", lpData->totalVolume, lpData->speakerNumber, devid.c_str());
	delete lpData;
}
void  CTeacher2ScreenDlg::onLastmileQuality(const void *wParam)
{
	LPAGE_LASTMILE_QUALITY lpData = (LPAGE_LASTMILE_QUALITY)wParam;
	printf("\n on quality ret  = %d\n", lpData->quality);
	///m_deviceManager->StopTestNetWork();
	delete lpData;
}

void CTeacher2ScreenDlg::setDevAtIndex(int index)
{
	std::vector<DevicesInfo> &videoDevs = m_deviceManager->GetVideoDeviceLists();

	DevicesInfo info = videoDevs.at(index);

	printf("set cur dev id  = %s\n", info.deviceId.c_str());
	m_deviceManager->SetCurrentVidoeInputDev(info.deviceId.c_str());
}