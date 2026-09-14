#include "stdafx.h"
#include "DataComparison.h"

// ==========================================
// DataComparison 싱글톤 구현
// ==========================================

DataComparison& DataComparison::GetInstance()
{
	static DataComparison instance;
	return instance;
}

DataComparison::DataComparison()

{
}

DataComparison::~DataComparison()
{
	Clear();
}

// ==========================================
// 데이터 설정 함수
// ==========================================

void DataComparison::SetOriginal(std::unique_ptr<DBstruct> original)
{
	m_original = std::move(original);
}

void DataComparison::SetDiff(std::unique_ptr<DBstruct> diff)
{
	m_diff = std::move(diff);
}

void DataComparison::SetOriginalCardInfo(std::unique_ptr<CARD_INFO> cardInfo)
{
	m_originalCardInfo = std::move(cardInfo);
}

void DataComparison::SetDiffCardInfo(std::unique_ptr<CARD_INFO> cardInfo)
{
	m_diffCardInfo = std::move(cardInfo);
}

void DataComparison::SetOriginalLogicVariable(std::vector<std::shared_ptr<LOGIC_VARIABLE>> logicVar)
{
	m_originalLogic = std::move(logicVar);
}

void DataComparison::SetDiffLogicVariable(std::vector<std::shared_ptr<LOGIC_VARIABLE>> logicVar)
{
	m_diffLogic = std::move(logicVar);
}
void DataComparison::AddOriginalLogicVariable(CString fileName)
{
	m_orignalLDAT.push_back(fileName);
}
void DataComparison::AddDiffLogicVariable(CString fileName)
{
	m_diffLDAT.push_back(fileName);
}

// ==========================================
// 데이터 조회 함수
// ==========================================

std::shared_ptr<DBstruct> DataComparison::GetOriginal() const
{
	return m_original;
}
StationInfoType DataComparison::GetOriginalStation()
{
	return m_original->_STN_Info;
}
std::span<InterLockInfoType> DataComparison::GetOriginalInterLock()
{
	return m_original->_ILK_Info;
}
std::span<RouteInfoType> DataComparison::GetOriginalRoute()
{
	return m_original->_RTE_Info;
}
std::span<TrackInfoType> DataComparison::GetOriginalTrack()
{
	return m_original->_TRK_Info;
}
std::span<SignalInfoType> DataComparison::GetOriginalSignal()
{
	return m_original->_SIG_Info;
}
std::span<SwitchInfoType> DataComparison::GetOriginalSwitch()
{
	return m_original->_SWH_Info;
}
std::span<BlockTagInfoType> DataComparison::GetOriginalBlock()
{
	return  m_original->_BLK_Info;
}
std::span<LevelCrossInfoType> DataComparison::GetOriginalLevelCross()
{
	return  m_original->_LCS_Info;
}
std::span<DeadSectionInfoType> DataComparison::GetOriginalDeadSection()
{
	return  m_original->_DeadSec_Info;
}
std::span<FallLockInfoType> DataComparison::GetOriginalFallLock()
{
	return  m_original->_FallLck_Info;
}
std::span<STLInfoType> DataComparison::GetOriginalSTL()
{
	return  m_original->_Stl_Info;
}
std::span<FaultInfoType> DataComparison::GetOriginalFault()
{
	return  m_original->_Fault_Info;
}
std::span<HeatInfoType> DataComparison::GetOriginalHeat()
{
	return  m_original->_Heat_Info;
}
// [추가] DBstruct 에는 있으나 기존 DataComparison 에는 빠져있던 접근자
std::span<SlowOrderInfoType> DataComparison::GetOriginalSlowOrder()
{
	return m_original->_SO_Info;
}
// [추가] DBstruct 에는 있으나 기존 DataComparison 에는 빠져있던 접근자
std::span<AttractionInfoType> DataComparison::GetOriginalAttraction()
{
	return m_original->_Attract_Info;
}
std::span<CPTInfoType> DataComparison::GetOriginalCPT()
{
	return m_original->_CPT_Info;
}
std::span<DwellInfoType> DataComparison::GetOriginalDWELL()
{
	return m_original->_DWL_Info;
}
// [구조체 변경] LC_INFO_TYPE/_LC_INFO -> LC_CTRL_INFO_TYPE/_LC_CTRL_INFO
std::span<LC_CTRL_INFO_TYPE> DataComparison::GetOriginalLC()
{
	return m_original->_LC_CTRL_INFO;
}
std::shared_ptr<DBstruct> DataComparison::GetDiff() const
{
	return m_diff;
}

StationInfoType DataComparison::GetDiffStation()
{
	return m_diff->_STN_Info;
}

std::span<InterLockInfoType> DataComparison::GetDiffInterLock()
{
	return m_diff->_ILK_Info;
}

std::span<RouteInfoType> DataComparison::GetDiffRoute()
{
	return m_diff->_RTE_Info;
}

std::span<TrackInfoType> DataComparison::GetDiffTrack()
{
	return m_diff->_TRK_Info;
}

std::span<SignalInfoType> DataComparison::GetDiffSignal()
{
	return m_diff->_SIG_Info;
}

std::span<SwitchInfoType> DataComparison::GetDiffSwitch()
{
	return m_diff->_SWH_Info;
}

std::span<BlockTagInfoType> DataComparison::GetDiffBlock()
{
	return m_diff->_BLK_Info;
}

std::span<LevelCrossInfoType> DataComparison::GetDiffLevelCross()
{
	return m_diff->_LCS_Info;
}

std::span<DeadSectionInfoType> DataComparison::GetDiffDeadSection()
{
	return m_diff->_DeadSec_Info;
}

std::span<FallLockInfoType> DataComparison::GetDiffFallLock()
{
	return m_diff->_FallLck_Info;
}

std::span<STLInfoType> DataComparison::GetDiffSTL()
{
	return m_diff->_Stl_Info;
}

std::span<FaultInfoType> DataComparison::GetDiffFault()
{
	return m_diff->_Fault_Info;
}

std::span<HeatInfoType> DataComparison::GetDiffHeat()
{
	return m_diff->_Heat_Info;
}

// [추가] DBstruct 에는 있으나 기존 DataComparison 에는 빠져있던 접근자
std::span<SlowOrderInfoType> DataComparison::GetDiffSlowOrder()
{
	return m_diff->_SO_Info;
}
// [추가] DBstruct 에는 있으나 기존 DataComparison 에는 빠져있던 접근자
std::span<AttractionInfoType> DataComparison::GetDiffAttraction()
{
	return m_diff->_Attract_Info;
}

std::span<CPTInfoType> DataComparison::GetDiffCPT()
{
	return m_diff->_CPT_Info;
}

std::span<DwellInfoType> DataComparison::GetDiffDWELL()
{
	return m_diff->_DWL_Info;
}

// [구조체 변경] LC_INFO_TYPE/_LC_INFO -> LC_CTRL_INFO_TYPE/_LC_CTRL_INFO
std::span<LC_CTRL_INFO_TYPE> DataComparison::GetDiffLC()
{
	return m_diff->_LC_CTRL_INFO;
}

std::shared_ptr<CARD_INFO> DataComparison::GetOriginalCardInfo() const
{
	return m_originalCardInfo;
}
// [구조체 변경] INCARDTABLE -> IN_CARDTABLE, INLIST -> INCARDLIST
std::span<IN_CARDTABLE> DataComparison::GetOriginalInCardInfo() {
	return m_originalCardInfo->INCARDLIST;
}
// [구조체 변경] OUTCARDTABLE -> OUT_CARDTABLE, OUTLIST -> OUTCARDLIST
std::span<OUT_CARDTABLE> DataComparison::GetOriginalOutCardInfo()
{
	return  m_originalCardInfo->OUTCARDLIST;
}
// [추가] PDOCARDTABLE(PDOLIST) 가 삭제되고 신호기용/선로전환기용으로 분리된 접근자
std::span<SIG_CARDTABLE> DataComparison::GetOriginalSignalCardInfo()
{
	return m_originalCardInfo->SIGNALLIST;
}
std::span<SWH_CARDTABLE> DataComparison::GetOriginalSwitchCardInfo()
{
	return m_originalCardInfo->SWITCHLIST;
}
std::shared_ptr<CARD_INFO> DataComparison::GetDiffCardInfo() const
{
	return m_diffCardInfo;
}

// [구조체 변경] INCARDTABLE -> IN_CARDTABLE, INLIST -> INCARDLIST
std::span<IN_CARDTABLE> DataComparison::GetDiffInCardInfo()
{
	return m_diffCardInfo->INCARDLIST;
}

// [구조체 변경] OUTCARDTABLE -> OUT_CARDTABLE, OUTLIST -> OUTCARDLIST
std::span<OUT_CARDTABLE> DataComparison::GetDiffOutCardInfo()
{
	return m_diffCardInfo->OUTCARDLIST;
}
// [추가] PDOCARDTABLE(PDOLIST) 가 삭제되고 신호기용/선로전환기용으로 분리된 접근자
std::span<SIG_CARDTABLE> DataComparison::GetDiffSignalCardInfo()
{
	return m_diffCardInfo->SIGNALLIST;
}
std::span<SWH_CARDTABLE> DataComparison::GetDiffSwitchCardInfo()
{
	return m_diffCardInfo->SWITCHLIST;
}
std::vector<std::shared_ptr<LOGIC_VARIABLE>> DataComparison::GetOriginalLogicVariable(UCHAR logicKind) const
{
	std::vector<std::shared_ptr<LOGIC_VARIABLE>> filteredList;

	for (const auto& item : m_originalLogic)
	{
		if (item && item->LogicKind == logicKind)
		{
			filteredList.push_back(item);
		}
	}

	return filteredList;
}

std::vector<std::shared_ptr<LOGIC_VARIABLE>> DataComparison::GetDiffLogicVariable(UCHAR logicKind) const
{
	std::vector<std::shared_ptr<LOGIC_VARIABLE>> filteredList;

	for (const auto& item : m_diffLogic)
	{
		if (item && item->LogicKind == logicKind)
		{
			filteredList.push_back(item);
		}
	}

	return filteredList;
}

const std::vector<CString>& DataComparison::GetOriginalLDAT() const
{
	return m_orignalLDAT;
}
const std::vector<CString>& DataComparison::GetDiffLDAT() const
{
	return m_diffLDAT;
}
// ==========================================
// 상태 확인 함수
// ==========================================

bool DataComparison::HasOriginal() const
{
	return m_original != nullptr;
}
bool DataComparison::HasOriginalCard()
{
	return m_originalCardInfo != nullptr;
}
bool DataComparison::HasOriginalLogic() {
	return !m_originalLogic.empty();
}
bool DataComparison::HasOriginalLDAT() {
	return !m_orignalLDAT.empty();
}
bool DataComparison::HasDiff() const
{
	return m_diff != nullptr;
}
bool DataComparison::HasDiffCard()
{
	return m_diffCardInfo != nullptr;
}
bool DataComparison::HasDiffLogic() {
	return !m_diffLogic.empty();
}
bool DataComparison::HasDiffLDAT() {
	return !m_diffLDAT.empty();
}
bool DataComparison::IsComparable() const
{
	return HasOriginal() && HasDiff();
}
// ==========================================
// 데이터 리셋 함수
// ==========================================
void DataComparison::ClearDiff()
{
	m_diff.reset();
	m_diffCardInfo.reset();
	m_diffLogic.clear();
	m_diffLDAT.clear();
}
void DataComparison::Clear()
{
	m_diff.reset();
	m_diffCardInfo.reset();
	m_diffLogic.clear();
	m_original.reset();
	m_originalCardInfo.reset();
	m_originalLogic.clear();
	m_orignalLDAT.clear();
	m_diffLDAT.clear();
}
void DataComparison::ClearOriginal()
{
	m_original.reset();
	m_originalCardInfo.reset();
	m_originalLogic.clear();
	m_orignalLDAT.clear();
}