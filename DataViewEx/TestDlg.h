#pragma once
#include "afxdialogex.h"
#include "TestLeftPane.h"
#include "TestRightPane.h"
// TestDlg 대화 상자

class TestDlg : public CBCGPDialog
{
	DECLARE_DYNAMIC(TestDlg)

public:
	TestDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~TestDlg();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum {
		IDD = IDD_TEST_DLG
	};
#endif
public:
	CBCGPSplitterWnd m_wndSplitter;
	TestLeftPane m_wndLeftPane;
	TestRightPane m_wndRightPane;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
