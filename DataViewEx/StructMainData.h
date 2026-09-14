#pragma once
#include "stdafx.h"
struct LDAT_FILE_CONTENT {
	CString fileName;
	std::vector<CString> contents;
};
class StructMainData
{
public:
	static StructMainData& GetInstance();

	StructMainData(const StructMainData&) = delete;
	StructMainData& operator=(const StructMainData&) = delete;
	StructMainData(StructMainData&&) = delete;
	StructMainData& operator=(StructMainData&&) = delete;

	// [명칭 변경] TSC -> EI
	void AddEIDBStruct(std::unique_ptr<DBstruct> DBStruct);
	void AddEICardStruct(std::unique_ptr<CARD_INFO> CardStruct);
	void AddLogicVariable(std::shared_ptr<LOGIC_VARIABLE> logicData);
	////////////////////////////////////////////////////////////////////////////////
	// LDAT
	std::vector<std::shared_ptr<LDAT_FILE_CONTENT>> m_ldatFileContentList; // [추가] LDAT 파일 리스트
	std::vector<std::shared_ptr<LDAT_FILE_CONTENT>> GetLDatFileContents();
	void AddLDatFileContent(std::shared_ptr<LDAT_FILE_CONTENT> content);
	////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////
	// EI_IOCard_Typedef  ([명칭 변경] TSC_IOCard_Typedef -> EI_IOCard_Typedef)
	std::vector<std::shared_ptr<LOGIC_VARIABLE>>m_logicVariableList;

	// [명칭 변경] TSCIOCard -> EIIOCard
	std::shared_ptr<CARD_INFO> EIIOCard;

	// [구조체 변경] EI_IP_IOCard_Typedef.h
	//  - INCARDTABLE/OUTCARDTABLE(flat) -> IN_CARDTABLE/OUT_CARDTABLE(RackNo/SlotNo + CardData[] 중첩 구조)
	//  - PDOCARDTABLE(PDOLIST) 삭제, SIG_CARDTABLE(SIGNALLIST) / SWH_CARDTABLE(SWITCHLIST) 로 분리 신설
	std::span<IN_CARDTABLE> GetINCardInfo();
	std::span<OUT_CARDTABLE> GetOUTCardInfo();
	std::span<SIG_CARDTABLE> GetSignalCardInfo();
	std::span<SWH_CARDTABLE> GetSwitchCardInfo();

	void ClearAllData();

	////////////////////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////////////////////

	// EI_DBStruct_Typedef  ([명칭 변경] TSC_DBStruct_Typedef -> EI_DBStruct_Typedef)
	// [구조체 변경] 아래 접근자들은 EI_IP_DBStruct_Typedef.h 의 DBstruct 멤버 선언 순서 그대로 1:1 대응되도록 정리함.
	//              DBstruct 에 없는 InputDataType/_INP_Info 는 이번 정리에서 제거함(확인 필요 시 말씀해주세요).

	// [명칭 변경] TSCDBStruct -> EIDBStruct
	std::shared_ptr<DBstruct> EIDBStruct;
	// [명칭 변경] GetTSCDBStruct -> GetEIDBStruct
	std::shared_ptr<DBstruct> GetEIDBStruct();

	StationInfoType                GetStationInfo();       // 역 정보 반환                  -> _STN_Info
	std::span<InterLockInfoType>   GetInterLockInfo();      // 연동 정보 반환                -> _ILK_Info
	std::span<RouteInfoType>       GetRouteInfo();          // 진로 정보 반환                -> _RTE_Info
	std::span<TrackInfoType>       GetTrackInfo();          // 궤도 정보 반환                -> _TRK_Info
	std::span<SignalInfoType>      GetSignalInfo();         // 신호기 정보 반환              -> _SIG_Info
	std::span<SwitchInfoType>      GetSwitchInfo();         // 선로전환기(전철기) 정보 반환  -> _SWH_Info
	std::span<BlockTagInfoType>    GetBlockInfo();           // 폐색 정보 반환               -> _BLK_Info
	std::span<LevelCrossInfoType>  GetCossInfo();            // 건널목 정보 반환             -> _LCS_Info

	std::span<DeadSectionInfoType> GetDeadSectionInfo();     // 절연구간 정보 반환           -> _DeadSec_Info
	std::span<FallLockInfoType>    GetFallLockInfo();        // 지장물 정보 반환             -> _FallLck_Info
	std::span<HeatInfoType>        GetHeatInfo();            // 히터 고장 정보 반환          -> _Heat_Info

	// [추가] DBstruct 에는 있으나 기존 StructMainData 에는 빠져있던 접근자
	std::span<SlowOrderInfoType>   GetSlowOrderInfo();       // 임시속도(Slow Order) 정보 반환 -> _SO_Info
	std::span<STLInfoType>         GetSTLInfo();             // 출발반응등 정보 반환         -> _Stl_Info
	// [추가] DBstruct 에는 있으나 기존 StructMainData 에는 빠져있던 접근자
	std::span<AttractionInfoType>  GetAttractionInfo();      // 유도 정보 반환               -> _Attract_Info
	std::span<FaultInfoType>       GetFaultInfo();           // 기타 고장 정보 반환          -> _Fault_Info

	std::span<CPTInfoType>         GetCPTInfo();             // CPT 정보 반환                -> _CPT_Info
	std::span<DwellInfoType>       GetDwellInfo();           // Dwell 정보 반환              -> _DWL_Info

	std::span<LC_CTRL_INFO_TYPE>   GetLCINFO();              // 제어 건널목 정보 반환        -> _LC_CTRL_INFO

	////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////

	// Logic_Typeof
	std::vector<std::shared_ptr<LOGIC_VARIABLE>> GetLogicVariableVec();

private:
	StructMainData();
	virtual ~StructMainData();
};