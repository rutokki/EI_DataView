#pragma once
class CustomBCGGridCtrl : public CBCGPGridCtrl
{
	DECLARE_DYNAMIC(CustomBCGGridCtrl) // DYNAMIC_DOWNCAST(CustomBCGGridCtrl, ...)로 IN Card 탭(CBCGPGridCtrl 직접 상속) 구분에 사용
public:
	CustomBCGGridCtrl();
	~CustomBCGGridCtrl();

	void InitGridControl();

	// =========================================================
	// 💡 개발자님이 찾아내신 함수를 가로채서 무조건 0을 반환하게 만듭니다.
	// (테마 엔진이 왼쪽 여백을 계산할 때 0픽셀을 주도록 속임)
	// =========================================================
	virtual int GetExtraHierarchyOffset() const
	{
		return 0; // 강제로 여백 소멸!
	}
	virtual void Sort(int nColumn, BOOL bAscending = TRUE, BOOL bAdd = FALSE) override {
		return; // 아무것도 안 함
	}
	//virtual void OnFilterBarApply();


	void SetHeaderFont(CFont* pFont);
	void SetHeaderHeight(int nHeight);

	int GetHeaderHeight();

	// 색상 변경을 위한 Setter 함수 추가
	void SetHeaderTextColor(COLORREF clrText);
	void SetHeaderBkColor(COLORREF clrBk);

	BOOL IsItemFilteredByFilterBar(CBCGPGridItem* pItem, int nColumn, const CString& strFilter);

	void RemoveAllData();

	// 이름검색/필터: nNameColumn 컬럼이 strSearch를 포함하는 행만 남기고 나머지는 숨김,
	// 그 중 첫(또는 다음) 일치 행으로 스크롤+선택 이동시킴. BCG의 내장 FilterBar/EnableFilter는
	// 전혀 쓰지 않고 FilterItem()을 직접 오버라이드해서 필터링 자체를 손으로 구현함 — 네이티브
	// FilterBar UI가 아예 생성되지 않으므로 "탭 전환 시 필터바 재노출" 같은 문제가 원천적으로 없음.
	// 같은 그리드에 연속으로 부르면 마지막으로 찾은 행 다음부터 이어서 찾음(다음 찾기).
	// strSearch가 비어있으면 필터만 해제하고 true를 반환. 반환값은 일치 행을 찾았는지 여부.
	bool SearchByName(const CString& strSearch, int nNameColumn);

protected:
	CFont* m_pHeaderFont;       // 헤더에 적용할 폰트 포인터
	int    m_nHeaderHeight;     // 커스텀 헤더 높이
	BOOL   m_bIsDrawingHeader;  // 현재 헤더를 그리고 있는지 여부 플래그
	BOOL m_bCanFilter;
	// 색상 저장을 위한 멤버 변수 추가
	COLORREF m_clrHeaderText; // 헤더 글자색
	COLORREF m_clrHeaderBk;   // 헤더 배경색
	CBCGPGridColumnsInfoEx m_Column;

	// SearchByName 관련 상태 (BCG FilterBar를 쓰지 않고 FilterItem()에서 직접 참조)
	CString m_strSearchFilter;     // 소문자로 변환된 현재 검색어. 비어있으면 필터 없음(전부 표시)
	int     m_nSearchColumn = -1;  // 검색 대상 컬럼 인덱스
	int     m_nLastFoundRow = -1;  // "다음 찾기" 이어하기용

	bool FindNextRowByName(const CString& strSearch, int nNameColumn);

	// 가상 함수 재정의
	virtual CRect OnGetHeaderRect(CDC* pDC, const CRect& rectDraw) override;
	virtual void OnPosSizeChanged() override;
	// 헤더를 그리는 함수만 재정의합니다.
	virtual void OnDrawHeader(CDC* pDC) override;
	// 색상 적용을 위한 가상 함수 재정의
	virtual COLORREF OnGetHeaderItemTextColor(BOOL bSelected, BOOL bIsGroupBox, int nCol, CBCGPHeaderItem* pHeaderItem = NULL) override;
	virtual void OnFillHeaderBackground(CDC* pDC, CRect rect) override;
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const; //툴팁

	// BCGP가 행 하나하나에 대해 "이 행을 화면에서 숨길지"를 물어보는 지점(모든 필터 방식이
	// 최종적으로 거쳐가는 단일 진입점). 여기서 m_strSearchFilter/m_nSearchColumn을 직접
	// 비교해서 판정함 — EnableFilterBar/EnableFilter 등 BCG 내장 필터 기능은 사용하지 않음.
	virtual BOOL FilterItem(const CBCGPGridRow* pRow) override;
};



