#include "stdafx.h"
#include "CustomBCGGridCtrl.h"

IMPLEMENT_DYNAMIC(CustomBCGGridCtrl, CBCGPGridCtrl)

CustomBCGGridCtrl::CustomBCGGridCtrl()
{
	m_bTextOverflowing = FALSE;
	m_pHeaderFont = NULL;
	m_clrHeaderText = CLR_DEFAULT;
	m_clrHeaderBk = CLR_DEFAULT;
	m_bVisualManagerStyle = TRUE; // 비주얼 매니저 스타일 적용
	//EnableGroupByBox(TRUE); // 그룹화 박스 비활성화
	m_Column.SetOwnerList(this);
	SetVisualManagerColorTheme(TRUE, TRUE, TRUE);
	EnableTextOverflowing(FALSE);
	AllowRowExtraHeight(FALSE);
}

CustomBCGGridCtrl::~CustomBCGGridCtrl()
{
	m_bCanFilter = FALSE;
	if (m_pHeaderFont != NULL && m_pHeaderFont->GetSafeHandle() != NULL)
	{
		m_pHeaderFont->DeleteObject();
		// 만약 외부에서 new로 동적 생성해서 넘겨준 폰트라면 delete m_pHeaderFont; 도 필요합니다.
	}
	EnableAlternateRows(FALSE);

}
void CustomBCGGridCtrl::InitGridControl()
{
	// ==========================================================
	// 3. 테마 및 색상 적용 (정정된 부분)
	// ==========================================================
	// 앱의 전체 테마(Visual Manager)와 표 내부의 배경/글씨 색상을 동기화합니다.


	// (선택 사항) 짝수/홀수 행의 색상을 번갈아가며 다르게 주어 가독성을 높입니다. 
	// SetVisualManagerColorTheme이 켜져 있으면 이 교차 색상도 테마에 어울리게 자동 배색됩니다.
	//EnableAlternateRows(TRUE);

	// (선택 사항) 스크롤바 역시 Windows 기본이 아닌 비주얼 매니저 테마로 맞추고 싶을 때 사용합니다.
	SetScrollBarsStyle(CBCGPScrollBar::BCGP_SBSTYLE_VISUAL_MANAGER);

	CBCGPGridColors colors;
	colors = GetColorTheme();

	// ==========================================================
	// 2. 선택된 행(Highlight)의 색상을 원하는 대로 변경합니다.
	// ==========================================================
	// 예시: 선택 시 배경을 '연한 파란색', 글씨를 '하얀색'으로 변경
	colors.m_SelColors.m_clrBackground = RGB(100, 150, 255); // 선택 영역 배경색
	colors.m_SelColors.m_clrText = RGB(255, 255, 255); // 선택 영역 글자색

	// (선택) 테마에 따라 그라데이션이나 테두리가 들어가는 경우가 있습니다.
	// 단색으로 깔끔하게 보이려면 아래처럼 비활성화(-1) 해주는 것이 좋습니다.
	/*colors.m_SelColors.m_clrGradient = (COLORREF)-1;
	colors.m_SelColors.m_clrBorder = (COLORREF)-1;*/

	// 외곽선 색상 지정 (다크 테마 기준 밝은 회색)
	colors.m_clrBorder = RGB(100, 100, 100);

	// 3. 수정된 색상 테마를 그리드에 다시 덮어씌웁니다.
	SetColorTheme(colors);

	// 3. 사용자가 열(Column)을 드래그해서 순서를 섞지 못하게 차단 (핵심)
	//m_wndGrid.EnableDragHeaderItems(FALSE);

	// ==========================================================
	// 💡 추가할 코드: 왼쪽 행 선택 마커(삼각형) 및 헤더 영역 완전히 숨기기
	// ==========================================================
	// 1. 행 헤더(Row Header) 및 삼각형 마커 제거
	EnableRowHeader(FALSE);   // 텍스트/번호가 들어가는 넓은 헤더 숨김
	SetRowMarker(FALSE);      // ▶ 삼각형(현재 행 표시기) 자체를 숨김

	// 2. [추가된 핵심 코드] 왼쪽 여백을 암묵적으로 점유하는 숨은 기능들을 모두 끕니다.
	SetShowDragContext(FALSE);          // 드래그 앤 드롭을 위한 컨텍스트 공간 제거
	EnableHeaderSelectAllMarker(FALSE); // 헤더 좌측 상단 여백 제거
	EnableDragSelectionBorder(FALSE);   // 테두리 선택 영역 제거
	EnableTrackingToolTips(TRUE);
	EnableToolTips(TRUE);
	// 2. 눈에 보이지 않지만 왼쪽 공간을 차지하는 기타 속성들 강제 비활성화
	EnableDragSelectionBorder(FALSE); // 마우스 드래그 선택을 위한 얇은 테두리 공간 제거
	EnableTreeLines(FALSE);           // 트리 구조의 선(+, -)이 들어갈 여유 공간 제거
	EnableTreeButtons(FALSE);         // 트리 버튼 공간 제거
	EnableCheckBoxes(FALSE);          // 행별 체크박스 기능 비활성화
	SetShowDragContext(FALSE);          // 드래그 앤 드롭을 위한 컨텍스트 공간 제거
	EnableHeaderSelectAllMarker(FALSE); // 헤더 좌측 상단 여백 제거
	EnableTextOverflowing(FALSE);
	// 표 전체 더블클릭 및 수정(Edit) 원천 차단
	SetReadOnly(TRUE);

	// 표가 읽기 전용(ReadOnly) 상태일 때만 정상 동작합니다.
	SetWholeRowSel(TRUE);

	// 헤더는 보이게(TRUE) 하되, 헤더 클릭 시 발생하는 모든 기능(정렬, 이동, 숨김)을 차단(0)합니다.
	EnableHeader(TRUE, 0);

}


void CustomBCGGridCtrl::SetHeaderFont(CFont* pFont)
{
	m_pHeaderFont = pFont;
	if (GetSafeHwnd() != NULL)
	{
		RedrawWindow(); // 폰트 변경 후 다시 그리기
	}
}

void CustomBCGGridCtrl::SetHeaderHeight(int nHeight)
{
	m_nHeaderHeight = nHeight;

	if (GetSafeHwnd() != NULL)
	{
		AdjustLayout(); // 높이 변경 후 레이아웃 재계산
	}
}

int CustomBCGGridCtrl::GetHeaderHeight()
{
	return m_nHeaderHeight;
}

// 1. 헤더 영역의 높이(Rect) 결정
CRect CustomBCGGridCtrl::OnGetHeaderRect(CDC* pDC, const CRect& rectDraw)
{
	// 원본 헤더 영역 계산
	CRect rect = CBCGPGridCtrl::OnGetHeaderRect(pDC, rectDraw);

	if (!rect.IsRectEmpty())
	{
		// 1. 헤더 라인 수가 1보다 크면 멀티라인 헤더로 간주하여 높이 확장
		int nLineCount = m_Column.GetHeaderLineCount();

		if (nLineCount > 1)
		{
			// 헤더 높이 = (기본 한 줄 높이) * (라인 수)
			// rect.Height()는 보통 한 줄의 높이를 반환합니다.
			rect.bottom = rect.top + (rect.Height() * nLineCount);
		}
		// 2. 만약 m_nHeaderHeight 변수가 별도로 설정되어 있다면 그 값을 우선시하도록 로직 연결
		else if (m_nHeaderHeight > 0)
		{
			rect.bottom = rect.top + m_nHeaderHeight;
		}
	}

	return rect;
}

void CustomBCGGridCtrl::OnPosSizeChanged()
{
	CBCGPGridCtrl::OnPosSizeChanged();
	m_Column.ReposHeaderItems();
}

// 이 부분에서 폰트를 완벽하게 덮어씌웁니다.
void CustomBCGGridCtrl::OnDrawHeader(CDC* pDC)
{
	CFont* pOldDCFont = NULL;
	HFONT hOldGridFont = m_hFont; // CBCGPGridCtrl의 원래 기본 폰트 핸들 백업
	m_Column.PrepareDrawHeader();
	if (m_pHeaderFont != NULL && m_pHeaderFont->GetSafeHandle() != NULL)
	{
		// 1. 현재 DC에 헤더 폰트를 쥐여줍니다.
		pOldDCFont = pDC->SelectObject(m_pHeaderFont);

		// 2. 내부 로직이 강제로 기본 폰트(m_hFont)를 참조하더라도 
		//    헤더 폰트가 나오도록 변수를 임시로 바꿔치기 합니다.
		m_hFont = (HFONT)m_pHeaderFont->GetSafeHandle();
	}

	// 원본 헤더 그리기 로직 (이제 무조건 우리가 설정한 폰트로 그려집니다)
	CBCGPGridCtrl::OnDrawHeader(pDC);

	// 헤더 그리기가 끝난 후 다른 영역(데이터 등)이 망가지지 않도록 원상 복구
	if (m_pHeaderFont != NULL && m_pHeaderFont->GetSafeHandle() != NULL)
	{
		m_hFont = hOldGridFont; // 원래 폰트로 복구
		if (pOldDCFont != NULL)
		{
			pDC->SelectObject(pOldDCFont);
		}
	}
}

void CustomBCGGridCtrl::SetHeaderTextColor(COLORREF clrText)
{
	m_clrHeaderText = clrText;

	if (GetSafeHwnd() != NULL) RedrawWindow();
}

void CustomBCGGridCtrl::SetHeaderBkColor(COLORREF clrBk)
{
	m_clrHeaderBk = clrBk;

	if (GetSafeHwnd() != NULL) RedrawWindow();
}

// 1. 헤더 텍스트 색상 적용
COLORREF CustomBCGGridCtrl::OnGetHeaderItemTextColor(BOOL bSelected, BOOL bIsGroupBox, int nCol, CBCGPHeaderItem* pHeaderItem)
{
	// 사용자가 글자색을 지정했다면 해당 색상 반환
	if (m_clrHeaderText != CLR_DEFAULT)
	{
		return m_clrHeaderText;
	}

	// 지정하지 않았다면 BCG 기본 로직(테마 색상 등) 사용
	return CBCGPGridCtrl::OnGetHeaderItemTextColor(bSelected, bIsGroupBox, nCol, pHeaderItem);
}
void CustomBCGGridCtrl::RemoveAllData() {
	RemoveAll();
}
// 2. 헤더 배경 색상 적용
void CustomBCGGridCtrl::OnFillHeaderBackground(CDC* pDC, CRect rect)
{
	// 사용자가 배경색을 지정했다면 해당 색상으로 단색 칠하기
	if (m_clrHeaderBk != CLR_DEFAULT)
	{
		CBrush br(m_clrHeaderBk);
		pDC->FillRect(rect, &br);
	}
	else
	{
		// 지정하지 않았다면 BCG 기본 배경 그리기 로직 수행
		CBCGPGridCtrl::OnFillHeaderBackground(pDC, rect);
	}
}

INT_PTR CustomBCGGridCtrl::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	return -1;
}

BOOL CustomBCGGridCtrl::IsItemFilteredByFilterBar(CBCGPGridItem* pItem, int nColumn, const CString& strFilter)
{

	return CBCGPGridCtrl::IsItemFilteredByFilterBar(pItem, nColumn, strFilter);
}

// BCGP가 행 하나하나를 화면에 그릴지 말지 최종 판정하는 단일 진입점(FilterBar 필터,
// 헤더 콤보 필터 등 BCG의 모든 내장 필터 기능도 결국 이 함수를 거쳐감 - CBCGPGridRow::
// IsItemFiltered()가 이 함수를 그대로 호출함). 저희는 EnableFilterBar/EnableFilter 같은
// BCG 내장 필터를 전혀 쓰지 않고, 검색어(m_strSearchFilter)/대상 컬럼(m_nSearchColumn)을
// 직접 비교해서 판정함. 네이티브 FilterBar UI 자체가 생성되지 않으므로 예전에 있었던
// "탭 전환 시 필터바가 다시 보이는" 문제도 원천적으로 발생하지 않음.
BOOL CustomBCGGridCtrl::FilterItem(const CBCGPGridRow* pRow)
{
	if (m_strSearchFilter.IsEmpty() || pRow == nullptr)
		return FALSE; // 검색어 없음: 전부 표시

	if (m_nSearchColumn < 0 || m_nSearchColumn >= pRow->GetItemCount())
		return TRUE; // 검색 대상 컬럼 자체가 없는 행(그룹 헤더 등)은 숨김

	CBCGPGridItem* pItem = pRow->GetItem(m_nSearchColumn);
	if (pItem == nullptr)
		return TRUE;

	_variant_t var = pItem->GetValue();
	if (var.vt == VT_EMPTY || var.vt == VT_NULL)
		return TRUE; // 값 없음: 검색어와 일치할 수 없으므로 숨김

	CString strValue = (LPCTSTR)(_bstr_t)var;
	strValue.MakeLower();

	return strValue.Find(m_strSearchFilter) == -1; // TRUE = 숨김(불일치)
}

// 이름 컬럼(nNameColumn)이 strSearch를 포함하는 다음 행을 찾아 화면에 스크롤 + 선택 이동시킴.
// 연속으로 부르면 마지막으로 찾은 행 다음부터 이어서 찾고(다음 찾기), 끝까지 못 찾으면
// 처음으로 돌아가 한 바퀴 더 검색함.
bool CustomBCGGridCtrl::FindNextRowByName(const CString& strSearch, int nNameColumn)
{
	if (strSearch.IsEmpty())
		return false;

	int nRowCount = GetRowCount();
	if (nRowCount <= 0)
		return false;

	CString strSearchLower = strSearch;
	strSearchLower.MakeLower();

	int nStart = (m_nLastFoundRow >= 0) ? (m_nLastFoundRow + 1) : 0;
	if (nStart >= nRowCount) nStart = 0;

	// pass 0: nStart부터 끝까지, pass 1: 처음부터 nStart 직전까지 (한 바퀴 순환 검색)
	for (int pass = 0; pass < 2; ++pass)
	{
		int nBegin = (pass == 0) ? nStart : 0;
		int nEnd = (pass == 0) ? nRowCount : nStart;

		for (int r = nBegin; r < nEnd; ++r)
		{
			CBCGPGridRow* pRow = GetRow(r);
			if (pRow == nullptr || nNameColumn >= pRow->GetItemCount())
				continue;

			CBCGPGridItem* pItem = pRow->GetItem(nNameColumn);
			if (pItem == nullptr)
				continue;

			_variant_t var = pItem->GetValue();
			if (var.vt == VT_EMPTY || var.vt == VT_NULL)
				continue;

			CString strName = (LPCTSTR)(_bstr_t)var;
			CString strNameLower = strName;
			strNameLower.MakeLower();

			if (strNameLower.Find(strSearchLower) != -1)
			{
				EnsureVisible(pRow, TRUE);
				pRow->Select(TRUE);
				Invalidate();

				m_nLastFoundRow = r;
				return true;
			}
		}

		if (nStart == 0) break; // 처음부터 이미 다 돌았으면 두 번째 순회는 불필요
	}

	// 못 찾음: 다음 검색은 다시 처음부터
	m_nLastFoundRow = -1;
	return false;
}

bool CustomBCGGridCtrl::SearchByName(const CString& strSearchIn, int nNameColumn)
{
	CString strSearch = strSearchIn;
	strSearch.Trim();

	// 레이아웃이 다시 잡히는(AdjustLayout) 동안 중간 상태가 화면에 그려지면 깜빡임이
	// 생길 수 있어서 SetRedraw로 이 구간 전체를 안 그리게 막음.
	SetRedraw(FALSE);

	// FilterItem()이 참조할 검색 상태를 갱신한 뒤 AdjustLayout()으로 전체 행을 다시
	// 평가/재배치시킴(BCG가 필터 조건이 바뀔 때 내부적으로 호출하는 것과 동일한 루틴).
	m_nSearchColumn = nNameColumn;
	m_strSearchFilter = strSearch;
	m_strSearchFilter.MakeLower();
	AdjustLayout();

	bool bFound = true;
	if (strSearch.IsEmpty())
	{
		m_nLastFoundRow = -1;
	}
	else
	{
		bFound = FindNextRowByName(strSearch, nNameColumn);
	}

	SetRedraw(TRUE);
	RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW);

	return bFound;
}
//void CustomBCGGridCtrl::OnFilterBarApply()
//{
//	// 1. 현재 포커스가 있는 윈도우(필터 에디트 칸) 핸들을 미리 저장
//	HWND hWndFocus = ::GetFocus();
//
//	// 2. 부모 클래스의 기본 필터 적용 로직 호출
//	// 이 호출로 인해 IsItemFilteredByFilterBar가 실행되며 그리드가 갱신됩니다.
//	m_bCanFilter = TRUE;
//	CBCGPGridCtrl::OnFilterBarApply();
//	m_bCanFilter = FALSE;
//
//	// 3. [핵심] 포커스 복구
//	// 그리드가 갱신되면서 포커스를 첫 행으로 가져가는 것을 방지합니다.
//	if (hWndFocus != NULL && ::IsWindow(hWndFocus))
//	{
//		::SetFocus(hWndFocus);
//
//		// 커서를 글자 맨 뒤로 보내기
//		int nLen = ::GetWindowTextLength(hWndFocus);
//		::SendMessage(hWndFocus, EM_SETSEL, nLen, nLen);
//	}
//}
