
// SimplistAgoral_WinSDKDlg.h : ͷ�ļ�
//

#pragma once

#include "agoralWrapper/AgoralWrapper.h"


// CSimplistAgoral_WinSDKDlg �Ի���
class CSimplistAgoral_WinSDKDlg : public CDialogEx
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
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedJoin();
	afx_msg void OnBnClickedLeave();

	CAgoralWrapper *m_pAgroObject;

	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
