#pragma once

#include "../stdafx.h"
#include "agoral/IAgoraMediaEngine.h"
#include "agoral/IAgoraRtcEngine.h"

#include <string>

using namespace agora::rtc;


class CAgoraPlayoutManager
{
public:
	CAgoraPlayoutManager();
	~CAgoraPlayoutManager();

	BOOL Create(IRtcEngine *lpRtcEngine);
	void Close();

	UINT GetVolume();
	BOOL SetVolume(UINT nVol);
	UINT GetDeviceCount();

	std::string  GetCurDeviceID();
	BOOL SetCurDevice(const char * lpDeviceID);

	BOOL GetDevice(UINT nIndex, std::string  &rDeviceName, std::string &rDeviceID);
	void TestPlaybackDevice(UINT nWavID, BOOL bTestOn = TRUE);

	BOOL IsTesting() { return m_bTestingOn; };
private:
	AAudioDeviceManager			*m_ptrDeviceManager;
	IAudioDeviceCollection		*m_lpCollection;

private:
	BOOL m_bTestingOn;
};

