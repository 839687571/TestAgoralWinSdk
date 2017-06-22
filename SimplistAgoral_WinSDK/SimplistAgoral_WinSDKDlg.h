
// SimplistAgoral_WinSDKDlg.h : ͷ�ļ�
//

#pragma once

#include "agoralWrapper/AgoralWrapper.h"
#include "agoralWrapper/DeviceManager.h"
#include "AGSliderCtrl.h"


// CSimplistAgoral_WinSDKDlg �Ի���
class CSimplistAgoral_WinSDKDlg : public CDialogEx, IDeviceMsgObserver
{
// ����
public:
	~CSimplistAgoral_WinSDKDlg();
	CSimplistAgoral_WinSDKDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SIMPLISTAGORAL_WINSDK_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()


	void onAudioVolumIndication(const void *param);
	void  onLastmileQuality(const void *wParam);
	CComboBox  *m_cmbInputAudDev;
	CComboBox  *m_cmbOutputAudDev;
	CComboBox  *m_cmbVideoDev;

	CAGSliderCtrl  m_sliderInVolume;
	CAGSliderCtrl  m_sliderOutVolume;

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
};
