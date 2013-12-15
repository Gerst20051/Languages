// OptionDlg.cpp : implementation file
//
#include "stdafx.h"
#include "resource.h"
#include "OptionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionDlg dialog


COptionDlg::COptionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COptionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COptionDlg)
	m_bAllPkt = FALSE;
	m_bDefPkt = FALSE;
	m_bDirPkt = FALSE;
	m_bLookAhead = FALSE;
	//}}AFX_DATA_INIT
}


void COptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptionDlg)
	DDX_Check(pDX, IDC_CHECK_ALL, m_bAllPkt);
	DDX_Check(pDX, IDC_CHECK_DEF, m_bDefPkt);
	DDX_Check(pDX, IDC_CHECK_DIR, m_bDirPkt);
	DDX_Check(pDX, IDC_CHECK_LA, m_bLookAhead);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptionDlg, CDialog)
	//{{AFX_MSG_MAP(COptionDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionDlg message handlers

void COptionDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
//	CDialog::OnCancel();
}

void COptionDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData ();

	CDialog::OnOK();

}

BOOL COptionDlg::OnInitDialog() 
{
	m_bLookAhead = FALSE;
	m_bDirPkt = TRUE;

	CDialog::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
