// AdBlock.h : main header file for the ADBLOCK application
//

#if !defined(AFX_ADBLOCK_H__7B7DEB60_5B82_4CBC_B998_4024B88C78B2__INCLUDED_)
#define AFX_ADBLOCK_H__7B7DEB60_5B82_4CBC_B998_4024B88C78B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CAdBlockApp:
// See AdBlock.cpp for the implementation of this class
//

class CAdBlockApp : public CWinApp
{
public:
	CAdBlockApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdBlockApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CAdBlockApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADBLOCK_H__7B7DEB60_5B82_4CBC_B998_4024B88C78B2__INCLUDED_)
