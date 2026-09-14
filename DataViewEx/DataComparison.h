#pragma once
#include "stdafx.h"

// ==========================================
// DataComparison: 데이터 쌍 관리 클래스
// ==========================================
// 원본 데이터와 비교용 데이터를 쌍으로 관리
// 메모리 자동 관리 (shared_ptr 사용)
class DataComparison
{
public:
	static DataComparison& GetInstance();

	// 복사 및 이동 불가 (싱글톤)
	DataComparison(const DataComparison&) = delete;
	DataComparison& operator=(const DataComparison&) = delete;
	DataComparison(DataComparison&&) = delete;
	DataComparison& operator=(DataComparison&&) = delete;

	// ==========================================
	// 데이터 설정 함수
	// ==========================================

	/// ======================
	/// 원본 데이터 설정
	/// ======================
	void SetOriginal(std::unique_ptr<DBstruct> original);

	/// ======================
	/// 비교용 데이터 설정
	/// ======================
	void SetDiff(std::unique_ptr<DBstruct> diff);

	/// ======================
	/// 원본 IO 카드 정보 설정
	/// ======================
	void SetOriginalCardInfo(std::unique_ptr<CARD_INFO> cardInfo);

	/// ======================
	/// 비교용 IO 카드 정보 설정v
	/// ======================
	void SetDiffCardInfo(std::unique_ptr<CARD_INFO> cardInfo);

	/// ======================
	/// 원본 로직 변수 설정
	/// ======================
	void SetOriginalLogicVariable(std::vector<std::shared_ptr<LOGIC_VARIABLE>> logicVar);

	/// ======================
	/// 비교용 로직 변수 설정
	/// ======================
	void SetDiffLogicVariable(std::vector<std::shared_ptr<LOGIC_VARIABLE>> logicVar);
	/// ======================
	/// 원본 LDAT 전체 파일 경로 추가
	/// ======================
	void AddOriginalLogicVariable(CString fileName);
	/// ======================
	/// 비교용 LDAT 전체 파일 경로 추가
	/// ======================
	void AddDiffLogicVariable(CString fileName);

	// ==========================================
	// 데이터 조회 함수
	// ==========================================

	/// ======================
	/// 원본 DB 구조 반환
	/// ======================
	std::shared_ptr<DBstruct> GetOriginal() const;
	/// ======================
	/// 역 정보 반환
	/// ======================
	StationInfoType GetOriginalStation();
	/// ======================
	/// 연동도표 반환
	/// ======================
	std::span<InterLockInfoType> GetOriginalInterLock();
	/// ======================
	/// 진로 정보반환
	/// ======================
	std::span<RouteInfoType> GetOriginalRoute();
	/// ======================
	/// 궤도 정보 반환
	/// ======================
	std::span<TrackInfoType> GetOriginalTrack();
	/// ======================
	/// 신호기 정보 반환
	/// ======================
	std::span<SignalInfoType> GetOriginalSignal();
	/// ======================
	/// 선로전환기 정보 반환
	/// ======================
	std::span<SwitchInfoType> GetOriginalSwitch();
	/// ======================
	/// 폐색 정보 반환
	/// ======================
	std::span<BlockTagInfoType> GetOriginalBlock();
	/// ======================
	/// 건널목 정보 반환
	/// ======================
	std::span<LevelCrossInfoType> GetOriginalLevelCross();
	/// ======================
	/// 절연구간 정보 반환
	/// ======================
	std::span<DeadSectionInfoType> GetOriginalDeadSection();
	/// ======================
	/// 지장물 정보 반환
	/// ======================
	std::span<FallLockInfoType> GetOriginalFallLock();
	/// ======================
	/// 출발반응등 정보 반환
	/// ======================
	std::span<STLInfoType> GetOriginalSTL();
	/// ======================
	/// 기타 고장 정보 반환
	/// ======================
	std::span<FaultInfoType> GetOriginalFault();
	/// ======================
	/// 히터 고장 정보 반환
	/// ======================
	std::span<HeatInfoType> GetOriginalHeat();
	/// ======================
	/// [추가] 임시속도(Slow Order) 정보 반환 (구조체엔 있으나 기존 코드에 없던 접근자)
	/// ======================
	std::span<SlowOrderInfoType> GetOriginalSlowOrder();
	/// ======================
	/// [추가] 유도(Attraction) 정보 반환 (구조체엔 있으나 기존 코드에 없던 접근자)
	/// ======================
	std::span<AttractionInfoType> GetOriginalAttraction();
	/// ======================
	/// CPT 정보 반환
	/// ======================
	std::span<CPTInfoType> GetOriginalCPT();
	/// ======================
	/// DWELL 정보 반환
	/// ======================
	std::span<DwellInfoType> GetOriginalDWELL();
	/// ======================
	/// 제어 건널목 정보 반환
	/// [구조체 변경] LC_INFO_TYPE -> LC_CTRL_INFO_TYPE
	/// ======================
	std::span<LC_CTRL_INFO_TYPE> GetOriginalLC();
	/// ======================



	/// ======================
	/// 비교용 DB 구조 반환
	/// ======================
	std::shared_ptr<DBstruct> GetDiff() const;
	/// ======================
	/// 비교용 역 정보 반환
	/// ======================
	StationInfoType GetDiffStation();
	/// ======================
	/// 비교용 연동도표 반환
	/// ======================
	std::span<InterLockInfoType> GetDiffInterLock();

	/// ======================
	/// 비교용 진로 정보 반환
	/// ======================
	std::span<RouteInfoType> GetDiffRoute();

	/// ======================
	/// 비교용 궤도 정보 반환
	/// ======================
	std::span<TrackInfoType> GetDiffTrack();

	/// ======================
	/// 비교용 신호기 정보 반환
	/// ======================
	std::span<SignalInfoType> GetDiffSignal();

	/// ======================
	/// 비교용 선로전환기 정보 반환
	/// ======================
	std::span<SwitchInfoType> GetDiffSwitch();

	/// ======================
	/// 비교용 폐색 정보 반환
	/// ======================
	std::span<BlockTagInfoType> GetDiffBlock();

	/// ======================
	/// 비교용 건널목 정보 반환
	/// ======================
	std::span<LevelCrossInfoType> GetDiffLevelCross();

	/// ======================
	/// 비교용 절연구간 정보 반환
	/// ======================
	std::span<DeadSectionInfoType> GetDiffDeadSection();

	/// ======================
	/// 비교용 지장물 정보 반환
	/// ======================
	std::span<FallLockInfoType> GetDiffFallLock();

	/// ======================
	/// 비교용 출발반응등 정보 반환
	/// ======================
	std::span<STLInfoType> GetDiffSTL();

	/// ======================
	/// 비교용 기타 고장 정보 반환
	/// ======================
	std::span<FaultInfoType> GetDiffFault();

	/// ======================
	/// 비교용 히터 고장 정보 반환
	/// ======================
	std::span<HeatInfoType> GetDiffHeat();

	/// ======================
	/// [추가] 비교용 임시속도(Slow Order) 정보 반환
	/// ======================
	std::span<SlowOrderInfoType> GetDiffSlowOrder();
	/// ======================
	/// [추가] 비교용 유도(Attraction) 정보 반환
	/// ======================
	std::span<AttractionInfoType> GetDiffAttraction();

	/// ======================
	/// 비교용 CPT 정보 반환
	/// ======================
	std::span<CPTInfoType> GetDiffCPT();

	/// ======================
	/// 비교용 DWELL 정보 반환
	/// ======================
	std::span<DwellInfoType> GetDiffDWELL();

	/// ======================
	/// 비교용 제어 건널목 정보 반환
	/// [구조체 변경] LC_INFO_TYPE -> LC_CTRL_INFO_TYPE
	/// ======================
	std::span<LC_CTRL_INFO_TYPE> GetDiffLC();
	/// ======================
	/// 원본 IO 카드 정보 반환
	/// ======================
	std::shared_ptr<CARD_INFO> GetOriginalCardInfo() const;
	/// ======================
	/// 원본 IO IN 카드 정보 반환
	/// [구조체 변경] INCARDTABLE -> IN_CARDTABLE
	/// ======================
	std::span<IN_CARDTABLE> GetOriginalInCardInfo();
	/// ======================
	/// 원본 IO OUT 카드 정보 반환
	/// [구조체 변경] OUTCARDTABLE -> OUT_CARDTABLE
	/// ======================
	std::span<OUT_CARDTABLE> GetOriginalOutCardInfo();
	/// ======================
	/// [추가] 원본 신호기 카드 정보 반환 (PDOCARDTABLE 이 신호기용/선로전환기용으로 분리됨)
	/// ======================
	std::span<SIG_CARDTABLE> GetOriginalSignalCardInfo();
	/// ======================
	/// [추가] 원본 선로전환기 카드 정보 반환
	/// ======================
	std::span<SWH_CARDTABLE> GetOriginalSwitchCardInfo();
	/// ======================
	/// 비교용 IO 카드 정보 반환
	/// ======================
	std::shared_ptr<CARD_INFO> GetDiffCardInfo() const;
	/// ======================
	/// 비교용 IO 카드 정보 반환
	/// [구조체 변경] INCARDTABLE -> IN_CARDTABLE
	/// ======================
	std::span<IN_CARDTABLE> GetDiffInCardInfo();
	/// ======================
	/// 비교용 IO 카드 정보 반환
	/// [구조체 변경] OUTCARDTABLE -> OUT_CARDTABLE
	/// ======================
	std::span<OUT_CARDTABLE> GetDiffOutCardInfo();
	/// ======================
	/// [추가] 비교용 신호기 카드 정보 반환
	/// ======================
	std::span<SIG_CARDTABLE> GetDiffSignalCardInfo();
	/// ======================
	/// [추가] 비교용 선로전환기 카드 정보 반환
	/// ======================
	std::span<SWH_CARDTABLE> GetDiffSwitchCardInfo();
	/// ======================
	/// 원본 로직 변수 반환
	/// ======================
	std::vector<std::shared_ptr<LOGIC_VARIABLE>> GetOriginalLogicVariable(UCHAR logicKind) const;

	/// ======================
	/// 비교용 로직 변수 반환
	/// ======================
	std::vector<std::shared_ptr<LOGIC_VARIABLE>> GetDiffLogicVariable(UCHAR logicKind) const;


	/// ======================
	/// 원본 LDAT 파일명 반환
	/// ======================
	const std::vector<CString>& GetOriginalLDAT() const;

	/// ======================
	/// 비교용 LDAT 파일명 반환
	/// ======================
	const std::vector<CString>& GetDiffLDAT() const;

	// ==========================================
	// 상태 확인 함수
	// ==========================================

	/// ======================
	/// 원본 데이터 존재 여부
	/// ======================
	bool HasOriginal() const;

	bool HasOriginalCard();

	bool HasOriginalLogic();

	bool HasOriginalLDAT();

	/// ======================
	/// 비교용 데이터 존재 여부
	/// ======================
	bool HasDiff() const;

	bool HasDiffCard();

	bool HasDiffLogic();

	bool HasDiffLDAT();

	/// ======================
	/// 비교 가능 여부 (둘 다 존재하는가)
	/// ======================
	bool IsComparable() const;

	/// ======================
	/// 개별 데이터 비우기
	/// ======================
	void ClearDiff();

	void ClearOriginal();

	/// ======================
	/// 모든 데이터 비우기
	/// ======================
	void Clear();

private:
	DataComparison();
	virtual ~DataComparison();

	// ==========================================
	// 멤버 변수
	// ==========================================
	std::shared_ptr<DBstruct> m_original;                           // 원본 DB 구조
	std::shared_ptr<DBstruct> m_diff;                               // 비교용 DB 구조
	std::shared_ptr<CARD_INFO> m_originalCardInfo;                  // 원본 IO 카드
	std::shared_ptr<CARD_INFO> m_diffCardInfo;                      // 비교용 IO 카드
	std::vector<CString>  m_orignalLDAT;		    //LDAT 파일명 저장하는 벡터
	std::vector<CString> 	m_diffLDAT;         //LDAT 파일명 저장하는 벡터
	std::vector<std::shared_ptr<LOGIC_VARIABLE>> m_originalLogic;   // 원본 로직 변수
	std::vector<std::shared_ptr<LOGIC_VARIABLE>> m_diffLogic;       // 비교용 로직 변수
};