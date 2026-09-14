#pragma once

class TestRightView : public CView
{
	DECLARE_DYNCREATE(TestRightView)

public:
	TestRightView();
	virtual ~TestRightView();

	// 상호쇄정, 선로전환기쇄정, 신호제어조건, 진로구분쇄정, 접근쇄정 용 컨트롤 배열 또는 개별 변수
	// BCGP 기반 UI 컴포넌트 정의
	CBCGPGroup   m_grpBox;       // 우측 상세 영역 그룹 박스

	// 1. 상호쇄정
	CBCGPStatic  m_lblInterlock;
	CBCGPEdit    m_edtInterlock;

	// 2. 선로전환기쇄정
	CBCGPStatic  m_lblSwitch;
	CBCGPEdit    m_edtSwitch;

	// 3. 신호제어조건
	CBCGPStatic  m_lblSigCond;
	CBCGPEdit    m_edtSigCond;

	// 4. 진로구분쇄정
	CBCGPStatic  m_lblRouteDiv;
	CBCGPEdit    m_edtRouteDiv;

	// 5. 접근쇄정
	CBCGPStatic  m_lblAppLock;
	CBCGPEdit    m_edtAppLock;
	// ... 필요한 만큼 추가

	virtual void OnDraw(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	void UpdateRouteData(WORD nRteNo);
	CString GetSignalLock(WORD nRteNo, bool bIsPrtData);
	CString GetSwitchLock(WORD nRteNo, bool bIsAddTotSig, bool bIsPrtData);
	CString GetTrackLockAll(WORD nRteNo, UCHAR ShuntCommon, bool bIsPrtData);
	CString GetTrackLockRoute(WORD nRteNo, bool bIsPrtData);
	CString GetAppLock(WORD nRteNo, bool bIsPrtData);
	//afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnFileSave();
	afx_msg void OnFileDiff();
	afx_msg void OnFilePrint();
	afx_msg void OnFileOpen();
	afx_msg void OnUpdatePrint(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileDiff(CCmdUI* pCmdUI);
	//BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	//afx_msg LRESULT OnChangeVisualManager(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

};