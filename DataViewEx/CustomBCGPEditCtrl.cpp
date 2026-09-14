#include "stdafx.h"
#include "CustomBCGPEditCtrl.h"

IMPLEMENT_DYNAMIC(CustomBCGPEditCtrl, CBCGPEditCtrl)

CustomBCGPEditCtrl::CustomBCGPEditCtrl()
	: m_pPeerEdit(nullptr), m_bIsScrolling(false), m_nDiffTotalRows(0)
{
	// 오너 드로우 스크롤바 스타일 적용
	m_bScrollVertEmptyPage = FALSE;
	m_bScrollBarHorz = FALSE;
}

CustomBCGPEditCtrl::~CustomBCGPEditCtrl()
{
}

BEGIN_MESSAGE_MAP(CustomBCGPEditCtrl, CBCGPEditCtrl)
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_NCPAINT()
	ON_WM_CREATE()
	ON_REGISTERED_MESSAGE(BCGM_ON_DRAW_SCROLLBAR, OnDrawScrollBar)
	//ON_MESSAGE(BCGM_ON_DRAW_SCROLLBAR, OnDrawScrollBar)
	ON_WM_SIZE()
END_MESSAGE_MAP()
// 1. 전체 데이터 등록 및 좌우 구분 설정
//void CustomBCGPEditCtrl::InitDiffData(const std::vector<DiffLine>& diffs, bool isLeft)
//{
//	m_allDiffResults = diffs;
//	m_bIsLeftView = isLeft;
//	int totalRows = static_cast<int>(diffs.size());
//
//	// A. 스크롤바 미니 마커 바인딩
//	std::vector<DiffScrollMarker> scrollMarkers;
//	for (int i = 0; i < totalRows; ++i)
//	{
//		COLORREF col = RGB(240, 240, 240);
//		bool hasMarker = true;
//		switch (diffs[i].type)
//		{
//		case DIFF_CHANGE: col = RGB(255, 230, 150); break;
//		case DIFF_DELETE:
//			if (m_bIsLeftView) col = RGB(255, 180, 180);
//			else col = RGB(240, 240, 240); // 우측은 삭제 영역 마커 없음(또는 빈칸)
//			break;
//		case DIFF_INSERT:
//			if (!m_bIsLeftView) col = RGB(180, 255, 180);
//			else col = RGB(240, 240, 240); // 좌측은 추가 영역 마커 없음(또는 빈칸)
//			break;
//		default: hasMarker = false; break;
//		}
//		if (hasMarker && col != RGB(240, 240, 240)) {
//			scrollMarkers.push_back({ i, col });
//		}
//	}
//	SetDiffScrollMarkers(scrollMarkers, totalRows);
//
//	// B. 최초 화면 렌더링
//	UpdateVisibleLineMarkers();
//}


BOOL CustomBCGPEditCtrl::ShowScrollBar(UINT nBar, BOOL bShow)
{
	if (nBar == SB_HORZ || nBar == SB_BOTH)
	{
		bShow = FALSE;
	}
	CBCGPEditCtrl::ShowScrollBar(nBar, bShow);
	return 0;
}

//
//void CustomBCGPEditCtrl::ResetScrollPosition()
//{
//	bool bOldScrolling = m_bIsScrolling;
//	m_bIsScrolling = true;
//
//	int nFirstLine = GetFirstVisibleLine();
//	if (nFirstLine > 0)
//	{
//		LineScroll(-nFirstLine);
//	}
//
//	m_bIsScrolling = bOldScrolling;
//
//	RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
//}
void CustomBCGPEditCtrl::ResetScrollPosition()
{
	bool bOldScrolling = m_bIsScrolling;
	m_bIsScrolling = true;

	// 1. 스크롤바 강제 최상단/최좌측 이동
	SendMessage(WM_VSCROLL, SB_TOP, 0);
	SendMessage(WM_HSCROLL, SB_LEFT, 0);

	// 2. 캐럿(커서)을 텍스트 맨 앞으로 이동시켜 화면 잘림 방지
	SetSel(0, 0);

	// 3. 내부 스크롤바 크기 및 위치 갱신
	UpdateScrollBars();

	m_bIsScrolling = bOldScrolling;
}
void CustomBCGPEditCtrl::SetDiffScrollMarkers(const std::vector<DiffScrollMarker>& markers, int nTotalRows)
{
	m_diffScrollMarkers = markers;
	m_nDiffTotalRows = nTotalRows;

	// 수직 스크롤바 객체에 OwnerDraw 스타일 직접 전달
	SetScrollBarsStyle(CBCGPScrollBar::BCGP_SBSTYLE_OWNER_DRAW);

	if (m_wndScrollVert.GetSafeHwnd())
	{
		m_wndScrollVert.SetVisualStyle(CBCGPScrollBar::BCGP_SBSTYLE_OWNER_DRAW);
		m_wndScrollVert.RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
	}
}

void CustomBCGPEditCtrl::SetDiffLineBkColors(const std::vector<COLORREF>& colors)
{
	m_lineBkColors = colors;
}

BOOL CustomBCGPEditCtrl::IsColoredLine(int nRow, COLORREF& clrFore, COLORREF& clrBack)
{
	if (nRow >= 0 && nRow < static_cast<int>(m_lineBkColors.size()) &&
		m_lineBkColors[nRow] != (COLORREF)-1)
	{
		clrFore = GetDefaultTextColor();
		clrBack = m_lineBkColors[nRow];
		return TRUE;
	}

	return CBCGPEditCtrl::IsColoredLine(nRow, clrFore, clrBack);
}

void CustomBCGPEditCtrl::DrawColorLine(CDC* pDC, int nRow, CRect rectRow)
{
	if (nRow < 0 || nRow >= static_cast<int>(m_lineBkColors.size()) ||
		m_lineBkColors[nRow] == (COLORREF)-1)
	{
		CBCGPEditCtrl::DrawColorLine(pDC, nRow, rectRow);
		return;
	}

	// 원본 DrawColorLine 의 bFullLine == TRUE 경로와 동일하게 채운다
	CRect rectFill = rectRow;
	if (m_nLastOverhang > 0)
	{
		rectFill.left += m_nLastOverhang;
	}

	pDC->FillSolidRect(rectFill, m_lineBkColors[nRow]);
}

int CustomBCGPEditCtrl::GetDiffVisibleLineCount() const
{
	if (m_nLineHeight <= 0) return 0;

	CRect rcClient;
	const_cast<CustomBCGPEditCtrl*>(this)->GetClientRect(&rcClient);
	return rcClient.Height() / m_nLineHeight;
}

void CustomBCGPEditCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int nOldFirstLine = GetFirstVisibleLine();
	CBCGPEditCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
	int nNewFirstLine = GetFirstVisibleLine();

	int nDelta = nNewFirstLine - nOldFirstLine;

	// 동기화 플래그를 확인하여 상대 에디터도 함께 이동
	if (!m_bIsScrolling && m_pPeerEdit && nDelta != 0)
	{
		m_bIsScrolling = true;
		m_pPeerEdit->m_bIsScrolling = true;

		// LineScroll 내부에서 이미 RedrawWindow 를 하므로 따로 부르지 않는다
		m_pPeerEdit->LineScroll(nDelta);

		m_pPeerEdit->m_bIsScrolling = false;
		m_bIsScrolling = false;
	}

}

BOOL CustomBCGPEditCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	bool bIsCtrlPressed = (nFlags & MK_CONTROL) || ((GetKeyState(VK_CONTROL) & 0x8000) != 0);

	if (bIsCtrlPressed && m_pPeerEdit && !m_bIsScrolling)
	{
		int nLinesToScroll = (zDelta > 0) ? -3 : 3;

		m_bIsScrolling = true;
		m_pPeerEdit->m_bIsScrolling = true;

		// LineScroll 내부에서 이미 RedrawWindow 를 하므로 따로 부르지 않는다
		this->LineScroll(nLinesToScroll);
		m_pPeerEdit->LineScroll(nLinesToScroll);

		m_pPeerEdit->m_bIsScrolling = false;
		m_bIsScrolling = false;


		return TRUE;
	}

	return CBCGPEditCtrl::OnMouseWheel(nFlags, zDelta, pt);
}

void CustomBCGPEditCtrl::MoveToLine(int nTargetLine)
{
	int nCurrentLine = GetFirstVisibleLine();
	int nDelta = nTargetLine - nCurrentLine;

	if (nDelta == 0) return;

	m_bIsScrolling = true;
	if (m_pPeerEdit) m_pPeerEdit->m_bIsScrolling = true;

	// LineScroll 내부에서 이미 RedrawWindow 를 하므로 따로 부르지 않는다
	LineScroll(nDelta);
	if (m_pPeerEdit) m_pPeerEdit->LineScroll(nDelta);

	if (m_pPeerEdit) m_pPeerEdit->m_bIsScrolling = false;
	m_bIsScrolling = false;
}

// CustomBCGPEditCtrl.cpp 또는 뷰(View)에서 스크롤/휠 발생 시 호출
//void CustomBCGPEditCtrl::UpdateVisibleLineMarkers()
//{
//	if (m_allDiffResults.empty()) return;
//
//	int firstVisibleLine = GetFirstVisibleLine();
//	int visibleCount = GetDiffVisibleLineCount();
//	if (visibleCount <= 0) visibleCount = 50;
//
//	int buffer = 30;
//	int startIndex = max(0, firstVisibleLine - buffer);
//	int endIndex = min(static_cast<int>(m_allDiffResults.size()) - 1, firstVisibleLine + visibleCount + buffer);
//
//	COLORREF dwDefText = RGB(0, 0, 0);
//	COLORREF clrChange = RGB(255, 230, 150); // 수정 색상
//	COLORREF clrDelete = RGB(255, 180, 180); // 삭제 색상 (왼쪽 전용)
//	COLORREF clrInsert = RGB(180, 255, 180); // 추가 색상 (오른쪽 전용)
//	COLORREF clrEmpty = RGB(240, 240, 240); // 빈 칸 / 기본 색상
//
//	for (int row = startIndex; row <= endIndex; ++row)
//	{
//		switch (m_allDiffResults[row].type)
//		{
//		case DIFF_CHANGE:
//			// 좌우 모두 수정 영역 표시
//			SetLineColorMarker(row, dwDefText, clrChange, TRUE, 0, g_dwBCGPEdit_LineColorMarker, 0, NULL, FALSE);
//			break;
//
//		case DIFF_DELETE:
//			if (m_bIsLeftView) {
//				// [왼쪽 원본]: 삭제된 영역 표시
//				SetLineColorMarker(row, dwDefText, clrDelete, TRUE, 0, g_dwBCGPEdit_LineColorMarker, 0, NULL, FALSE);
//			}
//			else {
//				// [오른쪽 대상]: 삭제된 영역은 추가된 게 아니므로 빈 칸 처리
//				SetLineColorMarker(row, dwDefText, clrEmpty, TRUE, 0, g_dwBCGPEdit_LineColorMarker, 0, NULL, FALSE);
//			}
//			break;
//
//		case DIFF_INSERT:
//			if (!m_bIsLeftView) {
//				// [오른쪽 대상]: 추가된 영역 표시
//				SetLineColorMarker(row, dwDefText, clrInsert, TRUE, 0, g_dwBCGPEdit_LineColorMarker, 0, NULL, FALSE);
//			}
//			else {
//				// [왼쪽 원본]: 추가된 영역이 아니므로 빈 칸 처리
//				SetLineColorMarker(row, dwDefText, clrEmpty, TRUE, 0, g_dwBCGPEdit_LineColorMarker, 0, NULL, FALSE);
//			}
//			break;
//
//		default:
//			// 일반 동일 라인은 마커 초기화 또는 빈칸
//			SetLineColorMarker(row, dwDefText, clrEmpty, TRUE, 0, g_dwBCGPEdit_LineColorMarker, 0, NULL, FALSE);
//			break;
//		}
//	}
//}
// ============================================================
// 오너 드로우 스크롤바 렌더링 로직 
// ============================================================
LRESULT CustomBCGPEditCtrl::OnDrawScrollBar(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);

	BCGPSCROLLBAR_DRAW_INFO* pInfo = reinterpret_cast<BCGPSCROLLBAR_DRAW_INFO*>(lParam);
	if (pInfo == nullptr || pInfo->m_pDC == nullptr) return 0;

	CDC* pDC = pInfo->m_pDC;
	CRect rect = pInfo->m_rect; // 현재 렌더링할 영역

	if (rect.IsRectEmpty()) return 0;

	// 스크롤바 전체 고정 클라이언트 영역
	CRect rcScrollBar(0, 0, 0, 0);
	if (m_wndScrollVert.GetSafeHwnd())
	{
		m_wndScrollVert.GetClientRect(&rcScrollBar);
	}
	else
	{
		GetClientRect(&rcScrollBar);
	}

	const int nTrackWidth = rcScrollBar.Width();
	if (nTrackWidth <= 0) return 0;

	// ============================================================
	// [핵심 구조 배치]
	// 우측: 6px 폭의 마커 전용 띠 영역
	// 좌측: 타원형 Thumb(손잡이) 영역
	// ============================================================
	const int nMarkerWidth = 6;  // 우측 마커 두께 (6px)
	const int nGap = 2;          // Thumb와 마커 사이 여백

	int xMarkerRight = rcScrollBar.right - 1;
	int xMarkerLeft = xMarkerRight - nMarkerWidth;

	int xThumbLeft = rcScrollBar.left + 2;
	int xThumbRight = xMarkerLeft - nGap;

	if (xThumbRight <= xThumbLeft)
	{
		xThumbRight = xThumbLeft + 4; // 최소 폭 보장
	}

	// ------------------------------------------------------------
	// 우측 마커 띠 전용 렌더링 람다
	// ------------------------------------------------------------
	auto DrawMarkersInRect = [&](const CRect& targetRect)
		{
			if (m_diffScrollMarkers.empty() || m_nDiffTotalRows <= 0) return;

			// 상/하단 화살표 버튼 높이 제외
			int nArrowHeight = rcScrollBar.Width();
			CRect rcTotalTrack = rcScrollBar;
			rcTotalTrack.top += nArrowHeight;
			rcTotalTrack.bottom -= nArrowHeight;

			const int nTrackHeight = rcTotalTrack.Height();
			if (nTrackHeight <= 0) return;

			// 1줄당 차지하는 스크롤바 높이 비율 (정밀 계산)
			double lineRatio = static_cast<double>(nTrackHeight) / static_cast<double>(m_nDiffTotalRows);
			int nMarkerHeight = static_cast<int>(lineRatio);
			if (nMarkerHeight < 2) nMarkerHeight = 2; // 최소 2px 보장

			// 행 수가 트랙 픽셀 수보다 훨씬 많아 마커가 같은 y 에 수천 개씩 겹친다.
			// 픽셀 줄 단위로 색을 먼저 접은 뒤, 같은 색이 연속된 구간만 한 번에 칠한다.
			const COLORREF clrNone = (COLORREF)-1;
			std::vector<COLORREF> bands(nTrackHeight, clrNone);

			for (const auto& marker : m_diffScrollMarkers)
			{
				if (marker.row < 0 || marker.row >= m_nDiffTotalRows) continue;

				int yStart = static_cast<int>(marker.row * lineRatio);
				if (yStart < 0) yStart = 0;

				int yEnd = yStart + nMarkerHeight;
				if (yEnd > nTrackHeight) yEnd = nTrackHeight;

				// 뒤쪽 행이 앞쪽 행을 덮는 기존 그리기 순서를 그대로 유지
				for (int y = yStart; y < yEnd; ++y)
				{
					bands[y] = marker.color;
				}
			}

			for (int y = 0; y < nTrackHeight; )
			{
				if (bands[y] == clrNone)
				{
					++y;
					continue;
				}

				int yRunEnd = y + 1;
				while (yRunEnd < nTrackHeight && bands[yRunEnd] == bands[y])
				{
					++yRunEnd;
				}

				// 우측 xMarkerLeft ~ xMarkerRight 구역에만 정확히 한정
				CRect markerRect(
					xMarkerLeft,
					rcTotalTrack.top + y,
					xMarkerRight,
					rcTotalTrack.top + yRunEnd
				);

				// 현재 BCGP가 갱신 중인 영역에 포함되는 경우에만 출력
				CRect rcIntersect;
				if (rcIntersect.IntersectRect(&markerRect, &targetRect))
				{
					pDC->FillSolidRect(&rcIntersect, bands[y]);
				}

				y = yRunEnd;
			}
		};

	// ------------------------------------------------------------
	// 1. 배경 트랙 그리기 (연한 회색 배경 + 우측 마커 띠)
	// ------------------------------------------------------------
	if (pInfo->m_part == BCGP_SCROLLBAR_BACKGROUND)
	{
		pDC->FillSolidRect(&rect, RGB(245, 245, 245));
		DrawMarkersInRect(rect);
		return 1;
	}

	// ------------------------------------------------------------
	// 2. 스크롤 Thumb (손잡이) 그리기 (좌측 타원 Thumb + 우측 마커 유지)
	// ------------------------------------------------------------
	if (pInfo->m_part == BCGP_SCROLLBAR_THUMB)
	{
		// A. 배경 채우기
		pDC->FillSolidRect(&rect, RGB(245, 245, 245));

		// B. 우측 마커 띠 렌더링 유지
		DrawMarkersInRect(rect);

		// C. 좌측 공간에 독립된 둥근 타원형 Thumb 렌더링
		CRect thumbRect(xThumbLeft, rect.top + 1, xThumbRight, rect.bottom - 1);
		if (thumbRect.Height() > 0 && thumbRect.Width() > 0)
		{
			int nRadius = thumbRect.Width(); // Capsule 모양

			CPen pen(PS_NULL, 0, (COLORREF)0);
			CBrush brush(RGB(160, 160, 160)); // 모던 그레이

			CPen* pOldPen = pDC->SelectObject(&pen);
			CBrush* pOldBrush = pDC->SelectObject(&brush);

			pDC->RoundRect(&thumbRect, CPoint(nRadius, nRadius));

			pDC->SelectObject(pOldPen);
			pDC->SelectObject(pOldBrush);
		}

		return 1;
	}

	// ------------------------------------------------------------
	// 3. 상/하단 화살표 버튼 그리기
	// ------------------------------------------------------------
	if (pInfo->m_part == BCGP_SCROLLBAR_BUTTON)
	{
		pDC->FillSolidRect(&rect, RGB(230, 230, 230));

		UINT nArrowType = (rect.top < 20) ? DFCS_SCROLLUP : DFCS_SCROLLDOWN;
		pDC->DrawFrameControl(&rect, DFC_SCROLL, nArrowType | DFCS_FLAT);

		return 1;
	}

	return 0;
}
void CustomBCGPEditCtrl::OnNcPaint()
{
	CBCGPEditCtrl::OnNcPaint();
}

int CustomBCGPEditCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPEditCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 가로 스크롤바 사용 안 함
	m_bScrollBarHorz = FALSE;

	// 세로 스크롤바만 Owner Draw
	SetScrollBarsStyle(CBCGPScrollBar::BCGP_SBSTYLE_OWNER_DRAW);

	// 한 번 레이아웃 갱신
	UpdateScrollBars();

	return 0;
}

void CustomBCGPEditCtrl::OnSize(UINT nType, int cx, int cy)
{
	CBCGPEditCtrl::OnSize(nType, cx, cy);

	if (GetSafeHwnd())
	{
		// 스크롤바 크기 및 위치 강제 재계산
		UpdateScrollBars();

		// 화면 즉시 갱신
		RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
	}
}