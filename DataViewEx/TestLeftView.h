#pragma once
class TestLeftView : public CView
{
	DECLARE_DYNCREATE(TestLeftView)

public:
	TestLeftView();
	virtual ~TestLeftView();

	CBCGPListBox m_wndListBox;
	CBCGPComboBox m_wndComboBox;
	virtual void OnDraw(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	void LoadRouteList();

	void SelectRouteByIndex(int nSel);

	afx_msg void OnSelChangeListBox();
	afx_msg void OnSelChangeComboBox();     // 새로 추가할 콤보박스 선택 핸들러
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnFileSave();
	afx_msg void OnFileOpen();
	afx_msg void OnFileDiff();
	afx_msg void OnFilePrint();
	afx_msg void OnUpdatePrint(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileDiff(CCmdUI* pCmdUI);
	//BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	DECLARE_MESSAGE_MAP()

};