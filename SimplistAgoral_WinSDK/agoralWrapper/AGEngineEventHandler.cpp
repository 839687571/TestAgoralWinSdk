#include "stdafx.h"
#include "AGEngineEventHandler.h"
#include "AGEventDef.h"
#include <string>



// #define LOG(level, format, ...) / 
// do {
// 	/
// 		fprintf(stderr, "[%s|%s@%s,%d] " format "/n", /
// 		level, __func__, __FILE__, __LINE__, ##__VA_ARGS__); /
// } while (0)

extern  void LogMessage(char *msg);
extern  void LogMessageHLevel(char *msg);
CAGEngineEventHandler::CAGEngineEventHandler(void)
{
}

CAGEngineEventHandler::~CAGEngineEventHandler(void)
{
}

void CAGEngineEventHandler::SetMsgReceiver(HWND hWnd)
{
	m_hMainWnd = hWnd;
}

void CAGEngineEventHandler::onJoinChannelSuccess(const char* channel, uid_t uid, int elapsed)
{
	LPAGE_JOINCHANNEL_SUCCESS lpData = new AGE_JOINCHANNEL_SUCCESS;

	int nChannelLen = strlen(channel) + 1;
	lpData->channel = new char[nChannelLen];
	lpData->uid = uid;
	lpData->elapsed = elapsed;

	strcpy_s(lpData->channel, nChannelLen, channel);

	if(m_hMainWnd != NULL)
		::PostMessage(m_hMainWnd, WM_MSGID(EID_JOINCHANNEL_SUCCESS), (WPARAM)lpData, 0);

	LogMessageHLevel(__FUNCTION__);
}

void CAGEngineEventHandler::onReJoinChannelSuccess(const char* channel, uid_t uid, int elapsed)
{
	LPAGE_REJOINCHANNEL_SUCCESS lpData = new AGE_REJOINCHANNEL_SUCCESS;

	int nChannelLen = strlen(channel) + 1;
	lpData->channel = new char[nChannelLen];
	lpData->uid = uid;
	lpData->elapsed = elapsed;

	strcpy_s(lpData->channel, nChannelLen, channel);

	if(m_hMainWnd != NULL)
		::PostMessage(m_hMainWnd, WM_MSGID(EID_REJOINCHANNEL_SUCCESS), (WPARAM)lpData, 0);

	LogMessageHLevel(__FUNCTION__);

}

void CAGEngineEventHandler::onWarning(int warn, const char* msg)
{
	std::string str;

	str = ("onWarning");

	LogMessage(__FUNCTION__);
	LogMessage((char *)msg);
}

void CAGEngineEventHandler::onError(int err, const char* msg)
{
	LPAGE_ERROR lpData = new AGE_ERROR;
	lpData->msg  = NULL;
	int nMsgLen = 0;

	// attention: the pointer of msg maybe NULL!!!
	if(msg != NULL) {
		nMsgLen = strlen(msg) + 1;
		lpData->msg = new char[nMsgLen];
		strcpy_s(lpData->msg, nMsgLen, msg);
	}
	else
		lpData->msg = NULL;
	
	lpData->err = err;

	

	if(m_hMainWnd != NULL)
		::PostMessage(m_hMainWnd, WM_MSGID(EID_ERROR), (WPARAM)lpData, 0);

	LogMessageHLevel(__FUNCTION__);

}

void CAGEngineEventHandler::onAudioQuality(uid_t uid, int quality, unsigned short delay, unsigned short lost)
{
	LPAGE_AUDIO_QUALITY lpData = new AGE_AUDIO_QUALITY;

	lpData->uid = uid;
	lpData->quality = quality;
	lpData->delay = delay;
	lpData->lost = lost;

	if(m_hMainWnd != NULL)
		::PostMessage(m_hMainWnd, WM_MSGID(EID_AUDIO_QUALITY), (WPARAM)lpData, 0);

	LogMessage(__FUNCTION__);
}


void CAGEngineEventHandler::onAudioVolumeIndication(const AudioVolumeInfo* speakers, unsigned int speakerNumber, int totalVolume)
{
	LPAGE_AUDIO_VOLUME_INDICATION lpData = new AGE_AUDIO_VOLUME_INDICATION;

	lpData->speakers = new AudioVolumeInfo[speakerNumber];
	memcpy(lpData->speakers, speakers, speakerNumber*sizeof(AudioVolumeInfo));
	lpData->speakerNumber = speakerNumber;
	lpData->totalVolume = totalVolume;

	if (m_hMainWnd != NULL)
		::PostMessage(m_hMainWnd, WM_MSGID(EID_AUDIO_VOLUME_INDICATION), (WPARAM)lpData, 0);
	LogMessage(__FUNCTION__);
}

void CAGEngineEventHandler::onLeaveChannel(const RtcStats& stat)
{
	LPAGE_LEAVE_CHANNEL lpData = new AGE_LEAVE_CHANNEL;

	memcpy(&lpData->rtcStat, &stat, sizeof(RtcStats));

	if(m_hMainWnd != NULL)
		::PostMessage(m_hMainWnd, WM_MSGID(EID_LEAVE_CHANNEL), (WPARAM)lpData, 0);

	LogMessageHLevel(__FUNCTION__);
}

void CAGEngineEventHandler::onRtcStats(const RtcStats& stat)
{
	std::string str;

	str = ("stat");
	LogMessage(__FUNCTION__);
}


void CAGEngineEventHandler::onMediaEngineEvent(int evt)
{
	LPAGE_MEDIA_ENGINE_EVENT lpData = new AGE_MEDIA_ENGINE_EVENT;

	lpData->evt = evt;

	if (m_hMainWnd != NULL)
		::PostMessage(m_hMainWnd, WM_MSGID(EID_MEDIA_ENGINE_EVENT), (WPARAM)lpData, 0);

	LogMessage(__FUNCTION__);

}

void CAGEngineEventHandler::onAudioDeviceStateChanged(const char* deviceId, int deviceType, int deviceState)
{
	LPAGE_AUDIO_DEVICE_STATE_CHANGED lpData = new AGE_AUDIO_DEVICE_STATE_CHANGED;

	int nDeviceIDLen = strlen(deviceId) + 1;

	lpData->deviceId = new char[nDeviceIDLen];

	strcpy_s(lpData->deviceId, nDeviceIDLen, deviceId);
	lpData->deviceType = deviceType;
	lpData->deviceState = deviceState;

	if (m_hMainWnd != NULL)
		::PostMessage(m_hMainWnd, WM_MSGID(EID_AUDIO_DEVICE_STATE_CHANGED), (WPARAM)lpData, 0);
	LogMessageHLevel(__FUNCTION__);

}

void CAGEngineEventHandler::onVideoDeviceStateChanged(const char* deviceId, int deviceType, int deviceState)
{
	LPAGE_VIDEO_DEVICE_STATE_CHANGED lpData = new AGE_VIDEO_DEVICE_STATE_CHANGED;

	int nDeviceIDLen = strlen(deviceId) + 1;

	lpData->deviceId = new char[nDeviceIDLen];

	strcpy_s(lpData->deviceId, nDeviceIDLen, deviceId);
	lpData->deviceType = deviceType;
	lpData->deviceState = deviceState;

	if (m_hMainWnd != NULL)
		::PostMessage(m_hMainWnd, WM_MSGID(EID_VIDEO_DEVICE_STATE_CHANGED), (WPARAM)lpData, 0);

	LogMessageHLevel(__FUNCTION__);

}



void CAGEngineEventHandler::onLastmileQuality(int quality)
{
	LPAGE_LASTMILE_QUALITY lpData = new AGE_LASTMILE_QUALITY;

	lpData->quality = quality;

	if (m_hMainWnd != NULL) {
		::PostMessage(m_hMainWnd, WM_MSGID(EID_LASTMILE_QUALITY), (WPARAM)lpData, 0);
	}

	LogMessage(__FUNCTION__);

}
void CAGEngineEventHandler::onNetworkQuality(uid_t uid, int txQuality, int rxQuality)
{ 

	LPAGE_NETWORK_QUALITY lpData = new AGE_NETWORK_QUALITY;

	lpData->uid = uid;
	lpData->txQuality = txQuality;
	lpData->rxQuality = rxQuality;

	if (m_hMainWnd != NULL) {
		::PostMessage(m_hMainWnd, WM_MSGID(EID_NETWORK_QULITY), (WPARAM)lpData, 0);
		///::PostMessage(m_hMainWnd, WM_USER + 6666, (WPARAM)lpData, 0);
	}
	LogMessage(__FUNCTION__);
}

void CAGEngineEventHandler::onFirstLocalVideoFrame(int width, int height, int elapsed)
{
	LPAGE_FIRST_LOCAL_VIDEO_FRAME lpData = new AGE_FIRST_LOCAL_VIDEO_FRAME;

	lpData->width = width;
	lpData->height = height;
	lpData->elapsed = elapsed;

	if(m_hMainWnd != NULL)
		::PostMessage(m_hMainWnd, WM_MSGID(EID_FIRST_LOCAL_VIDEO_FRAME), (WPARAM)lpData, 0);
	LogMessageHLevel(__FUNCTION__);
}

void CAGEngineEventHandler::onFirstRemoteVideoDecoded(uid_t uid, int width, int height, int elapsed)
{
	LPAGE_FIRST_REMOTE_VIDEO_DECODED lpData = new AGE_FIRST_REMOTE_VIDEO_DECODED;

	lpData->uid = uid;
	lpData->width = width;
	lpData->height = height;
	lpData->elapsed = elapsed;

	if(m_hMainWnd != NULL)
		::PostMessage(m_hMainWnd, WM_MSGID(EID_FIRST_REMOTE_VIDEO_DECODED), (WPARAM)lpData, 0);
	LogMessageHLevel(__FUNCTION__);
}

void CAGEngineEventHandler::onFirstRemoteVideoFrame(uid_t uid, int width, int height, int elapsed)
{
	LPAGE_FIRST_REMOTE_VIDEO_FRAME lpData = new AGE_FIRST_REMOTE_VIDEO_FRAME;

	lpData->uid = uid;
	lpData->width = width;
	lpData->height = height;
	lpData->elapsed = elapsed;

	if(m_hMainWnd != NULL)
		::PostMessage(m_hMainWnd, WM_MSGID(EID_FIRST_REMOTE_VIDEO_FRAME), (WPARAM)lpData, 0);
	LogMessageHLevel(__FUNCTION__);
}

void CAGEngineEventHandler::onUserJoined(uid_t uid, int elapsed)
{
	LPAGE_USER_JOINED lpData = new AGE_USER_JOINED;

	lpData->uid = uid;
	lpData->elapsed = elapsed;

	if(m_hMainWnd != NULL)
		::PostMessage(m_hMainWnd, WM_MSGID(EID_USER_JOINED), (WPARAM)lpData, 0);
	LogMessageHLevel(__FUNCTION__);
}

void CAGEngineEventHandler::onUserOffline(uid_t uid, USER_OFFLINE_REASON_TYPE reason)
{
	LPAGE_USER_OFFLINE lpData = new AGE_USER_OFFLINE;

	lpData->uid = uid;
	lpData->reason = reason;

	if(m_hMainWnd != NULL)
		::PostMessage(m_hMainWnd, WM_MSGID(EID_USER_OFFLINE), (WPARAM)lpData, 0);

	LogMessageHLevel(__FUNCTION__);
}

void CAGEngineEventHandler::onUserMuteAudio(uid_t uid, bool muted)
{
	LPAGE_USER_MUTE_AUDIO lpData = new AGE_USER_MUTE_AUDIO;

	lpData->uid = uid;
	lpData->muted = muted;

	if(m_hMainWnd != NULL)
		::PostMessage(m_hMainWnd, WM_MSGID(EID_USER_MUTE_AUDIO), (WPARAM)lpData, 0);

	LogMessageHLevel(__FUNCTION__);

}

void CAGEngineEventHandler::onUserMuteVideo(uid_t uid, bool muted)
{
	if (m_hMainWnd != NULL) {
		LPAGE_USER_MUTE_VIDEO lpData = new AGE_USER_MUTE_VIDEO;

		lpData->uid = uid;
		lpData->muted = muted;
		::PostMessage(m_hMainWnd, WM_MSGID(EID_USER_MUTE_VIDEO), (WPARAM)lpData, 0);
	}

	LogMessageHLevel(__FUNCTION__);
}

void CAGEngineEventHandler::onStreamMessage(uid_t uid, int streamId, const char* data, size_t length)
{
	if (m_hMainWnd != NULL) {
		LPAGE_STREAM_MESSAGE lpData = new AGE_STREAM_MESSAGE;
		lpData->uid = uid;
		lpData->streamId = streamId;
		lpData->data = new char[length];
		lpData->length = length;
		memcpy_s(lpData->data, length, data, length);

		::PostMessage(m_hMainWnd, WM_MSGID(EID_STREAM_MESSAGE), (WPARAM)lpData, 0);
	}
	LogMessage(__FUNCTION__);

}

void CAGEngineEventHandler::onApiCallExecuted(const char* api, int error)
{
	if (m_hMainWnd != NULL) {
		LPAGE_APICALL_EXECUTED lpData = new AGE_APICALL_EXECUTED;
		strcpy_s(lpData->api, 128, api);
		lpData->error = error;
		::PostMessage(m_hMainWnd, WM_MSGID(EID_APICALL_EXECUTED), (WPARAM)lpData, 0);

	}
	LogMessage(__FUNCTION__);
	LogMessage((char*)api);
}

void CAGEngineEventHandler::onLocalVideoStats(const LocalVideoStats& stats)
{

	if (m_hMainWnd != NULL) {
		LPAGE_LOCAL_VIDEO_STAT lpData = new AGE_LOCAL_VIDEO_STAT;

		lpData->sentBitrate = stats.sentBitrate;
		lpData->sentFrameRate = stats.sentFrameRate;
		::PostMessage(m_hMainWnd, WM_MSGID(EID_LOCAL_VIDEO_STAT), (WPARAM)lpData, 0);

	}

	LogMessage(__FUNCTION__);

}

void CAGEngineEventHandler::onRemoteVideoStats(const RemoteVideoStats& stats)
{
	if (m_hMainWnd != NULL) {
		LPAGE_REMOTE_VIDEO_STAT lpData = new AGE_REMOTE_VIDEO_STAT;

		lpData->uid = stats.uid;
		lpData->delay = stats.delay;
		lpData->width = stats.width;
		lpData->height = stats.height;
		lpData->receivedFrameRate = stats.receivedFrameRate;
		lpData->receivedBitrate = stats.receivedBitrate;
		lpData->receivedFrameRate = stats.receivedFrameRate;
		::PostMessage(m_hMainWnd, WM_MSGID(EID_REMOTE_VIDEO_STAT), (WPARAM)lpData, 0);

	}

	LogMessage(__FUNCTION__);
}

void CAGEngineEventHandler::onCameraReady()
{
	if(m_hMainWnd != NULL)
		::PostMessage(m_hMainWnd, WM_MSGID(EID_CAMERA_READY), 0, 0);


	LogMessage(__FUNCTION__);

}

void CAGEngineEventHandler::onVideoStopped()
{
	if (m_hMainWnd != NULL)
		::PostMessage(m_hMainWnd, WM_MSGID(EID_VIDEO_STOPPED), 0, 0);

	LogMessageHLevel(__FUNCTION__);
}

void CAGEngineEventHandler::onConnectionLost()
{
	if(m_hMainWnd != NULL)
		::PostMessage(m_hMainWnd, WM_MSGID(EID_CONNECTION_LOST), 0, 0);
	LogMessageHLevel(__FUNCTION__);
}

void CAGEngineEventHandler::onConnectionInterrupted()
{
	LogMessageHLevel(__FUNCTION__);
}

void CAGEngineEventHandler::onUserEnableVideo(uid_t uid, bool enabled)
{

	if (m_hMainWnd != NULL) {
		LPAGE_USER_MUTE_VIDEO lpData = new AGE_USER_MUTE_VIDEO;
		lpData->uid = uid;
		lpData->muted = !enabled;

		::PostMessage(m_hMainWnd, WM_MSGID(EID_VIDEO_ENABLE), (WPARAM)lpData, 0);
	}
	LogMessageHLevel(__FUNCTION__);
}

void CAGEngineEventHandler::onStartRecordingService(int error)
{
	if (m_hMainWnd != NULL)
		::PostMessage(m_hMainWnd, WM_MSGID(EID_START_RCDSRV), 0, 0);
	LogMessage(__FUNCTION__);
}

void CAGEngineEventHandler::onStopRecordingService(int error)
{
	if (m_hMainWnd != NULL)
		::PostMessage(m_hMainWnd, WM_MSGID(EID_STOP_RCDSRV), 0, 0);
	LogMessage(__FUNCTION__);
}

void CAGEngineEventHandler::onRefreshRecordingServiceStatus(int status)
{

	LPAGE_RCDSRV_STATUS lpData = new AGE_RCDSRV_STATUS;

	lpData->status = status;

	if (m_hMainWnd != NULL)
		::PostMessage(m_hMainWnd, WM_MSGID(EID_REFREASH_RCDSRV), (WPARAM)lpData, 0);
	LogMessage(__FUNCTION__);
}