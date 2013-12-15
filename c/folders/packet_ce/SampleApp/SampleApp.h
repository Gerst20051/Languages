// SampleApp.h : main header file for the SAMPLEAPP application
//

#if !defined(AFX_SAMPLEAPP_H__E258207C_1E43_4F23_90D6_857154623BEE__INCLUDED_)
#define AFX_SAMPLEAPP_H__E258207C_1E43_4F23_90D6_857154623BEE__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSampleApp:
// See SampleApp.cpp for the implementation of this class
//

class CSampleApp : public CWinApp
{
public:
	CSampleApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSampleApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSampleApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	BOOL m_bDrvLoaded;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft eMbedded Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SAMPLEAPP_H__E258207C_1E43_4F23_90D6_857154623BEE__INCLUDED_)
