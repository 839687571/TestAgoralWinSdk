#pragma once
#include  <vector>

#include "AgoraPlayoutManager.h"
#include "AgoraAudInputManager.h"
#include "AgoraCameraManager.h"

interface  IDeviceMsgObserver {
	virtual void onAudioVolumIndication(const void *param) {};
	// network test
	virtual void  onLastmileQuality(const void *wParam) {};

	virtual void  onAudioDeviceChange(const void *wParam) {};

	virtual void  onVideoDeviceChange(const void *wParam) {};
};

typedef struct _DeviceSatus {
	int cameraStatus;
	int micStatus;
	int speakerStatus;
	int networkStatus;

	bool cameraExist;
	bool micExist;
	bool speakerExist;

	_DeviceSatus::_DeviceSatus()
	{
		cameraStatus = FALSE;
		micStatus = speakerStatus = networkStatus - 1;
		speakerExist = micExist = cameraExist = false;
	}

	void Reset()
	{
		cameraStatus = FALSE;
		micStatus = speakerStatus = networkStatus - 1;
		speakerExist = micExist = cameraExist = false;
	}
}DeviceSatus;

typedef struct _DeviceInUse {

	std::wstring cameraDevId;
	std::wstring micDevId;
	std::wstring speakerDevId;
}DeviceInUse;

typedef struct _DevicesInfo {
	std::string  deviceName;
	std::string  deviceId;
}DevicesInfo;

class CDeviceManager {
	CDeviceManager();
private:
	static CDeviceManager* m_pInstance;
public:

	~CDeviceManager();

	static CDeviceManager* GetInstance();
	void  InitManager();
	void UnitManager();


	void SetMsgHandleWnd(HWND hwnd)
	{
		m_hMsgWnd = hwnd;

	}
	std::vector<DevicesInfo>  &GetVideoDeviceLists();
	std::vector<DevicesInfo>  &GetAudioInputDeviceLists();
	std::vector<DevicesInfo>  &GetAudioOutputDeviceLists();



	void UpdateDeviceList();

	void UpdateSpeakerList();
	void UpdateMicList();
	void UpdateCameraList();

	void ObtainSpeakerDevList();
	void ObtainMicDevList();
	void ObtainCameraDevList();


	void ObtainDeviceList();

	UINT  GetCurrentInputVolume();
	UINT  GetCurrentOutputVolume();

	BOOL  SetCurrentInputVolume(UINT volume);
	BOOL  SetCurrentOutputVolume(UINT volume);


	BOOL TestCurrentVideoDev(HWND hwnd);

	int  TestCurrentAudioInputDev();
	int  TestCurrentAudioOutDev(const char *auidoFile);


	BOOL SetCurrentVidoeInputDev(const char *devId);
	BOOL SetCurrentAudioInputDev(const char *devId);
	BOOL SetCurrentAudioOutputDev(const char *devId);


	std::string  GetCurrentUseMicDevId();
	std::string  GetCurrentUseSpeakerDevId();
	std::string  GetCurrentUseCameraDevId();

	BOOL MuteLocalVideo(BOOL mute);

	BOOL GetIsTestVideoDev()
	{
		return m_agCamera.IsTesting();
	}


	BOOL GetIsTestAudioInDev()
	{
		return m_agAudioin.IsTesting();
	}



	BOOL GetIsTestAudioOutDev()
	{
		return m_agPlayout.IsTesting();
	}

	int StartTestNetWork();

	int  StopTestNetWork();


	void SetDevInuse(DeviceInUse  &devInuse)
	{
		m_devInUse = devInuse;
	}

	void SetDevStatus(DeviceSatus &devStatus)
	{
		m_devStatus = devStatus;
	}

	DeviceSatus &GetDevStatus()
	{
		return m_devStatus;
	}

	DeviceInUse &GetDevInuse()
	{
		return m_devInUse;
	}

private:
	IDeviceMsgObserver      *m_pObserver;
	IRtcEngine			    *m_lpRtcEngine;
	CAgoraAudInputManager	m_agAudioin;
	CAgoraCameraManager		m_agCamera;
	CAgoraPlayoutManager	m_agPlayout;

	std::vector<DevicesInfo>  m_videoDevList;
	std::vector<DevicesInfo>  m_audioInputDevList;  // 音频输入设备. mic.
	std::vector<DevicesInfo>  m_audioOutputDevList; // 音频输出设备. 扬声器.

	HWND m_hMsgWnd;

	DeviceSatus   m_devStatus;
	DeviceInUse   m_devInUse;
};

