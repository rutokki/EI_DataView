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
		m_moduleLayouts.clear();
	}

	// [추가] 신호기 / 선로전환기 모듈 표찰 : 머리표 없이 이름 칸만 인쇄
	//  모듈 전면판 오른쪽 표찰 창(세로 띠)에 끼우는 용도라, 띠 길이 안에서 각 이름을
	//  전면판 명칭(신호기1, PM1 ...) 옆 위치에 맞춰 인쇄한다.
	//  - stripLenMm : 표찰 띠 전체 길이 (mm)
	//  - offsetsMm  : 띠 위쪽 끝에서 각 이름 칸 위쪽까지 거리 (mm), 개수 = 이름 칸 수
	//  tagName 은 모듈 순서대로 offsetsMm 개수씩 이어서 채워져 있어야 함
	struct ModuleLabelLayout
	{
		double stripLenMm = 0.0;
		std::vector<double> offsetsMm;
	};
	std::vector<ModuleLabelLayout> m_moduleLayouts;
	void SetModuleCards(const std::vector<ModuleLabelLayout>& layouts, LPCTSTR pszListTitle)
	{
		m_moduleLayouts = layouts;
		m_strCardLabel.Empty();
		m_strListTitle = pszListTitle;
	}
	bool IsNameOnly() const { return !m_moduleLayouts.empty(); }
	double GetStripLenMm(int nCard) const
	{
		return (nCard >= 0 && nCard < (int)m_moduleLayouts.size()) ? m_moduleLayouts[nCard].stripLenMm : 0.0;
	}
	double GetLabelOffsetMm(int nCard, int nCell) const
	{
		if (nCard < 0 || nCard >= (int)m_moduleLayouts.size()) return 0.0;
		const auto& offsets = m_moduleLayouts[nCard].offsetsMm;
		return (nCell >= 0 && nCell < (int)offsets.size()) ? offsets[nCell] : 0.0;
	}
	int GetCardCount() const;
	int GetCardStart(int nCard) const;
	int GetCardCells(int nCard) const;

	afx_msg void OnBnClickedCellChangeButton();
	afx_msg void OnBnClickedBtnPrevPage();
	afx_msg void OnBnClickedBtnNextPage();
};
