#pragma once
#include "CustomBCGGridCtrl.h"

#define MAIN_SIGNAL_TYPE 14

struct SGBitCheckInfo {
	UCHAR* pValue; // 변수의 주소
	UINT   bit;    // 체크할 비트 상수
};

class SignalGridInfo : public CustomBCGGridCtrl
{
public:
	SignalGridInfo();
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	virtual CBCGPGridColumnsInfo& GetColumnsInfo() override {
		return m_Column;
	}
	virtual const CBCGPGridColumnsInfo& GetColumnsInfo() const override {
		return m_Column;
	}
	void LoadAllSignal();
	void SetSignalType(CBCGPGridRow* pRow, SignalInfoType& sigItem);
	CRect OnGetHeaderRect(CDC* pDC, const CRect& rectDraw) override;
	void OnDrawHeader(CDC* pDC) override;
	void OnPrintHeader(CDC* pDC, CPrintInfo* pInfo) override;
	void OnPosSizeChanged() override;
public:
	void UpdateSignalData();

};

