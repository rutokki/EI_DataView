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
	double m_bottomBlankMm;  // [추가] 마지막 포트 칸과 아래 꼬리표 사이 공백 (mm)
	double m_maxSpacingYMm;  // [추가] 세로 간격 최대값 (IN/OUT 3mm, 신호기 / 선로전환기 모듈 40mm)
	double m_maxBottomBlankMm; // [추가] 하단 공백 최대값 (IN/OUT 30mm, 신호기 / 선로전환기 모듈 120mm)
	double m_maxCellHeightMm;  // [추가] 칸 세로 최대값 (IN/OUT 8mm, 신호기 / 선로전환기 모듈 20mm)
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
	//  - m_strCardLabel  : 카드 머리표 접두어 (예: "In" -> "In 1", "SIG" -> "SIG 1")
	//  - m_strListTitle  : 제목 (예: "IN Card List")
	int m_nPortsPerCard;
	CString m_strCardLabel;
	CString m_strListTitle;
	void SetCardKind(int nPortsPerCard, LPCTSTR pszCardLabel, LPCTSTR pszListTitle)
	{
		m_nPortsPerCard = (nPortsPerCard > 0) ? nPortsPerCard : 16;
		m_strCardLabel = pszCardLabel;
		m_strListTitle = pszListTitle;
		m_cardUsedCells.clear();

		// [추가] 신호기(4포트) / 선로전환기(2포트) 모듈은 표찰 창 길이에 맞추려면 포트 칸 사이를
		// IN/OUT(16포트)보다 넓게 띄워야 하므로 세로 간격 기본값 / 최대값을 따로 둠
		//  (저장된 값이 있으면 OnInitDialog 의 LoadSettings 에서 덮어씀)
		if (m_nPortsPerCard < 16)
		{
			m_cellSpacingYMm = (m_nPortsPerCard <= 2) ? 15.0 : 4.0;
			m_maxSpacingYMm = 40.0;
			m_maxBottomBlankMm = 120.0; // 모듈 표찰 창이 길어 하단 공백을 크게 잡을 수 있게
			m_maxCellHeightMm = 20.0;
		}
		else
		{
			m_maxSpacingYMm = 3.0;
			m_maxBottomBlankMm = 30.0;
			m_maxCellHeightMm = 8.0;
		}
	}

	// [추가] 카드별 실제 사용 포트 수 (비어있으면 모든 카드 m_nPortsPerCard)
	//  4등형 신호기는 포트가 2개뿐이라 2칸만 출력, 한 칸을 2슬롯 크기로 늘려 전체 크기는 2등형(4칸)과 동일
	std::vector<int> m_cardUsedCells;
	int GetCardUsedCells(int nCard) const
	{
		if (nCard < 0 || nCard >= (int)m_cardUsedCells.size()) return m_nPortsPerCard;
		return (std::min)(m_cardUsedCells[nCard], m_nPortsPerCard);
	}

	int GetCardCount() const;
	int GetCardStart(int nCard) const;
	int GetCardCells(int nCard) const;

	// [추가] 페이지당 카드 수 계산 (미리보기 / 인쇄 / 페이지 이동 공통)
	static constexpr double TAG_USABLE_WIDTH_MM = 265.0;  // A4 가로 297mm - 좌우 여백
	static constexpr double TAG_USABLE_HEIGHT_MM = 175.0; // A4 세로 210mm - 제목 / 상하 여백
	static constexpr double TAG_COL_GAP_MM = 0.5;         // 카드 사이 가로 간격
	static constexpr double TAG_ROW_GAP_MM = 3.0;         // 카드 줄 사이 세로 간격
	static constexpr double TAG_HEADER_HEIGHT_MM = 2.4;   // 머리표 높이 (칸 크기 조절과 무관하게 고정)
	static constexpr int TAG_CARDS_PER_ROW = 16;          // 한 줄 카드 수 (MAX_IO_CARD_SLOT : 서브랙당 슬롯 수)
	static constexpr int TAG_ROWS_PER_PAGE = 2;           // 위아래 줄 수
	static constexpr double TAG_MODULE_HEADER_GAP_MM = 0.5; // 신호기 / 선로전환기 모듈의 머리표 앞뒤 간격
	// [추가] 머리표 앞뒤 간격 : IN/OUT 은 기존대로 세로 간격과 같음, 모듈은 세로 간격을 크게 잡아도 머리표는 붙여서 출력
	double GetHeaderGapMm() const { return (m_nPortsPerCard < 16) ? TAG_MODULE_HEADER_GAP_MM : m_cellSpacingYMm; }
	double GetCardHeightMm() const;
	int GetCardsPerRow() const;
	int GetRowsPerPage() const;
	int GetCardsPerPage() const;
	int GetTotalPages() const;

	afx_msg void OnBnClickedCellChangeButton();

	// [추가] 표찰 크기 / 위치 조절 UI (현장에서 조절 후 인쇄, 값은 레지스트리에 저장)
protected:
	std::vector<std::unique_ptr<CStatic>> m_sizeLabels; // 칸 가로 / 세로 / 세로간격 / 하단 공백 이름표
	std::unique_ptr<CEdit> m_pEditBottom;               // [추가] 하단 공백 입력란
	CString GetProfileKey(LPCTSTR pszName) const;
	void LoadSettings();
	void SaveSettings();
	void LayoutSettingControls();
	void UpdateSettingEdits();
	virtual void OnOK(); // Enter 키 : 대화상자 닫지 않고 설정 적용
	afx_msg void OnBnClickedBtnPrevPage();
	afx_msg void OnBnClickedBtnNextPage();
};
