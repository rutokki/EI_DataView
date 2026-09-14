#pragma once
#include "TestLeftView.h"
#include "TestRightView.h"

class TestContainer : public CBCGPWnd
{
	DECLARE_DYNAMIC(TestContainer)

public:
	TestContainer();
	virtual ~TestContainer();

	CBCGPSplitterWnd m_wndSplitter;
	TestLeftView* testLeftView;
	TestRightView* testRightView;

	TestLeftView* GetLeftGridPane() {
		return testLeftView;
	}
	TestRightView* GetRightGridPane() {
		return testRightView;
	}
	BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	void LoadRouteData();
	void ChangeRouteSelection(WORD nRteNo);
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//afx_msg void OnFileSave();


	DECLARE_MESSAGE_MAP()
};