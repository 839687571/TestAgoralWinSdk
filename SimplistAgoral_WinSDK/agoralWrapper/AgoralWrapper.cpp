#include "stdafx.h"
#include "AgoralWrapper.h"

#pragma comment( lib, "agorartc.lib")

void LogMessage(char *msg)
{
	if (msg == NULL) return;
	OutputDebugStringA(msg);

	printf(msg);
	printf("\n");
}


IRtcEngine	 *CAgoralWrapper::m_pRtcEngine = NULL;
CAgoraObject *CAgoralWrapper::m_pAgoraObject = NULL;
std::string  CAgoralWrapper::m_strAppId = "";

CAgoralWrapper::CAgoralWrapper():
m_pObserver(NULL)
{
}

CAgoralWrapper::~CAgoralWrapper()
{
}


void CAgoralWrapper::GlobalInit(const char *app_id)
{
	m_strAppId = app_id;
	if (m_pRtcEngine == NULL) {
		m_pRtcEngine = CAgoraObject::GetEngine();
	}
	if (m_pAgoraObject == NULL) {
		m_pAgoraObject = CAgoraObject::GetAgoraObject(app_id);
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

	GlobalInit(m_strAppId.c_str());

	m_pAgoraObject->SetLogFilePath(logFile);
	///m_pAgoraObject->EnableNetworkTest(TRUE);
	m_pAgoraObject->SetMsgHandlerWnd(m_mMainHwnd);

}

void CAgoralWrapper::JoinChannel(const char * channelName, INT32 uid, int solution)
{
	IRtcEngine		*lpRtcEngine = CAgoraObject::GetEngine();
	CAgoraObject	*lpAgoraObject = CAgoraObject::GetAgoraObject();

	//m_nVideoSolution = BASE_120; //480P	40	640x480	15	500
	//应在调用 enableVideo 后设置视频属性。
	//应在调用 joinChannel / startPreview 前设置视频属性。
	lpRtcEngine->setVideoProfile((VIDEO_PROFILE_TYPE)solution, FALSE);
	
	//lpRtcEngine->disableAudio();

	lpRtcEngine->startPreview();

	lpAgoraObject->SetEncryptionSecret("", 0);// USE DEFAULT encryption
	lpAgoraObject->JoinChannel(channelName,uid);
	lpAgoraObject->SetMsgHandlerWnd(m_mMainHwnd);

	// 创建数据流(createDataStream)
	m_nStreamID = CAgoraObject::GetAgoraObject()->CreateMessageStream();
	///lpAgoraObject->EnableNetworkTest(TRUE);
}
void CAgoralWrapper::LeaveChanel()
{
	CAgoraObject	*lpAgoraObject = CAgoraObject::GetAgoraObject();
	lpAgoraObject->LeaveCahnnel();
}
void CAgoralWrapper::onUserJoinedMsg(DWORD msgId, WPARAM wParam)
{
	LPAGE_USER_JOINED lpData = (LPAGE_USER_JOINED)wParam;




	AGVIDEO_WNDINFO  videoInfo;
	videoInfo.nUID = lpData->uid;

	m_vecJoinedUsers.push_back(videoInfo);
	///if (m_vecJoinedUsers.size() == 1) 
	{  
		BindVideoWnd(&videoInfo);
	}

	char buf[512];
	sprintf_s(buf, "==============================role = %d Joined the channel ", videoInfo.nUID);
	LogMessage(buf);


	delete lpData;
}

void CAgoralWrapper::onUserOfflineMsg(DWORD msgId, WPARAM wParam)
{
	PAGE_USER_OFFLINE lpData = (PAGE_USER_OFFLINE)wParam;
	char buf[512];
	sprintf_s(buf, "==============================%d user leave the channel   ", lpData->uid);
	LogMessage(buf);

	for (std::vector<AGVIDEO_WNDINFO>::iterator iter = m_vecJoinedUsers.begin();
		iter != m_vecJoinedUsers.end(); iter++) {
		if (((AGVIDEO_WNDINFO)(*iter)).nUID == lpData->uid) {
			m_vecJoinedUsers.erase(iter);
			break;
		}
	}

	if (m_pObserver != NULL) {
		m_pObserver->OnUserOffline(lpData->uid);
	}
	delete lpData;
}
void CAgoralWrapper::onUserLeaveChannel(DWORD msgId, WPARAM wParam)
{
	PAGE_LEAVE_CHANNEL lpData = (PAGE_LEAVE_CHANNEL)wParam;

	char buf[512];
	sprintf_s(buf, "==============================%d leave the channel because postive ", lpData->rtcStat.users);
	LogMessage(buf);

	if (m_pObserver != NULL) {
		m_pObserver->OnUserOffline(lpData->rtcStat.users);
	}

	delete lpData;
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

	delete lpData;
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


	AGVIDEO_WNDINFO  videoInfo;
	videoInfo.nUID = lpData->uid;
	BindVideoWnd(&videoInfo);

	LogMessage(buf);
	delete lpData;
}
void CAgoralWrapper::onStatisticLocalVideoInfo(DWORD msgId, WPARAM wParam)
{
	PAGE_LOCAL_VIDEO_STAT lpData = (PAGE_LOCAL_VIDEO_STAT)wParam;

	char buf[512];
	sprintf_s(buf, "===sentbitrate = %d, sentload fps = %d ", lpData->sentBitrate,
		lpData->sentFrameRate);
	LogMessage(buf);
	delete lpData;
}

void CAgoralWrapper::onLostConnect(DWORD msgId, WPARAM wParam)
{
	char buf[512];
	sprintf_s(buf, "=== host is lost connect ");
	LogMessage(buf);
}


void CAgoralWrapper::onNetWorkQuality(DWORD msgId, WPARAM wParam)
{
	PAGE_LASTMILE_QUALITY pData = (PAGE_LASTMILE_QUALITY)wParam;
	char buf[512];
	sprintf_s(buf, "=== net work qulity  = %d", pData->quality);
	LogMessage(buf);

	delete pData;
}

void CAgoralWrapper::onGetUserSendMessage(DWORD msgId, WPARAM wParam)
{
	LPAGE_STREAM_MESSAGE lpData = new AGE_STREAM_MESSAGE;


	char buf[512];
	sprintf_s(buf, "=== receive user = %d send msg %s ", lpData->uid,lpData->data);
	LogMessage(buf);

	delete lpData;
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
	case WM_MSGID(EID_LOCAL_VIDEO_STAT):
		onStatisticLocalVideoInfo(msgId, wParam);
		break;
	case WM_MSGID(EID_CONNECTION_LOST):
		onLostConnect(msgId, wParam);
		break;
	case WM_MSGID(EID_NETWORK_QULITY): //频道内网络质量
		onNetWorkQuality(msgId, wParam);
		break;
	case WM_MSGID(EID_STREAM_MESSAGE):
		onGetUserSendMessage(msgId, wParam);
		break;
	case WM_MSGID(EID_LEAVE_CHANNEL):
		onUserLeaveChannel(msgId, wParam);
	//case WM_MSGID(EID_JOINCHANNEL_SUCCESS):
	//	onGetUserSendMessage(msgId, wParam);
		break;
	default:
		break;
	}
}

void CAgoralWrapper::BindVideoWnd(AGVIDEO_WNDINFO *videoInfo)
{
	UserInfo userType;
	userType.userId = videoInfo->nUID;
	if (userType.role == ROLE_TEACHER_CAMERA2) {
		videoInfo->hHwnd = m_localCamera2Wnd;
	} else if (userType.role == ROLE_TEACHER_MAIN) {
		videoInfo->hHwnd = m_TeacherVideoWnd;
	} else if (userType.role == ROLE_STUDENT) {
		videoInfo->hHwnd = m_remoteVideoWnd;
	}

	if (videoInfo->hHwnd == NULL) return;
	///assert(videoInfo->hHwnd != NULL);

	if (m_nClientType == CLIENT_TYPE_STUDENT) {//
		if (userType.role == ROLE_STUDENT) {
			VideoCanvas canvas;
			canvas.renderMode = RENDER_MODE_FIT;
			canvas.uid = videoInfo->nUID;
			canvas.view = videoInfo->hHwnd;
			CAgoraObject::GetEngine()->setupLocalVideo(canvas);
		} else {
			if (userType.role == ROLE_TEACHER_CAMERA2) {
			///	CAgoraObject::GetAgoraObject()->setRemoteVideoStreamType(userType.userId, REMOTE_VIDEO_STREAM_HIGH);
			}
			VideoCanvas canvas;
			canvas.renderMode = RENDER_MODE_FIT;
			canvas.uid = videoInfo->nUID;
			canvas.view = videoInfo->hHwnd;
			CAgoraObject::GetEngine()->setupRemoteVideo(canvas);
		}
		
	} else if (m_nClientType == CLIENT_TYPE_TEACHER) {
		if (userType.role == ROLE_TEACHER_MAIN) {
			VideoCanvas canvas;
			canvas.renderMode = RENDER_MODE_FIT;
			canvas.uid = videoInfo->nUID;
			canvas.view = videoInfo->hHwnd;
			CAgoraObject::GetEngine()->setupLocalVideo(canvas);
		} else {
			VideoCanvas canvas;
			canvas.uid = videoInfo->nUID;
			canvas.view = videoInfo->hHwnd;;
			canvas.renderMode = RENDER_MODE_TYPE::RENDER_MODE_FIT;
			CAgoraObject::GetEngine()->setupRemoteVideo(canvas);
		}

	} else if (m_nClientType == CLIENT_TYPE_TEACHER_CAMERA2) {
		if (userType.role == ROLE_TEACHER_CAMERA2) {
			VideoCanvas canvas;
			canvas.renderMode = RENDER_MODE_FIT;
			canvas.uid = videoInfo->nUID;
			canvas.view = videoInfo->hHwnd;
			CAgoraObject::GetEngine()->setupLocalVideo(canvas);
		}// 其他的不显示.
	}

}

BOOL  CAgoralWrapper::SendChatMessage(const char *msg)
{
	if (m_nStreamID < 0)
		return FALSE;
	return CAgoraObject::GetAgoraObject()->SendChatMessage(m_nStreamID, msg);
}





