#pragma once

// TagNamePrintDlg 대화 상자

class TagNamePrintDlg : public CBCGPDialog
{
	DECLARE_DYNAMIC(TagNamePrintDlg)

public:
	TagNamePrintDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~TagNamePrintDlg();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum {
		IDD = IDD_TAGNAME_DLG
	};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	double m_cellWidthMm;
	double m_cellHeightMm;
	double m_cellSpacingYMm;
	int m_nCurrentPage;
	DECLARE_MESSAGE_MAP()
public:
	std::vector<CString> tagName;
	afx_msg void OnPaint();
	afx_msg void OnBnClickedBtnPrint();
	virtual BOOL OnInitDialog();
	bool m_isOutCard;

	afx_msg void OnBnClickedCellChangeButton();
	afx_msg void OnBnClickedBtnPrevPage();
	afx_msg void OnBnClickedBtnNextPage();
};
