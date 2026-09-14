#pragma once

#include "CustomBCGGridCtrl.h"
#include "WorkSpaceBar.h"
#include "DataComparison.h"
#include "CustomBCGPEditCtrl.h"
#include "WThread.h"
#include "DiffLine.h"
#include <map>


// 1줄 단위의 비교 결과를 담는 구조체


// 1. 줄마다 명칭(Key)과 등장 순서를 관리하기 위한 구조체
struct LineToken {
	CString key;
	int occurrence;     // 동일한 명칭이 몇 번째로 등장했는지 (0부터 시작)
	CString fullLine;   // 원본 전체 텍스트
};
class CDiffGridRightView : public CView
{
	DECLARE_DYNCREATE(CDiffGridRightView)

public:
	CDiffGridRightView();
	virtual ~CDiffGridRightView();

	//CBCGPGridCtrl m_wndGrid;
	CustomBCGPEditCtrl m_wndRightEditLDat;
	CBCGPToolBar m_subFrameToolBar;

	//CBCGPCaptionBar m_wndRightCaptionBar;
	virtual void OnDraw(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//CString GetFileStationName(const CString& fileName);
	afx_msg void OnToolbarFileOpen();
	//void SetLogicData(const std::vector<CString>& lines, const std::vector<DiffLine>& diffs);
	afx_msg void OnNextDelete();
	afx_msg void OnNextInsert();
	afx_msg void OnNextChange();
	afx_msg void OnNextMove();

	//afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()


};

// ==========================================
// CDiffGridPaneView: 각 스플리터 패인의 그리드 뷰
// ==========================================
class CDiffGridPaneView : public CView
{
	DECLARE_DYNCREATE(CDiffGridPaneView)

public:
	CDiffGridPaneView();
	virtual ~CDiffGridPaneView();


	CBCGPToolBar m_subFrameToolBar;
	CustomBCGPEditCtrl m_wndLeftEditLDat;
	//CBCGPCaptionBar m_wndLeftCaptionBar;
	virtual void OnDraw(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CString GetFileStationName(const CString& fileName);
	afx_msg void OnToolbarFileOpen();
	//void SetLogicData(const std::vector<CString>& lines, const std::vector<DiffLine>& diffs);
	bool ReadTextFileLines(const CString& filePath, std::vector<CString>& outLines);
	afx_msg void OnNextDelete();
	afx_msg void OnNextInsert();
	afx_msg void OnNextChange();
	afx_msg void OnNextMove();
	//void LoadAndCompareFile(const CString& originalPath, const CString& diffPath, bool bIsOriginalView);
	//afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()

};

// ==========================================
// DiffCompareFrame: 비교 윈도우 프레임
// ==========================================
class DiffCompareFrame : public CBCGPFrameWnd
{
	DECLARE_DYNCREATE(DiffCompareFrame)

public:
	DiffCompareFrame();
	virtual ~DiffCompareFrame();
	WThread m_logicThread;
	CBCGPSplitterWnd m_wndSplitter;
	DiffCompareFrame** m_ppParentPtr;  // 부모 윈도우의 프레임 포인터 주소 저장
	CWorkSpaceBar m_pWorkSpaceBar;   // 부모 윈도우의 워크스페이스 바 포인터 저장
	CBCGPCaptionBar m_wndCaptionBar;
	CBCGPProgressDlg m_wndProgressDlg;
	CustomBCGPEditCtrl* GetLeftEdit();
	CustomBCGPEditCtrl* GetRightEdit();
	std::vector<DiffLine> m_tempDiffResults; // 렌더링 중 보관할 Diff 결과
	int m_nCurrentChunkIndex = 0;            // 현재 진행 중인 인덱스
	std::vector<DiffLine> m_diffResults;
	int m_CurrentChange;
	int m_CurrentDelete;
	int m_CurrnetAdd;
	int m_CurrentMove;
	int m_CurrentDiffRow;
	bool m_bSplitterReady = false;
	CString m_strOriginFile;   // 원본 파일명을 기억할 변수
	CString m_strCompareFile;  // 비교 파일명을 기억할 변수
	void CompareLDATFiles(const CString& originalPath, const CString& diffPath);
	void LeftViewLDATFiles(CString originalPath, bool isSingle);
	void ApplyDiffToUI(const std::vector<CString>& leftLines, const std::vector<CString>& rightLines);
	void ProcessLdatSelection(const CString& fileName, const CString& clickedOriginalPath);
	void ProcessLogicDiffSelection(CString& logicType);
	void ProcessBinDiffSelection(const CString& type);
	void MoveToDiffRow(int nRow);
	void ShowNoDataMessage(bool isLeft, const std::vector<CString>& lines);
	void ApplyCompareOrSingle(
		bool hasOrigin, bool hasDiff,
		const std::vector<CString>& leftLines,
		const std::vector<CString>& rightLines);
	void ResetEditContent();
	void SetCaptionText(CString strCaption, bool isOrigin);
	void FindNextDiff(DiffType type);
	void FindPrevDiff(DiffType type);
	void UpdateCaptionBar(CString str);
	void UpdateNoDataCaptionBar();
	void ShowDiffEndMessage(DiffType type, bool bPrevious);
	void DisplaySingle(const std::vector<CString>& lines, bool isLeft);



	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	void InitSplitterHalfAndReady();

	afx_msg void OnClose();
	afx_msg void OnTvnDeleteItem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	//afx_msg LRESULT OnLogicFinished(WPARAM wParam, LPARAM lParam);
	//afx_msg LRESULT OnRenderNextChunk(WPARAM wParam, LPARAM lParam);
	virtual void PostNcDestroy();

	DECLARE_MESSAGE_MAP()

private:
	// 1. 기본 설비 정보 설정 함수
	void SetStationInfo();        // 역 정보

	std::vector<CString> ConvertStationInfoText(StationInfoType StationInfo); // 역 정보 텍스트 반환
	void SetTrackInfo();          // 궤도 정보
	std::vector<CString> ConvertTrackInfoText(const std::span<TrackInfoType>& trackList, bool bIsOrigin);
	void SetSignalInfo();         // 신호기
	std::vector<CString> ConvertSignalInfoText(const std::span<SignalInfoType>& signalList, bool bIsOrigin);
	void SetSwitchInfo();         // 선로전환기
	std::vector<CString> ConvertSwitchInfoText(const std::span<SwitchInfoType>& switchList, bool bIsOrigin);
	void SetBlockInfo();          // 폐색 구간
	std::vector<CString> ConvertBlockInfoText(const std::span<BlockTagInfoType>& blockList, bool bIsOrigin);

	std::vector<CString> ConvertLevelCrossInfoText(const std::span<LevelCrossInfoType>& list, bool bIsOrigin);

	std::vector<CString> ConvertDeadSectionInfoText(const std::span<DeadSectionInfoType>& list, bool bIsOrigin);

	std::vector<CString> ConvertFallLockInfoText(const std::span<FallLockInfoType>& list, bool bIsOrigin);

	std::vector<CString> ConvertSTLInfoText(const std::span<STLInfoType>& list, bool bIsOrigin);

	std::vector<CString> ConvertFaultInfoText(const std::span<FaultInfoType>& list, bool bIsOrigin);

	// 2. 고장 및 이례사항 관리 함수
	void SetAllFaultInfo();
	std::vector<CString> ConvertHeatInfoText(const std::span<HeatInfoType>& list, bool bIsOrigin);


	// 3. 연동 및 시스템 설정 함수
	void SetInterLock();          // 연동도표(진로 정보)
	std::vector<CString> ConvertInterlockInfoText(const std::span<InterLockInfoType>& interLockList, const std::span<RouteInfoType>& routeList, bool bIsOrigin);
	void SetCPTInfo();            // CPT
	std::vector<CString> ConvertCPTInfoText(const std::span<CPTInfoType>& list, bool bIsOrigin);
	void SetDWELLInfo();          // Dwell Time (정차 시간)
	std::vector<CString> ConvertDwellInfoText(const std::span<DwellInfoType>& list, bool bIsOrigin);


	// [구조체 변경] LC_INFO_TYPE -> LC_CTRL_INFO_TYPE
	std::vector<CString> ConvertLCInfoText(const std::span<LC_CTRL_INFO_TYPE>& list, bool bIsOrigin); // 제어 건널목

	// 4. 하드웨어 I/O 카드 관리 함수
	// [구조체 변경] InputDataType/IoCardAddressType 은 삭제됨. PDOCARDTABLE 이 신호기용/선로전환기용으로 분리되어
	// ConvertInputDataInfoText/ConvertIoCardAddressInfoText 를 삭제하고 아래 두 함수로 대체함
	void SetAllIOCardInfo();
	std::vector<CString> ConvertSignalCardInfoText(const std::span<SIG_CARDTABLE>& list, bool bIsOrigin);
	std::vector<CString> ConvertSwitchCardInfoText(const std::span<SWH_CARDTABLE>& list, bool bIsOrigin);

	// 카드 Addr
	void SetIncardInfo();         // INCardList
	// [구조체 변경] INCARDTABLE -> IN_CARDTABLE
	std::vector<CString> ConvertInCardInfoText(const std::span<IN_CARDTABLE>& list, bool bIsOrigin);
	void SetOutCardInfo();        // OutCardList
	// [구조체 변경] OUTCARDTABLE -> OUT_CARDTABLE
	std::vector<CString> ConvertOutCardInfoText(const std::span<OUT_CARDTABLE>& list, bool bIsOrigin);

	void SetLogicInfo(UCHAR LogicKind);
	std::vector<CString> ConvertLogicInfoText(const std::span<std::shared_ptr<LOGIC_VARIABLE>>& logicLis, bool bIsOrigin);
};