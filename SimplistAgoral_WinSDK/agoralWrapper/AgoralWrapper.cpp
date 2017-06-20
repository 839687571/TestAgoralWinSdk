#include "stdafx.h"
#include "AgoralWrapper.h"

#pragma comment( lib, "agorartc.lib")

void LogMessage(char *msg)
{
	OutputDebugStringA(msg);

	printf(msg);
	printf("\n");
}


IRtcEngine	*CAgoralWrapper::m_pRtcEngine = NULL;
CAgoraObject	*CAgoralWrapper::m_pAgoraObject = NULL;

CAgoralWrapper::CAgoralWrapper() 
{
}

CAgoralWrapper::~CAgoralWrapper()
{
}


void CAgoralWrapper::GlobalInit()
{
	if (m_pRtcEngine == NULL) {
		m_pRtcEngine = CAgoraObject::GetEngine();
	}
	if (m_pAgoraObject == NULL) {
		m_pAgoraObject = CAgoraObject::GetAgoraObject(APP_ID);
	}

	m_pAgoraObject->EnableVideo(TRUE);
}

void CAgoralWrapper::GlobalUinit()
{
	CAgoraObject::CloseAgoraObject();

	m_pAgoraObject = NULL;
	m_pAgoraObject = NULL;
}


void CAgoralWrapper::InitAgoral(const char *logFile)
{

	GlobalInit();

	m_pAgoraObject->SetLogFilePath(logFile);
	m_pAgoraObject->EnableNetworkTest(TRUE);
	m_pAgoraObject->SetMsgHandlerWnd(m_mMainHwnd);

}

void CAgoralWrapper::JoinChannel(const char * channelName)
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

	lpAgoraObject->SetEncryptionSecret("", 0);// USE DEFAULT encryption
	lpAgoraObject->JoinChannel(channelName);
	lpAgoraObject->SetMsgHandlerWnd(m_mMainHwnd);
}

void CAgoralWrapper::onUserJoinedMsg(DWORD msgId, WPARAM wParam)
{
	LPAGE_USER_JOINED lpData = (LPAGE_USER_JOINED)wParam;

	char buf[512];
	sprintf_s(buf, "==============================%d Joined the channel ", lpData->uid);
	LogMessage(buf);

	AGVIDEO_WNDINFO  videoInfo;
	videoInfo.nUID = lpData->uid;
	videoInfo.hHwnd = m_remoteVideoWnd;


	m_vecJoinedUsers.push_back(videoInfo);
	/*  FIXME
	第一个进入的 ,认为学生,后面的 观看者. 这个判断方法需要修改.
	当学生 掉线再次进入有问题.
	*/
	if (m_vecJoinedUsers.size() == 1) {  
		BindRemoteVideoWnd(&videoInfo);
	}

}

void CAgoralWrapper::onUserOfflineMsg(DWORD msgId, WPARAM wParam)
{
	LPAGE_USER_OFFLINE lpData = (LPAGE_USER_OFFLINE)wParam;
	char buf[512];
	sprintf_s(buf, "==============================%d leave the channel ", lpData->uid);
	LogMessage(buf);

	for (std::vector<AGVIDEO_WNDINFO>::iterator iter = m_vecJoinedUsers.begin();
		iter != m_vecJoinedUsers.end(); iter++) {
		if (((AGVIDEO_WNDINFO)(*iter)).nUID == lpData->uid) {
			m_vecJoinedUsers.erase(iter);
			break;
		}
	}
	
}
void CAgoralWrapper::onStatisticRemoteVideoInfo(DWORD msgId, WPARAM wParam)
{
	LPAGE_REMOTE_VIDEO_STAT lpData = (LPAGE_REMOTE_VIDEO_STAT)wParam;

	AGVIDEO_WNDINFO rWndInfo;
	rWndInfo.nFramerate = lpData->receivedFrameRate;
	rWndInfo.nBitrate = lpData->receivedBitrate;
	rWndInfo.nWidth = lpData->width;
	rWndInfo.nHeight = lpData->height;
	LogWinInfo(rWndInfo);
}
void CAgoralWrapper::onHostJoinSuccess(DWORD msgId, WPARAM wParam)
{
	LPAGE_JOINCHANNEL_SUCCESS lpData = (LPAGE_JOINCHANNEL_SUCCESS)wParam;
	CAgoraObject::GetAgoraObject()->SetSelfUID(lpData->uid);
	CAgoraObject *_pAgoraObject = CAgoraObject::GetAgoraObject();
	char buf[512];
	sprintf_s(buf, "===%d host join the channel channel name = %s  call id = %s verderkey  = %s ", lpData->uid,
		_pAgoraObject->GetChanelName().c_str()
		, _pAgoraObject->GetCallID().c_str()
		, _pAgoraObject->GetVendorKey().c_str());
	LogMessage(buf);
}

void CAgoralWrapper::MsgHandle(DWORD msgId, WPARAM wParam)
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

void CAgoralWrapper::BindRemoteVideoWnd(AGVIDEO_WNDINFO *videoInfo)
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
