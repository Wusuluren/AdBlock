// AdBlockDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AdBlock.h"
#include "AdBlockDlg.h"
#include <vector>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define RECORDS_FILENAME _T("AdBlock_records.txt")
#define LOGS_FILENAME _T("AdBlock_logs.txt")
#define CONFIG_FILENAME _T("AdBlock_config.ini")
#define APP_NAME _T("AdBlock")

HWND g_hWndAd;
TCHAR g_title[BUF_LINE_SIZE];
vector<CString> g_records;
CString g_wndTitleToFind;

/////////////////////////////////////////////////////////////////////////////
// CAdBlockDlg dialog

CAdBlockDlg::CAdBlockDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAdBlockDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAdBlockDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_bStartCapture = false;
}

void CAdBlockDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdBlockDlg)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LIST_ADS, m_listBoxAds);
}

BEGIN_MESSAGE_MAP(CAdBlockDlg, CDialog)
	//{{AFX_MSG_MAP(CAdBlockDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_SYSTEMTRAY, CAdBlockDlg::OnSystemtray)
	ON_COMMAND(ID_MENU_MAIN_WINDOW, CAdBlockDlg::OnShowMainWindow)
	ON_COMMAND(ID_MENU_CAPTURE_WINDOW, CAdBlockDlg::OnCaptureWindow)
	ON_COMMAND(ID_MENU_EXIT, CAdBlockDlg::OnExit)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(ID_REMOVE_AD, &CAdBlockDlg::OnBnClickedRemoveAd)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_ENABLE_DEBUG, &CAdBlockDlg::OnBnClickedEnableDebug)
	ON_BN_CLICKED(IDC_START_ONBOOT, &CAdBlockDlg::OnBnClickedStartOnboot)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdBlockDlg message handlers

BOOL CAdBlockDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	m_NotifyIcon.cbSize = sizeof(NOTIFYICONDATA);
	m_NotifyIcon.hIcon = m_hIcon;
	m_NotifyIcon.hWnd = m_hWnd;
	_tcscpy_s(m_NotifyIcon.szTip, _T("AdBlock"));
	m_NotifyIcon.uCallbackMessage = WM_SYSTEMTRAY;
	m_NotifyIcon.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	Shell_NotifyIcon(NIM_ADD, &m_NotifyIcon);

	LoadConfig();
	CButton* checkbox;
	checkbox = (CButton*)GetDlgItem(IDC_ENABLE_DEBUG);
	checkbox->SetCheck(m_bEnableDebug);
	checkbox = (CButton*)GetDlgItem(IDC_ENABLE_BOOT);
	checkbox->SetCheck(m_bEnableBoot);

	LoadRecords();
	StartMonitorThread();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

bool CAdBlockDlg::LoadConfig()
{
	char moduleName[BUF_LINE_SIZE] = {0};
	MyGetModuleFileName(CONFIG_FILENAME, moduleName, BUF_LINE_SIZE);
	m_bEnableDebug = GetPrivateProfileInt(_T("config"), _T("EnableDebug"), 0, moduleName);
	m_bEnableBoot = GetPrivateProfileInt(_T("config"), _T("EnableBoot"), 0, moduleName);
	return true;
}

bool CAdBlockDlg::SaveConfig()
{
	char moduleName[BUF_LINE_SIZE] = {0};
	MyGetModuleFileName(CONFIG_FILENAME, moduleName, BUF_LINE_SIZE);
	CString tmp;
	tmp.Format("%d", m_bEnableDebug);
	WritePrivateProfileString(_T("config"), _T("EnableDebug"), tmp, moduleName);
	tmp.Format("%d", m_bEnableBoot);
	WritePrivateProfileString(_T("config"), _T("EnableBoot"), tmp, moduleName);
	return true;
}

CString GenerateLog(CString windTitle)
{
	SYSTEMTIME time;
	GetLocalTime(&time);
	CString log;
	log.Format("AdBlock[%d-%d-%d %d:%d:%d]:", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	return log + windTitle;
}

DWORD WINAPI MonitorThread(LPVOID lpParam)
{
	CAdBlockDlg *mainDlg = (CAdBlockDlg *)lpParam;
	HWND hWnd;
	CString log;

	char moduleName[BUF_LINE_SIZE] = {0};
	mainDlg->MyGetModuleFileName(LOGS_FILENAME, moduleName, BUF_LINE_SIZE);
	if(!mainDlg->m_fLog.Open(moduleName, CAdBlockFile::modeCreate|CAdBlockFile::modeReadWrite|CAdBlockFile::modeNoTruncate))
	{
		AfxMessageBox(_T("打开日志文件失败"));
	}
	while(1)
	{
		for(size_t i = 0; i < g_records.size(); i++)
		{		
			g_wndTitleToFind = g_records.at(i);
			if(hWnd = ::FindWindow(0, g_wndTitleToFind))
			{
				mainDlg->KillWindow(hWnd);
				mainDlg->Log(g_records.at(i));
			}
		}
		Sleep(100);
	}
	mainDlg->m_fLog.Close();
	return 0;
}

void CAdBlockDlg::Log(CString log)
{
	if(m_bEnableDebug)
	{
		log = GenerateLog(log);
		OutputDebugString(log);
		m_fLog.WriteLine(log.GetBuffer(0));
	}
}

bool CAdBlockDlg::StartMonitorThread()
{	
	return CreateThread(NULL, 0, MonitorThread, this, 0, NULL) != 0;
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAdBlockDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CAdBlockDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

afx_msg LRESULT CAdBlockDlg::OnSystemtray(WPARAM wParam, LPARAM lParam)
{   
    switch(lParam)     
    {     
    case  WM_RBUTTONDOWN:
        {       
            CPoint point;
            GetCursorPos(&point);
            CMenu menu;
            menu.LoadMenu(IDR_MENU1);
            CMenu *pPopup = menu.GetSubMenu(0);
			if(!pPopup) 
				break;
            SetForegroundWindow(); 
            pPopup->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);
			pPopup->DestroyMenu();
			menu.DestroyMenu();
        }     
        break;  
    case  WM_LBUTTONDOWN:   
        {     
			OnShowMainWindow();
        }     
        break;     
    } 
    return 0;
}

void CAdBlockDlg::OnShowMainWindow()
{
	LoadRecords();
	ModifyStyleEx(0, WS_EX_TOPMOST);
    ShowWindow(SW_SHOWNORMAL);
}

void CAdBlockDlg::OnCaptureWindow()
{
	m_bStartCapture = true;
	::SetCapture(m_hWnd);
}

void CAdBlockDlg::OnExit()
{
	SaveConfig();
	SaveRecords(false);
	m_fLog.Close();
	exit(0);
}

void CAdBlockDlg::MyGetModuleFileName(TCHAR *moduleName, char *buf, int size)
{
	GetModuleFileName(NULL, buf, size);
	for(int i = strlen(buf); i >= 0; i--)
	{
		if(buf[i] == '\\')
		{
			buf[i+1] = '\0';
			break;
		}
	}
	_tcscat_s(buf, BUF_LINE_SIZE, moduleName);
}

bool CAdBlockDlg::LoadRecords(bool updateListBox)
{
	char moduleName[BUF_LINE_SIZE] = {0};
	MyGetModuleFileName(RECORDS_FILENAME, moduleName, BUF_LINE_SIZE);
	if(!m_fRecords.Open(moduleName, CAdBlockFile::modeCreate|CAdBlockFile::modeReadWrite|CAdBlockFile::modeNoTruncate))
	{
		AfxMessageBox(_T("打开记录文件失败"));
		return false;
	}
	int bufSize = BUF_LINE_SIZE;
	if(updateListBox)
		m_listBoxAds.ResetContent();
	g_records.empty();
	while(m_fRecords.ReadLine(g_title, bufSize))
	{
		bufSize = BUF_LINE_SIZE;
		if(updateListBox)
			m_listBoxAds.AddString(g_title);
		CString title = g_title;
		g_records.push_back(title);
	}
	m_fRecords.Close();
	return true;
}

bool CAdBlockDlg::SaveRecords(bool updateListBox)
{
	char moduleName[BUF_LINE_SIZE] = {0};
	MyGetModuleFileName(RECORDS_FILENAME, moduleName, BUF_LINE_SIZE);
	if(!m_fRecords.Open(moduleName, CAdBlockFile::modeCreate|CAdBlockFile::modeReadWrite))
	{
		AfxMessageBox(_T("打开记录文件失败"));
		return false;
	}
	if(updateListBox)
		m_listBoxAds.ResetContent();
	vector<CString>::iterator iter;
	for(iter = g_records.begin(); iter != g_records.end(); iter++)
	{
		if(updateListBox)
			m_listBoxAds.AddString(iter->GetBuffer(0));
		m_fRecords.WriteLine(iter->GetBuffer(0));
	}
	m_fRecords.Close();
	return true;
}

void CAdBlockDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_bStartCapture)
	{
		POINT pt;
		GetCursorPos(&pt);
		g_hWndAd = ::WindowFromPoint(pt);
		if (g_hWndAd != NULL)
		{
			memset(g_title, 0, BUF_LINE_SIZE);
			::GetWindowText(g_hWndAd, g_title, BUF_LINE_SIZE);

			KillWindow(g_hWndAd);
			CString title = g_title;
			g_records.push_back(title);
			if(!SaveRecords())
				AfxMessageBox(_T("保存记录失败"));
			Log(g_title);
			ReleaseCapture();
			m_bStartCapture = false;
		}
	}
}

void CAdBlockDlg::KillWindow(HWND hWnd)
{
	::SendMessage(hWnd, WM_CLOSE, 0, 0);
	::SendMessage(hWnd, WM_DESTROY, 0, 0);
	::SendMessage(hWnd, WM_EXITMENULOOP, 0, 0);
	::SendMessage(hWnd, WM_QUIT, 0, 0);
}

void CAdBlockDlg::OnBnClickedRemoveAd()
{
	int idx = m_listBoxAds.GetCurSel();
	m_listBoxAds.DeleteString(idx);
	int i = 0;
	vector<CString>::iterator iter;
	for(iter = g_records.begin(); iter != g_records.end(); iter++)
	{
		if(i == idx)
		{
			g_records.erase(iter);
			break;
		}
		i++;
	}
	SaveRecords();
}

void CAdBlockDlg::OnClose()
{
	int style = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);
	::SetWindowLong(m_hWnd, GWL_EXSTYLE, style & (!WS_EX_TOPMOST));
	ShowWindow(SW_HIDE);
}

void CAdBlockDlg::OnBnClickedEnableDebug()
{
	CButton* checkbox = (CButton*)GetDlgItem(IDC_ENABLE_DEBUG);
	m_bEnableDebug = checkbox->GetCheck();
}

void CAdBlockDlg::OnBnClickedStartOnboot()
{
	CButton* checkbox = (CButton*)GetDlgItem(IDC_ENABLE_BOOT);
	m_bEnableBoot = checkbox->GetCheck();


	HKEY hKey;
	CString strRegPath = _T("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
	if(m_bEnableBoot)
	{
		if (RegOpenKeyEx(HKEY_CURRENT_USER, strRegPath, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)     
		{
			TCHAR moduleName[BUF_LINE_SIZE];
			GetModuleFileName(0, moduleName, BUF_LINE_SIZE); 
			RegSetValueEx(hKey, APP_NAME, 0, REG_SZ, (const BYTE *)moduleName, _tcslen(moduleName));
			RegCloseKey(hKey);
		}
		else
		{
			AfxMessageBox(_T("开机启动设置失败"));
		}
	}
	else
	{
		if (RegOpenKeyEx(HKEY_CURRENT_USER, strRegPath, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
		{
			RegDeleteValue(hKey, APP_NAME);
			RegCloseKey(hKey);
		}
	}
}
