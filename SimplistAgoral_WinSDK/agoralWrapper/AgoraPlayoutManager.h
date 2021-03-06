#pragma once

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

	BOOL ReCreateCollection();

	UINT GetVolume();
	BOOL SetVolume(UINT nVol);
	UINT GetDeviceCount();

	std::string  GetCurDeviceID();
	BOOL SetCurDevice(const char * lpDeviceID);

	BOOL GetDevice(UINT nIndex, std::string  &rDeviceName, std::string &rDeviceID);
	void TestPlaybackDevice(UINT nWavID, BOOL bTestOn = TRUE);

	int  TestPlaybackDevice(const char *fileName, HWND msgHwnd,BOOL bTestOn);

	BOOL IsTesting() { return m_bTestingOn; };
private:
	AAudioDeviceManager			*m_ptrDeviceManager;
	IAudioDeviceCollection		*m_lpCollection;

	IRtcEngine   *m_pRtcEngin;

private:
	BOOL m_bTestingOn;
};

