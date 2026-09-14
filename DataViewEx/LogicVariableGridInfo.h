#pragma once
#include "CustomBCGGridCtrl.h"
class LogicVariableGridInfo : public CustomBCGGridCtrl
{
public:
	LogicVariableGridInfo();

	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void LoadAllLogicData();
	CString GetszName(std::shared_ptr<LOGIC_VARIABLE> item);
public:
	void UpdateLogicData();
};

