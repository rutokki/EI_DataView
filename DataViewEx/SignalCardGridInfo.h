#pragma once
#include "CustomBCGGridCtrl.h"
class SignalCardGridInfo : public CustomBCGGridCtrl
{
public:
	SignalCardGridInfo();
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
	void LoadAllSignalCardData();
	//void GetDetailedKindName();
public:
	void UpdateSignalCardData();
	//CString GetSpeedCodeString(SIGNAL_CARDTABLE& a);BEGIN_MESSAGE_MAP(SignalCardGridInfo, CustomBCGGridCtrl)

};