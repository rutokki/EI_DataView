#pragma once

enum class EGridType
{
	Track = 0,
	Signal,
	Switch,
	Device,
	INCard,
	OUTCard,
	SignalCard,
	SwitchCard,
	LogicVariable,
	InterLockingData,
	Test,
	Unknown
};
struct ColumnInfo {
	CString columnName;
	int columnWidth;
};
//struct detailCoulumnInfo {
//	CString columnType;
//	CString columnName;
//	int columnWidth;
//};

class GridColumnDefine
{
public:
	static std::vector<ColumnInfo> GetColumnInfo(EGridType type)
	{
		switch (type)
		{
		case EGridType::Track:          return GetTrackColumnInfo();
		case EGridType::Signal:         return GetSignalColumnInfo();
		case EGridType::Switch:         return GetSwitchColumnInfo();
		case EGridType::Device:         return GetDeviceColumnInfo();
		case EGridType::INCard:         return GetINCardColumnInfo();
		case EGridType::OUTCard:        return GetOUTCardColumnInfo();
		case EGridType::SignalCard:     return GetSignalCardColumnInfo();
		case EGridType::SwitchCard:     return GetSwitchCardColumnInfo();
		case EGridType::LogicVariable:  return GetLogicVariableColumnInfo();
		case EGridType::InterLockingData: return GetInterLockingDataColumnInfo();
		default:                        return {};
		}
	}
	// 나중 ENUM타입 변수를 Switch문으로 넘겨 케이스 판별후 그리드 반환하는 함수 작성 
	// 궤도 칼럼 20~ 150 궤도 종류 묶음
	static std::vector<ColumnInfo> GetTrackColumnInfo()
	{ //7050
		return {
			{_T("궤도명"), 30},{_T("접근"), 20}, {_T("보류"), 20}, {_T("본선"), 20},
			{_T("폐색"), 20},{_T("신호기"), 20},{_T("폐색 신호기"),30}, {_T("선로전환기"), 20},
			{_T("접근 벨"), 20}, {_T("폐로쇄정"), 25},{_T("CPT"), 20},
			{_T("안전"),20}, {_T("장애x"), 25}, {_T("이상복구x"), 30}, {_T("타역"), 20},
			{_T("가상"), 20},{_T("제어불가"), 30},{_T("상선"), 20},{_T("하선"), 20}, {_T("Left"), 30}, {_T("Right"), 30},
			{_T("포함 신호기"), 50},{_T("포함 선로전환기"), 50},
			{_T("신호기 수"), 30}, {_T("선로전환기 수"), 35},
			{_T("폐색"), 25},

			{_T("복구시간"), 30}
		};
	}

	static std::vector<ColumnInfo> GetSignalColumnInfo()
	{
		//신호기 칼럼 2 ~ 12 신호기 종류 묶음   , 18 ~ 20 신호기 출력 묶음
		return {
			{_T("명칭"), 25}, {_T("주 신호기"), 35}, {_T("입환 표지"), 40},
			{_T("입환"), 25}, {_T("폐색"), 25},{_T("구내 폐색"), 40}, {_T("유도등포함"), 40},
			{_T("중계"), 25}, {_T("중계 포함"), 35},{_T("엄호"), 25}, {_T("TTB"), 25}, {_T("CPT"), 25},
			{_T("타역"), 25},{_T("현시 수"), 25},{_T("진로 수"),25},{_T("신호기 궤도"),35},
			{_T("전방신호기"), 40},{_T("후방폐색현시"), 40},{_T("ATS출력"), 30},{_T("후방출력"), 30}
		};
	}
	static std::vector<ColumnInfo> GetSwitchColumnInfo()
	{
		//선로전환기 칼럼 1 ~ 8 선로전환기 종류 묶음 , 9 ~ 10 방향 묶음
		// [수정] 인덱스 정합성 정리 (SwitchGridInfo.cpp와 1:1로 맞춤, 총 17개 칼럼 0~16):
		//  0:명칭 1:단동 2:쌍동 3:삼동 4:사동 5:시서스 6:노스가동 7:표시 8:타역 9:정위 10:반위
		//  11:A호 12:B호 13:A점 14:B점 15:C점 16:D점
		// [수정] 사용자 확인 - "A호"/"B호"는 명칭 칼럼이 아니라 이 선로전환기가 NS-AM인지 MJ81인지
		// 판별한 결과만 표시하는 칼럼임 (명칭은 이미 0번 "명칭" 칼럼 하나로 충분). 이전에 만들었던
		// "A호구분"/"B호구분" 분리 칼럼과 명칭 병기는 되돌림.
		return {
			 {_T("명칭"), 25}, {_T("단동"), 25}, {_T("쌍동"), 25}, {_T("삼동"), 25},
			 // [수정] "노스가동" 칼럼은 다른 종류 칼럼처럼 O 색표시만 한다 (사용자 확인: 노스가동
			 // 색표시만) - NS-AM인지 MJ81인지 판별 결과는 11~12번(A호/B호) 칼럼에 따로 표시.
			 {_T("사동"), 25},{_T("시서스"), 35}, {_T("노스가동"), 25}, {_T("표시"), 25},
			 {_T("타역"), 25}, {_T("정위"),25}, {_T("반위"),25},
			 // [수정] SwitchInfoType 자체가 아니라 SWH_CARDTABLE(카드/포트, EI_IP_IOCard_Typedef.h)의
			 // NoseAB에 있는 정보라 SwitchGridInfo.cpp에서 Table Index로 카드 테이블을 교차 조회해서
			 // 이 스위치가 카드상 A호/B호 중 어느 쪽인지 판별한 뒤, 그 쪽이 NS-AM인지 MJ81인지(단동인
			 // 경우 "노스가동")만 채움 (노스가동인 경우만).
			 {_T("A호"), 60}, {_T("B호"), 60},
			 {_T("A점"), 25}, {_T("B점"), 25}, {_T("C점"), 25}, {_T("D점"), 25}
		};
	}
	static std::vector<ColumnInfo> GetDeviceColumnInfo()
	{
		return {
			{_T("분류") ,30}, {_T("명칭"), 40}, {_T("구분"), 80}, {_T("종류"), 80}, {_T("Data"), 450}
		};
	}
	static std::vector<ColumnInfo> GetINCardColumnInfo() {
		return {
			{_T("Rack No"), 20}, {_T("Slot No"), 20}, {_T("Card No"), 20}, {_T("Port No"), 20},
			{_T("이름"), 70}, {_T("비트이름"), 70}, {_T("종류"), 100}, {_T("Table Index"), 55 }, {_T("BitNo"), 55}
		};
	}
	static std::vector<ColumnInfo> GetOUTCardColumnInfo() {
		return {
			 {_T("Rack No"), 20}, {_T("Slot No"), 20}, {_T("Card No"), 20}, {_T("Port No"), 20},
			{_T("이름"), 70}, {_T("비트이름"), 70}, {_T("종류"), 50}, {_T("Table Index"), 55}, {_T("BitNo"), 55}, {_T("출력 구분"), 100}
		};
		//	return {
		// {_T("Rack No"), 20}, {_T("Slot No"), 20}, {_T("Card No"), 20}, {_T("Port No"), 20},
		//{_T("이름"), 70}, {_T("계전기"), 70}, {_T("종류"), 100}, {_T("Table Index"), 55}, {_T("속도코드"),40}
		//	};
	}
	static std::vector<ColumnInfo> GetSignalCardColumnInfo() {
		return {
			 {_T("Rack No"), 20}, {_T("Slot No"), 20}, {_T("Card No"), 20},{_T("Port No"), 20} , {_T("신호기 카드 종류"), 60}
			, {_T("이름"), 50}, {_T("표찰이름"), 50}, {_T("Table Index"), 55}
		};
	}
	static std::vector<ColumnInfo> GetSwitchCardColumnInfo() {
		return {
			 {_T("Rack No"), 20}, {_T("Slot No"), 20}, {_T("Card No"), 20},{_T("Port No"), 20},  {_T("선로전환기 카드 종류"), 70}
			,{_T("노스가동"),30},{_T("이름"), 50}, { _T("표찰이름"), 50 },
			{ _T("노스 종류"), 100 }, { _T("Table Index"), 55 }
		};
	}
	//static std::vector<ColumnInfo> GetPDOCardColumnInfo() {
	//	return {
	//		 {_T("Rack No"), 45}, {_T("Slot No"), 45}, {_T("Card No"), 45}, {_T("Port No"), 45},
	//		{_T("이름"), 50}, {_T("정위(+)/반위(-)"), 100},{_T("Table Index"), 55}
	//	};
	//}
	static std::vector<ColumnInfo> GetLogicVariableColumnInfo() {
		return {
			{_T("로직IDX"), 35},{_T("명칭"), 100},{_T("연동구분"), 55},{_T("연동IDX"), 35},
			{_T("Card Type"), 45},{_T("Rack No"), 35},{_T("Slot No"), 35},{_T("Port No"), 35},{_T("로직구분"), 55},
			{_T("로직Type"), 45},{_T("Timer"), 70},{_T("Time"), 35},{_T("DB_Time"), 35}
		};
	}
	static std::vector<ColumnInfo> GetInterLockingDataColumnInfo() {
		return {
			{_T("번호"), 25},{_T("진로명"), 40},{_T("Data"), 300}
		};

	}

};