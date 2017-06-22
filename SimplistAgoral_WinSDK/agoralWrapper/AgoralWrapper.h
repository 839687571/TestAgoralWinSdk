#pragma once

#include "../stdafx.h"

#include "agoral/IAgoraMediaEngine.h"
#include "AgoraObject.h"
#include "AGEventDef.h"
#include <vector>

extern void LogMessage(char *msg);


typedef enum _AgoralVdieoProfile {
	//                         value resolution kbps
	BASE_120,                //	 0	160x120	15	65
	BASE_120_3 = 2,          //  2	120x120	15	50

	BASE_180 = 10,			//  10	320x180	15	140
	BASE_180_3 = 12,		//  12	180x180	15	100
	BASE_180_4 = 13,		//  13	240x180	15	120

	BASE_240 =  20,			//	20	320x240	15	200
	BASE_240_3 = 22,		//  22	240x240	15	140
	BASE_240_4 = 24,		//	24	424x240	15	220

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

class CAgoralWrapper
{
public:
	CAgoralWrapper();
	~CAgoralWrapper();


	static void GlobalInit();
	static void GlobalUinit();

	void InitAgoral(const char * logFile);

	void JoinChannel(const char * channelName);
	void BindRemoteVideoWnd(AGVIDEO_WNDINFO *videoInfo);

	void SetMainHWND(HWND hwnd){
		m_mMainHwnd = hwnd;
	}

	void SetLocalHwnd(HWND hwnd){
		m_loaclVideoWnd = hwnd;
	}

	void SetRemoetHwnd(HWND hwnd){
		m_remoteVideoWnd = hwnd;
	}

	// 处理引擎回调消息.
	void MsgHandle(DWORD msgId, WPARAM wParam);
private:

	void onUserJoinedMsg(DWORD msgId, WPARAM wParam);

	void onUserOfflineMsg(DWORD msgId, WPARAM wParam);

	void onHostJoinSuccess(DWORD msgId, WPARAM wParam);

	void onStatisticRemoteVideoInfo(DWORD msgId, WPARAM wParam);
	void onStatisticLocalVideoInfo(DWORD msgId, WPARAM wParam);

	void onLostConnect(DWORD msgId, WPARAM wParam);
	void onNetWorkQuality(DWORD msgId, WPARAM wParam);
	HWND  m_loaclVideoWnd; /* 本地视频 */
	HWND  m_remoteVideoWnd; /* 远端视频视频 -- 在 老师端  看到学生视频 为远程视频*/
	HWND  m_mMainHwnd;

	//分辨率
	int m_nVideoSolution;
	// agoral
	static IRtcEngine	*m_pRtcEngine;
	static CAgoraObject	*m_pAgoraObject;


	std::vector<AGVIDEO_WNDINFO>   m_vecJoinedUsers;
};

