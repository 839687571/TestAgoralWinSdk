#include "stdafx.h"
#include "AgoralWrapper.h"
#include "UserManager.h"

#pragma comment( lib, "agorartc.lib")


extern void LogMessage(char *msg);
extern void LogMessageHLevel(char *msg);

#define  FREE_PTR(ptr){if (ptr != NULL){delete ptr;ptr = NULL;}}

IRtcEngine	 *CAgoralWrapper::m_pRtcEngine = NULL;
CAgoraObject *CAgoralWrapper::m_pAgoraObject = NULL;
std::string  CAgoralWrapper::m_strAppId = "";

CAgoralWrapper::CAgoralWrapper() :m_pObserver(NULL), m_iHostId(0)
// , m_hLocalWnd(NULL)
// , m_hRemoteWnd(NULL)
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
//	m_pAgoraObject->SetMsgHandlerWnd(m_mMainHwnd);

}

BOOL CAgoralWrapper::JoinChannel(const char * channelName, INT32 uid, int solution)
{
	IRtcEngine		*lpRtcEngine = CAgoraObject::GetEngine();
	CAgoraObject	*lpAgoraObject = CAgoraObject::GetAgoraObject();

	BOOL ret = FALSE;
	//m_nVideoSolution = BASE_120; //480P	40	640x480	15	500
	//应在调用 enableVideo 后设置视频属性。
	//应在调用 joinChannel / startPreview 前设置视频属性。
	lpRtcEngine->setVideoProfile((VIDEO_PROFILE_TYPE)solution, FALSE);
	
	//lpRtcEngine->disableAudio();

	lpRtcEngine->startPreview();

	lpAgoraObject->SetEncryptionSecret("", 0);// USE DEFAULT encryption
	ret = lpAgoraObject->JoinChannel(channelName, uid);
	lpAgoraObject->SetMsgHandlerWnd(m_mMainHwnd);

	// 创建数据流(createDataStream)
	//m_nStreamID = CAgoraObject::GetAgoraObject()->CreateMessageStream();
	///lpAgoraObject->EnableNetworkTest(TRUE);
	return ret;
}
BOOL CAgoralWrapper::JoinChannel(const char* channelKey, const char *channelName, INT32 uid, int solution)
{
	IRtcEngine		*lpRtcEngine = CAgoraObject::GetEngine();
	CAgoraObject	*lpAgoraObject = CAgoraObject::GetAgoraObject();

	BOOL ret = FALSE;
	lpRtcEngine->setVideoProfile((VIDEO_PROFILE_TYPE)solution, FALSE);
	lpRtcEngine->startPreview();

	lpAgoraObject->SetEncryptionSecret("", 0);// USE DEFAULT encryption
	ret = lpAgoraObject->JoinChannel(channelKey,channelName, uid);
	lpAgoraObject->SetMsgHandlerWnd(m_mMainHwnd);
	return ret;
}
void CAgoralWrapper::LeaveChanel()
{
	CAgoraObject	*lpAgoraObject = CAgoraObject::GetAgoraObject();
	lpAgoraObject->LeaveCahnnel();

// 	if (m_bottoomVideoWnd != NULL) {
// 		::ShowWindow(m_bottoomVideoWnd, SW_HIDE);
// 	}
// 	if (m_topVideoWnd != NULL) {
// 		::ShowWindow(m_topVideoWnd, SW_HIDE);
// 	}

	CUserManager::GetInstance()->onLeaveChannel();
}
void CAgoralWrapper::onUserJoinedMsg(DWORD msgId, WPARAM wParam)
{
	LPAGE_USER_JOINED lpData = (LPAGE_USER_JOINED)wParam;

	BindVideoWnd( lpData->uid,false);

	char buf[512];
	sprintf_s(buf, "==============================role = %d Joined the channel ", lpData->uid);
	LogMessageHLevel(buf);


	delete lpData;
}

void CAgoralWrapper::onUserOfflineMsg(DWORD msgId, WPARAM wParam)
{
	PAGE_USER_OFFLINE lpData = (PAGE_USER_OFFLINE)wParam;
	char buf[512];
	sprintf_s(buf, "==============================%d user leave the channel   ", lpData->uid);
	LogMessage(buf);

	CUserManager::GetInstance()->onUserOffline(lpData->uid);

	if (m_pObserver != NULL) {
		m_pObserver->OnUserOffline(lpData->uid);
	}
	delete lpData;
}
void  CAgoralWrapper::onUserMuteAudio(DWORD msgId, WPARAM wParam)
{
	LPAGE_USER_MUTE_AUDIO lpData = (LPAGE_USER_MUTE_AUDIO)wParam;
	if (m_pObserver != NULL) {
		m_pObserver->OnUserMuteAudio(lpData->uid,lpData->muted);
	}
	FREE_PTR(lpData);

}
void CAgoralWrapper::onUserLeaveChannel(DWORD msgId, WPARAM wParam)
{
	PAGE_LEAVE_CHANNEL lpData = (PAGE_LEAVE_CHANNEL)wParam;

	char buf[512];
	sprintf_s(buf, "==============================%d leave the channel because postive ", lpData->rtcStat.users);
	LogMessageHLevel(buf);

	if (m_pObserver != NULL) {
		m_pObserver->OnUserOffline(lpData->rtcStat.users);
	}

	delete lpData;
}
void CAgoralWrapper::onErrorMsg(DWORD msgId, WPARAM wParam)
{
	LPAGE_ERROR lpData = (LPAGE_ERROR)wParam;
	if (lpData->msg != NULL) {
		//delete lpData->msg;
		char buf[512];
		sprintf_s(buf, "======Agoral onErrorMsg = %d  ", lpData->err);

		LogMessageHLevel(buf);
	}

	if (m_pObserver) {
		m_pObserver->OnError(lpData->err, lpData->msg);
	}

	delete lpData;

}

void  CAgoralWrapper::onAudioQuality(DWORD msgId, WPARAM wParam)
{
	LPAGE_AUDIO_QUALITY lpData = (LPAGE_AUDIO_QUALITY)wParam;

	if (m_pObserver) {
		m_pObserver->OnAudioQuality(lpData->uid, lpData->quality,lpData->delay,lpData->lost);
	}

	delete lpData;
}

void CAgoralWrapper::onAudioVolumeIndication(DWORD msgId, WPARAM wParam)
{
	LPAGE_AUDIO_VOLUME_INDICATION lpData = (LPAGE_AUDIO_VOLUME_INDICATION)wParam;
	if (m_pObserver) {
		m_pObserver->OnAuidoVolumeIndication(lpData->speakerNumber,lpData->totalVolume);
	}
	if (lpData->speakers != NULL) {
		delete []lpData->speakers;
	}
	delete lpData;
}


void CAgoralWrapper::onMediaEnginEvent(DWORD msgId, WPARAM wParam)
{
	LPAGE_MEDIA_ENGINE_EVENT lpData = (LPAGE_MEDIA_ENGINE_EVENT)wParam;
	FREE_PTR(lpData);
}

void CAgoralWrapper::onAudioDevStateChanged(DWORD msgId, WPARAM wParam)
{
	LPAGE_AUDIO_DEVICE_STATE_CHANGED lpData = (LPAGE_AUDIO_DEVICE_STATE_CHANGED)wParam;
	if (m_pObserver) {
		m_pObserver->OnAudioDevChange(lpData->deviceId,lpData->deviceType,lpData->deviceState);
	}
	FREE_PTR(lpData);
}

void CAgoralWrapper::onVideoDevStateChanged(DWORD msgId, WPARAM wParam)
{
	LPAGE_VIDEO_DEVICE_STATE_CHANGED lpData = (LPAGE_VIDEO_DEVICE_STATE_CHANGED)wParam;
	if (m_pObserver) {
		m_pObserver->OnVideoDevChange(lpData->deviceId, lpData->deviceType, lpData->deviceState);
	}
	FREE_PTR(lpData);
}

void CAgoralWrapper::onLastmileQuality(DWORD msgId, WPARAM wParam)
{
	LPAGE_LASTMILE_QUALITY lpData = (LPAGE_LASTMILE_QUALITY)wParam;
	if (m_pObserver) {
		m_pObserver->OnLastmileQuality(lpData->quality);
	}
	FREE_PTR(lpData);
}



void CAgoralWrapper::onNetWorkQuality(DWORD msgId, WPARAM wParam)
{
	PAGE_NETWORK_QUALITY pData = (PAGE_NETWORK_QUALITY)wParam;
	if (m_pObserver) {
		m_pObserver->OnNetWorkQuality(pData->uid,pData->txQuality,pData->rxQuality);
	}

	delete pData;
}

void CAgoralWrapper::onFirstLocalVideoFrame(DWORD msgId, WPARAM wParam)
{
	LPAGE_FIRST_LOCAL_VIDEO_FRAME lpData = new AGE_FIRST_LOCAL_VIDEO_FRAME;
	if (m_pObserver) {
		m_pObserver->OnLocalVideframeShow(lpData->elapsed);
	}

	CUserManager::GetInstance()->onFirstHostFrame(m_iHostId);
	char buf[512];
	sprintf_s(buf, "onFirstLocalVideoFrame  show\n");
	LogMessageHLevel(buf);

	delete lpData;
}

void CAgoralWrapper::onFirstRemoteVideoDecoded(DWORD msgId, WPARAM wParam)
{
	LPAGE_FIRST_REMOTE_VIDEO_DECODED lpData = new AGE_FIRST_REMOTE_VIDEO_DECODED;

	delete lpData;
}
void CAgoralWrapper::onFirstRemoteVideoShow(DWORD msgId, WPARAM wParam)
{
	LPAGE_FIRST_REMOTE_VIDEO_FRAME lpData = (LPAGE_FIRST_REMOTE_VIDEO_FRAME)wParam;
	//LPAGE_FIRST_REMOTE_VIDEO_FRAME lpData = new AGE_FIRST_REMOTE_VIDEO_FRAME;
	if (m_pObserver) {
		m_pObserver->OnFirstRemoteVidoeFrameShow(lpData->uid, lpData->elapsed);
	}
	CUserManager::GetInstance()->onFirstRemoteFrame(lpData->uid);
	delete lpData;
}
void CAgoralWrapper::onUserMuteAuido(DWORD msgId, WPARAM wParam)
{
	LPAGE_USER_MUTE_VIDEO lpData = (LPAGE_USER_MUTE_VIDEO)wParam;

	delete lpData;
}

void CAgoralWrapper::onUserMuteVideo(DWORD msgId, WPARAM wParam)
{
	throw std::logic_error("The method or operation is not implemented.");
}
void CAgoralWrapper::onApiCalled(DWORD msgId, WPARAM wParam)
{
	LPAGE_APICALL_EXECUTED lpData = (LPAGE_APICALL_EXECUTED)wParam;

	delete lpData;
}

void CAgoralWrapper::onVideoStoped(DWORD msgId, WPARAM wParam)
{
	char buf[512];
	sprintf_s(buf, "ERROR recv onVideoStoped  ");
	LogMessageHLevel(buf);

	if (m_pObserver) {
		m_pObserver->OnVideoStoped();
	}
}
void CAgoralWrapper::onVideoEnabled(DWORD msgId, WPARAM wParam)
{
	if (m_pObserver) {
		LPAGE_USER_MUTE_VIDEO lpData = (LPAGE_USER_MUTE_VIDEO)wParam;
		m_pObserver->OnVideoEnable(lpData->uid,lpData->muted);
		CUserManager::GetInstance()->onUserMuteVideo(lpData->uid,lpData->muted);

		char buf[512];
		sprintf_s(buf, "recv onVideoEnabled  uid = %d, enable  =  %d ", lpData->uid, lpData->muted);
		LogMessageHLevel(buf);

		delete lpData;
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

	RemoteVideoStats stats;
	stats.uid = lpData->uid;
	stats.delay = lpData->delay;
	stats.receivedBitrate = lpData->receivedBitrate ;
	stats.receivedFrameRate = lpData->receivedFrameRate;
	stats.width= lpData->width;
	stats.height = lpData->height;

	if (m_pObserver) {
		m_pObserver->OnRemoteVideoStats(stats);
	}

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


// 	AGVIDEO_WNDINFO  videoInfo;
// 	videoInfo.nUID = lpData->uid;
	BindVideoWnd(lpData->uid,true);
	m_iHostId = lpData->uid;

	LogMessageHLevel(buf);

	//1delete lpData->channel;
	delete lpData;
}
void CAgoralWrapper::onRejoinSuccess(DWORD msgId, WPARAM wParam)
{
	PAGE_REJOINCHANNEL_SUCCESS pData = (PAGE_REJOINCHANNEL_SUCCESS)wParam;
	BindVideoWnd(pData->uid, true);
	delete pData;
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
	LogMessageHLevel(buf);
}


void CAgoralWrapper::onGetUserSendMessage(DWORD msgId, WPARAM wParam)
{
	LPAGE_STREAM_MESSAGE lpData = new AGE_STREAM_MESSAGE;
	char buf[512];
	sprintf_s(buf, "=== receive user = %d send msg %s ", lpData->uid,lpData->data);
	LogMessage(buf);

	delete lpData;
}

BOOL CAgoralWrapper::MsgHandle(DWORD msgId, WPARAM wParam)
{
	switch (msgId) {
	case  WM_MSGID(EID_JOINCHANNEL_SUCCESS):
		onHostJoinSuccess(msgId, wParam);
		return TRUE;
		break;
	case WM_MSGID(EID_REJOINCHANNEL_SUCCESS):
		onRejoinSuccess(msgId, wParam);
		return TRUE;
		break;
	case WM_MSGID(EID_ERROR):
		onErrorMsg(msgId, wParam);
		return TRUE;
		break;
	case WM_MSGID(EID_AUDIO_QUALITY):
		onAudioQuality(msgId, wParam);
		return TRUE;
	case WM_MSGID(EID_LASTMILE_QUALITY):
		onLastmileQuality(msgId, wParam);
		return TRUE;
		break;
	case WM_MSGID(EID_NETWORK_QULITY): //频道内网络质量
		onNetWorkQuality(msgId, wParam);
		return TRUE;
		break;
	case WM_MSGID(EID_FIRST_LOCAL_VIDEO_FRAME):
		onFirstLocalVideoFrame(msgId, wParam);
		return TRUE;
		break;
	case WM_MSGID(EID_FIRST_REMOTE_VIDEO_DECODED):
		onFirstRemoteVideoDecoded(msgId, wParam);
		return TRUE;
		break;
	case  WM_MSGID(EID_FIRST_REMOTE_VIDEO_FRAME):
		onFirstRemoteVideoShow(msgId, wParam);
		return TRUE;
		break;

	case  WM_MSGID(EID_USER_JOINED):
		onUserJoinedMsg(msgId, wParam);
		return TRUE;
		break;
	case WM_MSGID(EID_USER_OFFLINE):
		onUserOfflineMsg(msgId, wParam);
		return TRUE;
		break;
	case WM_MSGID(EID_USER_MUTE_AUDIO):
		onUserMuteAuido(msgId,wParam);
		return TRUE;
		break;
	case WM_MSGID(EID_USER_MUTE_VIDEO):
		onUserMuteVideo(msgId, wParam);
		return TRUE;
		break;
	case WM_MSGID(EID_STREAM_MESSAGE):
		onGetUserSendMessage(msgId, wParam);
		return TRUE;
		break;
	case WM_MSGID(EID_APICALL_EXECUTED):
		onApiCalled(msgId, wParam);
		return TRUE;
		break;
	case WM_MSGID(EID_LOCAL_VIDEO_STAT):
		onStatisticLocalVideoInfo(msgId, wParam);
		return TRUE;
		break;
	case WM_MSGID(EID_REMOTE_VIDEO_STAT):
		onStatisticRemoteVideoInfo(msgId, wParam);
		return TRUE;
		break;
	case WM_MSGID(EID_VIDEO_STOPPED):
		onVideoStoped(msgId, wParam);
		return TRUE;
		break;
	case WM_MSGID(EID_CONNECTION_LOST):
		onLostConnect(msgId, wParam);
		return TRUE;
		break;

	case WM_MSGID(EID_LEAVE_CHANNEL):
		onUserLeaveChannel(msgId, wParam);
		return TRUE;
		break;
	case WM_MSGID(EID_AUDIO_VOLUME_INDICATION):
		onAudioVolumeIndication(msgId, wParam);
		return TRUE;
		break;
	case  WM_MSGID(EID_MEDIA_ENGINE_EVENT):
		onMediaEnginEvent(msgId, wParam);
		return TRUE;
		break;
	case WM_MSGID(EID_AUDIO_DEVICE_STATE_CHANGED):
		onAudioDevStateChanged(msgId, wParam);
		return TRUE;
		break;
	case WM_MSGID(EID_VIDEO_DEVICE_STATE_CHANGED):
		onVideoDevStateChanged(msgId, wParam);
		return TRUE;
		break;
	case WM_MSGID(EID_VIDEO_ENABLE):
		onVideoEnabled(msgId, wParam);
		return TRUE;

	default:
		break;
	}
	return FALSE;
}

void CAgoralWrapper::BindVideoWnd(unsigned int uid, bool host)
{
	char buf[512];
	int i = uid % 10;// 最后是1 老师, 0 学生.
	sprintf_s(buf, "on user join  uid = %d,role is = %s ", uid, i==0? "student" : (i == 1 ? "teacher" : "unkown"));
	LogMessageHLevel(buf);
	if (uid % 10 != 0 && uid %10 != 1) {
		sprintf_s(buf, "on user join but uid = %d,not Student Or Teacher", uid);
		LogMessageHLevel(buf);
		return;
	}
	if (host) {
		CUserManager::GetInstance()->onHostJoin(uid);
	} else {
		CUserManager::GetInstance()->onUserJoin(uid);

	}
}
BOOL CAgoralWrapper::EnableWebSdkInteroperability(bool bEnable)
{
	CAgoraObject *_pAgoraObject = CAgoraObject::GetAgoraObject();
	return _pAgoraObject->EnableWebSdkInteroperability(true);
}
BOOL  CAgoralWrapper::SendChatMessage(const char *msg)
{
	if (m_nStreamID < 0)
		return FALSE;
	return CAgoraObject::GetAgoraObject()->SendChatMessage(m_nStreamID, msg);
}
BOOL  CAgoralWrapper::IsHostUser(unsigned int uid)
{
	return m_iHostId == uid;
}









