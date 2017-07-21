
// Teacher2ScreenDlg.h : header file
//

#pragma once

#include "agoralWrapper/AgoralWrapper.h"
#include "agoralWrapper/DeviceManager.h"
// CTeacher2ScreenDlg dialog
class CTeacher2ScreenDlg : public CDialogEx
{
// Construction
public:
	CTeacher2ScreenDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TEACHER2SCREEN_DIALOG };
	void SetTartIndex(int index)
	{
		m_nStartIndex = index;
	}

	void SetTeache2Profile(unsigned int profile)
	{
		m_nStartProfile = profile;
	}
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	void AgoralMsgHandle(DWORD msgId, WPARAM wParam);
	void onLastmileQuality(const void *wParam);
	void onAudioVolumIndication(const void *param);

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions

	
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);


	CAgoralWrapper *m_pAgroObject;
	CDeviceManager  *m_deviceManager;
public:
	afx_msg void OnBnClickedJoin();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedSendtext();
	virtual BOOL PreTranslateMessage(MSG* pMsg);


	void setDevAtIndex(int index);

	int  m_nStartIndex;
	unsigned int m_nStartProfile;

	BOOL  m_bShowDlg;
};
