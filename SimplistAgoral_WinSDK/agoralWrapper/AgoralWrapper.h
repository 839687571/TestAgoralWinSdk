#include "../stdafx.h"

#include "agoral/IAgoraMediaEngine.h"
#include "AgoraObject.h"
#include "AGEventDef.h"

extern void LogMessage(char *msg);


typedef enum _AgoralVdieoProfile {
	BASE_120,                //120P	0	160x120	15	65
	BASE_120_3 = 2,          //	2	120x120	15	50

	BASE_180 = 10,			//10	320x180	15	140
	BASE_180_3 = 12,		//12	180x180	15	100
	BASE_180_4 = 13,		//13	240x180	15	120

	BASE_240 =  20,			//	20	320x240	15	200
	BASE_240_3 = 22,		//22	240x240	15	140
	BASE_240_4 = 24,		//	24	424x240	15	220

	BASE_360 = 30,			//	20	320x240	15	200
	BASE_360_3 = 32,		//22	240x240	15	140
	BASE_360_4 = 33,		//	24	424x240	15	220
	BASE_360_6 = 35,			//	20	320x240	15	200
	BASE_360_7 = 36,		//22	240x240	15	140
	BASE_360_8 = 37,		//	24	424x240	15	220
	BASE_360_9 = 38,			//	20	320x240	15	200
	BASE_360_10 = 39,		//22	240x240	15	140
	BASE_360_11 = 100,		//	24	424x240	15	220
};	

typedef struct _AGVIDEO_WNDINFO {
	UINT	nUID;
	int		nIndex;

	UINT	nWidth;
	UINT	nHeight;
	int		nBitrate;
	int		nFramerate;
	int		nCodec;
	HWND    hHwnd;

} AGVIDEO_WNDINFO, *PAGVIDEO_WNDINFO, *LPAGVIDEO_WNDINFO;



static void LogWinInfo(AGVIDEO_WNDINFO &info)
{
	char buf[512];
	sprintf_s(buf, " video width = %d  video height  = %d, br = %d kbps  fps = %d", info.nWidth,info.nHeight,info.nBitrate,info.nFramerate);
	LogMessage(buf);
}


typedef enum  _UserIdType
{
	USER_ID_STUDENT,
	USER_ID_TEACHER,
}UserIdType;

class CAgoralWrapper
{
public:
	CAgoralWrapper();
	~CAgoralWrapper();

	void initAgoral(){
		
		m_pAgoraObject = CAgoraObject::GetAgoraObject(APP_ID);
		m_pRtcEngine = CAgoraObject::GetEngine();

		m_pAgoraObject->EnableVideo(TRUE);
		m_pAgoraObject->SetLogFilePath(NULL);
		m_pAgoraObject->EnableNetworkTest(TRUE);
		m_pAgoraObject->SetMsgHandlerWnd(m_mMainHwnd);

	}

	void joinChannel( const char * channelName)
	{
		IRtcEngine		*lpRtcEngine = CAgoraObject::GetEngine();
		CAgoraObject	*lpAgoraObject = CAgoraObject::GetAgoraObject();

		VideoCanvas vc;
		vc.uid = 0;
		vc.view = m_loaclVideoWnd;
		vc.renderMode = RENDER_MODE_TYPE::RENDER_MODE_FIT;
		m_nVideoSolution = BASE_120; //480P	40	640x480	15	500
	
		lpRtcEngine->setVideoProfile((VIDEO_PROFILE_TYPE)m_nVideoSolution, FALSE);
		lpRtcEngine->setupLocalVideo(vc);
		lpRtcEngine->disableAudio();

		lpRtcEngine->startPreview();

		lpAgoraObject->JoinChannel(channelName);
		lpAgoraObject->SetMsgHandlerWnd(m_mMainHwnd);
	}
	void bindRemoteVideoWnd(AGVIDEO_WNDINFO *videoInfo)
	{
		if (m_loaclVideoWnd == NULL || m_remoteVideoWnd == NULL) {
			return;
		}
		VideoCanvas canvas;
		canvas.renderMode = RENDER_MODE_FIT;

		canvas.uid = videoInfo->nUID;
		canvas.view = videoInfo->hHwnd;
		CAgoraObject::GetEngine()->setupRemoteVideo(canvas);
	}

	void setMainHWND(HWND hwnd){
		m_mMainHwnd = hwnd;
	}

	void setLocalHwnd(HWND hwnd){
		m_loaclVideoWnd = hwnd;
	}

	void setRemoetHwnd(HWND hwnd){
		m_remoteVideoWnd = hwnd;
	}

	// 处理引擎回调消息.
	void MsgHandle(DWORD msgId, WPARAM wParam)
	{
		switch (msgId) {
		case  WM_MSGID(EID_JOINCHANNEL_SUCCESS):
			onHostJoinSuccess(msgId, wParam);
			break;
		case  WM_MSGID(EID_USER_JOINED):
			onUserJoinedMsg(msgId, wParam);
			break;
		case WM_MSGID(EID_USER_OFFLINE):
			onUserOfflineMsg(msgId, wParam);
			break;
		case WM_MSGID(EID_REMOTE_VIDEO_STAT):
			onStatisticRemoteVideoInfo(msgId, wParam);
			break;
// 		case WM_MSGID(EID_REMOTE_VIDEO_STAT):
// 			onStatisticRemoteVideoInfo(msgId, wParam);
// 			break;
		default:
			break;
		}
	}

	void onUserJoinedMsg(DWORD msgId, WPARAM wParam)
	{
		LPAGE_USER_JOINED lpData = (LPAGE_USER_JOINED)wParam;

		char buf[512];
		sprintf_s(buf, "==============================%d Joined the channel ", lpData->uid);
		LogMessage(buf);

		AGVIDEO_WNDINFO  videoInfo;
		videoInfo.nUID = lpData->uid;
		videoInfo.hHwnd = m_remoteVideoWnd;
		bindRemoteVideoWnd(&videoInfo);
	}

	void onUserOfflineMsg(DWORD msgId, WPARAM wParam)
	{
		LPAGE_USER_OFFLINE lpData = (LPAGE_USER_OFFLINE)wParam;
		char buf[512];
		sprintf_s(buf, "==============================%d leave the channel ", lpData->uid);
		LogMessage(buf);
	}

	void onHostJoinSuccess(DWORD msgId, WPARAM wParam)
	{
		LPAGE_JOINCHANNEL_SUCCESS lpData = (LPAGE_JOINCHANNEL_SUCCESS)wParam;
		CAgoraObject::GetAgoraObject()->SetSelfUID(lpData->uid);
		CAgoraObject *_pAgoraObject = CAgoraObject::GetAgoraObject();
		char buf[512];
		sprintf_s(buf, "===%d host join the channel channel name = %s  call id = %s verderkey  = %s ", lpData->uid,
			_pAgoraObject->GetChanelName().c_str()
			,_pAgoraObject->GetCallID().c_str()
			, _pAgoraObject->GetVendorKey().c_str());
		LogMessage(buf);
	}

	void onStatisticRemoteVideoInfo(DWORD msgId, WPARAM wParam)
	{
		LPAGE_REMOTE_VIDEO_STAT lpData = (LPAGE_REMOTE_VIDEO_STAT)wParam;

		AGVIDEO_WNDINFO rWndInfo;
		rWndInfo.nFramerate = lpData->receivedFrameRate;
		rWndInfo.nBitrate = lpData->receivedBitrate;
		rWndInfo.nWidth = lpData->width;
		rWndInfo.nHeight = lpData->height;
		LogWinInfo(rWndInfo);
	}
private:
	HWND  m_loaclVideoWnd; /* 本地视频 */
	HWND  m_remoteVideoWnd; /* 远端视频视频 -- 在 老师端  看到学生视频 为远程视频*/
	HWND  m_mMainHwnd;

	//分辨率
	int m_nVideoSolution;
	// agoral
	IRtcEngine		*m_pRtcEngine;
	CAgoraObject	*m_pAgoraObject;

};

