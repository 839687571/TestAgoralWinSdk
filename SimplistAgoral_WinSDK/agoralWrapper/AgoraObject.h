#pragma once

#include "agoral/IAgoraRtcEngine.h"
#include "AGEngineEventHandler.h"
#include <string>

#define AVC_VER _T("V1.7.0, Build233, 09/29/2016, SDK1.7.0")

using namespace agora::rtc;

// 引擎标记位
#define AG_ENGFLAG_ENNETTEST	0x00000001
#define AG_ENGFLAG_ECHOTEST		0x00000002
#define AG_ENGFLAG_SPKPHTEST	0x00000004
#define AG_ENGFLAG_MICPHTEST	0x00000008
#define AG_ENGFLAG_VIDEOTEST	0x00000010

#define AG_ENGFLAG_AGCON		0x00000100	// 自动增益
#define AG_ENGFLAG_NSON			0x00000200	// 降噪
#define AG_ENGFLAG_AECON		0x00000400	// 回声消除

#define AG_CODEC_E264	0x00000000
#define AG_CODEC_EVP	0x00000001
#define AG_CODEC_VP8	0x00000002

//#define APP_ID ("")


class CAgoraObject
{
public:
	~CAgoraObject(void);

	void SetNetworkTestFlag(BOOL bEnable);
	BOOL GetNetworkTestFlag();

	void SetEchoTestFlag(BOOL bEnable);
	BOOL GetEchoTestFlag();

	void SetSpeakerphoneTestFlag(BOOL bEnable);
	BOOL GetSpeakerphoneTestFlag();

	void SetMicrophoneTestFlag(BOOL bEnable);
	BOOL GetMicrophoneTestFlag();

	void SetVideoTestFlag(BOOL bEnable);
	BOOL GetVideoTestFlag();

	void SetMsgHandlerWnd(HWND hWnd);
	HWND GetMsgHandlerWnd();

	BOOL SetLogFilePath(const char *  lpLogPath = NULL);

	BOOL SetVideoProfile2(int nWidth, int nHeight, int nFrameRate, int nBitRate, BOOL bFineTurn);

	BOOL JoinChannel(const char* lpChannelName, UINT nUID = 0, const char* lpDynamicKey = NULL);
	//BOOL JoinChannel(const char * lpChannelName,UINT uUID);
	BOOL JoinChannel(const  char *channelKey, const char * lpChannelName, UINT nUID = 0);
	BOOL LeaveCahnnel();

	std::string GetChanelName();
	std::string GetCallID();
	std::string GetVendorKey() { return m_strVendorKey; };


	void SetSelfUID(UINT nUID) { m_nSelfUID = nUID; };
	UINT GetSelfUID() { return m_nSelfUID; };

	BOOL EnableVideo(BOOL bEnable = TRUE);
	BOOL IsVideoEnabled();

	BOOL EnableWebSdkInteroperability(bool bEnable);

	BOOL EnableScreenCapture(HWND hWnd, int nCapFPS = 15, LPCRECT lpCapRect = NULL, BOOL bEnable = TRUE);
	BOOL IsScreenCaptureEnabled();

	BOOL MuteLocalAudio(BOOL bMuted = TRUE);
	BOOL IsLocalAudioMuted();


	BOOL MuteLocalVideo(BOOL bMuted = TRUE);
	BOOL IsLocalVideoMuted();


	BOOL EnableAudioRecording(BOOL bEnable, LPCTSTR lpFilePath);

	BOOL EnableLastmileTest(BOOL bEnable);

	BOOL LocalVideoPreview(HWND hVideoWnd, BOOL bPreviewOn = TRUE);

	BOOL SetLogFilter(LOG_FILTER_TYPE logFilterType, LPCTSTR lpLogPath);

	BOOL SetEncryptionSecret(const char * lpKey, int nEncryptType = 0);

    BOOL EnableLocalRender(BOOL bEnable);


    int CreateMessageStream();
	BOOL SendChatMessage(int nStreamID, const char *  lpChatMessage);


	static IRtcEngine *GetEngine();
	
	static std::string GetSDKVersion();
	
	static BOOL EnableWhiteboardVer(BOOL bEnable);
	static BOOL EnableWhiteboardFeq(BOOL bEnable);

    

protected:
	CAgoraObject(void);

private:
	DWORD	m_dwEngineFlag;
	static  CAgoraObject	*m_lpAgoraObject;
	static	IRtcEngine	    *m_lpAgoraEngine;
	static	std::string 	m_strVendorKey;
	std::wstring			m_strAppCert;
	
	UINT		m_nSelfUID;
	std::string		m_strChannelName;
	BOOL		m_bVideoEnable;


	BOOL		m_bLocalAudioMuted;
	BOOL		m_bLocalVideoMuted;

	BOOL		m_bScreenCapture;

	std::string  m_logFileName;
//	int			m_nCodecType;
public:
	static CAgoraObject *GetAgoraObject(const char * lpVendorKey = NULL);
	static void CloseAgoraObject();

	static CAGEngineEventHandler m_EngineEventHandler;
};
