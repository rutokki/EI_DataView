#pragma once

#include <memory>
#include <vector>

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
		m_kindLayouts.clear();
		m_moduleKinds.clear();
	}

	// [추가] 신호기 / 선로전환기 모듈 표찰 : 머리표 없이 이름 칸만 인쇄
	//  모듈 전면판 오른쪽 표찰 창(세로 띠)에 끼우는 용도라, 띠 길이 안에서 각 이름을
	//  전면판 명칭(신호기1, PM1 ...) 옆 위치에 맞춰 인쇄한다.
	//  모듈 종류(CardKind)별 배치는 화면에서 사용자가 조절 가능 (레지스트리에 저장)
	//  - stripLenMm : 표찰 띠 전체 길이 (mm)
	//  - topMm      : 띠 위쪽 끝에서 첫 이름 칸 위쪽까지 거리 (mm)
	//  - pitchMm    : 이름 칸 사이 간격 (칸 위쪽 ~ 다음 칸 위쪽, mm)
	//  - nCells     : 이름 칸 수
	//  tagName 은 모듈 순서대로 해당 종류의 nCells 개수씩 이어서 채워져 있어야 함
	struct ModuleKindLayout
	{
		CString key;           // 레지스트리 저장 키 (예: "Kind3")
		CString name;          // 화면 표시 이름 (예: "4등형")
		double stripLenMm = 95.0;
		double topMm = 0.5;
		double pitchMm = 8.0;
		int nCells = 1;
	};
	std::vector<ModuleKindLayout> m_kindLayouts; // 모듈 종류별 배치
	std::vector<int> m_moduleKinds;              // 모듈별 m_kindLayouts 인덱스
	void SetModuleCards(const std::vector<ModuleKindLayout>& kindLayouts, const std::vector<int>& moduleKinds, LPCTSTR pszListTitle)
	{
		m_kindLayouts = kindLayouts;
		m_moduleKinds = moduleKinds;
		m_strCardLabel.Empty();
		m_strListTitle = pszListTitle;
	}
	bool IsNameOnly() const { return !m_moduleKinds.empty(); }
	const ModuleKindLayout* GetModuleLayout(int nCard) const
	{
		if (nCard < 0 || nCard >= (int)m_moduleKinds.size()) return nullptr;
		int nKind = m_moduleKinds[nCard];
		return (nKind >= 0 && nKind < (int)m_kindLayouts.size()) ? &m_kindLayouts[nKind] : nullptr;
	}
	double GetStripLenMm(int nCard) const
	{
		const auto* pLayout = GetModuleLayout(nCard);
		return (pLayout != nullptr) ? pLayout->stripLenMm : 0.0;
	}
	double GetLabelOffsetMm(int nCard, int nCell) const
	{
		const auto* pLayout = GetModuleLayout(nCard);
		return (pLayout != nullptr) ? pLayout->topMm + nCell * pLayout->pitchMm : 0.0;
	}
	int GetCardCount() const;
	int GetCardStart(int nCard) const;
	int GetCardCells(int nCard) const;

	// [추가] 페이지당 카드 수 계산 (미리보기 / 인쇄 / 페이지 이동 공통)
	static constexpr double TAG_USABLE_WIDTH_MM = 265.0;  // A4 가로 297mm - 좌우 여백
	static constexpr double TAG_USABLE_HEIGHT_MM = 175.0; // A4 세로 210mm - 제목 / 상하 여백
	static constexpr double TAG_COL_GAP_MM = 0.5;         // 카드 사이 가로 간격
	static constexpr double TAG_ROW_GAP_MM = 3.0;         // 카드 줄 사이 세로 간격
	static constexpr int TAG_MAX_CARDS_PER_PAGE = 20;     // 페이지당 최대 카드 수 (기존 조건)
	double GetCardHeightMm() const;
	int GetCardsPerRow() const;
	int GetRowsPerPage() const;
	int GetCardsPerPage() const;
	int GetTotalPages() const;

	afx_msg void OnBnClickedCellChangeButton();

	// [추가] 표찰 크기 / 위치 조절 UI (현장에서 조절 후 인쇄, 값은 레지스트리에 저장)
protected:
	struct KindEditRow
	{
		std::unique_ptr<CStatic> pTitle;
		std::unique_ptr<CStatic> pLabel[3];
		std::unique_ptr<CEdit> pEdit[3]; // 0 : 띠 길이, 1 : 첫 칸 위치, 2 : 칸 간격
	};
	std::vector<std::unique_ptr<CStatic>> m_sizeLabels; // 칸 가로 / 세로 / 세로간격 이름표
	std::vector<KindEditRow> m_kindRows;
	CString GetProfileKey(LPCTSTR pszName) const;
	void LoadSettings();
	void SaveSettings();
	void LayoutSettingControls();
	void UpdateSettingEdits();
	virtual void OnOK(); // Enter 키 : 대화상자 닫지 않고 설정 적용
	afx_msg void OnBnClickedBtnPrevPage();
	afx_msg void OnBnClickedBtnNextPage();
};
