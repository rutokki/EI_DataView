#pragma once
#include "CustomBCGGridCtrl.h"
class SwitchCardGridInfo : public CustomBCGGridCtrl
{
public:
	SwitchCardGridInfo();
	virtual CBCGPGridColumnsInfo& GetColumnsInfo() override {
		return m_Column;
	}
	virtual const CBCGPGridColumnsInfo& GetColumnsInfo() const override {
		return m_Column;
	}
public:
	void LoadAllSwitchCardData();
	void UpdateSwitchCardData();
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

