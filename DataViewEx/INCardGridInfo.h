#pragma once
#include "CustomBCGGridCtrl.h"
class INCardGridInfo : public CBCGPGridCtrl

{
public:
	INCardGridInfo();
	// CustomBCGGridCtrl 계열 그리드들과 동일하게 정렬 기능 자체를 차단(헤더 클릭 정렬 방지는
	// 생성자의 EnableHeader(TRUE, 0)이 담당, 이건 프로그램적으로 Sort()가 호출되는 경우까지 방어).
	virtual void Sort(int nColumn, BOOL bAscending = TRUE, BOOL bAdd = FALSE) override {
		return;
	}
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	//virtual CBCGPGridColumnsInfo& GetColumnsInfo() override {
	//	return m_Column;
	//}
	//virtual const CBCGPGridColumnsInfo& GetColumnsInfo() const override {
	//	return m_Column;
	//}
public:
	void UpdateInCardData();
	//CRect OnGetHeaderRect(CDC* pDC, const CRect& rectDraw) override;
	//void OnDrawHeader(CDC* pDC) override;
	//void OnPrintHeader(CDC* pDC, CPrintInfo* pInfo) override;
	//void OnPosSizeChanged() override;

public:
	void LoadAllInCardData();
	//afx_msg LRESULT OnChangeVisualManager(WPARAM wParam, LPARAM lParam);
};
