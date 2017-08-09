#include "stdafx.h"
#include "DeviceManager.h"
#include "AgoraObject.h"
#include "AGEventDef.h"
#include "../ComUtil/BugTrapWrapper.h"

#define CUR_MODULE L"DEVICEMGR"

CDeviceManager* CDeviceManager::m_pInstance = new CDeviceManager;
#define  CHECK_RTCENGIN {\
if (m_lpRtcEngine == NULL) {\
	return -1;\
}\
}

CDeviceManager::CDeviceManager():
m_lpRtcEngine(NULL)
{
}


CDeviceManager::~CDeviceManager()
{
	
}

CDeviceManager* CDeviceManager::GetInstance()
{
	return m_pInstance;
}

void CDeviceManager::UnitManager()
{
	m_agPlayout.Close();
	m_agAudioin.Close();
	m_agCamera.Close();
}
void CDeviceManager::InitManager(const char *logPath)
{
	m_lpRtcEngine = CAgoraObject::GetEngine();

	m_agPlayout.Create(m_lpRtcEngine);
	m_agAudioin.Create(m_lpRtcEngine);
	m_agCamera.Create(m_lpRtcEngine);

	ObtainDeviceList();
}

void CDeviceManager::UpdateDeviceList()
{

	m_agAudioin.Close();
	m_agAudioin.Create(m_lpRtcEngine);

	m_audioOutputDevList.clear();
	m_audioInputDevList.clear();
	m_videoDevList.clear();
	ObtainDeviceList();
}

void CDeviceManager::ObtainDeviceList()
{
	for (UINT nIndex = 0; nIndex < m_agPlayout.GetDeviceCount(); nIndex++) {
		std::string strDeviceName, strDeviceID;
		m_agPlayout.GetDevice(nIndex, strDeviceName, strDeviceID);

		DevicesInfo info;
		info.deviceName = strDeviceName;
		info.deviceId = strDeviceID;
		m_audioOutputDevList.push_back(info);

		BugTrapWrapper::GetQQLogger()->AppendF(BTLL_INFO, CUR_MODULE, L"Obtain Speaker name = %s,id= %s", CComUtil::AnsiToUnicode(strDeviceName.c_str()).c_str(),
			CComUtil::AnsiToUnicode(strDeviceID.c_str()).c_str());

	}

	for (UINT nIndex = 0; nIndex < m_agAudioin.GetDeviceCount(); nIndex++) {
		std::string strDeviceName, strDeviceID;
		m_agAudioin.GetDevice(nIndex, strDeviceName, strDeviceID);
		DevicesInfo info;
		info.deviceName = strDeviceName;
		info.deviceId = strDeviceID;
		m_audioInputDevList.push_back(info);
		BugTrapWrapper::GetQQLogger()->AppendF(BTLL_INFO, CUR_MODULE, L"Obtain Mic name = %s,id= %s", CComUtil::AnsiToUnicode(strDeviceName.c_str()).c_str(),
			CComUtil::AnsiToUnicode(strDeviceID.c_str()).c_str());
	}


	for (UINT nIndex = 0; nIndex < m_agCamera.GetDeviceCount(); nIndex++) {
		std::string strDeviceName, strDeviceID;
		m_agCamera.GetDevice(nIndex, strDeviceName, strDeviceID);
		DevicesInfo info;
		info.deviceName = strDeviceName;
		info.deviceId = strDeviceID;

		m_videoDevList.push_back(info);
		BugTrapWrapper::GetQQLogger()->AppendF(BTLL_INFO, CUR_MODULE, L"Obtain Camera name = %s,id= %s", CComUtil::AnsiToUnicode(strDeviceName.c_str()).c_str(),
			CComUtil::AnsiToUnicode(strDeviceID.c_str()).c_str());
	}
}


int   CDeviceManager::TestCurrentAudioInputDev()
{
	if (m_agAudioin.IsTesting()) {
		return	m_agAudioin.TestAudInputDevice(NULL, FALSE);
	} else {
		return	m_agAudioin.TestAudInputDevice(m_hMsgWnd, TRUE);
	}
}
int  CDeviceManager::TestCurrentAudioOutDev( const char *auidoFile)
{
		if (m_agPlayout.IsTesting()) {
			return	m_agPlayout.TestPlaybackDevice(auidoFile, m_hMsgWnd,FALSE);
		} else {
			return	m_agPlayout.TestPlaybackDevice(auidoFile, m_hMsgWnd,TRUE);
		}
}
int CDeviceManager::StartTestNetWork()
{
	CHECK_RTCENGIN
//	CAgoraObject::GetAgoraObject()->SetMsgHandlerWnd(m_hMsgWnd);
	return m_lpRtcEngine->enableLastmileTest();
}

int  CDeviceManager::StopTestNetWork()
{
	CHECK_RTCENGIN
	return m_lpRtcEngine->disableLastmileTest();
}


BOOL CDeviceManager::SetCurrentVidoeInputDev(const char *devId)
{
	return m_agCamera.SetCurDevice(devId);
}
BOOL CDeviceManager::SetCurrentAudioInputDev(const char *devId)
{
	return m_agAudioin.SetCurDevice(devId);
}
BOOL CDeviceManager::SetCurrentAudioOutputDev(const char *devId)
{
	return m_agPlayout.SetCurDevice(devId);
}

std::string  CDeviceManager::GetCurrentUseMicDevId()
{
	return m_agAudioin.GetCurDeviceID();
}
std::string  CDeviceManager::GetCurrentUseSpeakerDevId()
{
	return m_agPlayout.GetCurDeviceID();
}
std::string  CDeviceManager::GetCurrentUseCameraDevId()
{
	return m_agCamera.GetCurDeviceID();
}


BOOL  CDeviceManager::TestCurrentVideoDev(HWND hwnd)
{
	CHECK_RTCENGIN
	if (m_agCamera.IsTesting()) {
		return m_agCamera.TestCameraDevice(NULL, FALSE);
	} else {
		return	m_agCamera.TestCameraDevice(hwnd, TRUE);
	}
}


// void CDeviceManager::MsgHandle(DWORD msgId, WPARAM wParam)
// {
// 
// 	if (m_pObserver != NULL)
// 	{
// 		switch (msgId) {
// 		case WM_MSGID(EID_AUDIO_VOLUME_INDICATION):
// 			m_pObserver->onAudioVolumIndication((void*)wParam);
// 			break;
// 		case WM_MSGID(EID_LASTMILE_QUALITY):
// 			m_pObserver->onLastmileQuality((void*)wParam);
// 			break;
// 		case WM_MSGID(EID_AUDIO_DEVICE_STATE_CHANGED):
// 			m_pObserver->onAudioDeviceChange((void*)wParam);
// 			break;
// 		case WM_MSGID(EID_VIDEO_DEVICE_STATE_CHANGED):
// 			m_pObserver->onVideoDeviceChange((void*)wParam);
// 			break;
// 			default:
// 				break;
// 		}
// 		
// 	}
// }

UINT  CDeviceManager::GetCurrentInputVolume()
{
	return m_agAudioin.GetVolume();
}
UINT  CDeviceManager::GetCurrentOutputVolume()
{
	return m_agPlayout.GetVolume();
}


BOOL  CDeviceManager::SetCurrentInputVolume(UINT volume)
{
	CHECK_RTCENGIN
	if (volume == 0) {
		return CAgoraObject::GetAgoraObject()->MuteLocalAudio(TRUE);
	} else {
		CAgoraObject::GetAgoraObject()->MuteLocalAudio(FALSE);
	}
	return m_agAudioin.SetVolume(volume);

	BugTrapWrapper::GetQQLogger()->AppendF(BTLL_INFO, CUR_MODULE, L"%s", CComUtil::AnsiToUnicode(__FUNCTION__).c_str());
}
BOOL  CDeviceManager::SetCurrentOutputVolume(UINT volume)
{
	return m_agPlayout.SetVolume(volume);
}

BOOL CDeviceManager::MuteLocalVideo(BOOL mute)
{
	CHECK_RTCENGIN
	return CAgoraObject::GetAgoraObject()->MuteLocalVideo(mute);
}

std::vector<DevicesInfo>& CDeviceManager::GetVideoDeviceLists()
{
	return m_videoDevList;
}
std::vector<DevicesInfo>& CDeviceManager::GetAudioInputDeviceLists()
{
	return m_audioInputDevList;
}

std::vector<DevicesInfo>& CDeviceManager::GetAudioOutputDeviceLists()
{
	return m_audioOutputDevList;
}