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
	case GetDBNameByNum::InterLockIdx: // [수정] 기존에는 신호기 테이블을 조회하고 있었음 -> 연동(진로) 테이블
		return ExtractName(StructMainData::GetInstance().GetInterLockInfo());
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

// [추가] 진로 번호(Word_t, 1 ~ MAX_ROUTE-1)로 연동도표(_ILK_Info) 진로 이름 조회
CString CommonUtil::GetRouteNameByNumber(Word_t nRteNo)
{
	if (nRteNo == 0 || nRteNo == 0xFFFF)
		return _T("-");

	auto ilkList = StructMainData::GetInstance().GetInterLockInfo();
	if (nRteNo >= ilkList.size())
		return _T("-");

	CString strName = GetName(ilkList[nRteNo]);
	if (strName.IsEmpty()) return _T("-");
	return strName;
}

// ==========================================================
// [추가] Kind + TableIdx -> DB 테이블 구분 및 설비 이름
// ==========================================================
CString CommonUtil::GetTableNameByKind(Byte_t kind, UINT tableIdx)
{
	auto& md = StructMainData::GetInstance();
	if (md.GetEIDBStruct() == nullptr) return _T("");

	// 테이블 라벨 + 설비 이름. 인덱스가 범위를 벗어나거나 이름이 없으면 "라벨 : -"
	auto Pick = [&](const auto& list, LPCTSTR label) -> CString {
		CString strName;
		if (tableIdx > 0 && tableIdx < list.size())
			strName = GetName(list[tableIdx]);
		if (strName.IsEmpty()) strName = _T("-");
		return CString(label) + _T(" : ") + strName;
	};

	switch (kind)
	{
	case 'T': return Pick(md.GetTrackInfo(), _T("궤도"));
	case 't': return Pick(md.GetTrackInfo(), _T("타역 궤도"));
	case 'S': return Pick(md.GetSignalInfo(), _T("신호기"));
	case 's': return Pick(md.GetSignalInfo(), _T("타역 신호기"));
	case 'L': return Pick(md.GetSignalInfo(), _T("신호기(LMR)"));
	case 'P': return Pick(md.GetSwitchInfo(), _T("선로전환기"));
	case 'p': return Pick(md.GetSwitchInfo(), _T("타역 선로전환기"));
	case 'B': return Pick(md.GetBlockInfo(), _T("폐색"));
	case 'C': return Pick(md.GetCossInfo(), _T("건널목"));
	case 'c': return Pick(md.GetLCINFO(), _T("제어건널목"));
	case 'D': return Pick(md.GetDeadSectionInfo(), _T("절연구간"));
	case 'J': return Pick(md.GetFallLockInfo(), _T("지장물"));
	case 'H': return Pick(md.GetCPTInfo(), _T("CPT"));
	case 'h': return Pick(md.GetHeatInfo(), _T("히터"));
	case 'G': return Pick(md.GetAttractionInfo(), _T("끌림감시"));
	case 'Y': return Pick(md.GetSlowOrderInfo(), _T("임시속도"));
	case 'K': return Pick(md.GetSTLInfo(), _T("출발반응등"));
	case 'F': return Pick(md.GetFaultInfo(), _T("기타고장"));
	case 'W': return Pick(md.GetDwellInfo(), _T("소속역"));
	case 'R': return CString(_T("진로 : ")) + GetRouteNameByNumber(static_cast<Word_t>(tableIdx));
	case 'U': return _T("진로선별등");
	case 'N': return _T("역공통");
	case 'V': return _T("VRD");
	case 'E': return _T("연동장치");
	default:  return _T("");
	}
}

// ==========================================================
// [추가] Kind + TableIdx + BitNo -> 비트 의미 (Table BitNo 정의 Rev 1.0)
// ==========================================================
CString CommonUtil::GetBitNoName(Byte_t kind, UINT tableIdx, Byte_t bitNo)
{
	auto& md = StructMainData::GetInstance();
	if (md.GetEIDBStruct() == nullptr) return _T("");

	using BitTable = std::initializer_list<std::pair<int, LPCTSTR>>;
	auto Find = [&](BitTable tbl) -> CString {
		for (const auto& e : tbl)
			if (e.first == bitNo) return e.second;
		CString str;
		str.Format(_T("미정의(%d)"), bitNo);
		return str;
	};
	auto InRange = [&](int nFrom, int nTo, LPCTSTR label) -> CString {
		CString str;
		if (bitNo >= nFrom && bitNo <= nTo) str.Format(_T("%s %d"), label, bitNo - nFrom + 1);
		return str;
	};
	auto signalList = md.GetSignalInfo();
	const SignalInfoType* pSig = (tableIdx > 0 && tableIdx < signalList.size()) ? &signalList[tableIdx] : nullptr;

	switch (kind)
	{
	case 'S': // 신호기 (3.1)
	case 's': // 타역 신호기 : 입력 HR (신호기와 동일 BitNo 사용)
		// 폐색 신호기는 BitNo 0 이 고장정보(FAIL)
		if (bitNo == 0 && pSig != nullptr && (pSig->Kind.BlockS & 0x01))
			return _T("FAIL");
		return Find({ {0, _T("YR")}, {1, _T("GR")}, {2, _T("")}, {3, _T("REAR(후방폐색 제어출력)")}, {4, _T("HR")},
			{5, _T("ATS")}, {6, _T("ULMR(진로선별등)")}, {7, _T("SHR((무)유도-주신호기)")}, {8, _T("HCR(입환신호기 유도출력)")} });

	case 'L': // 신호기 LMR (3.2) - 중계 신호기는 GLMR(0) / CLMR(6)
		if (pSig != nullptr && (pSig->Kind.RepeatS & 0x01))
			return Find({ {0, _T("GLMR(중계)")}, {6, _T("CLMR(중계)")} });
		return Find({ {0, _T("GMLMR")}, {1, _T("GALMR")}, {2, _T("YMLMR")}, {3, _T("YALMR")},
			{4, _T("Y1MLMR")}, {5, _T("Y1ALMR")}, {6, _T("RMLMR")}, {7, _T("RALMR")} });

	case 'P': // 선로전환기 (3.3)
	case 'p': // 타역 선로전환기
		return Find({ {0, _T("(A)KR-N")}, {1, _T("(A)KR-R")}, {2, _T("BKR-N")}, {3, _T("BKR-R")},
			{11, _T("WLR")}, {12, _T("WR-N")}, {13, _T("WR-R")},
			{14, _T("(A)pNK")}, {15, _T("(A)pRK")}, {16, _T("(A)fNK")}, {17, _T("(A)fRK")}, {18, _T("PHPR")},
			{19, _T("BpNK")}, {20, _T("BpRK")}, {21, _T("BfNK")}, {22, _T("BfRK")},
			{24, _T("MCR")}, {25, _T("(A)pMC")}, {26, _T("(A)fMC")}, {27, _T("BpMC")}, {28, _T("BfMC")}, {29, _T("GCPR")} });

	case 'B': // 폐색 (3.4) - 폐색 종류(BlkKind) 별로 BitNo 의미가 다름
	{
		auto blockList = md.GetBlockInfo();
		if (tableIdx == 0 || tableIdx >= blockList.size()) return _T("");
		switch (blockList[tableIdx].BlkKind)
		{
		case BlockInfo_BlkKind::DoubleInterlocking: // 연동폐색
		case BlockInfo_BlkKind::SingleInterlocking:
			return Find({ {0, _T("ATpsr")}, {1, _T("AFr")}, {2, _T("AEhar")}, {3, _T("ABor")},
				{4, _T("DTpsr")}, {5, _T("DFr")}, {6, _T("Dar")}, {7, _T("DBor")},
				{11, _T("START")}, {12, _T("HOME")}, {13, _T("OPEN")}, {14, _T("CANCEL")} });
		case BlockInfo_BlkKind::DaeyaBlock: // 대야폐색
			return Find({ {0, _T("장내Y")}, {1, _T("장내R")}, {2, _T("출발Y")}, {3, _T("출발R")}, {4, _T("TPSR")},
				{11, _T("START")}, {12, _T("HOME")}, {13, _T("OPEN")}, {14, _T("CANCEL")} });
		case BlockInfo_BlkKind::SubwayBlock: // 지하철폐색
			return Find({ {0, _T("출발")}, {1, _T("개통")}, {2, _T("장내")}, {3, _T("신호")} });
		case BlockInfo_BlkKind::DoubleAuto_5Aspect: // 복선자동
			return Find({ {0, _T("YY")}, {1, _T("Y")}, {2, _T("YG")} });
		case BlockInfo_BlkKind::HighSpeedBlock: // 고속선폐색
			return Find({ {0, _T("YY")}, {1, _T("Y")}, {2, _T("YG")}, {14, _T("CNR")} });
		case BlockInfo_BlkKind::SingleAuto_3Aspect: // 단선자동
			return Find({ {0, _T("iDir")}, {1, _T("oBR")}, {2, _T("oDR")}, {3, _T("BLTR")},
				{4, _T("YY")}, {5, _T("Y")}, {6, _T("YG")}, {11, _T("RR")}, {14, _T("CNR")} });
		case BlockInfo_BlkKind::UiwangBlock: // 의왕폐색
			return Find({ {0, _T("HR")}, {1, _T("iBHR")}, {2, _T("TR")}, {3, _T("oTPSR")}, {4, _T("eHR")},
				{11, _T("START")}, {14, _T("CNR")} });
		case BlockInfo_BlkKind::BiDirectional: // 양방향폐색(출발정방향) : 정방향출발 && 역방향장내
			return Find({ {0, _T("oYY")}, {1, _T("oY")}, {2, _T("oYG")}, {4, _T("oRR")}, {5, _T("iRR")},
				{6, _T("iRDir")}, {7, _T("BLTR")}, {11, _T("ZDIR")}, {12, _T("RR")}, {14, _T("CNR")} });
		case BlockInfo_BlkKind::BiDirectional_3Aspect: // 양방향폐색(출발역방향) : 정방향장내 && 역방향출발
			return Find({ {0, _T("oBR")}, {1, _T("oDR")}, {2, _T("CNR")}, {4, _T("oZR")}, {5, _T("iZR")},
				{6, _T("iZDir")}, {7, _T("BLTR")}, {11, _T("ZDIR")}, {12, _T("ZR")}, {13, _T("OCCR")} });
		default: // 통표/삼각선/청량리 폐색은 문서에 BitNo 정의 없음
			return _T("");
		}
	}

	case 'R': // 진로
		return Find({ {1, _T("진로선별등")} });

	case 'D': // 전차선 절연구간
		return Find({ {1, _T("1계")}, {2, _T("2계")}, {3, _T("운용")} });

	case 'J': // 지장물
		return Find({ {1, _T("낙석")}, {2, _T("보호")}, {3, _T("표시(해제)")} });

	case 'h': // 히터
	{
		if (bitNo == 0) return _T("히터 동작(Feedback)");
		if (bitNo == 1) return _T("FAIL");
		if (bitNo >= 11 && bitNo <= 20)
		{
			CString str;
			str.Format(_T("알람DR %d"), bitNo - 10);
			auto heatList = md.GetHeatInfo();
			if (tableIdx > 0 && tableIdx < heatList.size())
			{
				CString strMsg = GetSafeString(heatList[tableIdx].AlmData[bitNo - 11].szAlmMsg);
				if (!strMsg.IsEmpty()) str += _T(" : ") + strMsg;
			}
			return str;
		}
		return Find({});
	}

	case 'C': // 건널목(고장검지)
	case 'c': // 제어건널목
	case 'H': // 열차진입방지(CPT)
	case 'G': // 끌림감시장치
	case 'Y': // 임시속도
	case 'K': // 출발반응등(STL) - EI_IP_IOCard_Typedef.h INP_STL_INFO / Logic_define.h LOGIC_BITNO_STL_IN(0)
	          // (BitNo 정의서 3.9 제목의 "(W)" 는 오기. 'W' 는 소속역(_DWL_Info) 이며 BitNo 정의 없음)
	case 'F': // 기타고장
		return Find({ {0, _T("인덱스 구분")} });

	case 'N': // 공통
	{
		CString str;
		if (!(str = InRange(14, 33, _T("연동논리부 정류기"))).IsEmpty()) return str;
		if (!(str = InRange(36, 55, _T("EI FUSE"))).IsEmpty()) return str;
		if (!(str = InRange(62, 65, _T("AF정류기"))).IsEmpty()) return str;
		if (!(str = InRange(66, 69, _T("계전기랙 정류기"))).IsEmpty()) return str;
		if (!(str = InRange(70, 77, _T("속도코드 정류기"))).IsEmpty()) return str;
		if (!(str = InRange(81, 88, _T("임시속도(SO)"))).IsEmpty()) return str;
		if (!(str = InRange(89, 100, _T("과주방지"))).IsEmpty()) return str;
		return Find({ {1, _T("SYSVRD1")}, {2, _T("SYSVRD2")}, {3, _T("비상 RUN")}, {4, _T("비상 CTC")},
			{8, _T("N1")}, {9, _T("N1-X1")}, {10, _T("N2")}, {11, _T("N2-X2")}, {12, _T("UPS")}, {13, _T("UPS AC")},
			{34, _T("축전지")}, {35, _T("FUSE(계전기랙)")}, {56, _T("FUSE EIS-1")}, {57, _T("FUSE EIS-2")},
			{58, _T("P남")}, {59, _T("P북")}, {60, _T("출입문")}, {61, _T("입환소등")},
			{78, _T("GDU")}, {79, _T("EMZ")}, {80, _T("EMS")} });
	}

	default: // 궤도(T/t), VRD(V), 연동장치(E), 출력(O), 진로선별등(U), 소속역(W) : 문서에 BitNo 정의 없음
		return _T("");
	}
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
	case GetDBNameByNum::InterLockIdx: // [수정] 신호기 -> 연동(진로) 테이블
		return ExtractName(DataComparison::GetInstance().GetOriginalInterLock());
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
	case GetDBNameByNum::InterLockIdx: // [수정] 신호기 -> 연동(진로) 테이블
		return ExtractName(DataComparison::GetInstance().GetDiffInterLock());
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
	case GetDBNameByNum::InterLockIdx: // [수정] 신호기 -> 연동(진로) 테이블
		return ExtractArrayNames(StructMainData::GetInstance().GetInterLockInfo());
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