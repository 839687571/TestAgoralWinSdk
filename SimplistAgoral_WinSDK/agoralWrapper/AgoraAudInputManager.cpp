
#include "stdafx.h"

#include "AgoraAudInputManager.h"
#include "AgoraObject.h"
#include <assert.h>
#include "AgoralUtils.h"
#include "../ComUtil/BugTrapWrapper.h"

CAgoraAudInputManager::CAgoraAudInputManager()
	: m_ptrDeviceManager(NULL)
	, m_lpCollection(NULL)
	, m_bTestingOn(FALSE)
	, m_lpRtcEngine(FALSE)
{
}


CAgoraAudInputManager::~CAgoraAudInputManager()
{
	Close();
}

BOOL CAgoraAudInputManager::Create(agora::rtc::IRtcEngine *lpRtcEngine)
{
	m_ptrDeviceManager = new AAudioDeviceManager(lpRtcEngine);
	if (m_ptrDeviceManager == NULL || m_ptrDeviceManager->get() == NULL){
		BugTrapWrapper::GetQQLogger()->Append(BTLL_ERROR, L" new AAudioDeviceManager Mic device  failed");
		return FALSE;
	}

	m_lpCollection = (*m_ptrDeviceManager)->enumerateRecordingDevices();
	if (m_lpCollection == NULL) {
		delete m_ptrDeviceManager;
		m_ptrDeviceManager = NULL;
		BugTrapWrapper::GetQQLogger()->Append(BTLL_ERROR, L" enumerateRecordingDevices Mic device  failed");
	}

	
	return m_lpCollection != NULL ? TRUE : FALSE;
}

// BOOL  CAgoraAudInputManager::UpataDevice()
// {
// 	if (m_lpCollection != NULL) {
// 		m_lpCollection->release();
// 		m_lpCollection = NULL;
// 	}
// 
// 	if (m_ptrDeviceManager == NULL) {
// 		m_ptrDeviceManager = new agora::rtc::AAudioDeviceManager(*m_lpRtcEngine);
// 		if (m_ptrDeviceManager->get() == NULL)
// 			return FALSE;
// 	}
// 
// 	m_lpCollection = (*m_ptrDeviceManager)->enumerateRecordingDevices();
// 	if (m_lpCollection == NULL) {
// 		
// 	}
// 
// 	return m_lpCollection != NULL ? TRUE : FALSE;
// }

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
	if (m_ptrDeviceManager == NULL  || m_ptrDeviceManager->get() == NULL  ) return 0;

	int nVol = 0;
	if (*m_ptrDeviceManager != NULL)
		(*m_ptrDeviceManager)->getRecordingDeviceVolume(&nVol);

	return (UINT)nVol;
}

BOOL CAgoraAudInputManager::SetVolume(UINT nVol)
{

	if (m_ptrDeviceManager == NULL|| m_ptrDeviceManager->get() == NULL ) return 0;
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
	CHAR szDeviceName[MAX_DEVICE_ID_LENGTH];
	CHAR szDeviceID[MAX_DEVICE_ID_LENGTH];

	assert(nIndex < GetDeviceCount());

	if (nIndex >= GetDeviceCount())
		return FALSE;

	if (m_lpCollection == NULL)
		return FALSE;

	int nRet = m_lpCollection->getDevice(nIndex, szDeviceName, szDeviceID);
	if (nRet != 0)
		return FALSE;

	char ansiDevName[MAX_DEVICE_ID_LENGTH];
	CAgoralUtils::Convert(szDeviceName, ansiDevName, CP_UTF8, CP_ACP);

	char ansiDevId[MAX_DEVICE_ID_LENGTH];
	CAgoralUtils::Convert(szDeviceID, ansiDevId, CP_UTF8, CP_ACP);

	rDeviceName = ansiDevName;
	rDeviceID = ansiDevId;

	return TRUE;
}

std::string CAgoraAudInputManager::GetCurDeviceID()
{
	std::string 		str;
	CHAR		szDeviceID[agora::rtc::MAX_DEVICE_ID_LENGTH] = { 0 };
	
	if (m_ptrDeviceManager != NULL  && *m_ptrDeviceManager != NULL)
		(*m_ptrDeviceManager)->getRecordingDevice(szDeviceID);

	char ansiDevId[MAX_DEVICE_ID_LENGTH];
	CAgoralUtils::Convert(szDeviceID, ansiDevId, CP_UTF8, CP_ACP);

	str = ansiDevId;
	return str;
}

BOOL CAgoraAudInputManager::SetCurDevice(const char *pDeviceID)
{
	if (*m_ptrDeviceManager == NULL)
		return FALSE;

	int nRet = (*m_ptrDeviceManager)->setRecordingDevice(pDeviceID);

	return nRet == 0 ? TRUE : FALSE;
}

int CAgoraAudInputManager::TestAudInputDevice(HWND hMsgWnd, BOOL bTestOn)
{
	int ret = 0;

	if (m_ptrDeviceManager == NULL) return FALSE;

	if (bTestOn && !m_bTestingOn) {
		m_hOldMsgWnd = CAgoraObject::GetAgoraObject()->GetMsgHandlerWnd();
		CAgoraObject::GetAgoraObject()->SetMsgHandlerWnd(hMsgWnd);

		IRtcEngine *lpRtcEngine = CAgoraObject::GetEngine();
		RtcEngineParameters rep(*lpRtcEngine);
		rep.enableAudioVolumeIndication(1000, 10);
		ret = (*m_ptrDeviceManager)->startRecordingDeviceTest(200);
	}
	else if (!bTestOn && m_bTestingOn){
		CAgoraObject::GetAgoraObject()->SetMsgHandlerWnd(m_hOldMsgWnd);
		ret = (*m_ptrDeviceManager)->stopRecordingDeviceTest();
	}

	m_bTestingOn = bTestOn;

	return ret;

}