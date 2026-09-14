#pragma once

class TestRightPane : public CView
{
	DECLARE_DYNCREATE(TestRightPane)

public:
	TestRightPane();
	virtual ~TestRightPane();

	CStatic   m_staticMutual;       // 상호쇄정
	CBCGPEdit m_editMutual;

	CStatic   m_staticSwitch;       // 선로전환기쇄정
	CBCGPEdit m_editSwitch;

	CStatic   m_staticSignal;       // 신호제어조건
	CBCGPEdit m_editSignal;

	CStatic   m_staticRoute;        // 진로구분쇄정
	CBCGPEdit m_editRoute;

	CStatic   m_staticApproach;     // 접근쇄정
	CBCGPEdit m_editApproach;

	void UpdateDetailData(const CString& strItemName);

public:
	virtual void OnDraw(CDC* pDC);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
};