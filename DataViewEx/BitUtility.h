#pragma once

#include <afx.h>

inline bool IsBitSet(BYTE val, int bitIndex)
{
	if (bitIndex < 0 || bitIndex >7) {
		return false;
	}
	return (val & (1 << bitIndex)) != 0;
}

namespace BlockInfo_BlkKind
{
	constexpr BYTE DoubleAuto_5Aspect = 0x01;  // 복선자동 5현시(YY, Y, YG) 1
	constexpr BYTE SingleAuto_3Aspect = 0x02;  // 단선자동 3현시(BR, DR) 2
	constexpr BYTE DoubleInterlocking = 0x03;  // 복선연동 3
	constexpr BYTE SingleInterlocking = 0x04;  // 단선연동 4
	constexpr BYTE TokenBlock = 0x05;  // 통표 폐색 5
	constexpr BYTE SubwayBlock = 0x06;  // 지하철 폐색 6
	constexpr BYTE DaeyaBlock = 0x07;  // 대야   폐색 7
	constexpr BYTE UiwangBlock = 0x08;  // 의왕   폐색 8
	constexpr BYTE TriangleBlock = 0x09;  // 삼각선 폐색 9
	constexpr BYTE Cheongnyangni_Mangu = 0x0A; // 청량리 - 망우 10
	constexpr BYTE BiDirectional = 0x0B; // 양방향 폐색       (정방향출발 && 역방향장내) 11
	constexpr BYTE BiDirectional_3Aspect = 0x0C; // 양방향 폐색 3현시 (정방향장내 && 역방향출발)(BR, DR) 12

	constexpr BYTE HighSpeedBlock = 0x10; // 고속선 폐색 16
}

namespace BlockInfo_BlockAspect
{
	constexpr BYTE Aspect2 = 0x02;
	constexpr BYTE Aspect3 = 0x03;
	constexpr BYTE Aspect4 = 0x04;
	constexpr BYTE Aspect5 = 0x05;
}

// BlockInfo_BlkKind 설명 문자열 반환. 비트 플래그가 아니라 값 하나로 종류가 정해지는
// 코드라서 switch로 값을 직접 매칭함 (값 하나만 넘기면 됨).
inline CString DescribeBlkKind_Aspect(BYTE kind, BYTE aspect)
{
	switch (kind)
	{
	case BlockInfo_BlkKind::DoubleAuto_5Aspect:   //복선 자동 5현시
		switch (aspect)
		{
		case BlockInfo_BlockAspect::Aspect3: return _T("복선자동 3현시 Y");
		default: return _T("복선자동 5현시 YY,Y,YG");
		}
	case BlockInfo_BlkKind::SingleAuto_3Aspect:
		switch (aspect)
		{
		case BlockInfo_BlockAspect::Aspect5: return _T("단선자동 5현시 BR, YY, Y, G");
		default: return _T("단선자동 3현시 BR , DR");
		}
	case BlockInfo_BlkKind::DoubleInterlocking:          return _T("복선연동");
	case BlockInfo_BlkKind::SingleInterlocking:          return _T("단선연동");
	case BlockInfo_BlkKind::TokenBlock:                  return _T("통표 폐색");
	case BlockInfo_BlkKind::SubwayBlock:                 return _T("지하철 폐색");
	case BlockInfo_BlkKind::DaeyaBlock:                  return _T("대야 폐색");
	case BlockInfo_BlkKind::UiwangBlock:
		switch (aspect)
		{
		case BlockInfo_BlockAspect::Aspect3: return _T("의왕 폐색 3현시 HR, EHR, TR, TPSR, eHR");
		case BlockInfo_BlockAspect::Aspect4:return _T("의왕 폐색 4현시 HR, EHR, TR, TPSR");
		default:return _T("의왕 폐색 5현시 HR, EHR, TR, TPSR");
		}
	case BlockInfo_BlkKind::TriangleBlock:               return _T("삼각선 폐색");
	case BlockInfo_BlkKind::Cheongnyangni_Mangu:         return _T("청량리 - 망우");
	case BlockInfo_BlkKind::BiDirectional:               return _T("양방향 폐색 (정방향출발 && 역방향장내)");
	case BlockInfo_BlkKind::BiDirectional_3Aspect:
		switch (aspect)
		{
		case BlockInfo_BlockAspect::Aspect5: return _T("양방향 폐색 3현시(정방향장내 && 역방향출발) BR");
		default:return _T("양방향 폐색 5현시 (정방향장내 && 역방향출발) BR,DR");
		}
	case BlockInfo_BlkKind::HighSpeedBlock:              return _T("고속선 폐색");
	default:                                             return _T("미정의 폐색 종류");
	}
}

// BlockInfo_BlockAspect 설명 문자열 반환. 값 하나로 현시 수가 정해지는 코드라
// switch로 값을 직접 매칭함 (값 하나만 넘기면 됨).
inline CString DescribeBlockAspect(BYTE val)
{
	switch (val)
	{
	case BlockInfo_BlockAspect::Aspect2: return _T("2현시");
	case BlockInfo_BlockAspect::Aspect3: return _T("3현시");
	case BlockInfo_BlockAspect::Aspect4: return _T("4현시");
	case BlockInfo_BlockAspect::Aspect5: return _T("5현시");
	default:                             return _T("");
	}
}

namespace BlockInfo_BlkKindValue
{
	// ExpBlk 관련 비트 플래그 (고속선 관련)
	namespace ExpBlk
	{
		constexpr BYTE HIGH_SPEED = 0x01;  // Bit0=1: 고속선 폐색 (국철) - 폐색취소출력 30초 설정(default)
	}

	// MetroRev 관련 비트 플래그 (지하철 역방향 관련)
	namespace MetroRev
	{
		constexpr BYTE METRO_34_REV = 0x01;  // Bit0=1: 서울교통 3,4호선 역방향 폐색
	}

	// JeonlaRev 관련 비트 플래그 (양방향 폐색 구역 제어)
	namespace JeonlaRev
	{
		constexpr BYTE GYEONGBU_BI = 0x00;  // Bit0=0: 경부선 양방향 폐색
		constexpr BYTE JEONLA_BI = 0x01;  // Bit0=1: 전라선 양방향 폐색
	}

	// RevStartRed 관련 비트 플래그 (역방향 출발 신호 제어)
	namespace RevStartRed
	{
		constexpr BYTE RED_OUT_ENABLE = 0x01;  // Bit0=1: 역방향 출발시(내방궤도 점유시) 폐색 적색 표시 및 출력
		constexpr BYTE STEADY_RED = 0x02;  // Bit1=1: 역방향 출발 폐색 적색 고정 표시 (적색점멸 없음)
	}

	// OutKind 관련 비트 플래그 (단선자동 및 출발신호 제어)
	namespace OutKind
	{
		constexpr BYTE PROCEED = 0x01;  // Bit0=1: BR, DR 모두 여자시 출발신호 진행 (단선자동 3현시)
		constexpr BYTE CAUTION = 0x02;  // Bit1=1: BR, DR 모두 여자시 출발신호 주의 (단선자동 3현시)
		constexpr BYTE MASK_3ASPECT = 0x03;  // Bit0 & Bit1: 최소 현시(0x00) 및 상호배제 연산 마스크용
		constexpr BYTE ACTIVE_SIG = 0x04;  // Bit2=1: 폐색 YY 여자 또는 착점궤도 여자시 출발신호 현시
	}

	// RevArrSig 관련 비트 플래그 (양방향 연동 장치 제어)
	namespace RevArrSig
	{
		constexpr BYTE REV_START_INTERLOCK = 0x01;  // Bit0=1: 정방향장내 && 역방향출발 시 상호 연동 금지 및 장내신호 잠금
		constexpr BYTE REV_HOME_INTERLOCK = 0x02;  // Bit1=1: 정방향출발 && 역방향장내 시 역방향장내 Y/R 상태 조건 제어 및 CNR 방지
	}
}

// BlockInfo_BlkKindValue::RevStartRed 설명 문자열 반환 (값 하나만 넘기면 됨).
// 두 비트가 동시에 켜질 수 있어서 가능한 조합(0x00~0x03)을 모두 case로 나열함.
inline CString DescribeRevStartRed(BYTE val)
{
	using namespace BlockInfo_BlkKindValue::RevStartRed;
	switch (val)
	{
	case 0x00:                        return _T("");
	case RED_OUT_ENABLE:               return _T("역방향 출발시(내방궤도 점유시) 폐색 적색 표시 및 출력");
	case STEADY_RED:                   return _T("역방향 출발 폐색 적색 고정 표시 (적색점멸 없음)");
	case RED_OUT_ENABLE | STEADY_RED:  return _T("역방향 출발시(내방궤도 점유시) 폐색 적색 표시 및 출력, 역방향 출발 폐색 적색 고정 표시 (적색점멸 없음)");
	default:return _T("");


	}
}

// BlockInfo_BlkKindValue::OutKind 설명 문자열 반환 (값 하나만 넘기면 됨).
// [주의] namespace 주석에는 bit2(ACTIVE_SIG)도 있지만, DiffCompareFrame.cpp의
// ConvertBlockInfoText 주석에 따르면 그 값은 실제로 OutKind가 아니라
// BlockBOthInfo.DepSig(bit0)에서 관리되는 것으로 확인되어 있어, bit0/bit1
// 두 비트 조합만 다루고 나머지 비트는 무시함(& 0x03).
inline CString DescribeOutKind(BYTE val)
{
	using namespace BlockInfo_BlkKindValue::OutKind;
	switch (val & 0x03)
	{
	case 0x00:              return _T("");
	case PROCEED:            return _T("BR, DR 모두 여자시 출발신호 진행 (단선자동 3현시)");
	case CAUTION:            return _T("BR, DR 모두 여자시 출발신호 주의 (단선자동 3현시)");
	case PROCEED | CAUTION:  return _T("BR, DR 모두 여자시 출발신호 진행 (단선자동 3현시), BR, DR 모두 여자시 출발신호 주의 (단선자동 3현시)");
	default: return _T("");
	}
}

// BlockInfo_BlkKindValue::RevArrSig 설명 문자열 반환 (값 하나만 넘기면 됨).
// 두 비트가 동시에 켜질 수 있어서 가능한 조합(0x00~0x03)을 모두 case로 나열함.
inline CString DescribeRevArrSig(BYTE val)
{
	using namespace BlockInfo_BlkKindValue::RevArrSig;
	switch (val)
	{
	case 0x00:                                          return _T("");
	case REV_START_INTERLOCK:                           return _T("정방향장내 && 역방향출발 시 상호 연동 금지 및 장내신호 잠금");
	case REV_HOME_INTERLOCK:                            return _T("정방향출발 && 역방향장내 시 역방향장내 Y/R 상태 조건 제어 및 CNR 방지");
	case REV_START_INTERLOCK | REV_HOME_INTERLOCK:      return _T("정방향장내 && 역방향출발 시 상호 연동 금지 및 장내신호 잠금, 정방향출발 && 역방향장내 시 역방향장내 Y/R 상태 조건 제어 및 CNR 방지");
	default: return _T("");
	}
}

// 소속역 정보 Type
namespace DwellInfoValue {
	constexpr BYTE ENDSTATION = 0x01; // bit1 : 종착역 구분 (Dwell Lamp 없음)
	constexpr BYTE UPKIND = 0x01; // bit 1 : 하선
	constexpr BYTE DOWNKIND = 0x00; // bit 0 : 상선
}
// 제어 건널목 정보 Type
namespace LCInfoTypeStatus
{
	// Bit0: 출력 및 입력의 여자/낙하 관계성 설정 (0=출력여자일 때 입력여자)
	constexpr BYTE INPUT_DROP_ON_OUTPUT_UP = 0x01;  // Bit0=1: 출력여자일 때 입력낙하

	// Bit1: 부정입력 검사 설정
	constexpr BYTE NO_ILLEGAL_INPUT = 0x02;  // Bit1=1: 부정입력 없음

	// Bit2: 부정출력 검사 설정 (기존에 삭제된 규격)
	constexpr BYTE NO_ILLEGAL_OUTPUT_DEL = 0x04;  // Bit2=1: 부정출력 없음 - 삭제

	// Bit3: 동작불능 상태 검사 설정
	constexpr BYTE NO_MALFUNCTION = 0x08;  // Bit3=1: 동작불능 없음

	// Bit7: 평상시 동작 상태 설정 (의미없음)
	constexpr BYTE NORMALLY_OFF_MEANINGLESS = 0x80;  // Bit7=1: 평상시 OFF, 즉 동작일 때 ON -- 의미없음
}
// 폐색 로직 정보 Type
namespace BlockLogicInfoTypeOnOffStatus {
	constexpr BYTE STATUSON = 0x01; // 설비 낙하
	constexpr BYTE STATUSOFF = 0x02; // 설비 여자
}
// 역 정보 Type
namespace StatuonInfoType {
	namespace StatoinValue {
		constexpr BYTE ISCTC = 0x01; // CTC 역
		constexpr BYTE ISCTCSTD = 0x01; // CTC 표준프로토콜(표준1)
		constexpr BYTE ISCTCSTD2 = 0x01; // CTC 개정프토토콜(표준2)
		constexpr BYTE ISRCCTRL = 0x01; // 원격제어
		constexpr BYTE LOCALCTC = 0x01; // 집중화장치
		constexpr BYTE LCTCMAIN = 0x01; // 집중화 거점역
		constexpr BYTE TNIUSE = 0x01; // 열번인식기
		constexpr BYTE LMCUSE = 0x01; // 체류 보수
		constexpr BYTE SUPPORTUSE = 0x01; // 전기설비 기술지원 시스템
		constexpr BYTE RBCUSE = 0x01; // RBC통신
		constexpr BYTE CPTSTN = 0x01; // 열차 진입방지 장비(CPT)
		constexpr BYTE ISCONTROLLC = 0x01; // 건널목 출력 제어
		constexpr BYTE TRKOPTION = 0x01; //
		constexpr BYTE STNKIND = 0x01; // 연동역 ***********************
	}
	namespace StationEquipValue
	{
		// IsEIsFuse 관련 비트 플래그
		constexpr BYTE HAS_FUSE = 0x01;  // Bit0=1: 연동논리부 FUSE 있음
		// IsUpsAc 관련 비트 플래그
		constexpr BYTE PRESENT_NORMAL_UP = 0x01;  // Bit0=1: UPS AC전원 있음 (여자 정상, 낙하 장애)
		constexpr BYTE PRESENT_FAULT_UP_UNUSED = 0x02;  // Bit1=1: UPS AC전원 있음 (여자 장애, 낙하 정상) - 미사용(결선)
		// SigLmrKind 관련 비트 플래그
		constexpr BYTE SINGLE_LINE_FAULT_DETECT = 0x01;  // Bit0=1: 신호기 고장검지 1회선 (LMR 주,부심 구분없음)
		// EtcEquip 관련 비트 플래그
		constexpr BYTE HEATER_PRE_REVISION_OP = 0x01;  // Bit0=1: 히터 개정 이전 동작설정
	}
}
// 연동도표 정보 구조체 비트 정의
namespace InterLockingInfo
{
	// RouteInfo (진로 정보 관련)
	namespace RouteInfo
	{
		namespace IsCallOnSig
		{
			constexpr BYTE CALLON_ON = 0x01;  // Bit0=1 : (무)유도등 ON [진로에 설정] (입환은 무유도등 ON)
		}
	}

	// Count (쇄정 수량 관련)
	namespace Count
	{
		namespace AppLock
		{
			constexpr BYTE ALWAYS_HOLD_LOCK = 0xFF;  // 0xFF : 접근궤도 점유상태 상관없이 신호취소시 항상 보류쇄정 동작
		}
	}

	// SwitchLock (선로전환기 쇄정 관련)
	namespace SwitchLock
	{
		namespace Direction
		{
			constexpr BYTE NORMAL = 0x01;  // 1: 정위
			constexpr BYTE REVERSE = 0x02;  // 2: 반위
		}

		namespace LockKind
		{
			constexpr BYTE ROUTE_LOCK = 0x01;  // Bit0=1 : 진로쇄정
			constexpr BYTE OVERLAP_LOCK = 0x02;  // Bit1=1 : Overlap
			constexpr BYTE TIME_LOCK_SET = 0x04;  // Bit2=1 : 진로설정시 일정시간동안 쇄정 (시간 값 : ReleaseTrkNo)
			constexpr BYTE TIME_LOCK_ARR_REC = 0x08;  // Bit3=1 : 착점도착 후 일정시간 후 해정, 착점궤도 복구시 해정 (시간 값 : ReleaseTrkNo)
			constexpr BYTE TIME_LOCK_ARR_KEEP = 0x10;  // Bit4=1 : 착점도착 후 일정시간 후 해정, 착점궤도 복구시에도 쇄정 유지 (시간 값 : ReleaseTrkNo)
		}
	}

	// RouteLock (진로쇄정 궤도 관련)
	namespace RouteLock
	{
		namespace Kind
		{
			constexpr BYTE ROUTE_LOCK_TRK = 0x01;  // 1 : 진로쇄정 궤도(TRACK_ROUTELOCK)
			constexpr BYTE TRACK_LOCK_TRK = 0x02;  // 2 : 접촉관계 쇄정(TRACK_TRACKLOCK)
			constexpr BYTE TEMP_LOCK_TRK = 0x03;  // 3 : 임의 쇄정(TRACK_TEMPLOCK) - DelTrack
			constexpr BYTE SPECIAL_LOCK_TRK = 0x04;  // 4 : 임의 쇄정(TRACK_SPECIALLOCK)
		}

		namespace UdoOffTrk
		{
			constexpr BYTE NORMAL_OFF = 0x00;  // bit0=0 : 궤도 낙하시 (무)유도와 신호기 모두 정지
			constexpr BYTE SHUNT_UDO_ONLY = 0x01;  // bit0=1 : 궤도 낙하시 무유도만 정지 (입환)
			constexpr BYTE MAIN_UDO_ONLY = 0x02;  // bit1=1 : 궤도 낙하시 유도만 현시 [취급시] (주신호)
			constexpr BYTE DELAY_RELEASE = 0x04;  // bit2=1 : 도착 후(후방궤도 복구) 설정 시간 후 진로 해정 ----- 추가예정
		}

		namespace Spare
		{
			constexpr BYTE ARRIVAL_RELEASE = 0x01;  // bit0=1 : 궤도 도착시 진로 해정(착점 이외의 궤도)
		}
	}

	// SpcState (타역설비 관련 조건)
	namespace SpcState
	{
		namespace SignalStat
		{
			constexpr BYTE STOP_STOP = 0x00;  // bit0=0: 정지시 신호정지
			constexpr BYTE PROCEED_STOP = 0x01;  // bit0=1: 진행시 신호정지
		}

		namespace SwitchStat
		{
			constexpr BYTE NORMAL = 0x01;  // 1: 정위시 신호정지
			constexpr BYTE REVERSE = 0x02;  // 2: 반위시 신호정지
		}

		namespace SpcFaultIDX
		{
			constexpr BYTE FALL_STOP = 0x00;  // MSB Bit=0 : 기타고장 설비 낙하시 신호정지
			constexpr BYTE RISE_STOP = 0x80;  // MSB Bit=1 : 기타고장 설비 여자시 신호정지
			constexpr BYTE MSB_MASK = 0x80;  // MSB 비트 마스크용
		}
	}
}
// RouteInfoType: 진로 정보 구조체 비트 정의
namespace RouteInfo
{

	constexpr BYTE SPECIAL_VALID_BIT0 = 0x01; // --- Kind.Value (진로 성격) ---
	// bit0=1 : 유효장 진로, bit1=1 : 단조건 진로
	constexpr BYTE SPECIAL_SHORT_BIT1 = 0x02;


	constexpr BYTE DIRECTION_LEFT_BIT0 = 0x01; // bit0=1 : 진로방향(Left:1, Right:0)


	constexpr BYTE ISTTB_BIT0 = 0x01;  // bit0=1 : TTB 존재 여부


	constexpr BYTE ISBLOCK_BIT0 = 0x01; // bit0=1 : 구내폐색 존재 여부


	constexpr BYTE ARRIVE_BIT0 = 0x01; // bit0=1 : 장내 진로


	constexpr BYTE DEPART_BIT0 = 0x01; // bit0=1 : 출발 진로


	constexpr BYTE SHUNT_BIT0 = 0x01; // bit0=1 : 입환 진로


	constexpr BYTE ISLC_BIT0 = 0x01;  // bit0=1 : 구내 건널목 포함여부


	constexpr BYTE JEONLAREVBLK_BIT0 = 0x01;  // bit0=1 : 전라선 역방향 폐색


	constexpr BYTE SHUNTSIG_COMMON_BIT0 = 0x01; // bit0=0 : 입환 신호기 전용, bit0=1 : 입환 표지/신호기 공용


	// --- RouteFlag.Value (진로 제어 플래그) ---

	constexpr BYTE MAINSIG_BIT0 = 0x01;  // bit0=1 : 주신호 연결


	constexpr BYTE FREET_BIT0 = 0x01; // bit0=1 : 착점궤도에 열차 도착하면 진로 전체 해정


	constexpr BYTE UDOSIG_BIT0 = 0x01;  // bit0=1 : (무)유도진로 ON


	constexpr BYTE ZROUT_BIT0 = 0x01; // bit0=1 : 신호 현시되면 ZR출력 차단


	constexpr BYTE DECTAPP_BIT0 = 0x01; // bit0=1 : 궤도점유 상관없이 진로취소시 보류쇄정 동작


	constexpr BYTE DETAPPRELTIME_BIT0 = 0x01; // bit0=1 : (DectApp & 0x01)에 대한 보류쇄정 시간 활성화 판정 비트


	// --- Signal Logic (최대/최소 현시 계열) ---
	constexpr BYTE SIGNAL_STOP = 0x00; // 정지
	constexpr BYTE SIGNAL_PROCEED = 0x02; // 진행
	constexpr BYTE SIGNAL_CAUTION = 0x04; // 주의
	constexpr BYTE SIGNAL_SLOW = 0x06; // 감속
	constexpr BYTE SIGNAL_BOUND = 0x0C; // 경계


	// --- AutoRteCtrl (자동 진로 제어 상태) ---

	constexpr BYTE TRACK_DROP_BIT0 = 0x00;// 궤도 : 낙하(0), 여자(1)
	constexpr BYTE TRACK_ENER_BIT0 = 0x01;


	constexpr BYTE SW_NORMAL_BIT0 = 0x01; // 선로전환기 : 정위(1), 반위(2)
	constexpr BYTE SW_REVERSE_BIT1 = 0x02;
}
// TrackInfoType : 궤도 정보 구조체 비트 정의
namespace TrackInfo
{
	// --- Kind.Value (궤도 종류 플래그) ---
	constexpr BYTE APPROACH_T_BIT0 = 0x01; // bit0=1 : 접근쇄정 궤도
	constexpr BYTE DETECTOR_T_BIT0 = 0x01; // bit0=1 : 보류쇄정 궤도
	constexpr BYTE MAIN_T_BIT0 = 0x01; // bit0=1 : 본선 궤도
	constexpr BYTE BLOCK_T_BIT0 = 0x01; // bit0=1 : 폐색 궤도
	constexpr BYTE INC_SIGNAL_T_BIT0 = 0x01; // bit0=1 : 신호기 포함 궤도
	constexpr BYTE INC_SIGNAL_T_BIT1 = 0x02; // bit0=1 : 신호기 포함 궤도
	constexpr BYTE INC_SWITCH_T_BIT0 = 0x01; // bit0=1 : 선로전환기 포함 궤도
	constexpr BYTE FIRST_T_BIT0 = 0x01; // bit0=1 : 구내 첫번째 궤도
	constexpr BYTE LOCK_T_BIT0 = 0x01; // bit0=1 : 폐로쇄정 구간 궤도
	constexpr BYTE CPT_TRACK_BIT0 = 0x01; // bit0=1 : CPT 궤도

	constexpr BYTE SIDE_TRACK_BIT0 = 0x01; // bit0=1 : 안전측선 또는 출구
	constexpr BYTE SIDE_TRACK_BIT1 = 0x02; // bit0=1 : 궤도장애 없음
	constexpr BYTE SIDE_TRACK_BIT2 = 0x04; // bit0=1 : 궤도 이상복구 없음

	constexpr BYTE SIDE_TRACK_NO_FAULT_BIT1 = 0x02; // bit1=1 : 궤도장애 없음
	constexpr BYTE SIDE_TRACK_NO_RECOVERY_BIT2 = 0x04; // bit2=1 : 궤도 이상복구 없음

	// --- SpcTrack ---
	constexpr BYTE SPC_TRACK_BIT0 = 0x01; // bit0=1 : 타역 궤도

	// --- VirtualTrk (가상 궤도) ---
	constexpr BYTE VIRTUAL_TRK_BIT0 = 0x01; // bit0=1 : 가상궤도
	constexpr BYTE VIRTUAL_TRK_BIT1 = 0x02; // bit0=1 : 가상궤도
	constexpr BYTE VIRTUAL_TRK_NO_MSG_BIT1 = 0x02; // bit1=1 : 궤도점유로 진로 제어불가 설정 없음

	// --- AppTime (궤도복구 시소시간) ---
	constexpr BYTE APP_TIME_DEFAULT = 0x00; // 0x00 : 복구 시소시간 2000 ms
	constexpr BYTE APP_TIME_0MS_BIT0 = 0x01; // bit0=1 : 0 초
	constexpr BYTE APP_TIME_500MS_BIT1 = 0x02; // bit1=1 : 500 ms
	constexpr BYTE APP_TIME_1000MS_BIT2 = 0x04; // bit2=1 : 1000 ms
	constexpr BYTE APP_TIME_1200MS_BIT3 = 0x08; // bit3=1 : 1200 ms
	constexpr BYTE APP_TIME_1500MS_BIT4 = 0x10; // bit4=1 : 1500 ms
	constexpr BYTE APP_TIME_1800MS_BIT5 = 0x20; // bit5=1 : 1800 ms
	constexpr BYTE APP_TIME_2500MS_BIT6 = 0x40; // bit6=1 : 2500 ms
	constexpr BYTE APP_TIME_3000MS_BIT7 = 0x80; // bit7=1 : 3000 ms

	// --- SpeedType (속도 제한 설정) ---
	constexpr BYTE SPEED_25_BIT0 = 0x01; // bit0=1 : 25Km/h
	constexpr BYTE SPEED_40_BIT1 = 0x02; // bit1=1 : 40km/h
	constexpr BYTE SPEED_60_BIT2 = 0x04; // bit2=1 : 60km/h
	constexpr BYTE SPEED_70_BIT3 = 0x08; // bit3=1 : 70km/h
	constexpr BYTE SPEED_80_BIT4 = 0x10; // bit4=1 : 80km/h
	constexpr BYTE SPEED_CAB_BIT5 = 0x20; // bit5=1 : CAB 출력
	constexpr BYTE SPEED_YD_BIT6 = 0x40; // bit6=1 : YD
	constexpr BYTE SPEED_YC_BIT7 = 0x80; // bit7=1 : YC

	// --- TrkLockSwitch (선로전환기 쇄정 구분) ---
	constexpr BYTE SW_DIR_NORMAL = 0x01; // 정위
	constexpr BYTE SW_DIR_REVERSE = 0x02; // 반위
	constexpr BYTE SW_DIR_BOTH = 0x03; // 정위, 반위 모두

	// --- PlatForm (플랫폼 설정) ---
	constexpr BYTE PLATFORM_BIT0 = 0x01; // bit0=1 : 플랫폼 궤도
	constexpr BYTE PLATFORM_UP_BIT0 = 0x00; // bit0=0 : 상선궤도
	constexpr BYTE PLATFORM_DOWN_BIT0 = 0x01; // bit0=1 : 하선궤도
}
namespace SignalInfo {
	constexpr BYTE MAINSBIT1 = 0x01; // bit0=1 주신호기
	constexpr BYTE MAINSBIT2 = 0x02; // bit1=1 진로 선별등 있음
	constexpr BYTE SHUNTD = 0x01; // bit0=1 입환 표기
	constexpr BYTE SHUNTS = 0x01; // bit0=1 입환 신호기
	constexpr BYTE SHUNTS1 = 0x02; // IN 카드 무유도 출력 있음
	constexpr BYTE SHUNTS2 = 0x04; // OUT 카드 무유도 출력 있음
	constexpr BYTE BLOCKS = 0x01; // bit0=1 폐색 신호기
	constexpr BYTE HOMEBLOCKS = 0x01; // bit0 =1 구내 폐색 신호기
	constexpr BYTE CALLONS = 0x01; //bit0=1 유도등포함 (주신호기)
	constexpr BYTE REPEATS = 0x00; // bit0=1 중계 신호기
	constexpr BYTE REPEATS2 = 0x01; // bit0=1 중계 신호기 포함
	constexpr BYTE UMHOSIG = 0x01; // bit0=1 엄호신호기
	constexpr BYTE ISTTB = 0x01; // bit0=1 TTB존재
	constexpr BYTE CPTSIGNAL = 0x01; // bit0=1 CPT 신호기
	constexpr BYTE SPCSIGNAL = 0x01; // bit0=1 타역 신호기
	constexpr BYTE SigDir = 0x02; // bit1=1 하행 신호기
	constexpr BYTE SigDir2 = 0x04; // bit2=1 상행신호기
}

namespace SwitchInfoT {
	constexpr BYTE SINGLE = 0x01;// bit0=1 단동
	constexpr BYTE DOUBLE = 0x01;  // bit0=1 쌍동
	constexpr BYTE TRIPLE = 0x01;  // bit0=1 삼동
	constexpr BYTE FOURTIME = 0x01;// bit0=1 사동
	constexpr BYTE SCISSORS = 0x01;// bit0=1 시서스
	constexpr BYTE NOSE = 0x01;// bit0=1 노스가동
	constexpr BYTE ISTWINAUTO = 0x01;// bit0=1 쌍동 표시분리
	constexpr BYTE TWIN = 0x01;// bit0=1 사용 x
	constexpr BYTE  SPCSWITCH = 0x01;// bit0=1 쌍동 선로전환기에 대한 표시분리 (메시지분리 설정 선로전환기)타역 선로전환기
	constexpr BYTE DIRKINDBIT1 = 0x01; // bit0=1
	constexpr BYTE DIRKINDBIT2 = 0x02; // bit1=1 // 추가예정 (bit0=1:정위 L방향,반위 R방향,  bit1=1:정위 R방향,반위 L방향)
}
namespace AutoSwitch {
	constexpr BYTE LOCK = 0x01; // 전환 후 쇄정 동작
	constexpr BYTE AUTONORMALIN = 0x01; // 입력 ON 이면 자동정위전환
	constexpr BYTE AUTONORMALRTE = 0x0a; // 입력 관계없이 자동전위 전환
}
namespace SwitchBlock {
	constexpr BYTE EQUIPSTATBIT0 = 0x01; // 궤도(T) bit0 =1 궤도 낙하이면 선로전환기 쇄정 ,  폐색(B) 폐색 현시되면 선로전환기 쇄정
	constexpr BYTE EQUIPSTATBIT1 = 0x02; // bit1=1 폐색 적색이면 선로전환기 쇄정
	constexpr BYTE EQUIPSTATBIT2 = 0x04; // bit2=1 폐색 현시 또는 적색이면 선로전환기 쇄정

	constexpr BYTE SWITCHDIRBIT0 = 0x01; // 선로전환기 정위일 때 쇄정
	constexpr BYTE SWITCHDIRBIT1 = 0x02; // 선로전환기 반위일 때 쇄정
	constexpr BYTE SWITCHDIRBIT2 = 0x04; // 선로전환기 정위 또는 반위일 때 쇄정
}
namespace DeadSection {
	constexpr BYTE TYPEBIT0 = 0x01; // 1계
	constexpr BYTE TYPEBIT1 = 0x02; // 2계
	constexpr BYTE TYPEBIT2 = 0x04; // 운용 (Input 여자 : 1계 주계, 낙하 : 2계 주계)
}
