// DataViewEx.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "DataViewEx.h"
#include "MainFrm.h"

#include "DataViewExDoc.h"
#include "DataViewExView.h"
#include "FontSetting.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

// CDataViewExApp

BEGIN_MESSAGE_MAP(CDataViewExApp, CBCGPWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CDataViewExApp::OnAppAbout)
	// Standard file based document commands
	//ON_COMMAND(ID_FILE_NEW, &CBCGPWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CBCGPWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CBCGPWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// CDataViewExApp construction

CDataViewExApp::CDataViewExApp()
{

	// Support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;

	CBCGPVisualManagerVS2012::m_bAutoGrayscaleImages = FALSE;

	SetApplicationLookMenu(ID_VIEW_APPLOOK, BCGP_THEMES_LIST_SCENIC | BCGP_THEMES_LIST_OFFICE_2016 | BCGP_THEMES_LIST_VS_2019 | BCGP_THEMES_LIST_OFFICE_2022 | BCGP_THEMES_LIST_VS_2022 | BCGP_THEMES_LIST_VS_2026);

	// Enable VS 2026 look (Light theme)
	SetVisualTheme(BCGP_VISUAL_THEME_VS_2026_LIGHT);

	CBCGPToolbarOptions toolbarOptions;

	toolbarOptions.m_nViewToolbarsMenuEntryID = ID_VIEW_TOOLBARS;
	//toolbarOptions.m_nCustomizeCommandID = ID_VIEW_CUSTOMIZE; // 커스터마이즈 툴바 옵션
	//toolbarOptions.m_strCustomizeCommandLabel = _T("Customize..."); // Customize CommandID 클릭 시 보이는 글자
	toolbarOptions.m_nUserToolbarCommandIDFirst = uiFirstUserToolBarId;
	toolbarOptions.m_nUserToolbarCommandIDLast = uiLastUserToolBarId;
	toolbarOptions.m_strToolbarCustomIconsPath = _T(".\\UserImages.png");

	SetToolbarOptions(toolbarOptions);

	m_bDPIAwareTooltips = TRUE;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CDataViewExApp object

CDataViewExApp theApp;
CBCGPOccManager theOccManager;


// CDataViewExApp initialization

BOOL CDataViewExApp::InitInstance()
{
	FontSetting::Initialize();


	//VLDDisable();
	// 
	// 
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);
	CBCGPToolBar::ResetAll();
	CBCGPWinApp::InitInstance();

	// Remove this line if you don't need themed product dialogs:
	globalData.m_bUseVisualManagerInBuiltInDialogs = TRUE;
	globalData.m_bIsDlgWsCaptionStyle = TRUE;
	globalData.m_bIsMiniFrameWsCaptionStyle = TRUE;
	globalData.m_nAutoInverseIconsInDarkTheme = BCGP_AUTOINVERSE_ALL_ICONS;
	globalData.m_FrameCornerType = globalData.m_DlgCornerType = globalData.m_PopupMenuCornerType = BCGP_DWMWCP_ROUND;
	globalData.m_MiniFrameCornerType = BCGP_DWMWCP_ROUNDSMALL;

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer(&theOccManager);

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: you should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("EI_DataView"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)

	SetRegistryBase(_T("Settings"));

	// Initialize all Managers for usage. They are automatically constructed
	// if not yet present
	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CDataViewExDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CDataViewExView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);



	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;



	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand

	return TRUE;
}

// CDataViewExApp message handlers

int CDataViewExApp::ExitInstance()
{
	FontSetting::ReleaseFonts();
	return CBCGPWinApp::ExitInstance();
}

// CAboutDlg dialog used for App About

class CAboutDlg : public CBCGPDialog
{
public:
	CAboutDlg();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum {
		IDD = IDD_ABOUTBOX
	};
#endif
	CBCGPURLLinkButton m_btnURL;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CBCGPDialog(IDD_ABOUTBOX)
{
	EnableVisualManagerStyle(TRUE, TRUE);
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMPANY_URL, m_btnURL);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CBCGPDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void CDataViewExApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CDataViewExApp message handlers


void CDataViewExApp::OnAfterChangeVisualTheme(CWnd* pMainWnd)
{
	CMainFrame* pFrame = DYNAMIC_DOWNCAST(CMainFrame, pMainWnd);
	if (pFrame->GetSafeHwnd() != NULL)
	{
		pFrame->OnAfterChangeVisualTheme();
	}
}
void CDataViewExApp::PreLoadState()
{
	GetContextMenuManager()->AddMenu(_T("My menu"), IDR_CONTEXT_MENU);

	// TODO: add another context menus here
}
