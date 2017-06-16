#pragma once
#include  <vector>

#include "AgoraPlayoutManager.h"
#include "AgoraAudInputManager.h"
#include "AgoraCameraManager.h"

interface  IDeviceMsgObserver {
	virtual void onAudioVolumIndication(const void *param) {};
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

	void  InitManager();


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


	/*
	  ���� ����Ƶ�豸�Ĺ����� ,����ͨ����Ϣ��ʽ�����ϴ�,
	  �����Ҫ���� ��Ϣ��������.
	*/
	void MsgHandle(DWORD msgId, WPARAM wParam);

private:
	IDeviceMsgObserver      *m_pObserver;
	IRtcEngine			    *m_lpRtcEngine;
	CAgoraAudInputManager	m_agAudioin;
	CAgoraCameraManager		m_agCamera;
	CAgoraPlayoutManager	m_agPlayout;

	std::vector<DevicesInfo>  m_videoDevList;
	std::vector<DevicesInfo>  m_audioInputDevList;  // ��Ƶ�����豸. mic.
	std::vector<DevicesInfo>  m_audioOutputDevList; // ��Ƶ����豸. ������.

	HWND m_hMsgWnd;
};
