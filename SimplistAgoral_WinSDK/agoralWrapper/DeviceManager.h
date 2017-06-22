#pragma once
#include  <vector>

#include "AgoraPlayoutManager.h"
#include "AgoraAudInputManager.h"
#include "AgoraCameraManager.h"

interface  IDeviceMsgObserver {
	virtual void onAudioVolumIndication(const void *param) {};
	// network test
	virtual void  onLastmileQuality(const void *wParam) {};
};

typedef struct _DevicesInfo {
	std::string  deviceName;
	std::string  deviceId;
}DevicesInfo;

class CDeviceManager {
public:
	CDeviceManager();
	~CDeviceManager();

	void SetCurrentObserver(IDeviceMsgObserver  *observer)
	{
		m_pObserver = observer;
	}

	void SetMsgHandleWnd(HWND hwnd)
	{
		m_hMsgWnd = hwnd;

	}
	std::vector<DevicesInfo>  &GetVideoDeviceLists();
	std::vector<DevicesInfo>  &GetAudioInputDeviceLists();
	std::vector<DevicesInfo>  &GetAudioOutputDeviceLists();

	void  InitManager(const char *logPath);


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


	std::string  GetCurrentUseAudioInputDevId();

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

	int StartTestNetWork(HWND hwnd);

	int  StopTestNetWork();
	/*
	  测试 音视频设备的过程中 ,引擎通过消息形式返回上传,
	  因此需要添加 消息处理函数.
	*/
	void MsgHandle(DWORD msgId, WPARAM wParam);

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
};

