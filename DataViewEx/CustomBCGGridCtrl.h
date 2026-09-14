#pragma once
class CustomBCGGridCtrl : public CBCGPGridCtrl
{
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

protected:
	CFont* m_pHeaderFont;       // 헤더에 적용할 폰트 포인터
	int    m_nHeaderHeight;     // 커스텀 헤더 높이
	BOOL   m_bIsDrawingHeader;  // 현재 헤더를 그리고 있는지 여부 플래그
	BOOL m_bCanFilter;
	// 색상 저장을 위한 멤버 변수 추가
	COLORREF m_clrHeaderText; // 헤더 글자색
	COLORREF m_clrHeaderBk;   // 헤더 배경색
	CBCGPGridColumnsInfoEx m_Column;

	// 가상 함수 재정의
	virtual CRect OnGetHeaderRect(CDC* pDC, const CRect& rectDraw) override;
	virtual void OnPosSizeChanged() override;
	// 헤더를 그리는 함수만 재정의합니다.
	virtual void OnDrawHeader(CDC* pDC) override;
	// 색상 적용을 위한 가상 함수 재정의
	virtual COLORREF OnGetHeaderItemTextColor(BOOL bSelected, BOOL bIsGroupBox, int nCol, CBCGPHeaderItem* pHeaderItem = NULL) override;
	virtual void OnFillHeaderBackground(CDC* pDC, CRect rect) override;
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const; //툴팁




};



