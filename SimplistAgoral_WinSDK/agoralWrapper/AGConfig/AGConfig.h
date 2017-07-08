#pragma once

#include <string>
# if 1
class CAGConfig
{
public:
	CAGConfig();
	~CAGConfig();

	//int GetCodec();
	//BOOL SetCodec(int nCodec);

/*	int GetResolution();
	BOOL SetResolution(int nResolution);

	int GetFPS();
	BOOL SetFPS(int nPFS);

	int GetMaxRate();
	BOOL SetMaxRate(int nMaxRate);
*/
// 	BOOL EnableAutoSave(BOOL bEnable);
// 	BOOL IsAutoSaveEnabled();
// 
    BOOL SetSolution(int nIndex);
    int GetSolution();


	BOOL SetPPTSolution(int nIndex);
	int GetPPTSolution();

private:
	///TCHAR m_szConfigFile[MAX_PATH];


	std::wstring wFile;
};

#endif
