// SampleApp.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "SampleApp.h"
#include "SampleAppDlg.h"
#include "Packet32.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSampleApp

BEGIN_MESSAGE_MAP(CSampleApp, CWinApp)
	//{{AFX_MSG_MAP(CSampleApp)
	//}}AFX_MSG
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSampleApp construction

CSampleApp::CSampleApp()
	: CWinApp()
{
	m_bDrvLoaded = FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSampleApp object

CSampleApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSampleApp initialization

BOOL CSampleApp::InitInstance()
{
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	// Initialize the device driver
	if (!PacketLoadDriver ()) {
		AfxMessageBox (IDP_DRIVER_LOAD_FAILED);
		return FALSE;
	}
	m_bDrvLoaded = TRUE;

	// Create the dialog box and display it
	CSampleDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();

	if (nResponse == IDOK) {
	} else if (nResponse == IDCANCEL) {
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

int CSampleApp::ExitInstance() 
{
	// if the driver is loaded then unload it
	if (m_bDrvLoaded) {
		m_bDrvLoaded = FALSE;
		PacketUnloadDriver();
	}
	
	return CWinApp::ExitInstance();
}
