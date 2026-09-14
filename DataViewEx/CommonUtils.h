#pragma once

//이름 가져올 enum 정의

namespace CommonUtil
{
	//enum GetType
	//{
	//	StationIdx = 1,
	//	InterLockIdx,
	//	RouteIdx,
	//	TrackIdx,
	//	SignalIdx,
	//	SwitchIdx,
	//	BlockIdx,
	//	LevelIdx,
	//	DeadSectionIdx,
	//	FallLockIdx,
	//	STLIdx,
	//	FaultIdx,
	//	HeatIdx,
	//	InputIdx,
	//	IoCardIdx,
	//	CPTIdx,
	//	DwellIdx,
	//	LCIdx
	//};
	enum GetNoseType {
		Nose = 1,
		ANSAMBMJ81,
		AMJ81BNSAM,
		ABMJ81
	};
	enum GetDBNameByNum
	{
		InterLockIdx = 1,
		TrackIdx,
		SignalIdx,
		SwitchIdx,
		BlockIdx,
		LevelIdx,
		DeadSectionIdx,
		FallLockIdx,
		STLIdx,
		FaultIdx,
		HeatIdx,
		CPTIdx,
		DwellIdx,
		LCIdx
	};
	inline CString GetSafeString(const unsigned char* pRawData, int nMaxLength)
	{
		if (pRawData == nullptr || nMaxLength <= 0)
			return _T("");

		const char* pCharData = reinterpret_cast<const char*>(pRawData);
		int nActualLen = static_cast<int>(strnlen(pCharData, nMaxLength));
		if (nActualLen == 0) return _T("");

		return CString(pCharData, nActualLen);
	}
	inline CString GetSafeString(const char* pRawData, int nMaxLength)
	{
		// char*를 Byte_t*(unsigned char*)로 캐스팅하여 기존 함수 호출
		return GetSafeString(reinterpret_cast<const Byte_t*>(pRawData), nMaxLength);
	}
	//Wrapper
	template<size_t N>
	inline CString GetSafeString(const Byte_t(&rawData)[N])
	{
		return GetSafeString(rawData, static_cast<int>(N));
	}
	CString GetDBNameByArray(const Byte_t* pArray, int nSize, GetDBNameByNum num);
	//Wrapper
	template <size_t N>
	inline CString GetDBNameByArray(const Byte_t(&array)[N], GetDBNameByNum num)
	{
		// 배열 포인터와 알아낸 크기(N)를 원본 함수에 자동으로 넘겨줍니다.
		return GetDBNameByArray(array, static_cast<int>(N), num);
	}
	CString GetDBNameByNumber(Byte_t pNum, GetDBNameByNum num);

	/////////////////////////////////////////////////////
	///////////////  DB STRUCT GetName //////////////////
	/////////////////////////////////////////////////////
	inline CString GetName(const InterLockInfoType& item) {
		return GetSafeString(item.Name, _countof(item.Name));
	}
	inline const CString GetName(const TrackInfoType& item) {
		return GetSafeString(item.Name, _countof(item.Name));
	}
	inline const CString GetName(const SignalInfoType& item) {
		return GetSafeString(item.Name, _countof(item.Name));
	}
	inline const CString GetName(const SwitchInfoType& item) {
		return GetSafeString(item.Name, _countof(item.Name));
	}
	inline const CString GetName(const BlockTagInfoType& item) {
		return GetSafeString(item.Name, _countof(item.Name));
	}
	inline const CString GetName(const LevelCrossInfoType& item) {
		return GetSafeString(item.Name, _countof(item.Name));
	}
	inline const CString GetName(const DeadSectionInfoType& item) {
		return GetSafeString(item.Name, _countof(item.Name));
	}
	inline const CString GetName(const FallLockInfoType& item) {
		return GetSafeString(item.Name, _countof(item.Name));
	}
	inline const CString GetName(const STLInfoType& item) {
		return GetSafeString(item.Name, _countof(item.Name));

	}
	inline const CString GetName(const FaultInfoType& item) {
		return GetSafeString(item.Name, _countof(item.Name));
	}

	// szHeatName Byte_t
	inline const CString GetName(const HeatInfoType& item) {
		return GetSafeString(item.szHeatName, _countof(item.szHeatName));
	}
	//  CptName char
	inline const CString GetName(const CPTInfoType& item) {
		return GetSafeString(item.CptName, _countof(item.CptName));
	}
	// SubStrName Byte_t
	inline const CString GetName(const DwellInfoType& item) {

		return GetSafeString(item.SubStnName, _countof(item.SubStnName));
	}
	// Name1 Name2 Byte_t
	// [구조체 변경] LC_INFO_TYPE -> LC_CTRL_INFO_TYPE (EI_IP_DBStruct_Typedef.h)
	inline const CString GetName(const LC_CTRL_INFO_TYPE& item) {
		return GetSafeString(item.Name1, _countof(item.Name1)) +
			GetSafeString(item.Name2, _countof(item.Name2));
	}
	inline const CString GetIOName(IO_Position io) {
		CString str;
		str.Format(_T("R-%02d, S-%02d, P-%02d"), io.Chassis, io.CardNo, io.PortNo);
		return str;
	}

	/////////////////////////////////////////////////////
	///////////////  IN Card GetName ////////////////////
	/////////////////////////////////////////////////////
	// [구조체 변경] IN_CARDTABLE 이 RackNo/SlotNo + CardData[] (포트별) 배열을 갖는
	//              중첩 구조로 바뀌었으므로, 이름 조회는 포트 단위 구조체(_CardData_t) 기준으로 동작해야 함
	inline const CString GetName(const IN_CARDTABLE::_CardData_t& item)
	{
		return GetSafeString(item.Name, _countof(item.Name));
	}
	inline const CString GetBitName(const IN_CARDTABLE::_CardData_t& item)
	{
		return GetSafeString(item.BitName, _countof(item.BitName));
	}
	inline const CString GetTagName(const IN_CARDTABLE::_CardData_t& item)
	{
		return GetSafeString(item.TagName, _countof(item.TagName));
	}
	/////////////////////////////////////////////////////
	///////////////  OUT Card GetName ////////////////////
	/////////////////////////////////////////////////////
	// [구조체 변경] OUT_CARDTABLE 도 동일하게 CardData[] 배열을 갖는 중첩 구조로 변경됨
	inline const CString GetName(const OUT_CARDTABLE::_CardData_t& item)
	{
		return GetSafeString(item.Name, _countof(item.Name));
	}
	inline const CString GetBitName(const OUT_CARDTABLE::_CardData_t& item)
	{
		return GetSafeString(item.BitName, _countof(item.BitName));
	}
	inline const CString GetTagName(const OUT_CARDTABLE::_CardData_t& item)
	{
		return GetSafeString(item.TagName, _countof(item.TagName));
	}
	inline const CString GetName(std::shared_ptr<LOGIC_VARIABLE>& item)
	{
		return GetSafeString(item->szVarName, _countof(item->szVarName));
	}

	/////////////////////////////////////////////////////
	///////////////  SIG Card GetName ////////////////////
	/////////////////////////////////////////////////////
	inline const CString GetName(const SIG_CARDTABLE::_CardData_t& item)
	{
		return GetSafeString(item.Name, _countof(item.Name));
	}
	inline const CString GetTagName(const SIG_CARDTABLE::_CardData_t& item)
	{
		return GetSafeString(item.TagName, _countof(item.TagName));
	}

	/////////////////////////////////////////////////////
	///////////////  SWH Card GetName ////////////////////
	/////////////////////////////////////////////////////
	inline const CString GetName(const SWH_CARDTABLE::_CardData_t& item)
	{
		return GetSafeString(item.Name, _countof(item.Name));
	}
	inline const CString GetTagName(const SWH_CARDTABLE::_CardData_t& item)
	{
		return GetSafeString(item.TagName, _countof(item.TagName));
	}

	inline const CString GetCardType(const char& CardType) {
		switch (CardType) {
		case 1:
			return _T("IN");
		case 2:
			return _T("OUT");

		case 4:
			return _T("POD");
		default:
			return _T("");
		}
	}
	inline const CString GetKindName(const char& item)
	{
		switch (item) {
		case INP_VRD:
			return _T("VRD");
		case INP_TRACK:
			return _T("궤도");
		case INP_SIGNAL:
			return _T("신호기");
		case INP_SWITCH:
			return _T("선로전환기");
		case INP_LMR:
			return _T("LMR");
		case INP_ROUTE_SELECT:
			return _T("진로");
		case INP_STATION:
			return _T("역공통");
		case INP_BLOCK:
			return _T("폐색");
		case INP_LEVEL_CROSS:
			return _T("건널목 (고장검지)");
		case INP_LEVEL_CONTROL:
			return _T("건널목 (제어건널목)");
		case INP_SPC_TRACK:
			return _T("타역 궤도");
		case INP_SPC_SIGNAL:
			return _T("타역 신호기");
		case INP_SPC_SWITCH:
			return _T("타역 선로전환기");
		case INP_DEAD_SECTION:
			return _T("전차선 절연구간");
		case INP_FALL_LOCK:
			return _T("지장물");
		case INP_STL_INFO:
			return _T("출발반응등");
		case INP_ETC_FAULT:
			return _T("기타 고장");
		case INP_DWL_INFO:
			return _T("소속역 정보");
		case INP_CPT_INFO:
			return _T("열차진입 방지(CPT)");
		case INP_ATTRACTION_INFO:
			return _T("끌림 감시장치");
		case INP_SLOW_ORDER:
			return _T("임시속도");
		case INP_ROUTE_INDICATOR:
			return _T("진로표시등");
		case INP_HEAT_INFO:
			return _T("히터");
		case INP_EIS_INFO:
			return _T("연동장치");
		default:
			return _T("");
		}
	}
	// SwitchInfoType.Kind.Nose/Kind.Double 비트플래그를 해석해 Table Index(Idx)의 선로전환기가
	// 노스가동인지, 쌍동인 경우 A호/B호 중 어느 쪽이 NS-AM/MJ81인지 판정한다. (구현: CommonUtils.cpp)
	// 반환값: GetNoseType(Nose/ANSAMBMJ81/AMJ81BNSAM/ABMJ81) 중 하나, 해당 없음/범위초과 시 0
	int const GetSwitchIsNose(Byte_t Idx);

	inline CString GetOUTKInd(Byte_t Kind, Byte_t outKind) {
		switch (outKind) {
		case 1: if (Kind == 'S') return _T("신호기 ATS 출력");
		case 2: if (Kind == 'S') return _T("신호기 후방제어 출력");
		case 3: if (Kind == 'E') return _T("비상정지 출력");
		case 4: if (Kind == 'Y') return _T("임시속도 출력");
		case 5: if (Kind == 'E') return _T("주계 출력");
		case 6: if (Kind == 'E') return _T("운영모드 출력");
		case 7: return _T("Switch Heater 출력");
		case 8: return _T("전차선 출력");
		default: return _T("");
		}
	}
	inline CString GetOutGubun(Byte_t outKind, Byte_t OutGubun) {
		switch (outKind) {
		case 1:
			switch (OutGubun) {
			case 1:return _T("YY");
			case 2:return _T("Y");
			case 3:return _T("YG");
			case 4:return _T("G");
			default: return _T("");
			}
		case 2:
			switch (OutGubun)
			{
			case 1:return _T("YY");
			case 2:return _T("Y");
			case 3:return _T("YG");
			case 4:return _T("G");
			default: return _T("");
			}

		case 3:
			switch (OutGubun)
			{
			case 1: return(_T("상선"));
			case 2: return(_T("하선"));
			default: return _T("");
			}
		case 4:
			switch (OutGubun) {
			case 0:
				return _T("0:자기역 임시속도 출력");
			case 1:
				return _T("1:인접역 임시속도 출력");
			default: return _T("");
			}

		case 5:
			switch (OutGubun) {
			case 1: return _T("1계 출력");
			case 2: return _T("2계 출력");
			default: return _T("");
			}
		case 6:
			switch (OutGubun) {
			case 1: return _T("Local 출력");
			case 2: return _T("CTC 출력");
			default: return _T("");
			}
		default: return _T("");
		}
	}
	inline CString const GetKindLogicTime(const char& time) {
		switch (time) {
		case 2:    return _T("DELAYED_ON");
		case 3:    return _T("DELAYED_OFF");
		case 4:    return _T("DELAYED_ON_OFF");
		case 5:    return _T("OSCILLATOR");
		case 6:    return _T("TRIGGER");
		default: return _T("");
		}
	}
	inline CString const GetSpeedT(const char& bitNo) {
		switch (bitNo)
		{
		case 1: return _T("25km");
		case 2: return _T("40km");
		case 3: return _T("60km");
		case 4: return _T("70km");
		case 5: return _T("80km");
		case 6: return _T("CAP");
		case 7: return _T("YD");
		case 8: return _T("YC");
		default:
			return _T("");
		}
	}
	std::string GetKindNameForOutCard(UCHAR kindIO, UINT tblIdx, UCHAR bitNo);

	// [구조체 변경] IO_Position_t -> IO_Position (EI_IP_DBStruct_Typedef.h 의 최종 typedef 명은 IO_Position)
	CString GetSafeIOPosition(const IO_Position& io);

	inline CommonUtil::GetDBNameByNum GetDBNameNumFromKind(char kind)
	{
		switch (kind) {
		case INP_TRACK:          return CommonUtil::TrackIdx;
		case INP_SIGNAL:         return CommonUtil::SignalIdx;
		case INP_SWITCH:         return CommonUtil::SwitchIdx;
		case INP_BLOCK:          return CommonUtil::BlockIdx;
		case INP_LEVEL_CROSS:
		case INP_LEVEL_CONTROL:  return CommonUtil::LevelIdx;
		case INP_DEAD_SECTION:   return CommonUtil::DeadSectionIdx;
		case INP_FALL_LOCK:      return CommonUtil::FallLockIdx;
		case INP_STL_INFO:       return CommonUtil::STLIdx;
		case INP_ETC_FAULT:      return CommonUtil::FaultIdx;
		case INP_HEAT_INFO:      return CommonUtil::HeatIdx;
		case INP_CPT_INFO:       return CommonUtil::CPTIdx;
		case INP_DWL_INFO:       return CommonUtil::DwellIdx;
		default:                 return (CommonUtil::GetDBNameByNum)0;
		}
	}
	inline CString GetBlockTextSBR(const CString& strSigName, BYTE blkKind, bool bIsAddSpace)
	{
		if (strSigName.IsEmpty())
			return _T("");

		// 신호기 이름에서 알파벳 등 제거하고 숫자만 추출 (예: "14RA" -> "14")
		CString strNumOnly;
		for (int i = 0; i < strSigName.GetLength(); ++i)
		{
			if (_istdigit(strSigName[i]))
			{
				strNumOnly += strSigName[i];
			}
		}

		if (strNumOnly.IsEmpty())
			return _T("");

		// 북부(홀수) / 남부(짝수) 판별 (1 = 북부, 0 = 남부)
		bool bIsNorth = (_ttoi(strNumOnly) % 2 == 1);

		// 폐색 종류(BlkKind)에 따른 문자열 결정
		CString strTextSBR;
		switch (blkKind)
		{
		case BLK_SINGLE_ABS:            // 단선자동 폐색 (3현시)
		case BLK_SINGLE_ABS_5ASPECT:    // 단선자동 폐색 (5현시)
		case BLK_SINGLE_REL:            // 단선연동
			strTextSBR = bIsNorth ? _T("3BR") : _T("4BR");
			break;

			// case BLK_DOUBLE_REL:
			// case BLK_DOUBLE_ABS:
			// case BLK_DOUBLE_ABS_3ASPECT:
			// case BLK_BOTH_ABS1:
		case BLK_BOTH_ABS2:             // 양방향 3현시
		case BLK_BOTH_ABS2_5ASPECT:     // 양방향 5현시
		case BLK_EXPRESS:               // 고속선 폐색
			strTextSBR = bIsNorth ? _T("3SBR") : _T("4SBR");
			break;
		}

		// 서식 지정 (출력용 여부 및 공백 추가)
		//if (!strTextSBR.IsEmpty())
		//{
		//	strTextSBR += _T("(  )");

		//}

		return strTextSBR;
	}
	CString GetOriginNameByNumber(Byte_t nNum, GetDBNameByNum num);
	CString GetDiffNameByNumber(Byte_t nNum, GetDBNameByNum num);
}