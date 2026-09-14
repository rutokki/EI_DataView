#pragma once
#include "CustomBCGGridCtrl.h"
class OUTCardGridInfo : public CustomBCGGridCtrl

{
public:
	OUTCardGridInfo();
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	virtual CBCGPGridColumnsInfo& GetColumnsInfo() override {
		return m_Column;
	}
	virtual const CBCGPGridColumnsInfo& GetColumnsInfo() const override {
		return m_Column;
	}

	//CRect OnGetHeaderRect(CDC* pDC, const CRect& rectDraw) override;
	//void OnDrawHeader(CDC* pDC) override;
	//void OnPrintHeader(CDC* pDC, CPrintInfo* pInfo) override;
	//void OnPosSizeChanged() override;

public:
	void LoadAllOutCardData();
	//void GetDetailedKindName();
public:
	void UpdateOutCardData();
	CString GetOutKindName(const OUT_CARDTABLE::_CardData_t& portItem);
	//CString GetSpeedCodeString(OUT_CARDTABLE& a);
};


