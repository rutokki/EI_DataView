// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "DataViewEx.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame


IMPLEMENT_DYNCREATE(CMainFrame, CBCGPFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CBCGPFrameWnd)
	ON_WM_CREATE()
	ON_REGISTERED_MESSAGE(BCGM_RESETTOOLBAR, &CMainFrame::OnToolbarReset)
	ON_MESSAGE(WM_DPICHANGED, &CMainFrame::OnDPIChanged)

END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	SetDockingBarsIconList(IDB_WORKSPACE, CSize(16, 16), CLR_NONE, TRUE, TRUE);
	SetDockingBarsIconList(ID_DIFF_TOOL, CSize(16, 16), CLR_NONE, TRUE, TRUE);
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CBCGPToolBar::EnableQuickCustomization();

	// Menu will not take the focus on activation:
	CBCGPPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("Failed to create menubar\n");

		return -1;      // fail to create
	}
	m_wndMenuBar.SetWindowText(_T("메뉴 모음")); // 또는 "메뉴 바"

	m_wndMenuBar.SetBarStyle(m_wndMenuBar.GetBarStyle() | CBRS_SIZE_DYNAMIC);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	m_wndMenuBar.SetWindowText(_T("메뉴 바"));


	if (!m_wndToolBar.LoadToolBar(IDR_MAINFRAME, 0, 0, FALSE, 0, 0, IDB_TOOLBAR_HC))
	{
		//m_wndToolBar.ReplaceButton()
		TRACE0("Failed to load toolbar\n");
		return -1;      // fail to createap
	}

	CString strMainToolbarTitle;
	strMainToolbarTitle.LoadString(IDS_MAIN_TOOLBAR);
	m_wndToolBar.SetWindowText(strMainToolbarTitle.GetString());

	m_wndToolBar.SetWindowText(_T("툴바"));
	//if (!m_wndStatusBar.Create(this) ||
	//	!m_wndStatusBar.SetIndicators(indicators,
	//		sizeof(indicators) / sizeof(UINT)))
	//{
	//	TRACE0("Failed to create status bar\n");
	//	return -1;      // fail to create
	//}

	const int nPaneSize = globalUtils.ScaleByDPI(200, this);

	//if (!m_wndWorkSpace.Create(_T("Diff 도구"), this, CRect(0, 0, nPaneSize, nPaneSize),
	//	TRUE, ID_VIEW_WORKSPACE,
	//	WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	//{
	//	TRACE0("Failed to create Workspace bar\n");
	//	return -1;      // fail to create
	//}

	//m_wndWorkSpace.SetIconIndex(2);

	if (!m_wndWorkSpace2.Create(_T("파일 목록"),
		this, TRUE, MAKEINTRESOURCE(IDD_DIALOGBAR),
		WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI,
		ID_VIEW_WORKSPACE2))
	{
		TRACE0("Failed to create Workspace bar 2\n");
		return -1;      // fail to create
	}

	m_wndWorkSpace2.SetIconIndex(1);

	// TODO: delete these three lines if you don't want the toolbar to be dockable
	//m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	//m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	//m_wndWorkSpace.EnableDocking(CBRS_ALIGN_ANY);
	m_wndWorkSpace2.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	EnableAutoHideBars(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndMenuBar);
	DockControlBar(&m_wndToolBar);
	DockControlBar(&m_wndWorkSpace2);
	//m_wndWorkSpace2.AttachToTabWnd(&m_wndWorkSpace, BCGP_DM_STANDARD, FALSE, NULL);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CBCGPFrameWnd::PreCreateWindow(cs))
		return FALSE;
	// TODO: modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

void CMainFrame::AddRecentFile(CString strFinePath)
{
	m_wndWorkSpace2.AddPath(strFinePath);
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CBCGPFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CBCGPFrameWnd::Dump(dc);
}

#endif //_DEBUG

// CMainFrame message handlers


LRESULT CMainFrame::OnToolbarReset(WPARAM /*wp*/, LPARAM)
{
	// TODO: reset toolbar with id = (UINT) wp to its initial state:
	//
	// UINT uiToolBarId = (UINT) wp;
	// if (uiToolBarId == IDR_MAINFRAME)
	// {
	//		do something with m_wndToolBar
	// }

	return 0;
}


//테마 변경
void CMainFrame::OnAfterChangeVisualTheme()
{
#ifndef _BCGSUITE_INC_
	CBCGPWinApp::BCGP_VISUAL_THEME theme = theApp.GetVisualTheme();

	if (theme == CBCGPWinApp::BCGP_VISUAL_THEME_OFFICE_2016_WHITE ||
		theme == CBCGPWinApp::BCGP_VISUAL_THEME_OFFICE_2016_COLORFUL ||
		theme == CBCGPWinApp::BCGP_VISUAL_THEME_OFFICE_2016_DARK_GRAY ||
		theme == CBCGPWinApp::BCGP_VISUAL_THEME_OFFICE_2016_BLACK ||
		theme == CBCGPWinApp::BCGP_VISUAL_THEME_OFFICE_2019_WHITE ||
		theme == CBCGPWinApp::BCGP_VISUAL_THEME_OFFICE_2019_COLORFUL ||
		theme == CBCGPWinApp::BCGP_VISUAL_THEME_OFFICE_2019_DARK_GRAY ||
		theme == CBCGPWinApp::BCGP_VISUAL_THEME_OFFICE_2019_BLACK ||
		theme == CBCGPWinApp::BCGP_VISUAL_THEME_OFFICE_2022_COLORFUL ||
		theme == CBCGPWinApp::BCGP_VISUAL_THEME_OFFICE_2022_DARK_GRAY ||
		theme == CBCGPWinApp::BCGP_VISUAL_THEME_OFFICE_2022_WHITE ||
		theme == CBCGPWinApp::BCGP_VISUAL_THEME_OFFICE_2022_BLACK ||
		theme == CBCGPWinApp::BCGP_VISUAL_THEME_VS_2022_LIGHT ||
		theme == CBCGPWinApp::BCGP_VISUAL_THEME_VS_2022_DARK,
		theme == CBCGPWinApp::BCGP_VISUAL_THEME_VS_2026_JUICY_PLUM ||
		theme == CBCGPWinApp::BCGP_VISUAL_THEME_VS_2026_COOL_BREEZE ||
		theme == CBCGPWinApp::BCGP_VISUAL_THEME_VS_2026_ICY_MINT ||
		theme == CBCGPWinApp::BCGP_VISUAL_THEME_VS_2026_MOONLIGHT_GLOW ||
		theme == CBCGPWinApp::BCGP_VISUAL_THEME_VS_2026_SPICY_RED ||
		theme == CBCGPWinApp::BCGP_VISUAL_THEME_VS_2026_MANGO_PARADISE ||
		theme == CBCGPWinApp::BCGP_VISUAL_THEME_VS_2026_MYSTICAL_FOREST ||
		theme == CBCGPWinApp::BCGP_VISUAL_THEME_VS_2026_COOL_SLATE ||
		theme == CBCGPWinApp::BCGP_VISUAL_THEME_VS_2026_BUBBLEGUM ||
		theme == CBCGPWinApp::BCGP_VISUAL_THEME_VS_2026_SUNNY_DAY ||
		theme == CBCGPWinApp::BCGP_VISUAL_THEME_VS_2026_SILKY_PINK ||
		theme == CBCGPWinApp::BCGP_VISUAL_THEME_VS_2026_CUSTOM)
	{
		CBCGPVisualManager2013::SetAccentColor(CBCGPVisualManagerVS2012::VS2012_Green);
	}
	else if (theme == CBCGPWinApp::BCGP_VISUAL_THEME_VS_2022_DARK)
	{
		CBCGPVisualManager2013::SetAccentColorRGB(RGB(113, 96, 232));
	}
	else
	{
		CBCGPVisualManager2013::SetAccentColor(CBCGPVisualManagerVS2012::VS2012_Default);
	}

	SendMessageToDescendants(BCGM_CHANGEVISUALMANAGER, 0, 0, TRUE);
#endif
}
void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CBCGPFrameWnd::OnSettingChange(uFlags, lpszSection);
	OnAfterChangeVisualTheme();
}
LRESULT CMainFrame::OnDPIChanged(WPARAM wp, LPARAM lp)
{
	LRESULT lRes = CBCGPFrameWnd::OnDPIChanged(wp, lp);

	return lRes;
}
