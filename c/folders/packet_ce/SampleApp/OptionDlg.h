#if !defined(AFX_OPTIONDLG_H__C6EE06EE_618C_4781_BFA4_6C2E404BF117__INCLUDED_)
#define AFX_OPTIONDLG_H__C6EE06EE_618C_4781_BFA4_6C2E404BF117__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// OptionDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptionDlg dialog

class COptionDlg : public CDialog
{
// Construction
public:
	COptionDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COptionDlg)
	enum { IDD = IDD_DLG_OPTIONS };
	BOOL	m_bAllPkt;
	BOOL	m_bDefPkt;
	BOOL	m_bDirPkt;
	BOOL	m_bLookAhead;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COptionDlg)
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONDLG_H__C6EE06EE_618C_4781_BFA4_6C2E404BF117__INCLUDED_)
