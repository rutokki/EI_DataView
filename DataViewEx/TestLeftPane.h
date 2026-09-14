#pragma once

class TestLeftPane : public CView
{
	DECLARE_DYNCREATE(TestLeftPane)

public:
	TestLeftPane();
	virtual ~TestLeftPane();

	CBCGPListBox m_wndListBox;

public:
	virtual void OnDraw(CDC* pDC);
	void LoadRouteData();
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSelChangeListBox();

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
};