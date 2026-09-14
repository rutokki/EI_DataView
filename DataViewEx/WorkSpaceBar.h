// workspacebar.h : interface of the CWorkSpaceBar class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CWorkSpaceBar : public CBCGPDockingControlBar
{
public:
	CWorkSpaceBar();

	// Attributes
protected:
	CBCGPTreeCtrl m_wndTree;
	HTREEITEM m_hRootNode;
	HTREEITEM m_hBinDiffNode;
	HTREEITEM m_hLogicDiffNode;
	HTREEITEM m_hLDATNode;
	bool m_bSuppressSelChanged = false;
	// Operations
public:

	// Overrides

	// Implementation
public:
	virtual ~CWorkSpaceBar();
public:
	bool SetTreeViewData();
	void ClearTreeViewData();
	void DeleteAllItems();
	// Generated message map functions
protected:
	afx_msg void OnSelChangedTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
public:
	//	virtual HRESULT accDoDefaultAction(VARIANT varChild);
};

/////////////////////////////////////////////////////////////////////////////
