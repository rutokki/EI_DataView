#include "stdafx.h"
#include "TestLeftPane.h"
#include "TestGridInfo.h"
#include "TestRightPane.h"
#include "StructMainData.h"

IMPLEMENT_DYNCREATE(TestLeftPane, CView)

TestLeftPane::TestLeftPane() {
}
TestLeftPane::~TestLeftPane() {
}

BEGIN_MESSAGE_MAP(TestLeftPane, CView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_LBN_SELCHANGE(1400, &TestLeftPane::OnSelChangeListBox)
END_MESSAGE_MAP()

void TestLeftPane::OnDraw(CDC* /*pDC*/) {
}

void TestLeftPane::LoadRouteData()
{
	auto route = StructMainData::GetInstance().GetInterLockInfo();
	if (route.size() == 0) return;
	for (int nIdx = 0; nIdx < route.size(); ++nIdx) // 혹은 크기만큼 반복
	{
		// 진로 이름 가져오기
		CString strName(route[nIdx].Name);

		// 리스트박스에 추가
		int nIndex = m_wndListBox.AddString(strName);

		// (선택) 나중에 선택된 항목의 원본 데이터를 쉽게 찾기 위해 인덱스(nIdx)를 ItemData에 저장할 수도 있습니다.
		m_wndListBox.SetItemData(nIndex, nIdx);
	}
}

int TestLeftPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// BCG 리스트박스 생성
	m_wndListBox.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_STANDARD | LBS_NOTIFY, CRect(0, 0, 0, 0), this, 1400);

	// 테스트 데이터 추가
	//m_wndListBox.AddString(_T("시험 항목 01"));
	//m_wndListBox.AddString(_T("시험 항목 02"));
	//m_wndListBox.AddString(_T("시험 항목 03"));


	return 0;
}

void TestLeftPane::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	if (m_wndListBox.GetSafeHwnd())
	{
		m_wndListBox.SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER | SWP_NOACTIVATE);
	}
}

void TestLeftPane::OnSelChangeListBox()
{
	int nSel = m_wndListBox.GetCurSel();
	if (nSel == LB_ERR) return;

	CString strText;
	m_wndListBox.GetText(nSel, strText);

	// 수정: TestLeftPane의 바로 위 부모가 이미 m_wndSplitter입니다!
	// GetParent()를 한 번만 호출하고 CBCGPSplitterWnd로 캐스팅합니다.
	CBCGPSplitterWnd* pSplitter = (CBCGPSplitterWnd*)GetParent();
	if (pSplitter != nullptr && pSplitter->GetSafeHwnd() != NULL)
	{
		// 우측 패널(0행, 1열) 가져오기
		CWnd* pPane = pSplitter->GetPane(0, 1);
		if (pPane != nullptr)
		{
			TestRightPane* pRightPane = (TestRightPane*)pPane;
			pRightPane->UpdateDetailData(strText);
		}
	}
}
void TestLeftPane::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	LoadRouteData();
}
