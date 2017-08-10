#pragma once

//#include "bugTrap/BugTrap.h"
// 
// #include "ComUtil.h"
// #include "profile.h"

#include <string>
#include <string>
// Include main BugTrap header.


class  CComUtil {
public:
	static std::wstring MutilByteToWide(const std::string& _src)
	{
		//计算字符串 string 转成 wchar_t 之后占用的内存字节数;
		int nBufSize = MultiByteToWideChar(GetACP(), 0, _src.c_str(), -1, NULL, 0);
		//为 wsbuf 分配内存 BufSize 个字节;
		wchar_t *wsBuf = new wchar_t[nBufSize];
		//转化为 unicode 的 WideString;
		MultiByteToWideChar(GetACP(), 0, _src.c_str(), -1, wsBuf, nBufSize);
		std::wstring wstrRet(wsBuf);
		delete[]wsBuf;
		wsBuf = NULL;
		return wstrRet;
	}
	static std::wstring AnsiToUnicode(const char* str)
	{
		DWORD dwNum = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
		wchar_t *pwText;
		pwText = new wchar_t[dwNum];
		if (!pwText) {
			delete[]pwText;
		}
		MultiByteToWideChar(CP_ACP, 0, str, -1, pwText, dwNum);
		std::wstring wstrTmp(pwText);
		delete[]pwText;
		return wstrTmp;
	}

	static std::string WChar2Ansi(const wchar_t* pwszSrc)
	{
		int nLen = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, NULL, 0, NULL, NULL);

		if (nLen <= 0) return std::string("");

		char* pszDst = new char[nLen];
		if (NULL == pszDst) return std::string("");

		WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, pszDst, nLen, NULL, NULL);
		pszDst[nLen - 1] = 0;

		std::string strTemp(pszDst);
		delete[] pszDst;

		return strTemp;
	}
};

typedef enum BUGTRAP_LOGLEVEL_tag {
	/**
	* @brief All message levels are prohibited.
	*/
	BTLL_NONE = 0,
	/**
	* @brief Error message.
	*/
	BTLL_ERROR = 1,
	/**
	* @brief Waning message.
	*/
	BTLL_WARNING = 2,
	/**
	* @brief Important information message.
	*/
	BTLL_IMPORTANT = 3,
	/**
	* @brief Regular information message.
	*/
	BTLL_INFO = 4,
	/**
	* @brief Verbose information message.
	*/
	BTLL_VERBOSE = 5,
	/**
	* @brief All levels of messages are accepted.
	*/
	BTLL_ALL = BTLL_INFO
}
BUGTRAP_LOGLEVEL;

typedef   struct _TLoggerSetting {
	bool showLogLevel;
	bool showLogTime;
	int  maxLogEntries;
	int  maxLogBytes;
	int  logLevel;
}TLoggerSetting;

class QQTrace;


extern void LogMessage(char *msg);

class  BugTrapWrapper {
public:
	BugTrapWrapper() {
	
	}

	~BugTrapWrapper()
	{

	}

public:

	static void SetupExceptionHandler(LPCTSTR appName)
	{

	}
	//static void InitLogger( CProfile *config);

	//static BTTrace* GetLogger() { return &m_logger; }
	static QQTrace *GetQQLogger() { return &m_qqLogger; }

private:
	static void CALLBACK postCrashCallback(INT_PTR cbParam)
	{

	}


private:
	//static BTTrace m_logger;
	static QQTrace m_qqLogger;
};



#define  LEN  1024

class  QQTrace {
	//
public:
	QQTrace()
	{
		m_pLogger = NULL;
	}

	void InitQQTrace()
	{
		//m_pLogger = BugTrapWrapper::GetLogger();
	}

	/// Append entry to the end of custom log file.
	BOOL AppendV(BUGTRAP_LOGLEVEL eLogLevel, LPCTSTR module, LPCTSTR pszFormat, va_list argList) const
	{
		BOOL ret;


		return ret;
	}

	/// Append entry to the end of custom log file.
	BOOL Append(LPCTSTR pszEntry) const
	{
		LogMessage((char *)(CComUtil::WChar2Ansi(pszEntry).c_str()));
		if (m_pLogger == NULL) return TRUE;
		BOOL ret;

		return ret;
	}

	BOOL Append(LPCTSTR module,LPCTSTR pszEntry) const
	{
		LogMessage((char *)(CComUtil::WChar2Ansi(pszEntry).c_str()));
		if (m_pLogger == NULL) return TRUE;
		BOOL ret;
	//	WCHAR  *module = L"MAINAPP";
	
		return ret;
	}

	BOOL Append(BUGTRAP_LOGLEVEL eLogLevel,LPCTSTR pszEntry) const
	{
		LogMessage((char *)(CComUtil::WChar2Ansi(pszEntry).c_str()));
		if (m_pLogger == NULL) return TRUE;

		return  FALSE;
	}

	BOOL Append(BUGTRAP_LOGLEVEL eLogLevel, LPCTSTR module , LPCTSTR pszEntry) const
	{
		LogMessage((char *)(CComUtil::WChar2Ansi(pszEntry).c_str()));
		if (m_pLogger == NULL) return TRUE;
		//WCHAR  *module = L"MAINAPP";

	}

	BOOL AppendF( LPCTSTR pszFormat, ...) const
	{
		va_list argList;
		va_start(argList, pszFormat);
		WCHAR  *module = L"MAINAPP";
		int len = wcslen(pszFormat);
	
		WCHAR *pszfmt = new WCHAR[len + LEN];
		int  cnt = vswprintf_s(pszfmt, len + LEN, pszFormat, argList);
		LogMessage((char *)(CComUtil::WChar2Ansi(pszfmt).c_str()));
		delete pszfmt;

	
		va_end(argList);
		return TRUE;

	}

	BOOL AppendF(LPCTSTR module,LPCTSTR pszFormat, ...) const
	{
		va_list argList;
		va_start(argList, pszFormat);
		int len = wcslen(pszFormat);

		WCHAR *pszfmt = new WCHAR[len + LEN];
		int  cnt = vswprintf_s(pszfmt, len + LEN, pszFormat, argList);
		LogMessage((char *)(CComUtil::WChar2Ansi(pszfmt).c_str()));
		delete pszfmt;


		va_end(argList);
		return TRUE;

	}



	BOOL AppendF(BUGTRAP_LOGLEVEL eLogLevel, LPCTSTR module,  LPCTSTR pszFormat, ...) const
	{
		va_list argList;
		va_start(argList, pszFormat);
		int len = wcslen(pszFormat);

		WCHAR *pszfmt = new WCHAR[len + LEN];
		int  cnt = vswprintf_s(pszfmt, len + LEN,pszFormat, argList);
		LogMessage((char *)(CComUtil::WChar2Ansi(pszfmt).c_str()));
		delete pszfmt;

		va_end(argList);
		return TRUE;
	}


private:
	 void *m_pLogger;
};

