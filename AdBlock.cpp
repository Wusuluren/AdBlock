// AdBlock.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "AdBlock.h"
#include "AdBlockDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdBlockApp

BEGIN_MESSAGE_MAP(CAdBlockApp, CWinApp)
	//{{AFX_MSG_MAP(CAdBlockApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdBlockApp construction

CAdBlockApp::CAdBlockApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CAdBlockApp object

CAdBlockApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CAdBlockApp initialization

BOOL CAdBlockApp::InitInstance()
{
	// Standard initialization

	::CreateMutex(NULL, false, _T("__AdBlock__"));
	if (GetLastError() == ERROR_ALREADY_EXISTS)
		return false;

	CAdBlockDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.Create(CAdBlockDlg::IDD);
	dlg.ShowWindow(SW_HIDE);
	dlg.RunModalLoop(); 

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
