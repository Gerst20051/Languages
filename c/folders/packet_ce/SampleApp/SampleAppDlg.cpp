// SampleAppDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SampleApp.h"
#include "SampleAppDlg.h"
#include "OptionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static volatile BOOL g_bRun = TRUE;
static UINT g_nPktCount = 0;

/////////////////////////////////////////////////////////////////////////////
// CSampleDlg dialog

CSampleDlg::CSampleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSampleDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSampleDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// init the vairables
	m_lpAdapter = NULL;

	// init the packet structure
	memset (&m_packet, 0, sizeof (PACKET));
	m_pBuffer = NULL;
}

void CSampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSampleDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSampleDlg, CDialog)
	//{{AFX_MSG_MAP(CSampleDlg)
	ON_BN_CLICKED(IDC_BTN_QUIT, OnBtnQuit)
	ON_BN_CLICKED(IDC_BTN_START, OnBtnStart)
	ON_BN_CLICKED(IDC_BTN_STOP, OnBtnStop)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSampleDlg message handlers

BOOL CSampleDlg::OnInitDialog()
{
	CListCtrl	*pListCtrl;

	// Get the list control
	pListCtrl = (CListCtrl*) GetDlgItem (IDC_LIST_CTRL);
	ASSERT(pListCtrl != NULL);

	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	CenterWindow(GetDesktopWindow());	// center to the hpc screen

	// Set the list control styles
	pListCtrl->ModifyStyle (0, LVS_SHOWSELALWAYS);
	pListCtrl->SetExtendedStyle (pListCtrl->GetExtendedStyle () | LVS_EX_FULLROWSELECT);
	

	// Show the list of the adapters available
	if (! ShowAdapterList ()) {
		return FALSE;
	}

	
	return TRUE;  // return TRUE  unless you set the focus to a control
}


// This function queries the driver for the adapters
// available in the device and displays it inthe list
BOOL CSampleDlg::ShowAdapterList()
{
	WCHAR		pNames[1024];	// place holder for adapter names
	DWORD		dwLen = 1024;
	CListCtrl	*pListCtrl;
	DWORD		dwCount;
	DWORD		nItem;
	CString		strNo;


	// reset the adapter names variable
	memset (pNames, 0, sizeof (pNames));

	// get the list of adapter names from the driver
	if (PacketGetAdapterNames (pNames, &dwLen) == FALSE) {
		AfxMessageBox (L"Unable to get the list of adapters."
			L"Atleast one adapter should be available.");
		return FALSE;
	}

	// prepare the list view control to show the names of the adapters
	pListCtrl = (CListCtrl*) GetDlgItem (IDC_LIST_CTRL);
	ASSERT(pListCtrl != NULL);

	pListCtrl->InsertColumn (0, TEXT("No."), LVCFMT_LEFT, 50);
	pListCtrl->InsertColumn (1, TEXT("Adapters Available"), LVCFMT_LEFT, 150);

	// the adapter names are unicode names seperated by unicode null character
	// the last adapter name will be followed by two unicode nulls
	dwCount = 0;
	nItem = 0;
	while (true) {

		strNo.Format (L"%d", nItem + 1);
		pListCtrl->InsertItem (nItem, strNo);
		pListCtrl->SetItemText (nItem++, 1, pNames + dwCount);
		
		dwCount += wcslen (pNames + dwCount);

		if (pNames[dwCount] == L'\0' && 
			pNames[dwCount + 1] == L'\0') {
			break;
		}
		dwCount ++;
	}

	// enable the open/close dialog window
	GetDlgItem (IDC_BTN_START)->EnableWindow (TRUE);

	return TRUE;
}

void CSampleDlg::OnOK() 
{
	// Do nothing on OK
	// this is just to disable the OK button command
}

void CSampleDlg::OnCancel()
{
	// Do nothing on Cancel
	// this is just to disable the Cancle button command
}

void CSampleDlg::OnBtnQuit() 
{
	// Check if the application can quit
	
	// quit the application
	CDialog::OnOK ();
}

DWORD WINAPI CaptureThread (LPVOID lpParam)
{
	CSampleDlg	*pDlg;
	int			nRet = 0;

	pDlg = (CSampleDlg*) lpParam;

	while (g_bRun) {

		if (PacketReceivePacket (pDlg->m_lpAdapter, &pDlg->m_packet, TRUE) == FALSE)	// last parameter is ignored
		{
			AfxMessageBox (IDP_PACKET_READ_ERROR);
			nRet = -1;
			break;
		}

		// print packets
		{
			ULONG	ulBytesReceived;
			char	*buffer;
			UINT	offset;
			UINT	tlen1;
			int		nItem;
			struct	bpf_hdr *hdr;
			CListCtrl *pListCtrl;
			CString	str;

			pListCtrl = (CListCtrl*) pDlg->GetDlgItem (IDC_LIST_CTRL);
			ASSERT (pListCtrl != NULL);

			ulBytesReceived = pDlg->m_packet.ulBytesReceived;
			buffer = (char*)pDlg->m_packet.Buffer;

			offset = 0;
			while (offset < ulBytesReceived) 
			{	
				hdr = (struct bpf_hdr *) (buffer + offset);
				tlen1 = hdr->bh_datalen;

				str.Format (L"%d", g_nPktCount);
				nItem = pListCtrl->InsertItem (g_nPktCount++, str);

				str.Format (L"%ld", hdr->bh_tstamp.tv_sec);
				pListCtrl->SetItemText (nItem, 1, str);

				str.Format (L"%ld", tlen1);
				pListCtrl->SetItemText (nItem, 2, str);

				offset += hdr->bh_hdrlen;
				
				offset = PACKET_WORDALIGN(offset + tlen1);
			}			
		}
		
	}

	// this second loop is required to prevent termination
	// of thread in case of the error. this is done because
	// the resources are freed in the stop message handler
	while (g_bRun) {
		Sleep (100);
	}

	return nRet;
}

void CSampleDlg::OnBtnStart() 
{
	CListCtrl	*pListCtrl;
	POSITION	pos;
	int			nItem;
	WCHAR		strName[512];
	DWORD		dwFilter;
	COptionDlg	dlg;

	// Open the selected adapter

	// Get the list control
	pListCtrl = (CListCtrl*) GetDlgItem (IDC_LIST_CTRL);
	ASSERT(pListCtrl != NULL);

	// Get the selected item position
	pos = pListCtrl->GetFirstSelectedItemPosition();
	if (pos == NULL) {
		AfxMessageBox (IDP_NO_ADAPTER_SELECTED);
		return;
	}

	// Get the item number
	nItem = pListCtrl->GetNextSelectedItem(pos);

	// Get the adapter name
	pListCtrl->GetItemText (nItem, 1, strName, 512);

	// Open the specified adapter
	if ((m_lpAdapter = PacketOpenAdapter(strName)) == NULL){
		AfxMessageBox (IDP_ADAP_OPEN_ERROR);
		return;
	}

	// Show the option dialog
	dlg.DoModal ();

	// do the processing of the options
	if (dlg.m_bLookAhead) {
		PacketSetMaxLookahead (m_lpAdapter);
	}

	if (dlg.m_bAllPkt) {
		// promiscuous mode
		dwFilter = NDIS_PACKET_TYPE_PROMISCUOUS;
	} else if (dlg.m_bDirPkt) {
		// directed mode
		dwFilter = NDIS_PACKET_TYPE_DIRECTED | NDIS_PACKET_TYPE_MULTICAST | NDIS_PACKET_TYPE_ALL_MULTICAST |
			NDIS_PACKET_TYPE_BROADCAST;
	} else {
		// default mode do nothing
	}

	if (dlg.m_bAllPkt || dlg.m_bDirPkt) {
		if (! PacketSetHwFilter (m_lpAdapter, dwFilter)) {
			AfxMessageBox (L"Cannot set mode");
			
			PacketCloseAdapter (m_lpAdapter);
			m_lpAdapter = NULL;
			return;
		}
	}

	m_pBuffer = new BYTE[256 * 1024];
	if (m_pBuffer == NULL) {
		AfxMessageBox (IDP_MEM_FAILURE);

		PacketCloseAdapter (m_lpAdapter);
		m_lpAdapter = NULL;
		return;
	}
	
	// init the packet structure
	m_packet.Buffer				= m_pBuffer;
	m_packet.Length				= 256 * 1024;
	m_packet.ulBytesReceived	= 0;
	m_packet.bIoComplete		= FALSE;


	// set the read time out to 3 sec for testing only
	PacketSetReadTimeout (m_lpAdapter, 1000);

	// set a 512k buffer in the driver
	PacketSetBuff (m_lpAdapter, 512 * 1024);

	// set number or write counter to one
	PacketSetNumWrites (m_lpAdapter, 1);

	// reset the list control
	pListCtrl->DeleteAllItems ();
	for (int nCtr = 0; nCtr < 2; nCtr++) {
		pListCtrl->DeleteColumn (0);
	}

	pListCtrl->InsertColumn (0, L"No.", LVCFMT_LEFT, 40);
	pListCtrl->InsertColumn (1, L"Time Stamp", LVCFMT_LEFT, 80);
	pListCtrl->InsertColumn (2, L"Pkt Size", LVCFMT_LEFT, 80);

	
	g_bRun = TRUE;
	g_nPktCount = 0;

	// allocate and set the thread parameters
	m_hThread = CreateThread (NULL, 0, CaptureThread, this, 0, NULL);
	if (m_hThread == NULL) {
		PacketCloseAdapter(m_lpAdapter);
		m_lpAdapter = NULL;
		delete []m_pBuffer;
		m_pBuffer = NULL;

		AfxMessageBox (IDP_CAPTURE_THREAD_ERROR);
		return;
	}


	// update the button status
	GetDlgItem (IDC_BTN_START)->EnableWindow (FALSE);
	GetDlgItem (IDC_BTN_STOP)->EnableWindow (TRUE);
	GetDlgItem (IDC_BTN_QUIT)->EnableWindow (FALSE);
}

void CSampleDlg::OnBtnStop() 
{
	// signal to stop
	g_bRun = FALSE;

	// Disable the stop button
	GetDlgItem (IDC_BTN_STOP)->EnableWindow (FALSE);
	
	// wait for 5 seconds till it is stopped
	if (WaitForSingleObject (m_hThread, 5000) == WAIT_TIMEOUT) {
		TerminateThread (m_hThread, -1);
	}

	// Close the adapter
	PacketCloseAdapter(m_lpAdapter);
	
	// reset the pointer
	m_lpAdapter = NULL;

	// free the buffer
	if (m_pBuffer) {
		delete []m_pBuffer;
		m_pBuffer = NULL;
	}
	
	// Update the button status
	GetDlgItem (IDC_BTN_QUIT)->EnableWindow (TRUE);

	return;	
}   
