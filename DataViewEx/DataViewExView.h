// DataViewExView.h : interface of the CDataViewExView class
//


#include "CustomBCGGridCtrl.h"
#include "CustomBCGTabWnd.h"
#include "TrackGridInfo.h"
#include "SignalGridInfo.h"
#include "SwitchGridInfo.h"
#include "DeviceGridInfo.h"
#include "INCardGridInfo.h"
#include "OUTCardGridInfo.h"
#include "LogicVariableGridInfo.h"
#include "InterlockGridInfo.h"
#include "SignalCardGridInfo.h"
#include "SwitchCardGridInfo.h"
#include "DiffCompareFrame.h"
#include "TestContainer.h"
#include "MainFrm.h"
#include "GridPrintDlg.h"



#pragma once


class CDataViewExView : public CBCGPGridView
{
protected: // create from serialization only
	CDataViewExView();
	DECLARE_DYNCREATE(CDataViewExView)

	// Attributes
public:
	CDataViewExDoc* GetDocument() const;

	// Operations
public:

	// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	CBCGPGridCtrl* GetActiveGrid();
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);

	// Implementation
public:
	virtual ~CDataViewExView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CMainFrame* pFrame;
	CBCGPTabWnd m_wndTabCtrl;
	TrackGridInfo m_wndTrackInfoGrid;
	SignalGridInfo m_wndSignalInfoGrid;
	SwitchGridInfo m_wndSwitchInfoGrid;
	DeviceGridInfo m_wndDeviceInfoGrid;
	INCardGridInfo m_wndInCardInfoGrid;
	OUTCardGridInfo m_wndOutCardInfoGrid;
	CustomBCGGridCtrl m_wndPDOCardInfoGrid;
	LogicVariableGridInfo m_wndLogicVariableGrid;
	InterlockGridInfo m_wndInterLockGird;
	SignalCardGridInfo m_wndSignalCardInfoGrid;
	SwitchCardGridInfo m_wndSwitchCardInfoGrid;
	TestContainer m_wndTestContainer;

	// 그리드 이름 검색("찾기") 관련. 현재 활성 탭의 그리드에서 실제 이름 컬럼을 대상으로
	// 부분일치(대소문자 무시) 필터링 + 해당 행으로 스크롤/선택 이동. 실제 검색/필터 동작은
	// CustomBCGGridCtrl::SearchByName()에 구현돼 있고, 여기서는 대상 그리드/컬럼만 결정해서 위임함.
	CEdit m_wndSearchEdit;
	CBCGPButton m_wndSearchNextBtn;
	EGridType TabIndexToGridType(int nActiveTab);
	//DBstruct m_TSCDBStruct; // 연동.bin 파일의 데이터를 저장하는 구조체 (속도코드 제어장치로 수정)
	//CARD_INFO m_TSCCardInfo; // IOCard.bin 파일의 데이터를 저장하는 구조체
	std::map<int, LOGIC_VARIABLE> m_TSCLogicVariableMap; // LogicVariable.Dat 파일의 데이터를 저장하는 맵
	CBCGPEditCtrl m_wndEditLDat;
	// Generated message map functions
protected:
	void DeleteAttributeTab();
	//CString GetFileStationName(CString& fileName);
	void updateLayOut();
protected:
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnNcPaint();
	afx_msg LRESULT OnContainerCommand(WPARAM wParam, LPARAM lParam);
	afx_msg void OnFilePrintPreview();
	afx_msg LRESULT OnPrintClient(WPARAM wp, LPARAM lp);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDataOpen();
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void SetupGrids();
	void UpdateGridsData();
	//void UpdateLDatEditData();
	DiffCompareFrame* m_pDiffFrame; // diff dlg 

	CBCGPGridColumnsInfoEx info;
	//afx_msg void OnPrintTag();
	//void OnPrintTagNameLabel();
	afx_msg void OnTagIncard();
	afx_msg void OnTagOutcard();
	afx_msg void OnDiffTool();
	void OnExportCSV();
	afx_msg void OnFileSave();
	void OnPrintGridTable();
	bool ExtractGridData(CBCGPGridCtrl* pGrid, const std::vector<CString>& headers, std::vector<std::vector<CString>>& rows);
	// [TEMP-DEBUG] GridPrintDlg UI 확인용 더미 데이터 생성. 확인 끝나면 선언/구현/호출부 모두 삭제하세요.
	void FillDummyRowsForPrintPreview(const std::vector<ColumnInfo>& columnInfos, std::vector<std::vector<CString>>& rows);
	afx_msg void OnAppDiff();
	//	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnSearchNext();
};

#ifndef _DEBUG  // debug version in DataViewExView.cpp
inline CDataViewExDoc* CDataViewExView::GetDocument() const
{
	return reinterpret_cast<CDataViewExDoc*>(m_pDocument);
}
#endif

