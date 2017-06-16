
// SimplistAgoral_WinSDKDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SimplistAgoral_WinSDK.h"
#include "SimplistAgoral_WinSDKDlg.h"
#include "afxdialogex.h"
#include "resource.h"
#include "agoralWrapper/DeviceManager.h"
#include <vector>
#include "agoralWrapper/Utils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSimplistAgoral_WinSDKDlg �Ի���



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
	ON_BN_CLICKED(IDC_CHECK1, &CSimplistAgoral_WinSDKDlg::OnBnClickedCheck)
	ON_BN_CLICKED(ID_BUTTON_PREVIEW, &CSimplistAgoral_WinSDKDlg::OnBnClickedButtonPreview)
	ON_BN_CLICKED(ID_BUTTON_TEST_AIN, &CSimplistAgoral_WinSDKDlg::OnBnClickedButtonTestAin)
	ON_BN_CLICKED(ID_BUTTON_TEST_AOUT, &CSimplistAgoral_WinSDKDlg::OnBnClickedButtonTestAout)
	ON_CBN_SELCHANGE(IDC_COMBO_AINPUT, &CSimplistAgoral_WinSDKDlg::OnCbnSelchangeComboAinput)
	ON_CBN_SELCHANGE(IDC_COMBOX_VINPUT, &CSimplistAgoral_WinSDKDlg::OnCbnSelchangeComboxVinput)
	ON_CBN_SELCHANGE(IDC_COMBOX_AOUT, &CSimplistAgoral_WinSDKDlg::OnCbnSelchangeComboxAout)
END_MESSAGE_MAP()


// CSimplistAgoral_WinSDKDlg ��Ϣ�������

BOOL CSimplistAgoral_WinSDKDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	CEdit  * chanelEdit = (CEdit *)GetDlgItem(IDC_EDIT_CHANNEL);
	chanelEdit->SetWindowTextW(_T("w"));

	CStatic * localCtrl = (CStatic*)GetDlgItem(IDC_STATIC_LOCAL);
	CStatic * remoteCtrl = (CStatic*)GetDlgItem(IDC_STATIC_REMOTE);

	m_cmbInputAudDev = (CComboBox*)GetDlgItem(IDC_COMBO_AINPUT);
	m_cmbOutputAudDev = (CComboBox*)GetDlgItem(IDC_COMBOX_AOUT);
	m_cmbVideoDev = (CComboBox*)GetDlgItem(IDC_COMBOX_VINPUT);
	m_silderVolIndicate = (CProgressCtrl*)GetDlgItem(IDC_PROGRESS_VOL_IDC);
	m_silderVolIndicate->SetRange(0, 1000);

	m_pAgroObject = new CAgoralWrapper;
	m_pAgroObject->SetLocalHwnd((HWND)localCtrl->GetSafeHwnd());
	m_pAgroObject->SetRemoetHwnd((HWND)remoteCtrl->GetSafeHwnd());
	m_pAgroObject->SetMainHWND(m_hWnd);

	std::string appPath = CUtils::getAppPath();
	appPath += "//agorl.log";
	m_pAgroObject->InitAgoral(appPath.c_str());

	m_deviceManager = new CDeviceManager;

	m_deviceManager->InitManager();
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

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE	
}


void CSimplistAgoral_WinSDKDlg::onAudioVolumIndication(const void *param)
{
	LPAGE_AUDIO_VOLUME_INDICATION lpData = (LPAGE_AUDIO_VOLUME_INDICATION)param;
	m_silderVolIndicate->SetPos(lpData->totalVolume);


	printf("vol = %d\n", lpData->totalVolume);
	delete lpData;
}
void CSimplistAgoral_WinSDKDlg::OnPaint()
{
	CDialogEx::OnPaint();
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
		btn->SetWindowText(_T("����"));
	} else {
		btn->SetWindowText(_T("ֹͣ"));
		remoteCtrl->Invalidate(TRUE);
	}
	m_deviceManager->TestCurrentVideoDev(remoteCtrl->GetSafeHwnd());
	
}


void CSimplistAgoral_WinSDKDlg::OnBnClickedButtonTestAin()
{

	CButton *btn = (CButton*)GetDlgItem(ID_BUTTON_TEST_AIN);

	if (m_deviceManager->GetIsTestAudioInDev()) {
		btn->SetWindowText(_T("����"));
	} else {
		btn->SetWindowText(_T("ֹͣ"));
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
		btn->SetWindowText(_T("����"));
	} else {
		btn->SetWindowText(_T("ֹͣ"));
	}
	m_deviceManager->TestCurrentAudioOutDev(str.c_str());
}



void CSimplistAgoral_WinSDKDlg::OnCbnSelchangeComboAinput()
{
	int index = m_cmbInputAudDev->GetCurSel();
	std::vector<DevicesInfo> &audioDevs = m_deviceManager->GetVideoDeviceLists();

	DevicesInfo info = audioDevs.at(index);

	m_deviceManager->SetCurrentAudioInputDev(info.deviceId.c_str());
}


void CSimplistAgoral_WinSDKDlg::OnCbnSelchangeComboxVinput()
{
	int index = m_cmbVideoDev->GetCurSel();
	std::vector<DevicesInfo> &videoDevs = m_deviceManager->GetVideoDeviceLists();

	DevicesInfo info = videoDevs.at(index);

	m_deviceManager->SetCurrentVidoeInputDev(info.deviceId.c_str());
}


void CSimplistAgoral_WinSDKDlg::OnCbnSelchangeComboxAout()
{
	int index = m_cmbInputAudDev->GetCurSel();
	std::vector<DevicesInfo> &audioODevs = m_deviceManager->GetVideoDeviceLists();

	DevicesInfo info = audioODevs.at(index);

	m_deviceManager->SetCurrentAudioOutputDev(info.deviceId.c_str());
}
