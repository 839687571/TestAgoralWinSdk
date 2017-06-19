#include "stdafx.h"
#include "DeviceManager.h"
#include "AgoraObject.h"
#include "AGEventDef.h"

CDeviceManager::CDeviceManager():
m_lpRtcEngine(NULL)
{
}


CDeviceManager::~CDeviceManager()
{


}
void CDeviceManager::InitManager()
{
	m_lpRtcEngine = CAgoraObject::GetEngine();

	m_agPlayout.Create(m_lpRtcEngine);
	m_agAudioin.Create(m_lpRtcEngine);
	m_agCamera.Create(m_lpRtcEngine);

	for (UINT nIndex = 0; nIndex < m_agPlayout.GetDeviceCount(); nIndex++) {
		std::string strDeviceName, strDeviceID;
		m_agPlayout.GetDevice(nIndex, strDeviceName, strDeviceID);

		DevicesInfo info;
		info.deviceName = strDeviceName;
		info.deviceId = strDeviceID;
		m_audioOutputDevList.push_back(info);

	}

	for (UINT nIndex = 0; nIndex < m_agAudioin.GetDeviceCount(); nIndex++) {
		std::string strDeviceName, strDeviceID;
		m_agAudioin.GetDevice(nIndex, strDeviceName, strDeviceID);
		DevicesInfo info;
		info.deviceName = strDeviceName;
		info.deviceId = strDeviceID;
		m_audioInputDevList.push_back(info);
	}


	for (UINT nIndex = 0; nIndex < m_agCamera.GetDeviceCount(); nIndex++) {
		std::string strDeviceName, strDeviceID;
		m_agCamera.GetDevice(nIndex, strDeviceName, strDeviceID);
		DevicesInfo info;
		info.deviceName = strDeviceName;
		info.deviceId = strDeviceID;

		m_videoDevList.push_back(info);
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
			return	m_agPlayout.TestPlaybackDevice(auidoFile,FALSE);
		} else {
			return	m_agPlayout.TestPlaybackDevice(auidoFile,TRUE);
		}
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

std::string  CDeviceManager::GetCurrentUseAudioInputDevId()
{
	return m_agAudioin.GetCurDeviceID();
}

BOOL  CDeviceManager::TestCurrentVideoDev(HWND hwnd)
{
	if (m_agCamera.IsTesting()) {
		return m_agCamera.TestCameraDevice(NULL, FALSE);
	} else {
		return	m_agCamera.TestCameraDevice(hwnd, TRUE);
	}
}


void CDeviceManager::MsgHandle(DWORD msgId, WPARAM wParam)
{

	if (m_pObserver != NULL)
	{
		switch (msgId) {
		case WM_MSGID(EID_AUDIO_VOLUME_INDICATION):
			m_pObserver->onAudioVolumIndication((void*)wParam);
			break;
			default:
				break;
		}
		
	}
}

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
	if (volume == 0) {
		return CAgoraObject::GetAgoraObject()->MuteLocalAudio(TRUE);
	} else {
		CAgoraObject::GetAgoraObject()->MuteLocalAudio(FALSE);
	}
	return m_agAudioin.SetVolume(volume);
}
BOOL  CDeviceManager::SetCurrentOutputVolume(UINT volume)
{
	return m_agPlayout.SetVolume(volume);
}

BOOL CDeviceManager::MuteLocalVideo(BOOL mute)
{
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