#pragma once

#include "../stdafx.h"

#include "agoral/IAgoraMediaEngine.h"
#include "AgoraObject.h"
#include "AGEventDef.h"
#include <vector>


//通信方式集成SDK, stundent 显示内容到老师端

// setChannelProfile  可以设置 模式:
/*
通信	通信为默认模式，用于常见的一对一或群聊，频道中的任何用户可以自由说话。
直播	直播模式有主播和观众两种用户角色，可以通过调用setClientRole设置。主播可收发语音和视频，但观众只能收，不能发。
游戏语音	频道内任何用户可自由发言。该模式下默认使用低功耗低码率的编解码器。

*/

extern void LogMessage(char *msg);

#define  VIRTUAL_IMPLE  {}

interface  IAgoralObjectMsgMsgObserver {
	    virtual void OnError(unsigned int err, const char *msg) VIRTUAL_IMPLE
		virtual void OnAuidoVolumeIndication(unsigned int uId, int volume) VIRTUAL_IMPLE
		virtual void OnAudioDevChange(const char *devId, int devType, int devState) VIRTUAL_IMPLE
		virtual void OnVideoDevChange(const char *devId, int devType, int devState) VIRTUAL_IMPLE
		virtual void OnLastmileQuality(int quality) VIRTUAL_IMPLE
		virtual void OnNetWorkQuality(int uid, int txQuality /*上行*/, int rxQuality/*下行*/) VIRTUAL_IMPLE

		virtual void OnLocalVideframeShow(unsigned int elapsed) VIRTUAL_IMPLE
		virtual void OnFirstRemoteVidoeFrameShow(unsigned uid, unsigned elapsed) VIRTUAL_IMPLE

		virtual void OnUserJoined(unsigned int uid, int elapsed)VIRTUAL_IMPLE
		virtual void OnUserOffline(unsigned int userId)VIRTUAL_IMPLE

		virtual void OnUserMuteAudio(unsigned int uid, bool muted) VIRTUAL_IMPLE
		virtual void OnUserMuteVideo(unsigned int uid, bool muted) VIRTUAL_IMPLE

		virtual void OnLocalVideoStats(const LocalVideoStats& stats) VIRTUAL_IMPLE
		virtual void OnRemoteVideoStats(const RemoteVideoStats& stats) VIRTUAL_IMPLE

		virtual void OnHostJoinSuccess(unsigned int userId) VIRTUAL_IMPLE

		virtual void OnVideoStoped() VIRTUAL_IMPLE
};



typedef enum _AgoralVdieoProfile {
	//                         value resolution kbps
	BASE_120,                //	 0	160x120	15	65
	BASE_120_3 = 2,          //  2	120x120	15	50

	BASE_180 = 10,			//  10	320x180	15	140
	BASE_180_3 = 12,		//  12	180x180	15	100
	BASE_180_4 = 13,		//  13	240x180	15	120

	BASE_240 =  20,			//	20	320x240	15	200
	BASE_240_3 = 22,		//  22	240x240	15	140
	BASE_240_4 = 23,		//	24	424x240	15	220

	BASE_360 = 30,			//	30	320x240	15	200
	BASE_360_3 = 32,		//  32	240x240	15	140
	BASE_360_4 = 33,		//	33	424x240	15	220
	BASE_360_6 = 35,		//	35	320x240	15	200
	BASE_360_7 = 36,		//  36	240x240	15	140
	BASE_360_8 = 37,		//	37	424x240	15	220
	BASE_360_9 = 38,		//	38	320x240	15	200
	BASE_360_10 = 39,		//  39	240x240	15	140
	BASE_360_11 = 100,		//	100	424x240	15	220

	BASE_480 = 40,			//	40	640x480	15	500
	BASE_480_3 = 42,		//  42	480x480	15	400
	BASE_480_4 = 43,		//	43	640x480	30	750
	BASE_480_6 = 45,		//	45	480x480	30	600
	BASE_480_8 = 47,		//	47	848x480	15	610
	BASE_480_9 = 48,		//  48	848x480	30	930


	BASE_720 = 50,		    //	50  1280x720 	15	1130
	BASE_720_3 = 52,		//	52	1280x720 	30	1710
	BASE_720_5 = 54,		//  54	960x720 	15	910
	BASE_720_6 = 55,		//	55	960x720 	30	1380
		 
	BASE_1080 = 60,		   //	60	1920x1080	15	2080
	BASE_1080_3 = 62,		//	62	1920x1080	30	3150
	BASE_1080_5 = 64,		//  64	1920x1080 	60	4780


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

typedef enum _ClientType {
	CLIENT_TYPE_TEACHER = 0,
	CLIENT_TYPE_STUDENT = 1,
	CLIENT_TYPE_TEACHER_CAMERA2 = 3,

}ClientType;

class CAgoralWrapper
{
public:
	CAgoralWrapper();
	~CAgoralWrapper();


	static void GlobalInit(const char *app_id);
	static void GlobalUinit();

	void InitAgoral(const char * logFile);

	BOOL JoinChannel(const char * channelName,INT32 uid,int solution);
	BOOL JoinChannel(const char* channelKey, const char *channelName, INT32 uid, int solution);

	void LeaveChanel();
	void BindVideoWnd(unsigned int uid,bool host);

	void SetMainHWND(HWND hwnd){
		m_mMainHwnd = hwnd;
		m_pAgoraObject->SetMsgHandlerWnd(m_mMainHwnd);
	}

	void SetBottomWnd(HWND hwnd){
		m_bottoomVideoWnd = hwnd;
	}
	
	void SetLocalCamera2Hwnd(HWND hwnd)
	{
		m_localCamera2Wnd = hwnd;
	}

	void SetTopWnd(HWND hwnd){
		m_topVideoWnd = hwnd;
	}

	void SetClientType(int type)
	{
		m_nClientType = type;
	}
	// 处理引擎回调消息.
	BOOL MsgHandle(DWORD msgId, WPARAM wParam);

	BOOL  IsHostUser(unsigned int uid);

	void SetMsgObserver(IAgoralObjectMsgMsgObserver *observer)
	{
		m_pObserver = observer;
	}
	//set before join channel 
	void SetAudioEnabled(BOOL enable)
	{
		IRtcEngine		*lpRtcEngine = CAgoraObject::GetEngine();
		if (!enable)
		lpRtcEngine->disableAudio();
		else {
			lpRtcEngine->enableAudio();
		}
	}
	BOOL SetVideoEnabled(BOOL enable)
	{
		return  m_pAgoraObject->EnableVideo(enable);
	}

	BOOL  SendChatMessage(const char *msg);
private:

	void onUserJoinedMsg(DWORD msgId, WPARAM wParam);
	void onUserOfflineMsg(DWORD msgId, WPARAM wParam);
	void onUserMuteAudio(DWORD msgId, WPARAM wParam);

	void onRejoinSuccess(DWORD msgId, WPARAM wParam);

	void onHostJoinSuccess(DWORD msgId, WPARAM wParam);
	void onUserLeaveChannel(DWORD msgId, WPARAM wParam);

	void onErrorMsg(DWORD msgId, WPARAM wParam);
	void onAudioQulity(DWORD msgId, WPARAM wParam);
	void onAudioVolumeIndication(DWORD msgId, WPARAM wParam);

	///void onLeaveChannel(DWORD msgId, WPARAM wParam);
	void onMediaEnginEvent(DWORD msgId, WPARAM wParam);

	void onAudioDevStateChanged(DWORD msgId, WPARAM wParam);
	void onVideoDevStateChanged(DWORD msgId, WPARAM wParam);
	void onLastmileQuality(DWORD msgId, WPARAM);
	/*
	QUALITY_UNKNOWN( = 0)
	QUALITY_EXCELLENT(1)
	QUALITY_GOOD(2)
	QUALITY_POOR(3)
	QUALITY_BAD(4)
	QUALITY_VBAD(5)
	QUALITY_DOWN(6)
	*/
	void onNetWorkQuality(DWORD msgId, WPARAM wParam);
	void onStatisticRemoteVideoInfo(DWORD msgId, WPARAM wParam);
	void onStatisticLocalVideoInfo(DWORD msgId, WPARAM wParam);

	void onLostConnect(DWORD msgId, WPARAM wParam);
	void onGetUserSendMessage(DWORD msgId, WPARAM wParam);
	void onFirstLocalVideoFrame(DWORD msgId, WPARAM wParam);
	void onFirstRemoteVideoDecoded(DWORD msgId, WPARAM wParam);
	void onFirstRemoteVideoShow(DWORD msgId, WPARAM wParam);
	void onUserMuteAuido(DWORD msgId, WPARAM wParam);
	void onUserMuteVideo(DWORD msgId, WPARAM wParam);
	void onApiCalled(DWORD msgId, WPARAM wParam);
	void onVideoStoped(DWORD msgId, WPARAM wParam);

	HWND  m_bottoomVideoWnd; /* 老师视频 */
	HWND  m_localCamera2Wnd; /*教师2的摄像头*/

	HWND  m_topVideoWnd; /* 远端视频视频 -- 在 老师端  看到学生视频 为远程视频*/


	HWND  m_mMainHwnd;

	//分辨率
	int m_nVideoSolution;
	// agoral
	static IRtcEngine	*m_pRtcEngine;
	static CAgoraObject	*m_pAgoraObject;

	static std::string  m_strAppId;

	int  m_nStreamID;
	/*
	1.学生.
	2.老师.
	3.老师2
	*/
	int  m_nClientType;

	std::vector<AGVIDEO_WNDINFO>   m_vecJoinedUsers;
	int  m_iHostId;

	HWND  m_hLocalWnd;
	HWND  m_hRemoteWnd;

	IAgoralObjectMsgMsgObserver  *m_pObserver;
};

