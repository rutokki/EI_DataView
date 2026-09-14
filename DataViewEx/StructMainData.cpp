#include "stdafx.h"
#include "StructMainData.h"

std::vector<std::shared_ptr<LOGIC_VARIABLE>> StructMainData::GetLogicVariableVec()
{
	return m_logicVariableList;
}

// 생성자 구현 추가
StructMainData::StructMainData()
{
	// [명칭 변경] TSCDBStruct -> EIDBStruct, TSCIOCard -> EIIOCard
	EIDBStruct = NULL;
	EIIOCard = NULL;
}

StructMainData::~StructMainData()
{
}

// [명칭 변경] GetTSCDBStruct -> GetEIDBStruct
std::shared_ptr<DBstruct> StructMainData::GetEIDBStruct()
{
	return EIDBStruct;
}

// --------------------------------------------------------------------
// [구조체 변경]  EI_IP_DBStruct_Typedef.h 의 DBstruct 멤버 선언 순서와
//              동일한 순서로 정리
// --------------------------------------------------------------------
StationInfoType StructMainData::GetStationInfo() // 역정보
{
	return EIDBStruct->_STN_Info;
}

std::span<InterLockInfoType> StructMainData::GetInterLockInfo() // 연동정보
{
	return EIDBStruct->_ILK_Info;
}

std::span<RouteInfoType> StructMainData::GetRouteInfo() //진로 정보
{
	return EIDBStruct->_RTE_Info;
}

std::span<TrackInfoType> StructMainData::GetTrackInfo() //궤도 정보
{
	return EIDBStruct->_TRK_Info;
}

std::span<SignalInfoType> StructMainData::GetSignalInfo() //신호기 정보
{
	return EIDBStruct->_SIG_Info;
}

std::span<SwitchInfoType> StructMainData::GetSwitchInfo() //선로전환기(전철기) 정보
{
	return EIDBStruct->_SWH_Info;
}

std::span<BlockTagInfoType> StructMainData::GetBlockInfo() // 폐색 정보
{
	return EIDBStruct->_BLK_Info;
}

std::span<LevelCrossInfoType> StructMainData::GetCossInfo() //건널목 정보
{
	return EIDBStruct->_LCS_Info;
}

std::span<DeadSectionInfoType> StructMainData::GetDeadSectionInfo() //절연구간 정보
{
	return EIDBStruct->_DeadSec_Info;
}

std::span<FallLockInfoType> StructMainData::GetFallLockInfo()
{
	return EIDBStruct->_FallLck_Info;
}

std::span<HeatInfoType> StructMainData::GetHeatInfo()
{
	return EIDBStruct->_Heat_Info;
}


std::span<SlowOrderInfoType> StructMainData::GetSlowOrderInfo()
{
	return EIDBStruct->_SO_Info;
}

std::span<STLInfoType> StructMainData::GetSTLInfo()
{
	return EIDBStruct->_Stl_Info;
}


std::span<AttractionInfoType> StructMainData::GetAttractionInfo()
{
	return EIDBStruct->_Attract_Info;
}

std::span<FaultInfoType> StructMainData::GetFaultInfo()
{
	return EIDBStruct->_Fault_Info;
}

std::span<CPTInfoType> StructMainData::GetCPTInfo()
{
	return EIDBStruct->_CPT_Info;
}

std::span<DwellInfoType> StructMainData::GetDwellInfo()
{
	return EIDBStruct->_DWL_Info;
}


std::span<LC_CTRL_INFO_TYPE> StructMainData::GetLCINFO()
{
	return EIDBStruct->_LC_CTRL_INFO;
}


StructMainData& StructMainData::GetInstance() {
	static StructMainData instance; // 정적 지역 변수를 사용하여 스레드 안전하게 생성
	return instance;
}


void StructMainData::AddEIDBStruct(std::unique_ptr<DBstruct> DBStruct)
{
	EIDBStruct = std::move(DBStruct);
}


void StructMainData::AddEICardStruct(std::unique_ptr<CARD_INFO> CardStruct)
{
	EIIOCard = std::move(CardStruct);
}

void StructMainData::AddLogicVariable(std::shared_ptr<LOGIC_VARIABLE> logicData)
{
	m_logicVariableList.push_back(logicData);
}

std::vector<std::shared_ptr<LDAT_FILE_CONTENT>> StructMainData::GetLDatFileContents()
{
	return m_ldatFileContentList;
}

void StructMainData::AddLDatFileContent(std::shared_ptr<LDAT_FILE_CONTENT> content)
{
	m_ldatFileContentList.push_back(content);
}



std::span<IN_CARDTABLE> StructMainData::GetINCardInfo()
{
	return EIIOCard->INCARDLIST;
}

std::span<OUT_CARDTABLE> StructMainData::GetOUTCardInfo()
{
	return EIIOCard->OUTCARDLIST;
}

std::span<SIG_CARDTABLE> StructMainData::GetSignalCardInfo()
{
	return EIIOCard->SIGNALLIST;
}

std::span<SWH_CARDTABLE> StructMainData::GetSwitchCardInfo()
{
	return EIIOCard->SWITCHLIST;
}

void StructMainData::ClearAllData()
{
	// 로직 변수 리스트 비우기 및 여유 메모리 반환
	m_logicVariableList.clear();
	m_logicVariableList.shrink_to_fit();
	// LDAT 파일 리스트 비우기 및 메모리 반환
	m_ldatFileContentList.clear();
	m_ldatFileContentList.shrink_to_fit();
	// 카드 정보 및 DB 구조체 스마트 포인터 초기화
	if (EIIOCard)
	{
		EIIOCard.reset();
	}

	if (EIDBStruct)
	{
		EIDBStruct.reset();
	}
}