#include "stdafx.h"
#include "CommonUtils.h"
#include "StructMainData.h"
#include "DataComparison.h"
// ==========================================================
// 번호를 넘겨받아 이름을 반환하는 공용 함수
// ==========================================================
CString CommonUtil::GetDBNameByNumber(Byte_t nNum, GetDBNameByNum num)
{
	if (nNum == 0 || nNum == 0xFF)
	{
		return _T("-");
	}
	auto ExtractName = [&](const auto& List)->CString {

		int nIndex = static_cast<int>(nNum);

		// 전체 배열 사이즈를 초과하는 인덱스 접근 차단 
		if (nIndex >= List.size())
		{
			return _T("-");
		}

		// 안전하게 텍스트 추출
		CString strName = GetName(List[nIndex]);
		if (strName.IsEmpty()) return _T("-");
		return strName;
		};
	switch (num)
	{
	case GetDBNameByNum::InterLockIdx:
		return ExtractName(StructMainData::GetInstance().GetSignalInfo());
	case GetDBNameByNum::TrackIdx:
		return ExtractName(StructMainData::GetInstance().GetTrackInfo());
	case GetDBNameByNum::SignalIdx:
		return ExtractName(StructMainData::GetInstance().GetSignalInfo());
	case GetDBNameByNum::SwitchIdx:
		return ExtractName(StructMainData::GetInstance().GetSwitchInfo());
	case GetDBNameByNum::BlockIdx:
		return ExtractName(StructMainData::GetInstance().GetBlockInfo());
	case GetDBNameByNum::LevelIdx:
		return ExtractName(StructMainData::GetInstance().GetCossInfo());
	case GetDBNameByNum::DeadSectionIdx:
		return ExtractName(StructMainData::GetInstance().GetDeadSectionInfo());
	case GetDBNameByNum::FallLockIdx:
		return ExtractName(StructMainData::GetInstance().GetFallLockInfo());
	case GetDBNameByNum::STLIdx:
		return ExtractName(StructMainData::GetInstance().GetSTLInfo());
	case GetDBNameByNum::FaultIdx:
		return ExtractName(StructMainData::GetInstance().GetFaultInfo());
	case GetDBNameByNum::HeatIdx:
		return ExtractName(StructMainData::GetInstance().GetHeatInfo());
	case GetDBNameByNum::CPTIdx:
		return ExtractName(StructMainData::GetInstance().GetCPTInfo());
	case GetDBNameByNum::DwellIdx:
		return ExtractName(StructMainData::GetInstance().GetDwellInfo());
	case GetDBNameByNum::LCIdx:
		return ExtractName(StructMainData::GetInstance().GetLCINFO());
	}

	return _T("-");
}

// SwitchInfoType(EI_IP_DBStruct_Typedef.h)의 Kind.Nose / Kind.Double 비트플래그를 해석해
// 해당 Table Index(Idx)의 선로전환기가 "노스가동"인지, 쌍동인 경우 A호/B호 중 어느 쪽이
// NS-AM/MJ81(노스가동)인지 판정한다.
//   Kind.Nose  bit0=1 : 노스가동 활성
//              bit1=1 : A호=NS-AM,    B호=노스가동(MJ81) - 쌍동인 경우에만 의미 있음
//              bit2=1 : A호=노스가동(MJ81), B호=NS-AM    - 쌍동인 경우에만 의미 있음
//              bit1=0 && bit2=0 (쌍동) : A호/B호 모두 노스가동(MJ81)
//   Kind.Double bit0=1 : 쌍동
// [주의] Nose 값을 1/2/4로 "정확히 일치"하는 switch로 비교하면, bit0(노스가동 활성)과
// bit1/bit2가 함께 켜진 실제 값(예: 0x03, 0x05)을 못 잡고, 쌍동 여부(Kind.Double)도
// 구분하지 못해 단동 노스가동과 "bit1=bit2=0인 쌍동(A/B 모두 MJ81)"을 구별할 수 없다.
// 그래서 정확한 값 비교 대신 비트 단위(&)로 판정한다.
// 반환값은 GetNoseType(Nose/ANSAMBMJ81/AMJ81BNSAM/ABMJ81) 중 하나이며,
// 노스가동이 아니거나 Idx가 범위를 벗어나면 0을 반환한다.
int const CommonUtil::GetSwitchIsNose(Byte_t Idx)
{
	const auto switchInfo = StructMainData::GetInstance().GetSwitchInfo();

	// Table Index 범위 체크 (범위를 벗어나면 판단 불가)
	if (Idx >= switchInfo.size())
		return 0;

	const auto& kind = switchInfo[Idx].Kind;

	// 노스가동 활성 비트가 꺼져 있으면 노스가동이 아님
	if ((kind.Nose & 0x01) == 0)
		return 0;

	// 쌍동(Double bit0=1)인 경우에만 A호/B호 구분 비트(bit1/bit2)가 의미 있음
	if (kind.Double & 0x01)
	{
		if (kind.Nose & 0x02)       // A호 = NS-AM, B호 = 노스가동(MJ81)
			return CommonUtil::ANSAMBMJ81;
		if (kind.Nose & 0x04)       // A호 = 노스가동(MJ81), B호 = NS-AM
			return CommonUtil::AMJ81BNSAM;
		return CommonUtil::ABMJ81;  // bit1=0 && bit2=0 : A호/B호 모두 노스가동(MJ81)
	}

	// 단동 노스가동
	return CommonUtil::Nose;
}

std::string CommonUtil::GetKindNameForOutCard(UCHAR kindIO, UINT tblIdx, UCHAR bitNo)
{
	switch (kindIO)
	{
	case INP_TRACK:			return "궤도";
	case INP_SIGNAL:        return "신호기";
	case INP_SWITCH:        return "선로전환기";
	case INP_LMR:           return "LMR";
	case INP_ROUTE_SELECT: return "진로";
	case INP_STATION:       return "공통";
	case INP_BLOCK:         return "폐색";
	case INP_LEVEL_CROSS:   return "건널목 (고장검지)";
	case INP_LEVEL_CONTROL: return "건널목	 (제어건널목)";
	case INP_SPC_TRACK:     return "타역 궤도";
	case INP_SPC_SIGNAL:    return "타역 신호기";
	case INP_SPC_SWITCH:    return "타역 선로전환기";
	case INP_DEAD_SECTION:  return "전차선 절연구간";
	case INP_FALL_LOCK:     return "지장물";
	case INP_CPT_INFO:      return "열차진입방지";
	case INP_HEAT_INFO: return "히터";
	case INP_STL_INFO:      return "출발반응등";
	case INP_ETC_FAULT:     return "기타 고장";
	case INP_DWL_INFO:      return "소속역 정보";

	case INP_EIS_INFO:      return "연동장치";



	default:
		return "";
	}
}
// [구조체 변경] IO_Position_t -> IO_Position (선언부인 CommonUtils.h 와 시그니처 일치)
CString CommonUtil::GetSafeIOPosition(const IO_Position& io)
{
	// 모든 값이 0이거나 미설정 상태인 경우
	CString temp;
	temp.Format(_T("서브랙: %d, 모듈: %d, 카드(슬롯): %d, 포트: %d"),
		io.Chassis, io.ModuleNo, io.CardNo, io.PortNo);
	return temp;
}
CString CommonUtil::GetOriginNameByNumber(Byte_t nNum, GetDBNameByNum num)
{
	if (nNum == 0 || nNum == 0xFF)
	{
		return _T("-");
	}

	auto ExtractName = [&](const auto& List)->CString {
		int nIndex = static_cast<int>(nNum);
		if (nIndex >= List.size())
		{
			return _T("-");
		}
		CString strName = GetName(List[nIndex]);
		if (strName.IsEmpty()) return _T("-");
		return strName;
		};

	// StructMainData 대신 DataComparison의 원본(Original) 데이터를 참조합니다.
	switch (num)
	{
	case GetDBNameByNum::InterLockIdx:
		return ExtractName(DataComparison::GetInstance().GetOriginalSignal());
	case GetDBNameByNum::TrackIdx:
		return ExtractName(DataComparison::GetInstance().GetOriginalTrack());
	case GetDBNameByNum::SignalIdx:
		return ExtractName(DataComparison::GetInstance().GetOriginalSignal());
	case GetDBNameByNum::SwitchIdx:
		return ExtractName(DataComparison::GetInstance().GetOriginalSwitch());
	case GetDBNameByNum::BlockIdx:
		return ExtractName(DataComparison::GetInstance().GetOriginalBlock());
	case GetDBNameByNum::LevelIdx:
		return ExtractName(DataComparison::GetInstance().GetOriginalLevelCross());
	case GetDBNameByNum::DeadSectionIdx:
		return ExtractName(DataComparison::GetInstance().GetOriginalDeadSection());
	case GetDBNameByNum::FallLockIdx:
		return ExtractName(DataComparison::GetInstance().GetOriginalFallLock());
	case GetDBNameByNum::STLIdx:
		return ExtractName(DataComparison::GetInstance().GetOriginalSTL());
	case GetDBNameByNum::FaultIdx:
		return ExtractName(DataComparison::GetInstance().GetOriginalFault());
	case GetDBNameByNum::HeatIdx:
		return ExtractName(DataComparison::GetInstance().GetOriginalHeat());
	case GetDBNameByNum::CPTIdx:
		return ExtractName(DataComparison::GetInstance().GetOriginalCPT());
	case GetDBNameByNum::DwellIdx:
		return ExtractName(DataComparison::GetInstance().GetOriginalDWELL());
	case GetDBNameByNum::LCIdx:
		return ExtractName(DataComparison::GetInstance().GetOriginalLC());
	}

	return _T("-");
}

CString CommonUtil::GetDiffNameByNumber(Byte_t nNum, GetDBNameByNum num)
{
	if (nNum == 0 || nNum == 0xFF)
	{
		return _T("-");
	}

	auto ExtractName = [&](const auto& List)->CString {
		int nIndex = static_cast<int>(nNum);
		if (nIndex >= List.size())
		{
			return _T("-");
		}
		CString strName = GetName(List[nIndex]);
		if (strName.IsEmpty()) return _T("-");
		return strName;
		};

	// DataComparison의 비교군(Diff) 데이터를 참조합니다.
	switch (num)
	{
	case GetDBNameByNum::InterLockIdx:
		return ExtractName(DataComparison::GetInstance().GetDiffSignal());
	case GetDBNameByNum::TrackIdx:
		return ExtractName(DataComparison::GetInstance().GetDiffTrack());
	case GetDBNameByNum::SignalIdx:
		return ExtractName(DataComparison::GetInstance().GetDiffSignal());
	case GetDBNameByNum::SwitchIdx:
		return ExtractName(DataComparison::GetInstance().GetDiffSwitch());
	case GetDBNameByNum::BlockIdx:
		return ExtractName(DataComparison::GetInstance().GetDiffBlock());
	case GetDBNameByNum::LevelIdx:
		return ExtractName(DataComparison::GetInstance().GetDiffLevelCross());
	case GetDBNameByNum::DeadSectionIdx:
		return ExtractName(DataComparison::GetInstance().GetDiffDeadSection());
	case GetDBNameByNum::FallLockIdx:
		return ExtractName(DataComparison::GetInstance().GetDiffFallLock());
	case GetDBNameByNum::STLIdx:
		return ExtractName(DataComparison::GetInstance().GetDiffSTL());
	case GetDBNameByNum::FaultIdx:
		return ExtractName(DataComparison::GetInstance().GetDiffFault());
	case GetDBNameByNum::HeatIdx:
		return ExtractName(DataComparison::GetInstance().GetDiffHeat());
	case GetDBNameByNum::CPTIdx:
		return ExtractName(DataComparison::GetInstance().GetDiffCPT());
	case GetDBNameByNum::DwellIdx:
		return ExtractName(DataComparison::GetInstance().GetDiffDWELL());
	case GetDBNameByNum::LCIdx:
		return ExtractName(DataComparison::GetInstance().GetDiffLC());
	}

	return _T("---"); // 매칭되는 case가 없을 경우
}

// ==========================================================
// 번호 배열을 넘겨받아 이름을 반환하는 공용 함수
// ==========================================================
CString CommonUtil::GetDBNameByArray(const Byte_t* pArray, int nSize, GetDBNameByNum num)
{
	if (pArray == nullptr || nSize <= 0) return _T("-");

	// [해결책] 반복문(for) 로직을 'ExtractArrayNames'라는 익명 함수(람다)로 분리합니다.
	// auto& List 로 받기 때문에 신호기든 선로전환기든 구조체 타입에 상관없이 전부 받아줍니다.
	auto ExtractArrayNames = [&](const auto& List) -> CString {
		CString strTotalNames = _T("");
		bool bFound = false;

		for (int i = 0; i < nSize; i++) {
			Byte_t nSigNo = pArray[i];

			// 유효하지 않은 값이나 인덱스 범위 초과 시 건너뜀
			if (nSigNo == 0 || nSigNo == 0xFF) continue;
			if (nSigNo >= List.size()) continue;

			// List[nSigNo].Name 배열 크기가 20이라고 가정
			CString strName = GetName(List[nSigNo]);

			if (bFound) {
				strTotalNames += _T(", ");
			}
			strTotalNames += strName;
			bFound = true;
		}

		return strTotalNames;
		};

	// --------------------------------------------------------
	// 실제 로직 실행부 call back Type
	// --------------------------------------------------------
	switch (num)
	{
	case GetDBNameByNum::InterLockIdx:
		// 신호기 리스트를 가져와서 람다 함수에 전달
		return ExtractArrayNames(StructMainData::GetInstance().GetSignalInfo());
	case GetDBNameByNum::TrackIdx:
		return ExtractArrayNames(StructMainData::GetInstance().GetTrackInfo());
	case GetDBNameByNum::SignalIdx:
		return ExtractArrayNames(StructMainData::GetInstance().GetSignalInfo());
	case GetDBNameByNum::SwitchIdx:
		return ExtractArrayNames(StructMainData::GetInstance().GetSwitchInfo());
	case GetDBNameByNum::BlockIdx:
		return ExtractArrayNames(StructMainData::GetInstance().GetBlockInfo());
	case GetDBNameByNum::LevelIdx:
		return ExtractArrayNames(StructMainData::GetInstance().GetCossInfo());
	case GetDBNameByNum::DeadSectionIdx:
		return ExtractArrayNames(StructMainData::GetInstance().GetDeadSectionInfo());
	case GetDBNameByNum::FallLockIdx:
		return ExtractArrayNames(StructMainData::GetInstance().GetFallLockInfo());
	case GetDBNameByNum::STLIdx:
		return ExtractArrayNames(StructMainData::GetInstance().GetSTLInfo());
	case GetDBNameByNum::FaultIdx:
		return ExtractArrayNames(StructMainData::GetInstance().GetFaultInfo());
	case GetDBNameByNum::HeatIdx:
		return ExtractArrayNames(StructMainData::GetInstance().GetHeatInfo());
	case GetDBNameByNum::CPTIdx:
		return ExtractArrayNames(StructMainData::GetInstance().GetCPTInfo());
	case GetDBNameByNum::DwellIdx:
		return ExtractArrayNames(StructMainData::GetInstance().GetDwellInfo());
	case GetDBNameByNum::LCIdx:
		return ExtractArrayNames(StructMainData::GetInstance().GetLCINFO());

	}

	return _T("---"); // 매칭되는 case가 없을 경우
}