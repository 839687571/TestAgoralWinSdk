
// SimplistAgoral_WinSDKDlg.h : 头文件
//

#pragma once

#include "agoralWrapper/AgoralWrapper.h"
#include "agoralWrapper/DeviceManager.h"
#include "AGSliderCtrl.h"
#include "agoralWrapper/AGConfig/AGConfig.h"


// CSimplistAgoral_WinSDKDlg 对话框
class CSimplistAgoral_WinSDKDlg : public CDialogEx, IDeviceMsgObserver,IAgoralObjectMsgMsgObserver
{
// 构造
public:
	~CSimplistAgoral_WinSDKDlg();
	CSimplistAgoral_WinSDKDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SIMPLISTAGORAL_WINSDK_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();

	void InitRescombox();

	void HideChildWnd(BOOL show);
	HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()


	/** IAgoralObjectMsgMsgObserver --------------------- start*/
	void  OnAuidoVolumeIndication(unsigned int uId, int volume);
	void  OnLastmileQuality(int quality);
	void  OnAudioDevChange(const char *devId, int devType, int devState);
	void  OnVideoDevChange(const char *devId, int devType, int devState);
	void  OnVideoStoped();
	/** IAgoralObjectMsgMsgObserver --------------------- end*/

	CComboBox  *m_cmbInputAudDev;
	CComboBox  *m_cmbOutputAudDev;

	CComboBox  *m_cmbVideoDev;
	CComboBox  *m_cmbVideoDev2;

	CComboBox  *m_cmbVideoRes;
	CComboBox  *m_cmbVideoPPTRes;

	std::string  m_strInputAudDevId;
	std::string  m_strOutputAudDevId;
	std::string  m_strVideoDevId;


	CAGSliderCtrl  m_sliderInVolume;
	CAGSliderCtrl  m_sliderOutVolume;

	CProgressCtrl  *m_silderVolIndicate;

	LPTSTR			    m_szProfileDes[64];
	int					m_nProfileValue[64];

public:
	afx_msg void OnBnClickedJoin();
	afx_msg void OnBnClickedLeave();

	CAgoralWrapper *m_pAgroObject;
	CDeviceManager  *m_deviceManager;
//	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedCheck();
	afx_msg void OnBnClickedButtonPreview();
	afx_msg void OnBnClickedButtonTestAin();
	afx_msg void OnBnClickedButtonTestAout();
	afx_msg void OnCbnSelchangeComboAinput();
	afx_msg void OnCbnSelchangeComboxVinput();
	afx_msg void OnCbnSelchangeComboxAout();
	afx_msg void OnNMCustomdrawSliderAin(NMHDR *pNMHDR, LRESULT *pResult);
// 	BEGIN_MSG_MAP(CSimplistAgoral_WinSDKDlg)
// 		MESSAGE_HANDLER(WM_HSCROLL, OnHScroll)
// 	END_MSG_MAP()
// 	LRESULT OnHScroll(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	afx_msg void OnHScroll(
		UINT nSBCode,
		UINT nPos,
		CScrollBar* pScrollBar
		);

	//LRESULT OnHScroll(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	afx_msg void OnBnClickedButtonNetwork();


	void UpdateAoutDev( int devState);
	void UpdateAinDev(int devState);

	void UpdateVideoDev(int devState);
	afx_msg void OnBnClickedButtonClose2();
	afx_msg void OnBnClickedButtonLeave2();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonPreview2();
	afx_msg void OnCbnSelchangeComboxVinput2();

private:
	void setDevWithCursel(int index);


	void PostTeacher2Msg(DWORD message,WPARAM wParam,LPARAM lParam);
public:
	afx_msg void OnBnClickedCancel();

	int     m_nCamera1Index;
	int     m_nCamera2Index;
	afx_msg void OnBnClickedButtonClose();
	afx_msg void OnStnClickedStaticLocal2();

	CRect rectTeacher2;
	CRect m_rcAroundSize;
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	virtual void OnUserOffline(unsigned int userId );

	afx_msg void OnCbnSelchangeComboResulotion();

	CAGConfig		*m_agConfig;
	afx_msg void OnCbnSelchangeComboResulotionPpt();
	afx_msg void OnBnClickedButtonOpen2();
	afx_msg void OnBnClickedCheckIsteacher();
};
