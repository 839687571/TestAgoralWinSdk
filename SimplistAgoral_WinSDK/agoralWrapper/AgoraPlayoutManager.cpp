#include "StdAfx.h"
#include "AgoraPlayoutManager.h"
#include "AGResourceVisitor.h"
#include "AgoralUtils.h"




CAgoraPlayoutManager::CAgoraPlayoutManager()
	: m_ptrDeviceManager(NULL)
	, m_lpCollection(NULL)
	, m_bTestingOn(FALSE)
{
}


CAgoraPlayoutManager::~CAgoraPlayoutManager()
{
	Close();
}

BOOL CAgoraPlayoutManager::Create(IRtcEngine *lpRtcEngine)
{
	m_ptrDeviceManager = new AAudioDeviceManager(*lpRtcEngine);
	if (m_ptrDeviceManager->get() == NULL)
		return FALSE;

	m_lpCollection = (*m_ptrDeviceManager)->enumeratePlaybackDevices();
	if (m_lpCollection == NULL) {
		delete m_ptrDeviceManager;
		m_ptrDeviceManager = NULL;
	}

	return m_lpCollection != NULL ? TRUE : FALSE;
}

void CAgoraPlayoutManager::Close()
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

UINT CAgoraPlayoutManager::GetVolume()
{
	int nVol = 0;

	(*m_ptrDeviceManager)->getPlaybackDeviceVolume(&nVol);

	return (UINT)nVol;
}

BOOL CAgoraPlayoutManager::SetVolume(UINT nVol)
{
	int nRet = (*m_ptrDeviceManager)->setPlaybackDeviceVolume((int)nVol);

	return nRet == 0 ? TRUE : FALSE;
}

UINT CAgoraPlayoutManager::GetDeviceCount()
{
	return (UINT)m_lpCollection->getCount();
}

BOOL CAgoraPlayoutManager::GetDevice(UINT nIndex, std::string &rDeviceName, std::string &rDeviceID)
{
	CHAR szDeviceName[MAX_DEVICE_ID_LENGTH];
	CHAR szDeviceID[MAX_DEVICE_ID_LENGTH];

	ASSERT(nIndex < GetDeviceCount());
	if (nIndex >= GetDeviceCount())
		return FALSE;


	// ·µ»ØUTF8±àÂë
	int nRet = m_lpCollection->getDevice(nIndex, szDeviceName, szDeviceID);
	if (nRet != 0)
		return FALSE;

	char ansiDevName[MAX_DEVICE_ID_LENGTH];
	CAgoralUtils::Convert(szDeviceName, ansiDevName, CP_UTF8, CP_ACP);

	char ansiDevId[MAX_DEVICE_ID_LENGTH];
	CAgoralUtils::Convert(szDeviceID, ansiDevId, CP_UTF8, CP_ACP);

	rDeviceName = ansiDevName;
	rDeviceID = szDeviceID;

	return TRUE;
}

std::string CAgoraPlayoutManager::GetCurDeviceID()
{
	std::string		str;
	CHAR		szDeviceID[MAX_DEVICE_ID_LENGTH];
	
	(*m_ptrDeviceManager)->getPlaybackDevice(szDeviceID);

	char ansiDevId[MAX_DEVICE_ID_LENGTH];
	CAgoralUtils::Convert(szDeviceID, ansiDevId, CP_UTF8, CP_ACP);

	str = ansiDevId;

	return str;
}

BOOL CAgoraPlayoutManager::SetCurDevice(const char * lpDeviceID)
{

	int nRet = (*m_ptrDeviceManager)->setPlaybackDevice(lpDeviceID);

	return nRet == 0 ? TRUE : FALSE;
}

int   CAgoraPlayoutManager::TestPlaybackDevice(const char *fileName, BOOL bTestOn)
{
	int ret = 0;
	if (bTestOn && !m_bTestingOn) {
		ret = (*m_ptrDeviceManager)->startPlaybackDeviceTest(fileName);
	} if (!bTestOn && m_bTestingOn){
	ret  = (*m_ptrDeviceManager)->stopPlaybackDeviceTest();

	}
	if (ret == 0) {
		m_bTestingOn = bTestOn;
	}
	return ret;
}



void CAgoraPlayoutManager::TestPlaybackDevice(UINT nWavID, BOOL bTestOn)
{
	TCHAR	szWavPath[MAX_PATH];

	::GetModuleFileName(NULL, szWavPath, MAX_PATH);
	LPTSTR lpLastSlash = (LPTSTR)_tcsrchr(szWavPath, _T('\\')) + 1;
	_tcscpy_s(lpLastSlash, 16, _T("test.wav"));

	if (bTestOn && !m_bTestingOn) {
		CAGResourceVisitor::SaveResourceToFile(_T("WAVE"), nWavID, szWavPath);

#ifdef UNICODE
		CHAR szWavPathA[MAX_PATH];

		::WideCharToMultiByte(CP_ACP, 0, szWavPath, -1,szWavPathA , MAX_PATH, NULL, NULL);
		(*m_ptrDeviceManager)->startPlaybackDeviceTest(szWavPathA);
#else
		(*m_ptrDeviceManager)->startPlaybackDeviceTest(szWavPathA);
#endif


	}
	else if (!bTestOn && m_bTestingOn)
		(*m_ptrDeviceManager)->stopPlaybackDeviceTest();

	m_bTestingOn = bTestOn;

}