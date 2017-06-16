
// SimplistAgoral_WinSDKDlg.h : 头文件
//

#pragma once

#include "agoralWrapper/AgoralWrapper.h"
#include "agoralWrapper/DeviceManager.h"


// CSimplistAgoral_WinSDKDlg 对话框
class CSimplistAgoral_WinSDKDlg : public CDialogEx, IDeviceMsgObserver
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
	HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()


	void onAudioVolumIndication(const void *param);
	CComboBox  *m_cmbInputAudDev;
	CComboBox  *m_cmbOutputAudDev;
	CComboBox  *m_cmbVideoDev;

	CProgressCtrl  *m_silderVolIndicate;

public:
	afx_msg void OnBnClickedJoin();
	afx_msg void OnBnClickedLeave();

	CAgoralWrapper *m_pAgroObject;
	CDeviceManager  *m_deviceManager;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedCheck();
	afx_msg void OnBnClickedButtonPreview();
	afx_msg void OnBnClickedButtonTestAin();
	afx_msg void OnBnClickedButtonTestAout();
	afx_msg void OnCbnSelchangeComboAinput();
	afx_msg void OnCbnSelchangeComboxVinput();
	afx_msg void OnCbnSelchangeComboxAout();
};
