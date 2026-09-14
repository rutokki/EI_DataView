#pragma once
#include "CustomBCGGridCtrl.h"
class INCardGridInfo : public CBCGPGridCtrl

{
public:
	INCardGridInfo();
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
