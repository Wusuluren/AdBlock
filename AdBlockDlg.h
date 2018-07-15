// AdBlockDlg.h : header file
//

#if !defined(AFX_ADBLOCKDLG_H__459D5D63_5D06_477A_8698_E4D2B6E6AAA4__INCLUDED_)
#define AFX_ADBLOCKDLG_H__459D5D63_5D06_477A_8698_E4D2B6E6AAA4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AdBlockFile.h"
#include "afxwin.h"

#define WM_SYSTEMTRAY	WM_USER+5

#define BUF_LINE_SIZE	1024

/////////////////////////////////////////////////////////////////////////////
// CAdBlockDlg dialog

class CAdBlockDlg : public CDialog
{
// Construction
public:
	CAdBlockDlg(CWnd* pParent = NULL);	// standard constructor
	void OnShowMainWindow();
	void OnCaptureWindow();
	void OnExit();
	bool StartMonitorThread();
	void KillWindow(HWND hWnd);
	void MyGetModuleFileName(TCHAR *moduleName, char *buf, int size);
	bool LoadRecords(bool updateListBox=true);
	bool SaveRecords(bool updateListBox=true);
	bool LoadConfig();
	bool SaveConfig();
	void Log(CString log);

// Dialog Data
	//{{AFX_DATA(CAdBlockDlg)
	enum { IDD = IDD_ADBLOCK_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAdBlockDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

public:
	NOTIFYICONDATA m_NotifyIcon;
	bool m_bStartCapture;
	CAdBlockFile m_fRecords;
	CAdBlockFile m_fLog;
	int m_bEnableDebug;
	int m_bEnableBoot;

protected:
	afx_msg LRESULT OnSystemtray(WPARAM wParam, LPARAM lParam);

	// Generated message map functions
	//{{AFX_MSG(CAdBlockDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CListBox m_listBoxAds;
	afx_msg void OnBnClickedRemoveAd();
	afx_msg void OnClose();
	afx_msg void OnBnClickedEnableDebug();
	afx_msg void OnBnClickedStartOnboot();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADBLOCKDLG_H__459D5D63_5D06_477A_8698_E4D2B6E6AAA4__INCLUDED_)
