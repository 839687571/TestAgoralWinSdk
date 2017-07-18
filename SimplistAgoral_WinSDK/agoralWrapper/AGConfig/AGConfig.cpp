#include "stdafx.h"
#include "AGConfig.h"
#include "../AgoralUtils.h"




CAGConfig::CAGConfig()
{

	std::string configFile  =  CAgoralUtils::getAppPath();
	configFile += "VideoConfig.ini";

	wFile = CAgoralUtils::StringToWString(configFile);

	if (::GetFileAttributes(wFile.c_str()) == INVALID_FILE_ATTRIBUTES) {
		HANDLE hFile = ::CreateFile(wFile.c_str(), GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		::CloseHandle(hFile);
	}

	
}


CAGConfig::~CAGConfig()
{
}


int CAGConfig::GetSolution()
{

#ifdef _MFC_
	CString strResolution;

	::GetPrivateProfileString(_T("VideoConfig"), _T("SolutionIndex"), _T("1"), strResolution.GetBuffer(MAX_PATH), MAX_PATH, wFile.c_str());

	strResolution.ReleaseBuffer();

	return _ttoi(strResolution);
#else 
	return 0;
#endif
}

BOOL CAGConfig::SetSolution(int nResolution)
{
#ifdef _MFC_
	CString strResolution;

	strResolution.Format(_T("%d"), nResolution);

	return ::WritePrivateProfileString(_T("VideoConfig"), _T("SolutionIndex"), strResolution, wFile.c_str());
#else 
	return TRUE;
#endif
}

BOOL CAGConfig::SetPPTSolution(int nResolution)
{
#ifdef _MFC_
	CString strResolution;

	strResolution.Format(_T("%d"), nResolution);

	return ::WritePrivateProfileString(_T("VideoConfig"), _T("PPTSolutionIndex"), strResolution, wFile.c_str());
#else 
	return TRUE;
#endif
}
int CAGConfig::GetPPTSolution()
{
#ifdef _MFC_
	CString strResolution;

	::GetPrivateProfileString(_T("VideoConfig"), _T("PPTSolutionIndex"), _T("1"), strResolution.GetBuffer(MAX_PATH), MAX_PATH, wFile.c_str());

	strResolution.ReleaseBuffer();

	return _ttoi(strResolution);
#else 
	return 0;
#endif
}

/*

int CAGConfig::GetCodec()
{
#ifdef ENABLE_CODEC
	CString strCodec;

	::GetPrivateProfileString(_T("VideoConfig"), _T("CodecIndex"), _T("1"), strCodec.GetBuffer(MAX_PATH), MAX_PATH, m_szConfigFile);

	strCodec.ReleaseBuffer();

	return _ttoi(strCodec);
#else
	return 0;
#endif
}

BOOL CAGConfig::SetCodec(int nCodec)
{
#ifdef ENABLE_CODEC
	CString strCodec;

	strCodec.Format(_T("%d"), nCodec);

	return ::WritePrivateProfileString(_T("VideoConfig"), _T("CodecIndex"), strCodec, m_szConfigFile);
#else
	return TRUE;
#endif
}

#endif

*/
/*
int CAGConfig::GetResolution()
{
	CString strResolution;

	::GetPrivateProfileString(_T("VideoConfig"), _T("ResolutionIndex"), _T("1"), strResolution.GetBuffer(MAX_PATH), MAX_PATH, m_szConfigFile);

	strResolution.ReleaseBuffer();

	return _ttoi(strResolution);
}

BOOL CAGConfig::SetResolution(int nResolution)
{
	CString strResolution;

	strResolution.Format(_T("%d"), nResolution);

	return ::WritePrivateProfileString(_T("VideoConfig"), _T("ResolutionIndex"), strResolution, m_szConfigFile);
}
/*
int CAGConfig::GetFPS()
{
	CString strFps;

	::GetPrivateProfileString(_T("VideoConfig"), _T("FPSIndex"), _T("1"), strFps.GetBuffer(MAX_PATH), MAX_PATH, m_szConfigFile);

	strFps.ReleaseBuffer();

	return _ttoi(strFps);
}

BOOL CAGConfig::SetFPS(int nPFS)
{
	CString strFPS;

	strFPS.Format(_T("%d"), nPFS);

	return ::WritePrivateProfileString(_T("VideoConfig"), _T("FPSIndex"), strFPS, m_szConfigFile);

}

int CAGConfig::GetMaxRate()
{
	CString strMaxRate;

	::GetPrivateProfileString(_T("VideoConfig"), _T("MaxRatePos"), _T("1"), strMaxRate.GetBuffer(MAX_PATH), MAX_PATH, m_szConfigFile);

	strMaxRate.ReleaseBuffer();

	return _ttoi(strMaxRate);
}

BOOL CAGConfig::SetMaxRate(int nMaxRate)
{
	CString strMaxRate;

	strMaxRate.Format(_T("%d"), nMaxRate);

	return ::WritePrivateProfileString(_T("VideoConfig"), _T("MaxRatePos"), strMaxRate, m_szConfigFile);
}
*/






// 
// BOOL CAGConfig::EnableAutoSave(BOOL bEnable)
// {
// 	CString strSave;
// 
// 	if (bEnable)
// 		strSave = _T("1");
// 	else
// 		strSave = _T("0");
// 
// 	return ::WritePrivateProfileString(_T("VideoConfig"), _T("SaveSetting"), strSave, m_szConfigFile);
// }
// 
// BOOL CAGConfig::IsAutoSaveEnabled()
// {
// 	WCHAR buf[];
// 
// 	::GetPrivateProfileString(_T("VideoConfig"), _T("SaveSetting"), _T("0"), strSaveSetting.GetBuffer(MAX_PATH), MAX_PATH, m_szConfigFile);
// 
// 	strSaveSetting.ReleaseBuffer();
// 
// 	return (_ttoi(strSaveSetting) == 1) ? TRUE : FALSE;
// }

