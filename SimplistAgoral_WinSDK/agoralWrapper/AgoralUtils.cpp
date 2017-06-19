#include "stdafx.h"
#include "AgoralUtils.h"


CAgoralUtils::CAgoralUtils()
{
}


CAgoralUtils::~CAgoralUtils()
{
}

int  CAgoralUtils::Convert(const char* strIn, char* strOut, int sourceCodepage, int targetCodepage)
{

	int len = strlen(strIn);
	int unicodeLen = MultiByteToWideChar(sourceCodepage, 0, strIn, -1, NULL, 0);
	wchar_t* pUnicode;
	pUnicode = new wchar_t[unicodeLen + 1];
	memset(pUnicode, 0, (unicodeLen + 1)*sizeof(wchar_t));
	MultiByteToWideChar(sourceCodepage, 0, strIn, -1, (LPWSTR)pUnicode, unicodeLen);
	BYTE * pTargetData = NULL;
	int targetLen = WideCharToMultiByte(targetCodepage, 0, (LPWSTR)pUnicode, -1, (char *)pTargetData, 0, NULL, NULL);
	pTargetData = new BYTE[targetLen + 1];
	memset(pTargetData, 0, targetLen + 1);
	int outLen = WideCharToMultiByte(targetCodepage, 0, (LPWSTR)pUnicode, -1, (char *)pTargetData, targetLen, NULL, NULL);
	strcpy(strOut, (char*)pTargetData);
	delete pUnicode;
	delete pTargetData;

	return outLen;

}


std::wstring CAgoralUtils::StringToWString(const std::string &str)
{
	std::wstring wstr(str.length(), L' ');
	std::copy(str.begin(), str.end(), wstr.begin());
	return wstr;
}


std::string CAgoralUtils::WStringToString(const std::wstring &wstr)
{
	std::string str(wstr.length(), ' ');
	std::copy(wstr.begin(), wstr.end(), str.begin());
	return str;
}


std::string  CAgoralUtils::getAppPath()
{
	TCHAR szPath[MAX_PATH] = { 0 };
	::GetModuleFileName(NULL, szPath, MAX_PATH);

	std::wstring strPath(szPath);
	for (int nPos = (int)strPath.size() - 1; nPos >= 0; --nPos) {
		TCHAR cChar = strPath[nPos];
		if (_T('\\') == cChar || _T('/') == cChar)
			return CAgoralUtils::WStringToString(strPath.substr(0, nPos + 1));
	}
	return CAgoralUtils::WStringToString(strPath);
}
