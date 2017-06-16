#pragma once

#include <string>

class CUtils {
public:
	CUtils();
	~CUtils();

	/**
	
	Convert(strA_in,strB_out,CP_UTF8,CP_ACP)//UTF8ת��ANSI
	Convert(strA_out,strB_in,CP_ACP,CP_UTF8)//ANSIת��UTF8
	*/
	static int Convert(const char* strIn, char* strOut, int sourceCodepage, int targetCodepage);

	static std::string getAppPath();

	static  std::wstring StringToWString(const std::string &str);

	 //ֻ�������ֽ���string�� 
	static std::string WStringToString(const std::wstring &wstr);
};

