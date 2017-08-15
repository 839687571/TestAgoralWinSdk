#include "stdafx.h"
#include "AgoraCameraManager.h"
#include "AgoraObject.h"
#include "AgoralUtils.h"
#include "../ComUtil/BugTrapWrapper.h"

CAgoraCameraManager::CAgoraCameraManager()
	: m_ptrDeviceManager(NULL)
	, m_lpCollection(NULL)
	, m_bTestingOn(FALSE)
{
}


CAgoraCameraManager::~CAgoraCameraManager()
{
	Close();
}

BOOL CAgoraCameraManager::Create(IRtcEngine *lpRtcEngine)
{

	m_ptrDeviceManager = new AVideoDeviceManager(lpRtcEngine);
	if (m_ptrDeviceManager == NULL || m_ptrDeviceManager->get() == NULL) {
		BugTrapWrapper::GetQQLogger()->Append(BTLL_ERROR, L" new AVideoDeviceManager Camera  device manager failed");
		return FALSE;
	}

	m_lpCollection = (*m_ptrDeviceManager)->enumerateVideoDevices();
	if (m_lpCollection == NULL) {
		delete m_ptrDeviceManager;
		m_ptrDeviceManager = NULL;
		BugTrapWrapper::GetQQLogger()->Append(BTLL_ERROR, L" enumerateVideoDevices Camera  device  failed");
	}

	return m_lpCollection != NULL ? TRUE : FALSE;
}

void CAgoraCameraManager::Close()
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

UINT CAgoraCameraManager::GetDeviceCount()
{
	if (m_lpCollection == NULL)
		return 0;

	return (UINT)m_lpCollection->getCount();
}

std::string CAgoraCameraManager::GetCurDeviceID()
{
	std::string 		str;
	if (m_ptrDeviceManager == NULL || m_ptrDeviceManager->get() == NULL)
		return str;
		
	CHAR		szDeviceID[MAX_DEVICE_ID_LENGTH] = { 0 };

	memset(szDeviceID, 0x00, MAX_DEVICE_ID_LENGTH);
	if (m_ptrDeviceManager != NULL && *m_ptrDeviceManager != NULL)
		(*m_ptrDeviceManager)->getDevice(szDeviceID);

	str = szDeviceID;
#ifdef UNICODE
	//::MultiByteToWideChar(CP_UTF8, 0, szDeviceID, -1, str.GetBuffer(MAX_DEVICE_ID_LENGTH), MAX_DEVICE_ID_LENGTH);
//	str.ReleaseBuffer();
#else
	strDeviceName = szDeviceID;
#endif

	return str;
}

BOOL CAgoraCameraManager::GetDevice(UINT nIndex, std::string &rDeviceName, std::string &rDeviceID)
{
	CHAR szDeviceName[MAX_DEVICE_ID_LENGTH];
	CHAR szDeviceID[MAX_DEVICE_ID_LENGTH];

	ASSERT(nIndex < GetDeviceCount());
	if (nIndex >= GetDeviceCount())
		return FALSE;

	if(m_lpCollection == NULL)
		return FALSE;

	int nRet = m_lpCollection->getDevice(nIndex, szDeviceName, szDeviceID);
	if (nRet != 0)
		return FALSE;

	char ansiDevName[MAX_DEVICE_ID_LENGTH];
	CAgoralUtils::Convert(szDeviceName, ansiDevName, CP_UTF8, CP_ACP);

	char ansiDevId[MAX_DEVICE_ID_LENGTH];
	CAgoralUtils::Convert(szDeviceID, ansiDevId, CP_UTF8, CP_ACP);


	rDeviceID = ansiDevId;
	rDeviceName = ansiDevName;

	return TRUE;
}

BOOL CAgoraCameraManager::SetCurDevice(const char * pDeviceID)
{
	if (m_ptrDeviceManager == NULL || *m_ptrDeviceManager == NULL)
		return FALSE;

	int nRet = (*m_ptrDeviceManager)->setDevice(pDeviceID);


	return nRet == 0 ? TRUE : FALSE;
}

BOOL CAgoraCameraManager::TestCameraDevice(HWND hVideoWnd, BOOL bTestOn)
{
	BOOL ret = FALSE;
	if (bTestOn && !m_bTestingOn) {
		ASSERT(hVideoWnd != NULL);
		ret = CAgoraObject::GetAgoraObject()->LocalVideoPreview(hVideoWnd, TRUE);
	}
	else if(!bTestOn && m_bTestingOn){
		ret = CAgoraObject::GetAgoraObject()->LocalVideoPreview(NULL, FALSE);
	}

	if (ret) {
		m_bTestingOn = bTestOn;
	}
	return ret;
}