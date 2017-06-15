#pragma once

#include "../stdafx.h"
#include <string>


#include "agoral/IAgoraMediaEngine.h"
#include "agoral/IAgoraRtcEngine.h"

using namespace agora::rtc;

class CAgoraCameraManager
{
public:
	CAgoraCameraManager();
	~CAgoraCameraManager();

	BOOL Create(IRtcEngine *lpRtcEngine);
	void Close();

	UINT GetDeviceCount();

	std::string GetCurDeviceID();
	BOOL SetCurDevice(LPCTSTR lpDeviceID);

	BOOL GetDevice(UINT nIndex, std::string  &rDeviceName, std::string &rDeviceID);
	void TestCameraDevice(HWND hVideoWnd, BOOL bTestOn = TRUE);

	BOOL IsTesting() { return m_bTestingOn; };

private:
	AVideoDeviceManager			*m_ptrDeviceManager;
	IVideoDeviceCollection		*m_lpCollection;
	BOOL						m_bTestingOn;
};

