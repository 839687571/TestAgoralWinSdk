#pragma once

#include "agoral/IAgoraMediaEngine.h"
#include "agoral/IAgoraRtcEngine.h"

using namespace  agora::rtc;


///namespace  rtc

//{
	class CAGEngineEventHandler :
		public  IRtcEngineEventHandler
	{
	public:
		CAGEngineEventHandler(void);
		~CAGEngineEventHandler(void);

		void SetMsgReceiver(HWND hWnd = NULL);
		HWND GetMsgReceiver() { return m_hMainWnd; };

		virtual void onJoinChannelSuccess(const char* channel, uid_t uid, int elapsed);
		virtual void onReJoinChannelSuccess(const char* channel, uid_t uid, int elapsed);
		virtual void onWarning(int warn, const char* msg);
		virtual void onError(int err, const char* msg);
		virtual void onAudioQuality(uid_t uid, int quality, unsigned short delay, unsigned short lost);
		virtual void onAudioVolumeIndication(const  AudioVolumeInfo* speakers, unsigned int speakerNumber, int totalVolume);

		virtual void onLeaveChannel(const  RtcStats& stat);

		//SDK定期向应用程序报告当前通话的统计信息，每两秒触发一次。
		/*
		duration: 通话时长（秒），累计值
		txBytes: 发送字节数（bytes），累计值
		rxBytes: 接收字节数（bytes），累计值
		txKBitRate: 发送码率（kbps），瞬时值
		rxKBitRate: 接收码率（kbps），瞬时值
		lastmileQuality: 以瞬时值表示的客户端机器的当前网络连接质量。
		*/
		virtual void onRtcStats(const  RtcStats& stat);
		virtual void onMediaEngineEvent(int evt);

		virtual void onAudioDeviceStateChanged(const char* deviceId, int deviceType, int deviceState);
		virtual void onVideoDeviceStateChanged(const char* deviceId, int deviceType, int deviceState);

		virtual void onLastmileQuality(int quality);
		virtual void onNetworkQuality(uid_t uid, int txQuality, int rxQuality);
		virtual void onFirstLocalVideoFrame(int width, int height, int elapsed);
		virtual void onFirstRemoteVideoDecoded(uid_t uid, int width, int height, int elapsed);
		virtual void onFirstRemoteVideoFrame(uid_t uid, int width, int height, int elapsed);
		virtual void onUserJoined(uid_t uid, int elapsed);
		virtual void onUserOffline(uid_t uid, USER_OFFLINE_REASON_TYPE reason);
		virtual void onUserMuteAudio(uid_t uid, bool muted);
		virtual void onUserMuteVideo(uid_t uid, bool muted);
		virtual void onApiCallExecuted(const char* api, int error);

		virtual void onStreamMessage(uid_t uid, int streamId, const char* data, size_t length);

		virtual void onLocalVideoStats(const LocalVideoStats& stats);
		virtual void onRemoteVideoStats(const RemoteVideoStats& stats);
		virtual void onCameraReady();
		virtual void onVideoStopped();
		virtual void onConnectionLost();
		virtual void onConnectionInterrupted();

		virtual void onUserEnableVideo(uid_t uid, bool enabled);

		virtual void onStartRecordingService(int error);
		virtual void onStopRecordingService(int error);
		virtual void onRefreshRecordingServiceStatus(int status);
	private:
		HWND		m_hMainWnd;
	};

//}
