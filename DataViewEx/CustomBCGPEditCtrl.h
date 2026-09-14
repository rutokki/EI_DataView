#pragma once
#include <vector>
#include "DiffLine.h"
// DiffCompareFrame.h에 정의된 enum DiffType 선언 참조를 위해
// DiffType 전방 선언 또는 필요한 구조체 정의
enum DiffType;

// ============================================================
// 스크롤바 Diff 마커 구조체
// ============================================================
struct DiffScrollMarker
{
	int row;          // 행 번호 (0-based)
	COLORREF color;   // 마커 색상

	// 1. 기본 생성자
	DiffScrollMarker() : row(0), color(RGB(0, 0, 0)) {
	}

	// 2. 행번호와 색상 직접 지정 생성자
	DiffScrollMarker(int nRow, COLORREF clr)
		: row(nRow), color(clr) {
	}
};

// ============================================================
// CustomBCGPEditCtrl 클래스
// ============================================================
class CustomBCGPEditCtrl : public CBCGPEditCtrl
{
	DECLARE_DYNAMIC(CustomBCGPEditCtrl)

public:
	CustomBCGPEditCtrl();
	virtual ~CustomBCGPEditCtrl();

	// 양방향 동기화를 위한 상대 에디터 포인터
	CustomBCGPEditCtrl* m_pPeerEdit;
	bool m_bIsScrolling;
	//void InitDiffData(const std::vector<DiffLine>& diffsm, bool isLeft);
	//void UpdateVisibleLineMarkers();
	virtual BOOL ShowScrollBar(UINT nBar, BOOL bShow = TRUE);
	std::vector<DiffLine> m_allDiffResults;
	bool m_bIsLeftView = true; // 좌우 구분 플래그
	// 스크롤 및 마커 제어
	void ResetScrollPosition();
	void SetDiffScrollMarkers(const std::vector<DiffScrollMarker>& markers, int nTotalRows);
	int  GetDiffVisibleLineCount() const;

	void MoveToLine(int nTargetLine);

	// 행 배경색 테이블 설정 (인덱스 = 행 번호, (COLORREF)-1 = 색상 없음)
	void SetDiffLineBkColors(const std::vector<COLORREF>& colors);
	void SetDiffTrailingBkColor(COLORREF clr) {
		m_clrDiffTrailing = clr;
	}
protected:
	std::vector<DiffScrollMarker> m_diffScrollMarkers;
	std::vector<COLORREF> m_lineBkColors;
	int m_nDiffTotalRows;

	// 줄 색상 Marker 대신 m_lineBkColors 를 O(1) 로 참조
	virtual BOOL IsColoredLine(int nRow, COLORREF& clrFore, COLORREF& clrBack);
	virtual void DrawColorLine(CDC* pDC, int nRow, CRect rectRow);

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	afx_msg void OnNcPaint();
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnDrawScrollBar(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
private:
	COLORREF m_clrDiffTrailing = (COLORREF)-1; // Diff 영역 이후 배경색
};