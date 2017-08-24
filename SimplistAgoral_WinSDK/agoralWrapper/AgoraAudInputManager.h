#pragma once

#include <string>
#include "agoral/IAgoraRtcEngine.h"
#include "agoral/IAgoraMediaEngine.h"



class CAgoraAudInputManager
{
public:
	CAgoraAudInputManager();
	~CAgoraAudInputManager();

	BOOL Create(agora::rtc::IRtcEngine *lpRtcEngine);
	void Close();
	BOOL ReCreateCollection();

	UINT GetVolume();
	BOOL SetVolume(UINT nVol);
	UINT GetDeviceCount();

	std::string GetCurDeviceID();
	BOOL SetCurDevice(const char *pDeviceID);

	BOOL GetDevice(UINT nIndex, std::string &rDeviceName, std::string &rDeviceID);
	
	int TestAudInputDevice(HWND hMsgWnd, BOOL bTestOn);

	BOOL IsTesting() { return m_bTestingOn; };

	//BOOL  UpataDevice();
private:
	BOOL						m_bTestingOn;
	HWND						m_hOldMsgWnd;
	agora::rtc::AAudioDeviceManager			*m_ptrDeviceManager;
	agora::rtc::IAudioDeviceCollection		*m_lpCollection;

	agora::rtc::IRtcEngine *                 m_lpRtcEngine;
};

