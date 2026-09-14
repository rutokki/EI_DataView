#include "stdafx.h"
#include "TestRightPane.h"

IMPLEMENT_DYNCREATE(TestRightPane, CView)

TestRightPane::TestRightPane() {
}
TestRightPane::~TestRightPane() {
}

BEGIN_MESSAGE_MAP(TestRightPane, CView)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

void TestRightPane::OnDraw(CDC* /*pDC*/) {
}

int TestRightPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 컨트롤 생성
	m_staticMutual.Create(_T("상호쇄정"), WS_CHILD | WS_VISIBLE | SS_LEFT, CRect(0, 0, 0, 0), this, 1501);
	m_editMutual.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, CRect(0, 0, 0, 0), this, 1502);

	m_staticSwitch.Create(_T("선로전환기쇄정"), WS_CHILD | WS_VISIBLE | SS_LEFT, CRect(0, 0, 0, 0), this, 1503);
	m_editSwitch.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, CRect(0, 0, 0, 0), this, 1504);

	m_staticSignal.Create(_T("신호제어조건"), WS_CHILD | WS_VISIBLE | SS_LEFT, CRect(0, 0, 0, 0), this, 1505);
	m_editSignal.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, CRect(0, 0, 0, 0), this, 1506);

	m_staticRoute.Create(_T("진로구분쇄정"), WS_CHILD | WS_VISIBLE | SS_LEFT, CRect(0, 0, 0, 0), this, 1507);
	m_editRoute.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, CRect(0, 0, 0, 0), this, 1508);

	m_staticApproach.Create(_T("접근쇄정"), WS_CHILD | WS_VISIBLE | SS_LEFT, CRect(0, 0, 0, 0), this, 1509);
	m_editApproach.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, CRect(0, 0, 0, 0), this, 1510);

	return 0;
}

void TestRightPane::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	int nMarginX = 20;
	int nMarginY = 20;
	int nLabelWidth = 120;
	int nHeight = 25;
	int nSpacing = 20;

	int y = nMarginY;
	int nEditWidth = cx - nMarginX * 2 - nLabelWidth - 10;
	if (nEditWidth < 100) nEditWidth = 100;

	// 1. 상호쇄정
	m_staticMutual.SetWindowPos(NULL, nMarginX, y, nLabelWidth, nHeight, SWP_NOZORDER);
	m_editMutual.SetWindowPos(NULL, nMarginX + nLabelWidth + 10, y, nEditWidth, nHeight, SWP_NOZORDER);
	y += nHeight + nSpacing;

	// 2. 선로전환기쇄정
	m_staticSwitch.SetWindowPos(NULL, nMarginX, y, nLabelWidth, nHeight, SWP_NOZORDER);
	m_editSwitch.SetWindowPos(NULL, nMarginX + nLabelWidth + 10, y, nEditWidth, nHeight, SWP_NOZORDER);
	y += nHeight + nSpacing;

	// 3. 신호제어조건
	m_staticSignal.SetWindowPos(NULL, nMarginX, y, nLabelWidth, nHeight, SWP_NOZORDER);
	m_editSignal.SetWindowPos(NULL, nMarginX + nLabelWidth + 10, y, nEditWidth, nHeight, SWP_NOZORDER);
	y += nHeight + nSpacing;

	// 4. 진로구분쇄정
	m_staticRoute.SetWindowPos(NULL, nMarginX, y, nLabelWidth, nHeight, SWP_NOZORDER);
	m_editRoute.SetWindowPos(NULL, nMarginX + nLabelWidth + 10, y, nEditWidth, nHeight, SWP_NOZORDER);
	y += nHeight + nSpacing;

	// 5. 접근쇄정
	m_staticApproach.SetWindowPos(NULL, nMarginX, y, nLabelWidth, nHeight, SWP_NOZORDER);
	m_editApproach.SetWindowPos(NULL, nMarginX + nLabelWidth + 10, y, nEditWidth, nHeight, SWP_NOZORDER);
}

void TestRightPane::UpdateDetailData(const CString& strItemName)
{
	CString strVal;
	strVal.Format(_T("[%s] 상호쇄정 값"), strItemName);
	m_editMutual.SetWindowText(strVal);

	strVal.Format(_T("[%s] 선로전환기쇄정 값"), strItemName);
	m_editSwitch.SetWindowText(strVal);

	strVal.Format(_T("[%s] 신호제어조건 값"), strItemName);
	m_editSignal.SetWindowText(strVal);

	strVal.Format(_T("[%s] 진로구분쇄정 값"), strItemName);
	m_editRoute.SetWindowText(strVal);

	strVal.Format(_T("[%s] 접근쇄정 값"), strItemName);
	m_editApproach.SetWindowText(strVal);
}