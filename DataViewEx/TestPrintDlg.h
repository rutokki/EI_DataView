#pragma once
// TestPrintDlg 대화 상자

class TestPrintDlg : public CBCGPDialog
{
	DECLARE_DYNAMIC(TestPrintDlg)

public:
	TestPrintDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~TestPrintDlg();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum {
		IDD = IDD_TEST_PRINT_DLG
	};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
private:
private:
	// 총 페이지 수 계산 (선로전환기 페이지 수도 함께 반환하도록 레퍼런스 인자 추가)
	int GetTotalPageCount(std::vector<size_t>& validIndices, int& maxDisplayRows, int rowHeight, const CRect& rectA4, int startY, int& outSwitchPages, int& outIndicatorPages);

	// 공통 영역 (기존과 동일)
	int DrawPageHeader(CDC* pDC, const CRect& rectA4, double scaleX, double scaleY, CFont* pTitleFont, CFont* pHeaderFont, const CString& strDocTitle, int nCurPage, int nTotalPages);
	void DrawPageFooter(CDC* pDC, const CRect& rectA4, double scaleX, double scaleY, CFont* pHeaderFont, int nCurPage, int nTotalPages);

	// 개별 페이지 그리기
	void DrawIndicatorTestPage(CDC* pDC, const CRect& rectA4, double scaleX, double scaleY, int startX, int startY, int totalWidth, CFont* pHeaderFont, CFont* pSmallFont, int nIndicatorSubPage); // 1페이지 전용
	void DrawSwitchTestPage(CDC* pDC, const CRect& rectA4, double scaleX, double scaleY, int startX, int startY, int totalWidth, CFont* pHeaderFont, CFont* pSmallFont, int nSwitchSubPage); // 2페이지부터 (페이징 지원)
	void DrawChecklistPage(CDC* pDC, const CRect& rectA4, double scaleX, double scaleY, int startX, int startY, int totalWidth, CFont* pHeaderFont, CFont* pSmallFont, const std::vector<size_t>& validIndices, int maxDisplayRows, int nChecklistSubPage); // 그 이후 페이지
public:
	int m_nCurPage; // 현재 미리보기 페이지
	CString m_strStationName;
	CBCGPButton printBtn;

	CString GetSignalLock(WORD nRteNo);
	CString GetSwitchLock(WORD nRteNo, bool bIsAddTotSig);
	CString GetTrackLockAll(WORD nRteNo, UCHAR ShuntCommon);
	CString GetTrackLockRoute(WORD nRteNo);
	CString GetAppLock(WORD nRteNo);
	void SetStationName(CString strStationName);

	afx_msg void OnBnClickedTestPrintBtn();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedTestLeftBtn();
	afx_msg void OnBnClickedTestRightButton();
};
