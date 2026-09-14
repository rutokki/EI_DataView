
#pragma once

#pragma pack(push)
#pragma pack(1)


//===========================================================================
// 폐색 구분 코드
#define BLK_ARRIVAL                        1               // 장내 폐색
#define BLK_DEPARTURE                      2               // 출발 폐색

#define BLK_DOUBLE_ABS                     1               // 복선자동 폐색 (5현시)
#define BLK_SINGLE_ABS                     2               // 단선자동 폐색 (3현시)
#define BLK_DOUBLE_REL                     3               // 복선연동 폐색
#define BLK_SINGLE_REL                     4               // 단선연동 폐색
#define BLK_TONGPYO                        5               // 통표     폐색
#define BLK_SUBWAY                         6               // 지하철   폐색
#define BLK_DAEYA                          7               // 대야     폐색
#define BLK_EUIWANG                        8               // 의왕     폐색
#define BLK_TRI                            9               // 삼거리   폐색
#define BLK_CRRI                           10              // 청량리-망우 폐색
#define BLK_BOTH_ABS1                      11              // 양방향       (정방향출발 & 역방향 장내)
#define BLK_BOTH_ABS2                      12              // 양방향 3현시 (역방향출발 & 정방향 장내)
#define BLK_SINGLE_ABS_5ASPECT             13              // 단선자동 폐색 (5현시)
#define BLK_BOTH_ABS2_5ASPECT              14              // 양방향 5현시 (역방향출발 & 정방향 장내)
#define BLK_DOUBLE_ABS_3ASPECT             15              // 복선자동 폐색 (3현시)
#define BLK_EXPRESS                        16              // 고속선 폐색


//------------------------------------------------------------
#define STN_INFO                            g_EiDbTable._STN_Info
#define ILK_INFO                            g_EiDbTable._ILK_Info
#define RTE_INFO                            g_EiDbTable._RTE_Info
#define TRK_INFO                            g_EiDbTable._TRK_Info
#define SIG_INFO                            g_EiDbTable._SIG_Info
#define SWH_INFO                            g_EiDbTable._SWH_Info
#define BLK_INFO                            g_EiDbTable._BLK_Info
#define LCS_INFO                            g_EiDbTable._LCS_Info       // 건널목 정보

#define DEAD_INFO                           g_EiDbTable._DeadSec_Info   // 절연구간
#define FALLCK_INFO                         g_EiDbTable._FallLck_Info   // 지장물 정보
#define STL_INFO                            g_EiDbTable._Stl_Info
#define FAULT_INFO                          g_EiDbTable._Fault_Info     // 기타고장정보
#define HEAT_INFO                           g_EiDbTable._Heat_Info

#define INP_INFO                            g_EiDbTable._INP_Info
#define IOA_INFO                            g_EiDbTable._IOA_Info

#define CPT_INFO                            g_EiDbTable._CPT_Info
#define DWL_INFO                            g_EiDbTable._DWL_Info

#define LCSCTRL_INFO                        g_EiDbTable._LC_INFO        // 제어건널목 정보

//#define TRN_INFO                            g_EiDbTable._TRN_Info
//#define CTC_STD                             g_EiDbTable._CtcIndInfoStd
//#define RBC_INFO                            g_EiDbTable._RbcIndInfo

//------------------------------------------------------------
//#define INCARDLIST                          g_EiCardInfo.INLIST
//#define OUTCARDLIST                         g_EiCardInfo.OUTLIST
//#define PDOCARDLIST                         g_EiCardInfo.PDOLIST

//------------------------------------------------------------
#define BLK_LOGIC                           g_EiLogic._BLK_Logic
#define RTE_LOGIC                           g_EiLogic._RTE_Logic


//============================================================
#define GET_COUNT_ROUTE                     STN_INFO.NoOfRouteExt
#define GET_COUNT_TRACK                     STN_INFO.NoOfTrack
#define GET_COUNT_SIGNAL                    STN_INFO.NoOfSignal
#define GET_COUNT_SWITCH                    STN_INFO.NoOfSwitch
#define GET_COUNT_BLOCK                     STN_INFO.NoOfBlock

//============================================================
// return value
#define VALUE_GOOD                            0x01
#define VALUE_NOGOOD                          0x00

//------------------------------------------------------------
#define TRACK_DIRECTION_KIND_LSR              0x01U    // TLS
#define TRACK_DIRECTION_KIND_RSR              0x02U    // TRS

#define ROUTE_DIRECTION_KIND_LEFT             0x01U    // 진로방향 Left
#define ROUTE_DIRECTION_KIND_RIGHT            0x02U    // 진로방향 Right

#define SWITCH_DIRECTION_KIND_NORMAL          0x01U    // 선로전환기 정위
#define SWITCH_DIRECTION_KIND_REVERSE         0x02U    // 선로전환기 반위

//------------------------------------------------------------
// DB Table 설정 값 (진로테이블:RTE_INFO)
#define ROUTE_DIRECTION_KIND_DB_LEFT          1U       // 진로방향 Left
#define ROUTE_DIRECTION_KIND_DB_RIGHT         0U       // 진로방향 Right

#define TRACK_LOCK_KIND_DB_ROUTE_LOCK         1U       // 진로 내 쇄정 궤도
#define TRACK_LOCK_KIND_DB_TRACK_LOCK         2U       // 진로 외 쇄정 궤도
#define TRACK_LOCK_KIND_DB_DEL_LOCK           3U

#define SWITCH_LOCK_KIND_DB_ROUTE_LOCK        0x01U    // 선로전환기 진로쇄정
#define SWITCH_LOCK_KIND_DB_OVER_LOCK         0x02U    // 선로전환기 Overlap 쇄정
#define SWITCH_LOCK_KIND_DB_TIME_LOCK         0x04U    // 진로설정시 선로전환기 일정시간동안 쇄정
#define SWITCH_LOCK_KIND_DB_TIME_RELEASE      0x08U    // 착점도착 후 일정시간 후 해정, 착점궤도 복구시 해정
#define SWITCH_LOCK_KIND_DB_TIME_RELEASE_ONLY 0x10U    // 착점도착 후 일정시간 후 해정, 착점궤도 복구시에도 쇄정 유지

#define SWITCH_DIRECTION_KIND_DB_NORMAL       1U       // 선로전환기 정위
#define SWITCH_DIRECTION_KIND_DB_REVERSE      2U       // 선로전환기 반위

// 단조건 구분
#define DANCASE_KIND_SIGNAL                   _T('S')
#define DANCASE_KIND_TRACK                    _T('T')
//#define DANCASE_KIND_SWITCH                   _T('P')
//#define DANCASE_KIND_ROUTE                    _T('R')


// 신호 현시수 
#define SIG_ASPECT_COUNT_2                    2U
#define SIG_ASPECT_COUNT_3                    3U
#define SIG_ASPECT_COUNT_4                    4U
#define SIG_ASPECT_COUNT_5                    5U

// 현시계열
#define SIG_ASPECT_VALUE_G                    0x02U
#define SIG_ASPECT_VALUE_YG                   0x06U
#define SIG_ASPECT_VALUE_Y                    0x04U
#define SIG_ASPECT_VALUE_YY                   0x0CU
#define SIG_ASPECT_VALUE_R                    0x00U

// 지장물
#define FALL_LOCK_DETECT                      1    // 낙석
#define FALL_LOCK_PROTECT                     2    // 보호
#define FALL_LOCK_TRLEASE                     3    // 해제

// ILK_INFO/EquipLock.Gubun for 지장물
#define FALL_LOCK_GUBUN_DETECT                _T('F')   // 낙석
#define FALL_LOCK_GUBUN_PROTECT               _T('P')   // 보호

// 절연구간
#define DEAD_SECTION_UNIT1                    0x01   // 1계
#define DEAD_SECTION_UNIT2                    0x02   // 2계
#define DEAD_SECTION_USE                      0x04   // 운용

// 고속선 폐색 출력에 대한 조건 구분
#define BLK_EXPRESS_OUT_GUBUN_CANCEL          _T('C')   // CNR 출력


//------------------------------------
// 조건에 따른 진로의 신호기 정지
//------------------------------------
#define SIG_STOP_ALL_EQUIP_NOT_ALIVE             1 // 설비 낙하시 (무)유도와 신호기 모두 정지
#define SIG_STOP_SH_ONLY_UDO_EQUIP_NOT_ALIVE     2 // 설비 낙하시 무유도만 정지      (입환)
#define SIG_MAIN_ONLY_UDO_ON_EQUIP_NOT_ALIVE     3 // 설비 낙하시 유도만 현시        (주신호)
#define SIG_STOP_EQUIP_ALIVE                     4 // 설비 여자일 때 신호정지
#define SIG_STOP_SWH_NOT_NORMAL                  5 // 설비가 선로전환기이면 정위가 아닐 때 신호정지
#define SIG_STOP_SWH_NOT_REVERSE                 6 // 설비가 선로전환기이면 반위가 아닐 때 신호정지
#define SIG_STOP_SWH_LOCK                        7 // 설비가 선로전환기이면 쇄정상태일 때  신호정지


//============================================================
// CTC
#define IS_STATION_CTC                      ((0x01 == (STN_INFO.StationInfo.Value.IsCtc & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)
// CTC 표준프로토콜(표준1)
#define IS_STATION_CTC_STD1                 ((0x01 == (STN_INFO.StationInfo.Value.IsCtcStd & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)
// CTC 표준프로토콜(표준2)
#define IS_STATION_CTC_STD2                 ((0x01 == (STN_INFO.StationInfo.Value.IsCtcStd2 & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)
// 원격제어
#define IS_STATION_RC                       ((0x01 == (STN_INFO.StationInfo.Value.IsRcCtrl & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)
// 집중화장치
#define IS_STATION_LOCAL_CTC                ((0x01 == (STN_INFO.StationInfo.Value.LocalCTC & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)
// 집중화 거점역
#define IS_STATION_LCTC_MAIN                ((0x01 == (STN_INFO.StationInfo.Value.LCtcMain & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)
// 열번인식기
#define IS_STATION_TNI_USE                  ((0x01 == (STN_INFO.StationInfo.Value.TniUse & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)
// 체류보수
#define IS_STATION_LMC_USE                  ((0x01 == (STN_INFO.StationInfo.Value.LmcUse & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)
// 전기설비 기술지원 시스템
#define IS_STATION_SUPPORT_USE              ((0x01 == (STN_INFO.StationInfo.Value.SupportUse & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)
// RBC통신
#define IS_STATION_RBC_USE                  ((0x01 == (STN_INFO.StationInfo.Value.RbcUse & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)
// 열차 진입방지 장치(CPT)
#define IS_STATION_CPT                      ((0x01 == (STN_INFO.StationInfo.Value.CptStn & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)
// 건널목 출력 제어
#define IS_STATION_LC_CTRL                  ((0x01 == (STN_INFO.StationInfo.Value.IsControlLC & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)

// 궤도 이상복구(적색 라인표시) 없음
//#define IS_NO_TRACK_ABFREE_LINE             ((0x01 == (STN_INFO.StationInfo.Value.TrkOption & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)
// 착점궤도 등 모든궤도에 대해 비상해정 취급 후 해정
#define IS_NO_TRACK_ARR_RELEASE             ((0x02 == (STN_INFO.StationInfo.Value.TrkOption & 0x02)) ? VALUE_GOOD : VALUE_NOGOOD)


// 연동논리부 FUSE 있음
#define IS_STATION_EQUIP_EIS_FUSE           ((0x01 == (STN_INFO.StationEquip.Value.IsEIsFuse & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)
// UPS AC전원 있음 (여자 정상, 낙하 장애)
#define IS_STATION_EQUIP_UPS_AC             ((0x01 == (STN_INFO.StationEquip.Value.IsUpsAc & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)
// UPS AC전원 있음 (여자 장애, 낙하 정상) - 사용하지 않음(결선처리)
//#define IS_STATION_EQUIP_UPS_AC_NEGATIVE    ((0x02 == (STN_INFO.StationEquip.Value.IsUpsAc & 0x02)) ? VALUE_GOOD : VALUE_NOGOOD)
// 신호기 고장검지 1회선 (LMR 주,부심 구분없음)
#define IS_SIG_LMR_ONE_USE                  ((0x01 == (STN_INFO.StationEquip.Value.SigLmrKind & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)
// 히터 개정 이전 동작설정
#define IS_HEAT_OLD_USE                     ((0x01 == (STN_INFO.StationEquip.Value.EtcEquip & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)


//------------------------------------------------------------
// 진로처리 로직사용
//#define IS_DO_ROUTE_LOGIC                   ((0x01 == (STN_INFO.DoLogicRoute & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)

// 진로처리 프로그램사용(SR, LOCK, ZR 에 대해서는 로직사용)
#define IS_DO_ROUTE_PROGRAM                 ((0x00 == (STN_INFO.DoLogicRoute & 0xFF)) ? VALUE_GOOD : VALUE_NOGOOD)
// 진로처리 로직사용 (일부로직: SZR, SHZR, SHUNT, REQ, CHG, EMG, FAIL, FAILT, FAILRT, FAILP, FAILRP, 단조건)
#define IS_DO_ROUTE_LOGIC_PART              ((0x01 == (STN_INFO.DoLogicRoute & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)
// 진로처리 로직사용 (전체로직)
#define IS_DO_ROUTE_LOGIC_ALL               ((0x08 == (STN_INFO.DoLogicRoute & 0x08)) ? VALUE_GOOD : VALUE_NOGOOD)

//------------------------------------------------------------
// 접근쇄정 궤도
#define IS_TRACK_APP(nTrackNo)              ((0x01 == (TRK_INFO[nTrackNo].Kind.Value.ApproachT & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)

// 보류쇄정 궤도
#define IS_TRACK_APPD(nTrackNo)             ((0x01 == (TRK_INFO[nTrackNo].Kind.Value.DetectorT & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)

// 본선     궤도
#define IS_TRACK_MAIN(nTrackNo)             ((0x01 == (TRK_INFO[nTrackNo].Kind.Value.MainT & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)

// 폐색     궤도
#define IS_TRACK_BLOCK(nTrackNo)            ((0x01 == (TRK_INFO[nTrackNo].Kind.Value.BlockT & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)

// 신호기   포함 궤도
#define IS_TRACK_INC_SIGNAL(nTrackNo)       ((0x01 == (TRK_INFO[nTrackNo].Kind.Value.IncSignalT & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)

// 선로전환기 포함 궤도
#define IS_TRACK_INC_SWITCH(nTrackNo)       ((0x01 == (TRK_INFO[nTrackNo].Kind.Value.IncSwitchT & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)

// 구내 첫번째  궤도 (접근벨 설정 궤도)
#define IS_TRACK_FIRST(nTrackNo)            ((0x01 == (TRK_INFO[nTrackNo].Kind.Value.FirstT & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)

// 폐로쇄정 구간 궤도
#define IS_TRACK_CLOSE(nTrackNo)            ((0x01 == (TRK_INFO[nTrackNo].Kind.Value.LockT & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)

// CPT 궤도
#define IS_TRACK_CPT(nTrackNo)              ((0x01 == (TRK_INFO[nTrackNo].Kind.Value.CptTrack & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)

// 안전측선 또는 출구 (End Track : 낙하시 항상 궤도 장애)
#define IS_TRACK_SIDE(nTrackNo)             ((0x01 == (TRK_INFO[nTrackNo].Kind.Value.SideTrack & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)
// 궤도장애 없음
#define IS_TRACK_NONE_FAIL(nTrackNo)        ((0x02 == (TRK_INFO[nTrackNo].Kind.Value.SideTrack & 0x02)) ? VALUE_GOOD : VALUE_NOGOOD)
// 궤도이상복구 없음
#define IS_TRACK_NONE_ABNORMAL(nTrackNo)    ((0x04 == (TRK_INFO[nTrackNo].Kind.Value.SideTrack & 0x04)) ? VALUE_GOOD : VALUE_NOGOOD)

// 타역 궤도
#define IS_TRACK_SPC(nTrackNo)              ((0x01 == (TRK_INFO[nTrackNo].Kind.Value.SpcTrack & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)

// 가상궤도 (InCard 정보 없음)
#define IS_TRACK_VIRTUAL(nTrackNo)          ((0x01 == (TRK_INFO[nTrackNo].Kind.Value.VirtualTrk & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)
// 궤도점유로 진로 제어불가 메시지 없음
#define IS_TRACK_NO_MSG_OCC_CTRL(nTrackNo)  ((0x02 == (TRK_INFO[nTrackNo].Kind.Value.VirtualTrk & 0x02)) ? VALUE_GOOD : VALUE_NOGOOD)


// 접근궤도 (궤도 점멸 표시 및 접근벨) : 접근궤도 && 본선 첫궤도 && 폐색 궤도
//#define IS_TRACK_APPROACH(nTrackNo)        \
//          (((VALUE_GOOD == IS_TRACK_APP(nTrackNo)) && (VALUE_GOOD == IS_TRACK_FIRST(nTrackNo)) && (VALUE_GOOD == IS_TRACK_BLOCK(nTrackNo))) ? VALUE_GOOD : VALUE_NOGOOD)
// 접근궤도 (궤도 점멸 표시 및 접근벨) : 본선 첫궤도 && 폐색 궤도
#define IS_TRACK_APPROACH(nTrackNo)        (((VALUE_GOOD == IS_TRACK_FIRST(nTrackNo)) && (VALUE_GOOD == IS_TRACK_BLOCK(nTrackNo))) ? VALUE_GOOD : VALUE_NOGOOD)

// Left,Right 궤도 모두 없음 && 관련 신호기 없음 && 관련 선로전환기 없음 : 이상복구, 비상, 해정, 장애, PB 없음
#define IS_TRACK_NOT_SIDE_TRACK(nTrackNo)      \
          (((0 == TRK_INFO[nTrackNo].LeftTrack) && (0 == TRK_INFO[nTrackNo].RightTrack) && \
           (VALUE_NOGOOD == IS_TRACK_INC_SIGNAL(nTrackNo)) && (VALUE_NOGOOD == IS_TRACK_INC_SWITCH(nTrackNo))) ? VALUE_GOOD : VALUE_NOGOOD)
// Left 또는 Right 궤도 없음 && 관련 신호기 없음 && 관련 선로전환기 없음 : 이상복구, 해정, 장애 없음
#define IS_TRACK_NOT_SIDE_TRACK_ONE(nTrackNo)  \
          (((0 == TRK_INFO[nTrackNo].LeftTrack) || (0 == TRK_INFO[nTrackNo].RightTrack) && \
           (VALUE_NOGOOD == IS_TRACK_INC_SIGNAL(nTrackNo)) && (VALUE_NOGOOD == IS_TRACK_INC_SWITCH(nTrackNo))) ? VALUE_GOOD : VALUE_NOGOOD)

// 궤도 장애 없음 처리 (Left,Right 궤도 모두 없음)
#define IS_TRACK_NOT_FAIL_OCC(nTrackNo)      \
          ((VALUE_NOGOOD == IS_TRACK_SIDE(nTrackNo)) && ((VALUE_GOOD == IS_TRACK_SPC(nTrackNo)) || \
            (VALUE_GOOD == IS_TRACK_NONE_FAIL(nTrackNo)) || (VALUE_GOOD == IS_TRACK_NOT_SIDE_TRACK(nTrackNo))) ? VALUE_GOOD : VALUE_NOGOOD)
// 궤도 장애 없음 처리 (Left 또는 Right 궤도 없음)
#define IS_TRACK_NOT_FAIL_OCC_ONE(nTrackNo)  \
          ((VALUE_NOGOOD == IS_TRACK_SIDE(nTrackNo)) && ((VALUE_GOOD == IS_TRACK_SPC(nTrackNo)) || \
            (VALUE_GOOD == IS_TRACK_NONE_FAIL(nTrackNo)) || (VALUE_GOOD == IS_TRACK_NOT_SIDE_TRACK_ONE(nTrackNo))) ? VALUE_GOOD : VALUE_NOGOOD)

// 궤도 이상복구 없음 처리 (Left,Right 궤도 모두 없음)
#define IS_TRACK_NOT_ABNORMAL_RECV(nTrackNo)      \
          (((VALUE_GOOD == IS_TRACK_SPC(nTrackNo)) || (VALUE_GOOD == IS_TRACK_NONE_ABNORMAL(nTrackNo)) || \
            (VALUE_GOOD == IS_TRACK_NOT_SIDE_TRACK(nTrackNo))) ? VALUE_GOOD : VALUE_NOGOOD)
// 궤도 이상복구 없음 처리 (Left 또는 Right 궤도 없음)
#define IS_TRACK_NOT_ABNORMAL_RECV_ONE(nTrackNo)  \
          (((VALUE_GOOD == IS_TRACK_SPC(nTrackNo)) || (VALUE_GOOD == IS_TRACK_NONE_ABNORMAL(nTrackNo)) || \
            (VALUE_GOOD == IS_TRACK_NOT_SIDE_TRACK_ONE(nTrackNo))) ? VALUE_GOOD : VALUE_NOGOOD)


//--------------------------------
// 궤도 복구 시소시간
// 궤도 복구 시소시간 0 초
#define IS_TRACK_APP_NONE(nTrackNo)         ((0x01 == (TRK_INFO[nTrackNo].AppTime.Value.AppTime & 0xFF)) ? VALUE_GOOD : VALUE_NOGOOD)
// 궤도 복구 시소시간 500ms
#define IS_TRACK_APP_500(nTrackNo)          ((0x02 == (TRK_INFO[nTrackNo].AppTime.Value.AppTime & 0xFF)) ? VALUE_GOOD : VALUE_NOGOOD)
// 궤도 복구 시소시간 1000ms
#define IS_TRACK_APP_1000(nTrackNo)         ((0x04 == (TRK_INFO[nTrackNo].AppTime.Value.AppTime & 0xFF)) ? VALUE_GOOD : VALUE_NOGOOD)
// 궤도 복구 시소시간 1200ms
#define IS_TRACK_APP_1200(nTrackNo)         ((0x08 == (TRK_INFO[nTrackNo].AppTime.Value.AppTime & 0xFF)) ? VALUE_GOOD : VALUE_NOGOOD)
// 궤도 복구 시소시간 1500ms
#define IS_TRACK_APP_1500(nTrackNo)         ((0x10 == (TRK_INFO[nTrackNo].AppTime.Value.AppTime & 0xFF)) ? VALUE_GOOD : VALUE_NOGOOD)
// 궤도 복구 시소시간 1800ms
#define IS_TRACK_APP_1800(nTrackNo)         ((0x20 == (TRK_INFO[nTrackNo].AppTime.Value.AppTime & 0xFF)) ? VALUE_GOOD : VALUE_NOGOOD)
// 궤도 복구 시소시간 2000ms
#define IS_TRACK_APP_2000(nTrackNo)         ((0x00 == (TRK_INFO[nTrackNo].AppTime.Value.AppTime & 0xFF)) ? VALUE_GOOD : VALUE_NOGOOD)
// 궤도 복구 시소시간 2500ms
#define IS_TRACK_APP_2500(nTrackNo)         ((0x40 == (TRK_INFO[nTrackNo].AppTime.Value.AppTime & 0xFF)) ? VALUE_GOOD : VALUE_NOGOOD)
// 궤도 복구 시소시간 3000ms
#define IS_TRACK_APP_3000(nTrackNo)         ((0x80 == (TRK_INFO[nTrackNo].AppTime.Value.AppTime & 0xFF)) ? VALUE_GOOD : VALUE_NOGOOD)

//--------------------------------
// 궤도 낙하 시소시간
// 궤도 낙하 시소시간 0 초
#define IS_TRACK_OCC_TIME_NONE(nTrackNo)    ((0x00 == (TRK_INFO[nTrackNo].AppTime.Value.OccTime & 0xFF)) ? VALUE_GOOD : VALUE_NOGOOD)
// 궤도 낙하 시소시간 200ms
#define IS_TRACK_OCC_TIME_200(nTrackNo)     ((0x01 == (TRK_INFO[nTrackNo].AppTime.Value.OccTime & 0xFF)) ? VALUE_GOOD : VALUE_NOGOOD)
// 궤도 낙하 시소시간 500ms
#define IS_TRACK_OCC_TIME_500(nTrackNo)     ((0x02 == (TRK_INFO[nTrackNo].AppTime.Value.OccTime & 0xFF)) ? VALUE_GOOD : VALUE_NOGOOD)
// 궤도 낙하 시소시간 1000ms
#define IS_TRACK_OCC_TIME_1000(nTrackNo)    ((0x04 == (TRK_INFO[nTrackNo].AppTime.Value.OccTime & 0xFF)) ? VALUE_GOOD : VALUE_NOGOOD)
// 궤도 낙하 시소시간 1200ms
#define IS_TRACK_OCC_TIME_1200(nTrackNo)    ((0x08 == (TRK_INFO[nTrackNo].AppTime.Value.OccTime & 0xFF)) ? VALUE_GOOD : VALUE_NOGOOD)
// 궤도 낙하 시소시간 1500ms
#define IS_TRACK_OCC_TIME_1500(nTrackNo)    ((0x10 == (TRK_INFO[nTrackNo].AppTime.Value.OccTime & 0xFF)) ? VALUE_GOOD : VALUE_NOGOOD)
// 궤도 낙하 시소시간 2000ms
#define IS_TRACK_OCC_TIME_2000(nTrackNo)    ((0x20 == (TRK_INFO[nTrackNo].AppTime.Value.OccTime & 0xFF)) ? VALUE_GOOD : VALUE_NOGOOD)
// 궤도 낙하 시소시간 2500ms
#define IS_TRACK_OCC_TIME_2500(nTrackNo)    ((0x40 == (TRK_INFO[nTrackNo].AppTime.Value.OccTime & 0xFF)) ? VALUE_GOOD : VALUE_NOGOOD)
// 궤도 낙하 시소시간 3000ms
#define IS_TRACK_OCC_TIME_3000(nTrackNo)    ((0x80 == (TRK_INFO[nTrackNo].AppTime.Value.OccTime & 0xFF)) ? VALUE_GOOD : VALUE_NOGOOD)


// 계전기 생성 : OccUR, OldOcc
#define IS_TRACK_HAS_OCC_UR(nTrackNo)       (((VALUE_GOOD == IS_TRACK_BLOCK(nTrackNo)) && (VALUE_NOGOOD == IS_TRACK_OCC_TIME_NONE(nTrackNo))) ? VALUE_GOOD : VALUE_NOGOOD)


//------------------------------------------------------------
// 주신호기
#define IS_SIGNAL_MAIN(nSigNo)              ((0x01 == (SIG_INFO[nSigNo].Kind.Value.MainS & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)
// 진로선별등 있음
#define IS_SIGNAL_HAS_INDIDATION(nSigNo)    ((0x02 == (SIG_INFO[nSigNo].Kind.Value.MainS & 0x02)) ? VALUE_GOOD : VALUE_NOGOOD)

// 입환신호기
#define IS_SIGNAL_SHUNT_SIG(nSigNo)         ((0x01 == (SIG_INFO[nSigNo].Kind.Value.ShuntS & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)

// 입환표지
#define IS_SIGNAL_SHUNTD(nSigNo)            ((0x01 == (SIG_INFO[nSigNo].Kind.Value.ShuntD & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)

// 폐색 신호기
#define IS_SIGNAL_BLOCK(nSigNo)             ((0x01 == (SIG_INFO[nSigNo].Kind.Value.BlockS & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)

// 구내폐색 신호기
#define IS_SIGNAL_HOME_BLOCK(nSigNo)        ((0x01 == (SIG_INFO[nSigNo].Kind.Value.HomeBlockS & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)

// 유도등포함 (주신호기)
#define IS_SIGNAL_CALLON(nSigNo)            ((0x01 == (SIG_INFO[nSigNo].Kind.Value.CallOnS & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)

// 중계 신호기
#define IS_SIGNAL_REPEAT(nSigNo)            ((0x01 == (SIG_INFO[nSigNo].Kind.Value.RepeatS & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)

// 중계 신호기 포함
#define IS_SIGNAL_INREPEAT(nSigNo)          ((0x01 == (SIG_INFO[nSigNo].Kind.Value.IncRepeatS & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)

// 엄호신호기
#define IS_SIGNAL_UNHO(nSigNo)              ((0x01 == (SIG_INFO[nSigNo].Kind.Value.UmhoSig & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)

// TTB 존재
#define IS_SIGNAL_TTB(nSigNo)               ((0x01 == (SIG_INFO[nSigNo].Kind.Value.IsTTB & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)

// CPT 신호기
#define IS_SIGNAL_CPT(nSigNo)               ((0x01 == (SIG_INFO[nSigNo].Kind.Value.CptSignal & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)

// 타역 신호기
#define IS_SIGNAL_SPC(nSigNo)               ((0x01 == (SIG_INFO[nSigNo].Kind.Value.SpcSignal & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)

//------------------------------------------------------------
// 단동 선로전환기
#define IS_SWITCH_SINGLE(nSwhNo)            ((0x01 == (SWH_INFO[nSwhNo].Kind.Value.Single & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)

// 쌍동 선로전환기
#define IS_SWITCH_DOUBLE(nSwhNo)            ((0x01 == (SWH_INFO[nSwhNo].Kind.Value.Double & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)

// 삼동 선로전환기
#define IS_SWITCH_TRIPLE(nSwhNo)            ((0x01 == (SWH_INFO[nSwhNo].Kind.Value.Triple & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)

// 사동 선로전환기
#define IS_SWITCH_FOURTIME(nSwhNo)          ((0x01 == (SWH_INFO[nSwhNo].Kind.Value.FourTime & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)

// 시서스 선로전환기
#define IS_SWITCH_SCISSORS(nSwhNo)          ((0x01 == (SWH_INFO[nSwhNo].Kind.Value.Scissors & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)

// 노스가동 선로전환기
#define IS_SWITCH_NOSE(nSwhNo)              ((0x01 == (SWH_INFO[nSwhNo].Kind.Value.Nose & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)

// 쌍동 선로전환기에대해 표시를 분리시 B점 체크 - 사용하지 않음
#define IS_SWITCH_TWINAUTO(nSwhNo)          ((0x01 == (SWH_INFO[nSwhNo].Kind.Value.IsTwinAuto & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)

// 쌍동 선로전환기에대해 표시를 분리 설정 선로전환기 구분
#define IS_SWITCH_TWIN(nSwhNo)              ((0x01 == (SWH_INFO[nSwhNo].Kind.Value.Twin & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)

// 타역 선로전환기
#define IS_SWITCH_SPC(nSwhNo)               ((0x01 == (SWH_INFO[nSwhNo].Kind.Value.SpcSwitch & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)


// A호 B호 모두 노스가동(MJ81)
#define IS_SWITCH_AMJ81_BMJ81(nSwhNo)       ((0x00 == (SWH_INFO[nSwhNo].NoseAB & 0x03)) ? VALUE_GOOD : VALUE_NOGOOD)

// A호 NS-AM, B호 노스가동(MJ81)
#define IS_SWITCH_ANSAM_BMJ81(nSwhNo)       ((0x01 == (SWH_INFO[nSwhNo].NoseAB & 0x03)) ? VALUE_GOOD : VALUE_NOGOOD)

// A호 노스가동(MJ81), B호 NS-AM
#define IS_SWITCH_AMJ81_BNSAM(nSwhNo)       ((0x02 == (SWH_INFO[nSwhNo].NoseAB & 0x03)) ? VALUE_GOOD : VALUE_NOGOOD)


//------------------------------------
// 선로전환기 진로쇄정
#define IS_SWITCH_LOCK_ROUTE_LOCK(nRteNo, nIdxLock)      \
                   ((SWITCH_LOCK_KIND_DB_ROUTE_LOCK == (ILK_INFO[nRteNo].SwitchLock[nIdxLock].LockKind & SWITCH_LOCK_KIND_DB_ROUTE_LOCK)) ? VALUE_GOOD : VALUE_NOGOOD)

// 선로전환기 Overlap 쇄정
#define IS_SWITCH_LOCK_OVER_LOCK(nRteNo, nIdxLock)       \
                   ((SWITCH_LOCK_KIND_DB_OVER_LOCK == (ILK_INFO[nRteNo].SwitchLock[nIdxLock].LockKind & SWITCH_LOCK_KIND_DB_OVER_LOCK)) ? VALUE_GOOD : VALUE_NOGOOD)

// 진로설정시 일정시간동안 쇄정 - 설정시간 후 해정
#define IS_SWITCH_LOCK_TIME_LOCK(nRteNo, nIdxLock)       \
                   ((SWITCH_LOCK_KIND_DB_TIME_LOCK == (ILK_INFO[nRteNo].SwitchLock[nIdxLock].LockKind & SWITCH_LOCK_KIND_DB_TIME_LOCK)) ? VALUE_GOOD : VALUE_NOGOOD)

// 진로의 착점도착 후 선로전환기 일정시간 후 해정, 착점궤도 복구시 해정
#define IS_SWITCH_LOCK_TIME_RELEASE(nRteNo, nIdxLock)         \
                   ((SWITCH_LOCK_KIND_DB_TIME_RELEASE == (ILK_INFO[nRteNo].SwitchLock[nIdxLock].LockKind & SWITCH_LOCK_KIND_DB_TIME_RELEASE)) ? VALUE_GOOD : VALUE_NOGOOD)

// 진로의 착점도착 후 선로전환기 일정시간 후 해정, 착점궤도 복구시에도 쇄정 유지
#define IS_SWITCH_LOCK_TIME_RELEASE_ONLY(nRteNo, nIdxLock)    \
                   ((SWITCH_LOCK_KIND_DB_TIME_RELEASE_ONLY == (ILK_INFO[nRteNo].SwitchLock[nIdxLock].LockKind & SWITCH_LOCK_KIND_DB_TIME_RELEASE_ONLY)) ? VALUE_GOOD : VALUE_NOGOOD)


// 궤도 복구시 선로전환기 해정 궤도
#define IS_SWITCH_LOCK_RELEASE_TRACK_UR(nRteNo, nIdxLock) \
                   (((VALUE_NOGOOD == IS_SWITCH_LOCK_TIME_LOCK(nRteNo, nIdxLock)) && (VALUE_NOGOOD == IS_SWITCH_LOCK_TIME_RELEASE(nRteNo, nIdxLock)) && \
                   (VALUE_NOGOOD == IS_SWITCH_LOCK_TIME_RELEASE_ONLY(nRteNo, nIdxLock)) && (ILK_INFO[nRteNo].SwitchLock[nIdxLock].ReleaseTrkNo > 0)) ? VALUE_GOOD : VALUE_NOGOOD)


//------------------------------------------------------------
// 유효장 진로
#define IS_ROUTE_UHYO(nRteNo)               ((0x01 == (RTE_INFO[nRteNo].Kind.Value.Special & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)

// 착점 궤도 도착시 유효장 해정 진로
// 유효장 착점궤도(유효장궤도) 끝까지 점유시 진로해정
#define IS_ROUTE_UHYO_RELEASE(nRteNo)       ((0x01 == (ILK_INFO[nRteNo].UhoInfo.UhoRelease & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)

// 유효장 && 지정된 시간 후 유효장 해정
#define IS_ROUTE_UHYO_TIMER(nRteNo)         \
                   (((VALUE_GOOD == IS_ROUTE_UHYO(nRteNo)) && (VALUE_GOOD != IS_ROUTE_UHYO_RELEASE(nRteNo)) && (ILK_INFO[nRteNo].UhoInfo.LockTime > 0)) ? VALUE_GOOD : VALUE_NOGOOD)


// 단조건 진로
#define IS_ROUTE_DANCASE(nRteNo)            ((0x02 == (RTE_INFO[nRteNo].Kind.Value.Special & 0x02)) ? VALUE_GOOD : VALUE_NOGOOD)

// Left  방향 진로
#define IS_ROUTE_LEFT(nRteNo)               ((0x01 == (RTE_INFO[nRteNo].Kind.Value.Direction & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)
// Right 방향 진로
#define IS_ROUTE_RIGHT(nRteNo)              ((0x00 == (RTE_INFO[nRteNo].Kind.Value.Direction & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)

// TTB 진로
#define IS_ROUTE_TTB(nRteNo)                ((0x01 == (RTE_INFO[nRteNo].Kind.Value.IsTTB & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)

// 구내폐색 존재
#define IS_ROUTE_BLOCK_SIGNAL(nRteNo)       ((0x01 == (RTE_INFO[nRteNo].Kind.Value.IsBlock & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)

// 장내 진로
#define IS_ROUTE_ARRIVE(nRteNo)             ((0x01 == (RTE_INFO[nRteNo].Kind.Value.Arrive & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)
// 출발 진로
#define IS_ROUTE_DEPART(nRteNo)             ((0x01 == (RTE_INFO[nRteNo].Kind.Value.Depart & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)

// 입환 진로
#define IS_ROUTE_SHUNT(nRteNo)              ((0x01 == (RTE_INFO[nRteNo].Kind.Value.Shunt & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)

// 입환 표지, 신호기 공용 (입환공용)
#define IS_ROUTE_SHUNT_SIG(nRteNo)          ((0x01 == (RTE_INFO[nRteNo].Kind.Value.ShuntSig & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)

// 입환 신호기 전용
#define IS_ROUTE_SHUNT_SIGNAL_ONLY(nRteNo, nSigNo)    \
                   (((0x00 == (RTE_INFO[nRteNo].Kind.Value.ShuntSig & 0x01)) && (VALUE_GOOD == IS_SIGNAL_SHUNT_SIG(nSigNo))) ? VALUE_GOOD : VALUE_NOGOOD)


// 주신호 연결 (전방신호기 현시되어야 제어 가능)
#define IS_ROUTE_MAINSIG(nRteNo)            ((0x01 == (RTE_INFO[nRteNo].RouteFlag.Value.MainSig & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)

// (무)유도진로
#define IS_ROUTE_UDOSIG(nRteNo)             ((0x01 == (RTE_INFO[nRteNo].RouteFlag.Value.UdoSig & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)

// ZR 출력 차단
//#define IS_ROUTE_ZROUT(nRteNo)             ((0x01 == (RTE_INFO[nRteNo].RouteFlag.Value.ZROut & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)


//------------------------------------------------------------
// 보류쇄정 진로
#define IS_ROUTE_DELLOCK(nRteNo)                  \
                   ((((0 == ILK_INFO[nRteNo].Count.NoOfAppLock) && (0 == ILK_INFO[nRteNo].RouteInfo.DepartureT)) || \
                     (0xFF == ILK_INFO[nRteNo].Count.NoOfAppLock)) ? VALUE_GOOD : VALUE_NOGOOD)

// 주신호   유도등   OFF
// 입환신호 무유도등 ON
#define IS_ROUTE_CALLON_SIG(nRteNo)               ((0x01 == (ILK_INFO[nRteNo].RouteInfo.IsCallOnSig & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)


// 주신호 유도진로 (유도Off 진로) - (진로, 신호기 모두 체크)
#define IS_ROUTE_SIG_MAIN_UDOSIG(nRteNo, nSigNo)    \
                   (((VALUE_GOOD == IS_SIGNAL_MAIN(nSigNo)) && (VALUE_GOOD == IS_ROUTE_CALLON_SIG(nRteNo))) ? VALUE_GOOD : VALUE_NOGOOD)

// 입환공용진로 (진로, 신호기 모두 체크)
#define IS_ROUTE_SIG_SHUNT_COMMON(nRteNo, nSigNo)    \
                   (((VALUE_GOOD == IS_ROUTE_SHUNT_SIG(nRteNo)) && (VALUE_GOOD == IS_SIGNAL_SHUNT_SIG(nSigNo))) ? VALUE_GOOD : VALUE_NOGOOD)


// 궤도 낙하시 무유도만 정지             (입환)
#define IS_ROUTE_UDOOFF_TRACK(nRteNo, nIdxLock)   ((0x01 == (ILK_INFO[nRteNo].RouteLock[nIdxLock].UdoOffTrk & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)
// 궤도 낙하시 유도만 현시               (주신호)
#define IS_ROUTE_UDOON_TRACK(nRteNo, nIdxLock)    ((0x02 == (ILK_INFO[nRteNo].RouteLock[nIdxLock].UdoOffTrk & 0x02)) ? VALUE_GOOD : VALUE_NOGOOD)


// 진로쇄정 없을 때 접근쇄정 동작
//#define IS_APP_WHEN_NOT_RTELOCK(nRteNo, nIdxTrk, nIdxSwh)    \
//                   ((0x01 == (ILK_INFO[nRteNo].ApproachSwitch[nIdxTrk][nIdxSwh].NoRteLock & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)

//------------------------------------
// 조건에 따른 진로의 신호기 정지
//------------------------------------
// 해당설비가 낙하일 때 (무)유도와 신호기 모두 정지
#define IS_SIG_STOP_WHEN_OFF(nRteNo, nIdxLock)              ((1 == ILK_INFO[nRteNo].EquipLock[nIdxLock].stateCond) ? VALUE_GOOD : VALUE_NOGOOD)
// 해당설비가 여자일 때 신호정지
#define IS_SIG_STOP_WHEN_ON(nRteNo, nIdxLock)               ((4 == ILK_INFO[nRteNo].EquipLock[nIdxLock].stateCond) ? VALUE_GOOD : VALUE_NOGOOD)

// 해당설비가 낙하일 때 무유도만 정지      (입환)
#define IS_SIG_STOP_SHUNT_UDO_WHEN_OFF(nRteNo, nIdxLock)    ((2 == ILK_INFO[nRteNo].EquipLock[nIdxLock].stateCond) ? VALUE_GOOD : VALUE_NOGOOD)
// 해당설비가 낙하일 때 유도만 현시        (주신호)
#define IS_SIG_STOP_MAIN_UDO_ON_WHEN_OFF(nRteNo, nIdxLock)  ((3 == ILK_INFO[nRteNo].EquipLock[nIdxLock].stateCond) ? VALUE_GOOD : VALUE_NOGOOD)

// 설비가 선로전환기일 때 정위가 아닐 때 신호정지
#define IS_SIG_STOP_WHEN_SWH_NOT_NOR(nRteNo, nIdxLock)      ((5 == ILK_INFO[nRteNo].EquipLock[nIdxLock].stateCond) ? VALUE_GOOD : VALUE_NOGOOD)
// 설비가 선로전환기일 때 반위가 아닐 때 신호정지
#define IS_SIG_STOP_WHEN_SWH_NOT_REV(nRteNo, nIdxLock)      ((6 == ILK_INFO[nRteNo].EquipLock[nIdxLock].stateCond) ? VALUE_GOOD : VALUE_NOGOOD)
// 설비가 선로전환기일 때 쇄정상태일 때 신호정지
#define IS_SIG_STOP_WHEN_SWH_LOCK(nRteNo, nIdxLock)         ((7 == ILK_INFO[nRteNo].EquipLock[nIdxLock].stateCond) ? VALUE_GOOD : VALUE_NOGOOD)
// 설비가 선로전환기일 때 쇄정상태 무관
//#define IS_SIG_STOP_WHEN_SWH_IGNOR_LOCK(nRteNo, nIdxLock) ((8 == ILK_INFO[nRteNo].EquipLock[nIdxLock].stateCond) ? VALUE_GOOD : VALUE_NOGOOD)


//------------------------------------------------------------
// 타역설비의 조건 - 기타고장(F)
//------------------------------------
// 타역신호기 정지일 때 신호 정지
#define IS_SIG_STOP_SPC_SIG_OFF(nRteNo, nIdxLock)           ((0x00 == (ILK_INFO[nRteNo].SpcState[nIdxLock].SignalStat & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)
// 타역신호기 진행일 때 신호 정지
#define IS_SIG_STOP_SPC_SIG_ON(nRteNo, nIdxLock)            ((0x01 == (ILK_INFO[nRteNo].SpcState[nIdxLock].SignalStat & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)

// 타역 선로전환기 정위일 때 신호 정지
#define IS_SIG_STOP_SPC_SWH_NORMAL(nRteNo, nIdxLock)        ((SWITCH_DIRECTION_KIND_DB_NORMAL == (ILK_INFO[nRteNo].SpcState[nIdxLock].SwitchStat)) ? VALUE_GOOD : VALUE_NOGOOD)
// 타역 선로전환기 반위일 때 신호 정지
#define IS_SIG_STOP_SPC_SWH_REVERSE(nRteNo, nIdxLock)       ((SWITCH_DIRECTION_KIND_DB_REVERSE == (ILK_INFO[nRteNo].SpcState[nIdxLock].SwitchStat)) ? VALUE_GOOD : VALUE_NOGOOD)

// 기타고장설비 인덱스
#define GET_SIG_STOP_SPC_FAULT_IDX(nRteNo, nIdxLock)        (ILK_INFO[nRteNo].SpcState[nIdxLock].SpcFaultIDX & 0x7F)
// 설비 여자시 신호 정지
#define IS_SIG_STOP_SPC_FAULT_ON(nRteNo, nIdxLock)          ((0x80 == (ILK_INFO[nRteNo].SpcState[nIdxLock].SpcFaultIDX & 0x80)) ? VALUE_GOOD : VALUE_NOGOOD)
// 설비 낙하시 신호 정지
#define IS_SIG_STOP_SPC_FAULT_OFF(nRteNo, nIdxLock)         ((0x00 == (ILK_INFO[nRteNo].SpcState[nIdxLock].SpcFaultIDX & 0x80)) ? VALUE_GOOD : VALUE_NOGOOD)


//------------------------------------------------------------
// 복선자동 5현시
#define IS_BLOCK_ABS_DOUBLE(nBlockNo)            ((BLK_DOUBLE_ABS == BLK_INFO[nBlockNo].BlkKind) ? VALUE_GOOD : VALUE_NOGOOD)
// 복선자동 3현시
#define IS_BLOCK_ABS_DOUBLE_3ASPEC(nBlockNo)     ((BLK_DOUBLE_ABS_3ASPECT == BLK_INFO[nBlockNo].BlkKind) ? VALUE_GOOD : VALUE_NOGOOD)

//  단선자동 3현시
#define IS_BLOCK_ABS_SINGLE_3ASPEC(nBlockNo)     ((BLK_SINGLE_ABS == BLK_INFO[nBlockNo].BlkKind) ? VALUE_GOOD : VALUE_NOGOOD)
// 단선자동 5현시
#define IS_BLOCK_ABS_SINGLE_5ASPEC(nBlockNo)     ((BLK_SINGLE_ABS_5ASPECT == BLK_INFO[nBlockNo].BlkKind) ? VALUE_GOOD : VALUE_NOGOOD)

//  복선연동
#define IS_BLOCK_REL_DOUBLE(nBlockNo)            ((BLK_DOUBLE_REL == BLK_INFO[nBlockNo].BlkKind) ? VALUE_GOOD : VALUE_NOGOOD)
//  단선연동
#define IS_BLOCK_REL_SINGLE(nBlockNo)            ((BLK_SINGLE_REL == BLK_INFO[nBlockNo].BlkKind) ? VALUE_GOOD : VALUE_NOGOOD)
//  통표 폐색
//#define IS_BLOCK_(nBlockNo)                      ((BLK_TONGPYO == BLK_INFO[nBlockNo].BlkKind) ? VALUE_GOOD : VALUE_NOGOOD)
//  지하철 폐색
#define IS_BLOCK_SUBWAY(nBlockNo)                ((BLK_SUBWAY == BLK_INFO[nBlockNo].BlkKind) ? VALUE_GOOD : VALUE_NOGOOD)
//  대야   폐색
#define IS_BLOCK_DEAYA(nBlockNo)                 ((BLK_DAEYA == BLK_INFO[nBlockNo].BlkKind) ? VALUE_GOOD : VALUE_NOGOOD)
//  의왕   폐색
#define IS_BLOCK_EUIWANG(nBlockNo)               ((BLK_EUIWANG == BLK_INFO[nBlockNo].BlkKind) ? VALUE_GOOD : VALUE_NOGOOD)
//  삼각선 폐색
#define IS_BLOCK_(nBlockNo)                      ((BLK_TRI == BLK_INFO[nBlockNo].BlkKind) ? VALUE_GOOD : VALUE_NOGOOD)
// 청량리 - 망우
//#define IS_BLOCK_(nBlockNo)                      ((BLK_CRRI == BLK_INFO[nBlockNo].BlkKind) ? VALUE_GOOD : VALUE_NOGOOD)

// 양방향 폐색 (정방향출발 && 역방향장내)
#define IS_BLOCK_BOTH1(nBlockNo)                 ((BLK_BOTH_ABS1 == BLK_INFO[nBlockNo].BlkKind) ? VALUE_GOOD : VALUE_NOGOOD)

// 양방향 폐색 3현시 (정방향장내 && 역방향출발)
#define IS_BLOCK_BOTH2(nBlockNo)                 ((BLK_BOTH_ABS2 == BLK_INFO[nBlockNo].BlkKind) ? VALUE_GOOD : VALUE_NOGOOD)
// 양방향 폐색 5현시 (정방향장내 && 역방향출발)
#define IS_BLOCK_BOTH2_5ASPEC(nBlockNo)          ((BLK_BOTH_ABS2_5ASPECT == BLK_INFO[nBlockNo].BlkKind) ? VALUE_GOOD : VALUE_NOGOOD)

// 고속선 폐색
#define IS_BLOCK_EXPRESS(nBlockNo)               ((BLK_EXPRESS == BLK_INFO[nBlockNo].BlkKind) ? VALUE_GOOD : VALUE_NOGOOD)


//  의왕 폐색 eHR 입력 있음
#define IS_BLOCK_EUIWANG_EHR(nBlockNo)           ((0x01 == (BLK_INFO[nBlockNo].InputAdd & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)
// 복선자동폐색일 때 출  발 개통표시등 있음
// 지 하 철폐색일 때 정방향 개통표시등 있음
#define IS_BLOCK_DEP_SIG_LIGHT(nBlockNo)         ((0x02 == (BLK_INFO[nBlockNo].InputAdd & 0x02)) ? VALUE_GOOD : VALUE_NOGOOD)

// 고속선 폐색 - 폐색취소출력 30초 설정(default)
#define IS_BLOCK_EXPRESS_TIME(nBlockNo)          ((0x01 == (BLK_INFO[nBlockNo].KindInfo.Value.ExpBlk & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)

// 서울교통 3,4호선 역방향 폐색
#define IS_BLOCK_METRO_REV(nBlockNo)             ((0x01 == (BLK_INFO[nBlockNo].KindInfo.Value.MetroRev & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)

// 경부선 양방향 폐색
#define IS_BLOCK_REV_SEOUL_BUSAN(nBlockNo)       ((0x00 == (BLK_INFO[nBlockNo].KindInfo.Value.JeonlaRev & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)
// 전라선 양방향 폐색
#define IS_BLOCK_REV_JEONLA(nBlockNo)            ((0x01 == (BLK_INFO[nBlockNo].KindInfo.Value.JeonlaRev & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)


// 양방향 폐색 (정방향장내 && 역방향출발) 일 때 역방향 폐색 현시 후 출발신호기 내방궤도 점유시 출력 및 출발폐색 적색
#define IS_BLOCK_BOTH2_OUT_TRACK_OCC(nBlockNo)   ((0x01 == (BLK_INFO[nBlockNo].KindInfo.Value.RevStartRed & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)

// 양방향 폐색 (정방향장내 && 역방향출발) : BLTR, BR 낙하시 역방향 출발폐색 적색점등 표시
// (일반의 경우 BLTR, BR 낙하시 출발 적색 점멸)
#define IS_BLOCK_BOTH2_REV_START_RED_BROFF(nBlockNo)     \
                                                 ((0x02 == (BLK_INFO[nBlockNo].KindInfo.Value.RevStartRed & 0x02)) ? VALUE_GOOD : VALUE_NOGOOD)


// BR, DR 모두 여자시 출발신호 최소 현시 출력
#define IS_BLOCK_SIG_OUT_MIN_BRDR_ON(nBlockNo)   ((0x00 == (BLK_INFO[nBlockNo].KindInfo.Value.OutKind & 0x03)) ? VALUE_GOOD : VALUE_NOGOOD)
// BR, DR 모두 여자시 출발신호 진행 출력
#define IS_BLOCK_SIG_OUT_G_BRDR_ON(nBlockNo)     ((0x01 == (BLK_INFO[nBlockNo].KindInfo.Value.OutKind & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)
// BR, DR 모두 여자시 출발신호 주의 출력
#define IS_BLOCK_SIG_OUT_Y_BRDR_ON(nBlockNo)     ((0x02 == (BLK_INFO[nBlockNo].KindInfo.Value.OutKind & 0x02)) ? VALUE_GOOD : VALUE_NOGOOD)

// 폐색 출력시 BR, DR 모두 동시 출력
//#define IS_BLOCK_OUT_BR_DR_ALL(nBlockNo)         ((0x04 == (BLK_INFO[nBlockNo].KindInfo.Value.OutKind & 0x04)) ? VALUE_GOOD : VALUE_NOGOOD)
// 입력카드에 BR만 있음, DR 없음 (출발신호 진행)
//#define IS_BLOCK_SIG_OUT_ONLY_BR_ON(nBlockNo)      ((0x08 == (BLK_INFO[nBlockNo].KindInfo.Value.OutKind & 0x08)) ? VALUE_GOOD : VALUE_NOGOOD)
									  //// Bit2=1 : 폐색 출력시 BR, DR 모두 동시 출력 - 삭제
									  // Bit3=1 : BR 여자일 때 출발신호 진행현시
									  //          입력카드에 BR만 있고, DR은 없음(DR 생성하지 않음)
									  //          예) 역방향 출발(양방향폐색) 출발신호 5현시


// 양방향 폐색 (정방향장내 && 역방향출발)  일 때 역방향출발 폐색 현시이면 장내신호 취급불가
//                                               장내신호 현시상태에서 역방향출발 폐색취급 불가
#define IS_BLOCK_BOTH2_NOT_DEP(nBlockNo)         ((0x01 == (BLK_INFO[nBlockNo].KindInfo.Value.RevArrSig & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)

// 양방향 폐색 (정방향출발 && 역방향장내)일 때
//            역방향장내 폐색 Y 또는 R 상태에서만 장내신호 취급 및 현시 가능
//            역방향장내 폐색현시 && 장내신호 현시상태에서 신호내방 궤도 낙하시 폐색취소 취급 불가
#define IS_BLOCK_BOTH1_NOT_CANCEL(nBlockNo)      ((0x02 == (BLK_INFO[nBlockNo].KindInfo.Value.RevArrSig & 0x02)) ? VALUE_GOOD : VALUE_NOGOOD)


// 장내
#define IS_BLOCK_DIR_ARR(nBlockNo)               ((BLK_ARRIVAL == BLK_INFO[nBlockNo].DirKind) ? VALUE_GOOD : VALUE_NOGOOD)
// 출발
#define IS_BLOCK_DIR_DEP(nBlockNo)               ((BLK_DEPARTURE == BLK_INFO[nBlockNo].DirKind) ? VALUE_GOOD : VALUE_NOGOOD)

//------------------------------------
// 폐색 CNR 출력 동작에 대한 조건
//------------------------------------
// 설비 여자일 때 출력
#define IS_BLOCK_OUT_CNR_WHEN_ON(nBlockNo, nIdxLock)      \
                                                 ((0x01 == (BLK_LOGIC[nBlockNo].BlockCond[nIdxLock].OnOffState & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)
// 설비 낙하일 때 출력
#define IS_BLOCK_OUT_CNR_WHEN_OFF(nBlockNo, nIdxLock)     \
                                                 ((0x00 == (BLK_LOGIC[nBlockNo].BlockCond[nIdxLock].OnOffState & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)

//------------------------------------------------------------
// 출력여자일 때 입력낙하
#define IS_LCSCTRL_INOFF_WHEN_OUT(nLcNo)         ((0x01 == (LCSCTRL_INFO[nLcNo].Status & 0x01)) ? VALUE_GOOD : VALUE_NOGOOD)
// 부정입력 없음
#define IS_LCSCTRL_NO_NOT_EXPECTEDINPUT(nLcNo)   ((0x02 == (LCSCTRL_INFO[nLcNo].Status & 0x02)) ? VALUE_GOOD : VALUE_NOGOOD)
// 동작불능 없음
#define IS_LCSCTRL_NO_NOT_INPUT_OUT(nLcNo)       ((0x08 == (LCSCTRL_INFO[nLcNo].Status & 0x08)) ? VALUE_GOOD : VALUE_NOGOOD)

//------------------------------------------------------------
#pragma pack(pop)
