#include "stdafx.h"
#include "TestLeftView.h"
#include "TestContainer.h"
#include "StructMainData.h"
#include "resource.h"
IMPLEMENT_DYNCREATE(TestLeftView, CView)

TestLeftView::TestLeftView() {
}
TestLeftView::~TestLeftView() {
}

BEGIN_MESSAGE_MAP(TestLeftView, CView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_LBN_SELCHANGE(1501, &TestLeftView::OnSelChangeListBox)
	ON_CBN_SELCHANGE(1502, &TestLeftView::OnSelChangeComboBox)
	ON_COMMAND(ID_FILE_SAVE, &TestLeftView::OnFileSave)
	ON_COMMAND(ID_FILE_OPEN, &TestLeftView::OnFileOpen)
	ON_COMMAND(ID_DIFF_TOOL, &TestLeftView::OnFileDiff)
	ON_COMMAND(ID_FILE_PRINT, &TestLeftView::OnFilePrint)
	ON_UPDATE_COMMAND_UI(ID_FILE_PRINT, &TestLeftView::OnUpdatePrint)
	ON_UPDATE_COMMAND_UI(ID_DIFF_TOOL, &TestLeftView::OnUpdateFileDiff)
	ON_WM_SETFOCUS()
END_MESSAGE_MAP()

void TestLeftView::OnDraw(CDC* pDC)
{
	COLORREF clrText = globalData.clrWindowText;
	if (!pDC->IsPrinting())
	{
		CRect rectClient;
		GetClientRect(rectClient);

		// BCG 비주얼 매니저의 현재 테마 배경색으로 채우기
		pDC->FillSolidRect(rectClient, CBCGPVisualManager::GetInstance()->GetControlFillColor());
		clrText = CBCGPVisualManager::GetInstance()->GetControlTextColor();
	}
}

int TestLeftView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 리스트박스 생성 
	DWORD dwStyle = WS_CHILD | WS_VISIBLE | LBS_STANDARD | WS_VSCROLL;
	CRect rect(0, 0, 0, 0);

	DWORD dwListStyle = WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | LBS_NOTIFY;
	if (!m_wndListBox.Create(dwListStyle, rect, this, 1501))
	{
		return -1;
	}

	// 콤보박스 생성 
	DWORD dwComboStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_VSCROLL | LBS_NOTIFY;
	if (!m_wndComboBox.Create(dwComboStyle, rect, this, 1502))
	{
		return -1;
	}
	m_wndComboBox.SetMinVisibleItems(5);

	return 0;
}

void TestLeftView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if (cx <= 20 || cy <= 30)
		return;

	int nMargin = 10;
	int nSpacing = 10;

	int availWidth = cx - (nMargin * 2);
	int availHeight = cy - (nMargin * 2) - nSpacing;

	// 상단 리스트박스: 가용 높이의 80%
	int listHeight = static_cast<int>(availHeight * 0.8);

	// 하단 콤보박스 시작 위치 및 높이
	int comboY = nMargin + listHeight + nSpacing;
	int comboHeight = availHeight - listHeight;

	// 1. 리스트박스 크기 및 위치 조절
	if (m_wndListBox.GetSafeHwnd() != NULL)
	{
		m_wndListBox.MoveWindow(nMargin, nMargin, availWidth, listHeight);
	}

	// 2. 콤보박스 크기 및 위치 조절 (드롭다운 영역 확보를 위해 높이 여유분 150px 부여)
	if (m_wndComboBox.GetSafeHwnd() != NULL)
	{
		m_wndComboBox.MoveWindow(nMargin, comboY, availWidth, comboHeight + 150);
	}
}
//void TestLeftView::OnSelChangeListBox()
//{
//	int nSel = m_wndListBox.GetCurSel();
//	if (nSel == LB_ERR)
//		return;
//
//	size_t nRouteIdx = static_cast<size_t>(m_wndListBox.GetItemData(nSel));
//	WORD nRteNo = static_cast<WORD>(nRouteIdx + 1);
//
//	auto routeSpan = StructMainData::GetInstance().GetRouteInfo();
//	if (nRouteIdx >= routeSpan.size())
//		return;
//
//	// 스플리터 창을 거쳐 공통 부모인 TestContainer 획득
//	CWnd* pParentWnd = GetParent(); // 스플리터 윈도우
//	if (pParentWnd != nullptr)
//	{
//		TestContainer* pContainer = DYNAMIC_DOWNCAST(TestContainer, pParentWnd->GetParent());
//		if (pContainer != nullptr)
//		{
//			// TestContainer를 통해 우측 뷰 데이터 갱신 요청
//			pContainer->ChangeRouteSelection(nRteNo);
//		}
//	}
//}
void TestLeftView::LoadRouteList()
{
	m_wndListBox.ResetContent();
	m_wndComboBox.ResetContent(); // 콤보박스 초기화 추가

	auto routeSpan = StructMainData::GetInstance().GetRouteInfo();
	auto ilkSpan = StructMainData::GetInstance().GetInterLockInfo();

	if (routeSpan.empty() || ilkSpan.empty())
		return;

	for (size_t i = 0; i < routeSpan.size(); ++i)
	{
		WORD nRteNo = static_cast<WORD>(i + 1);
		if (nRteNo >= ilkSpan.size())
			break;

		CString strRouteName(ilkSpan[nRteNo].Name);
		if (strRouteName.IsEmpty())
			continue;

		// 1. 리스트박스 추가 및 ItemData 설정
		int nIndex = m_wndListBox.AddString(strRouteName);
		if (nIndex != LB_ERR && nIndex != LB_ERRSPACE)
		{
			m_wndListBox.SetItemData(nIndex, static_cast<DWORD_PTR>(i));
		}

		// 2. 콤보박스 추가 및 ItemData 설정
		int nComboIndex = m_wndComboBox.AddString(strRouteName);
		if (nComboIndex != CB_ERR && nComboIndex != CB_ERRSPACE)
		{
			m_wndComboBox.SetItemData(nComboIndex, static_cast<DWORD_PTR>(i));
		}
	}
}
void TestLeftView::SelectRouteByIndex(int nSel)
{
	if (nSel == LB_ERR || nSel == CB_ERR)
		return;

	// 1. 리스트박스와 콤보박스 상호 선택 상태 동기화
	if (m_wndListBox.GetCurSel() != nSel)
		m_wndListBox.SetCurSel(nSel);

	if (m_wndComboBox.GetCurSel() != nSel)
		m_wndComboBox.SetCurSel(nSel);

	// 2. 바인딩된 0기반 벡터 인덱스 및 1기반 논리 번호 획득
	size_t nRouteIdx = static_cast<size_t>(m_wndListBox.GetItemData(nSel));
	WORD nRteNo = static_cast<WORD>(nRouteIdx + 1);

	auto routeSpan = StructMainData::GetInstance().GetRouteInfo();
	if (nRouteIdx >= routeSpan.size())
		return;

	// 3. TestContainer를 거쳐 우측 뷰(TestRightView) 데이터 갱신 요청
	CWnd* pParentWnd = GetParent(); // 스플리터 윈도우
	if (pParentWnd != nullptr)
	{
		TestContainer* pContainer = DYNAMIC_DOWNCAST(TestContainer, pParentWnd->GetParent());
		if (pContainer != nullptr)
		{
			pContainer->ChangeRouteSelection(nRteNo);
		}
	}
}

// 기존 리스트박스 클릭/선택 핸들러
void TestLeftView::OnSelChangeListBox()
{
	int nSel = m_wndListBox.GetCurSel();
	SelectRouteByIndex(nSel);
}

// 새로 추가된 콤보박스 선택 핸들러
void TestLeftView::OnSelChangeComboBox()
{
	int nSel = m_wndComboBox.GetCurSel();
	SelectRouteByIndex(nSel);
}
void TestLeftView::OnSetFocus(CWnd* pOldWnd)
{
	CView::OnSetFocus(pOldWnd);

	// 뷰가 클릭되어 포커스를 얻을 때, 메인 프레임에 현재 활성 뷰가 CDataViewExView임을 강제로 알림

	CFrameWnd* pMainFrame = DYNAMIC_DOWNCAST(CFrameWnd, AfxGetMainWnd());
	if (pMainFrame != nullptr)
	{
		CView* pMainView = DYNAMIC_DOWNCAST(CView, pMainFrame->GetActiveView());
		// 만약 현재 활성 뷰가 자신이 아니라면 메인 뷰를 CDataViewExView로 고정 유지

	}
}

void TestLeftView::OnFileSave()
{
	CWnd* pParentWnd = GetParent()->GetParent(); // TestContainer 스플리터 윈도우
	if (pParentWnd != nullptr)
	{
		AFX_CMDHANDLERINFO info = { 0 };
		if (pParentWnd->OnCmdMsg(ID_FILE_SAVE, CN_COMMAND, NULL, &info))
		{
			return;
		}
	}
}
void TestLeftView::OnFileOpen()
{
	CWnd* pParentWnd = GetParent()->GetParent(); // TestContainer 스플리터 윈도우
	if (pParentWnd != nullptr)
	{
		AFX_CMDHANDLERINFO info = { 0 };
		if (pParentWnd->OnCmdMsg(ID_FILE_OPEN, CN_COMMAND, NULL, &info))
		{
			return;
		}
	}
}

void TestLeftView::OnFileDiff()
{
	CWnd* pParentWnd = GetParent()->GetParent(); // TestContainer 스플리터 윈도우
	if (pParentWnd != nullptr)
	{
		AFX_CMDHANDLERINFO info = { 0 };
		if (pParentWnd->OnCmdMsg(ID_DIFF_TOOL, CN_COMMAND, NULL, &info))
		{
			return;
		}
	}
}

void TestLeftView::OnFilePrint()
{
	CWnd* pParentWnd = GetParent()->GetParent(); // TestContainer 스플리터 윈도우
	if (pParentWnd != nullptr)
	{
		AFX_CMDHANDLERINFO info = { 0 };
		if (pParentWnd->OnCmdMsg(ID_FILE_PRINT, CN_COMMAND, NULL, &info))
		{
			return;
		}
	}
}

void TestLeftView::OnUpdatePrint(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void TestLeftView::OnUpdateFileDiff(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}
