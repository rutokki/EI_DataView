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

	// [추가] 카드 종류별 표찰 설정 (기본값 : IN 카드 16포트)
	//  - m_nPortsPerCard : 카드 1장당 포트(표찰) 수. tagName 은 카드 순서대로 이 개수씩 채워져 있어야 함
	//  - m_strCardLabel  : 카드 머리표 접두어 (예: "In" -> "In 1", "Sig" -> "Sig 1")
	//  - m_strListTitle  : 제목 (예: "IN Card List")
	int m_nPortsPerCard;
	CString m_strCardLabel;
	CString m_strListTitle;
	void SetCardKind(int nPortsPerCard, LPCTSTR pszCardLabel, LPCTSTR pszListTitle)
	{
		m_nPortsPerCard = (nPortsPerCard > 0) ? nPortsPerCard : 16;
		m_strCardLabel = pszCardLabel;
		m_strListTitle = pszListTitle;
	}

	afx_msg void OnBnClickedCellChangeButton();
	afx_msg void OnBnClickedBtnPrevPage();
	afx_msg void OnBnClickedBtnNextPage();
};
