
#include "stdafx.h"

#include "AgoraAudInputManager.h"
#include "AgoraObject.h"
#include <assert.h>

CAgoraAudInputManager::CAgoraAudInputManager()
	: m_ptrDeviceManager(NULL)
	, m_lpCollection(NULL)
	, m_bTestingOn(FALSE)
{
}


CAgoraAudInputManager::~CAgoraAudInputManager()
{
	Close();
}

BOOL CAgoraAudInputManager::Create(agora::rtc::IRtcEngine *lpRtcEngine)
{
	m_ptrDeviceManager = new agora::rtc::AAudioDeviceManager(*lpRtcEngine);
	if (m_ptrDeviceManager->get() == NULL)
		return FALSE;

	m_lpCollection = (*m_ptrDeviceManager)->enumerateRecordingDevices();
	if (m_lpCollection == NULL) {
		delete m_ptrDeviceManager;
		m_ptrDeviceManager = NULL;
	}

	
	return m_lpCollection != NULL ? TRUE : FALSE;
}

void CAgoraAudInputManager::Close()
{
	if (m_lpCollection != NULL){
		m_lpCollection->release();
		m_lpCollection = NULL;
	}

	if (m_ptrDeviceManager != NULL) {
		delete m_ptrDeviceManager;
		m_ptrDeviceManager = NULL;
	}
}

UINT CAgoraAudInputManager::GetVolume()
{
	int nVol = 0;

	if (*m_ptrDeviceManager != NULL)
		(*m_ptrDeviceManager)->getRecordingDeviceVolume(&nVol);

	return (UINT)nVol;
}

BOOL CAgoraAudInputManager::SetVolume(UINT nVol)
{
	int nRet = -1;
	
	if (*m_ptrDeviceManager != NULL)
		nRet = (*m_ptrDeviceManager)->setRecordingDeviceVolume((int)nVol);

	return nRet == 0 ? TRUE : FALSE;
}

UINT CAgoraAudInputManager::GetDeviceCount()
{
	if (m_lpCollection != NULL)
		return (UINT)m_lpCollection->getCount();

	return 0;
}

BOOL CAgoraAudInputManager::GetDevice(UINT nIndex, std::string &rDeviceName, std::string &rDeviceID)
{
	CHAR szDeviceName[agora::rtc::MAX_DEVICE_ID_LENGTH];
	CHAR szDeviceID[agora::rtc::MAX_DEVICE_ID_LENGTH];

	assert(nIndex < GetDeviceCount());

	if (nIndex >= GetDeviceCount())
		return FALSE;

	if (m_lpCollection == NULL)
		return FALSE;

	int nRet = m_lpCollection->getDevice(nIndex, szDeviceName, szDeviceID);
	if (nRet != 0)
		return FALSE;


	rDeviceName = szDeviceName;
	rDeviceID = szDeviceID;
#ifdef UNICODE
	//::MultiByteToWideChar(CP_UTF8, 0, szDeviceName, -1, rDeviceName.GetBuffer(MAX_DEVICE_ID_LENGTH), MAX_DEVICE_ID_LENGTH);
//	::MultiByteToWideChar(CP_UTF8, 0, szDeviceID, -1, rDeviceID.GetBuffer(MAX_DEVICE_ID_LENGTH), MAX_DEVICE_ID_LENGTH);
//
	//rDeviceName.ReleaseBuffer();
	//rDeviceID.ReleaseBuffer();
#else
	strDeviceName = szDeviceName;
	strDeviceID = szDeviceID;
#endif

	return TRUE;
}

std::string CAgoraAudInputManager::GetCurDeviceID()
{
	std::string 		str;
	CHAR		szDeviceID[agora::rtc::MAX_DEVICE_ID_LENGTH];

	WCHAR    wszDeviceId[agora::rtc::MAX_DEVICE_ID_LENGTH];
	
	if (*m_ptrDeviceManager != NULL)
		(*m_ptrDeviceManager)->getRecordingDevice(szDeviceID);

#ifdef UNICODE
	::MultiByteToWideChar(CP_UTF8, 0, szDeviceID, -1, wszDeviceId, agora::rtc::MAX_DEVICE_ID_LENGTH);
	
#else
	strDeviceName = szDeviceID;
#endif

	str = szDeviceID;
	return str;
}

BOOL CAgoraAudInputManager::SetCurDevice(LPCTSTR lpDeviceID)
{
	if (*m_ptrDeviceManager == NULL)
		return FALSE;

#ifdef UNICODE
	CHAR szDeviceID[128];
	::WideCharToMultiByte(CP_ACP, 0, lpDeviceID, -1, szDeviceID, 128, NULL, NULL);
	int nRet = (*m_ptrDeviceManager)->setRecordingDevice(szDeviceID);
#else
	int nRet = (*m_ptrDeviceManager)->setRecordingDevice(lpDeviceID);
#endif

	return nRet == 0 ? TRUE : FALSE;
}

void CAgoraAudInputManager::TestAudInputDevice(HWND hMsgWnd, BOOL bTestOn)
{
	if (bTestOn && !m_bTestingOn) {
		m_hOldMsgWnd = CAgoraObject::GetAgoraObject()->GetMsgHandlerWnd();
		CAgoraObject::GetAgoraObject()->SetMsgHandlerWnd(hMsgWnd);

		IRtcEngine *lpRtcEngine = CAgoraObject::GetEngine();
		RtcEngineParameters rep(*lpRtcEngine);
		rep.enableAudioVolumeIndication(1000, 10);
		(*m_ptrDeviceManager)->startRecordingDeviceTest(1000);
	}
	else if (!bTestOn && m_bTestingOn){
		CAgoraObject::GetAgoraObject()->SetMsgHandlerWnd(m_hOldMsgWnd);
		(*m_ptrDeviceManager)->stopRecordingDeviceTest();
	}

	m_bTestingOn = bTestOn;

}