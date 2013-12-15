// SampleAppDlg.h : header file
//

#if !defined(AFX_SAMPLEAPPDLG_H__6C20B4EB_A12A_4B24_858D_17611F5F9225__INCLUDED_)
#define AFX_SAMPLEAPPDLG_H__6C20B4EB_A12A_4B24_858D_17611F5F9225__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "Packet32.H"

/////////////////////////////////////////////////////////////////////////////
// CSampleDlg dialog
// forward declaration

//typedef struct _THREAD_PARAM {
//	LPADAPTER	lpAdapter;
//	LPPACKET	lpPacket;
//	CSampleDlg	*pDlg;
//} THREAD_PARAM, *PTHREAD_PARAM;

static DWORD WINAPI CaptureThread (LPVOID);

class CSampleDlg : public CDialog
{
	friend static DWORD WINAPI CaptureThread (LPVOID);
// Construction
public:
	BOOL ShowAdapterList();
	CSampleDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSampleDlg)
	enum { IDD = IDD_SAMPLEAPP_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSampleDlg)
	public:
	virtual void OnOK();
	virtual void OnCancel();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HANDLE m_hThread;
	PBYTE m_pBuffer;
	PACKET m_packet;
	LPADAPTER m_lpAdapter;
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSampleDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnQuit();
	afx_msg void OnBtnStart();
	afx_msg void OnBtnStop();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft eMbedded Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SAMPLEAPPDLG_H__6C20B4EB_A12A_4B24_858D_17611F5F9225__INCLUDED_)
