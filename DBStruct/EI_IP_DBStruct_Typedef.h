#ifndef __EI_IP_SB_STRUCT__H__
#define __EI_IP_SB_STRUCT__H__


#if defined (_WIN32)
#  pragma pack (push)
#  pragma pack (1)
#endif


/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#if defined(__TI_ARM__)
#  define DATA_PACKED_START
#  define DATA_PACKED_END __attribute__((__packed__))
#elif defined(_WIN32)
#  define DATA_PACKED_START
#  define DATA_PACKED_END
#endif


/////////////////////////////////////////////////////////////////////////////
//===========================================================================



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
typedef unsigned int   Bit_t;
typedef unsigned char  Byte_t;
typedef unsigned int   Long_t;
typedef unsigned short Word_t;


/////////////////////////////////////////////////////////////////////////////
//===========================================================================
// Card Type (IOA_INFO)
#define CARD_DB_TYPE_INPUT           1               // 입력
#define CARD_DB_TYPE_OUTPUT          2               // 출력
#define CARD_DB_TYPE_SIGNAL4         3               // 4등용 신호기(주신호기)
#define CARD_DB_TYPE_SIGNAL2         4               // 2등용 신호기(입환 표지, 신호기)
#define CARD_DB_TYPE_SWITCH          5               // 선로전환기


/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#define NO_OF_LOCK_TRK               30              // 진로의 쇄정궤도 수
#define NO_OF_LOCK_SWH               30              // 진로의 쇄정 전철기 수
#define NO_OF_EXCEPTION              8               // 단조건 수
#define NO_OF_SWITCH                 8               // 궤도의 선로전환기 수

#define NO_OF_LOCK_EQUIP             20              // 진로의 쇄정설비 수

#define MAX_ROUTE                   (512 * 3)        // 최대 진로   수

#define MAX_TRACK                    256             // 최대 궤도   수
#define MAX_SIGNAL                   256             // 최대 신호기 수
#define MAX_SWITCH                   256             // 최대 전철기 수
//#define MAX_TRAIN_NO                 5               // 최대 열차번호 창 정보

#define MAX_LEVERLOCK_TRACK          5               // 폐로쇄정 궤도 최대 수
#define MAX_TTB_DEL_TRACK            5               // TTB상태에서 궤도 최대 수

#define MAX_HOME_BLOCK_ROUTE         20              // 구내폐색 진로 최대 수
#define MAX_TOTRTE_SIG               4               // 일괄(총괄) 진로에 제어되는 신호기 최대 수
#define MAX_INHIBIT_ROUTE            20              // 최대 대항진로 수

#define MAX_INCSIG_TRACK             10              // 궤도에 포함된 신호기 최대 수
#define MAX_SWITCH_LOCK_EQUIP        6               // 선로전환기에 대한 설비 최대 수

#define MAX_BLOCK_COND               5               // 폐색 동작에 대한 설비 조건 최대 수
#define MAX_BLOCK_TRACK_ARR          10              // 폐색 관련 장내 궤도 최대 수
#define MAX_BLOCK_TRACK_DEP          10              // 폐색 관련 출발 궤도 최대 수
#define MAX_BLOCK_TRACK_DEPRED       10              // 출발폐색 적색으로 표시하는 궤도 최대 수
#define MAX_BLOCK_OUT                15              // 폐색에 대한 출력조건의 설비 쵀대 수

#define MAX_LEVEL_CROSS              64              // 건널목 수 (건널목 고장검지)
#define MAX_LEVELCROSS_CTRL          64              // 제어 건널목 수

#define MAX_CPT_INFO                 64              // CPT 수
#define MAX_CPT_EQUIP                20              // CPT 관련 설비 최대 수
#define MAX_DEAD_SECTION             90              // 절연구간 (운용, 1계, 2계)
#define MAX_FALL_LOCK                60              // 지장물 (낙석, 보호)

#define MAX_STL                      5               // 출발반응등 최대 수
#define MAX_ATTRACT                  30              // 끌림 감시장치 최대 수
#define MAX_ETC_FAULT                20              // 기타 고장정보 최대 수

#define MAX_BLOCK                    16              // 최대 폐색제어 수

#define MAX_APP_LOCK                 20              // 접근궤도 수
#define MAX_APP_LOCK_SWITCH          10              // 접근쇄정시 선로전환기 수

#define MAX_SIGNAL_LOCK              40              // 쇄정 신호기 수
#define MAX_CHECK_SIGNAL             45              // 착점이 동일한 신호기 수

#define MAX_CHECK_SWITCH             5               // 진로취급시 궤도 점유상태에서 선로전환기 전환 불가

#define MAX_EMG_STOP                 3               // 비상정지 최대 수
#define MAX_EMG_STOP_SIGNAL          10              // 비상정지 관련 신호기 최대 수
#define MAX_EMG_STOP_TRACK           40              // 비상정지 관련 궤  도 최대 수

#define MAX_SLOW_ORDER               8               // Slow Order(임시속도) 설정 최대 수
#define MAX_SLOW_ORDER_SIGNAL        5               // Slow Order(임시속도) 관련 신호기 최대 수
#define MAX_SLOW_ORDER_TRACK         20              // Slow Order(임시속도) 관련 궤  도 최대 수

#define MAX_DWELL                    10              // DwellInfoType 최대 수
#define MAX_DWELL_TRACK_FLATFORM     4               // 플랫폼 궤도 최대 수
#define MAX_DWELL_TRACK_EMG          5               // 비상정지 설정 궤도 최대 수

#define MAX_OUT_HEATER               4               // Switch Heater 출력 최대 수
#define MAX_OUT_EL                   2               // 전차선 출력 최대 수 (대불공단)

#define MAX_CNT_EF_TRACK             10              // 유효장 궤도 최대 수
#define MAX_CNT_SPC_STATE            10              // 타역설비의 조건 최대 수

#define MAX_CNT_HEATER               10              // Heat 최대 수
#define MAX_CNT_HEATER_ALM           10              // Heat Alarm 최대 수

//#define MAX_CNT_ALARM                200             // Alarm 최대 수



/////////////////////////////////////////////////////////////////////////////
//===========================================================================
typedef struct DATA_PACKED_START _IO_Position_t
{
    Byte_t Chassis;    // 서브랙 번호
    Byte_t ModuleNo;   // 전체 카드에 대한 번호 (1부터 시작)
                       // 실제 카드 번호

    Byte_t CardNo;     // 랙단위 카드번호 (1~16)
                       // 카드 위치 (슬롯번호)
    Byte_t PortNo;

    Byte_t Spare1;
    Byte_t Spare2;
} DATA_PACKED_END    IO_Position;

//--------------------------------------------------------------------
//       역 고유 정보 Type
//--------------------------------------------------------------------
typedef struct DATA_PACKED_START  _StationInfoType_t
{
    Byte_t  StationNo;
    Byte_t  LdtsNo;          // LDTS 번호
    Byte_t  LctcStnNo;       // 집중화 역번호
    Byte_t  RbcStnNo;        // RBC    역번호

    struct DATA_PACKED_START _StationInfo_t
    {
        Byte_t  IsCtc;        // bit0=1 : CTC 역
                              // bit1=1 : CTC 표준프로토콜(표준1)
                              // bit2=1 : CTC 개정프로토콜(표준2)

        Byte_t  IsRcCtrl;     // bit0=1 : 원격제어
                              // bit1=1 : 집중화장치 거점역
                              // bit2=1 : 집중화장치 소속역

        Byte_t  TniUse;       // bit0=1 : 열번인식기
        Byte_t  LmcUse;       // bit0=1 : 체류보수
        Byte_t  SupportUse;   // bit0=1 : 전기설비 기술지원 시스템

        Byte_t  RbcUse;       // bit0=1 : RBC통신
        Byte_t  CptStn;       // bit0=1 : 열차 진입방지 장치(CPT)
        Byte_t  IsControlLC;  // bit0=1 : 건널목 출력 제어

        Byte_t  TrkOption;    // bit0=0 : 비상해정시 착점궤도가 점유상태에서 착점궤도는 진로내의 모든궤도 해정시 자동해정(default)
                              // bit0=1 : 착점궤도 등 모든궤도에 대해 비상해정 취급 후 해정
                              //          각 궤도에 대해 보호구역 설정 및 화면에 보호구역 상태표시
                              // bit1=1 : 각각의 궤도에 대한 보호설정 및 해정
                              //          화면에 '보호설정' 상태 표시에 대한 버튼 추가
                              //          각 궤도에 대해 보호설정 및 화면에 보호설정(버튼) 상태표시

        Byte_t  StnKind;      // bit0=1 : 연동역

    } DATA_PACKED_END  StationInfo;

    struct DATA_PACKED_START _StationEquip_t
    {
        Byte_t  IsEIsFuse;    // bit0=1 : 연동논리부 FUSE 있음

        Byte_t  IsUpsAc;      // bit0=0 : UPS AC전원 있음 (여자 정상, 낙하 장애) - default
                              // bit0=1 : UPS AC전원 있음 (여자 장애, 낙하 정상)

        Byte_t  SigLmrKind;   // bit0=0 : 신호기 고장검지 2회선 (default)
                              // bit0=1 : 신호기 고장검지 1회선 (LMR 주,부심 구분없음)

        Byte_t  EtcEquip;     // bit0=1 : 히터 개정(2025.01) 이전 동작

        Byte_t  Spare[2];
    } DATA_PACKED_END  StationEquip;

    // ---------------------------------------------------------------
    Word_t  NoOfRouteExt;                 // 진로   수 (2byte 사용)

    Byte_t  NoOfTrack;                    // 궤도   수
    Byte_t  NoOfSignal;                   // 신호기 수
    Byte_t  NoOfSwitch;                   // 선로전환기 수
    Byte_t  NoOfLevelCross;               // 고장검지 건널목 수
    Byte_t  NoOfBlock;                    // 폐색제어 수

    Byte_t  NoOfDeadSection;              // 전차선 절연구간 수
    Byte_t  NoOfFallLock;                 // 지장물 수
    Byte_t  NoOfSO;                       // 임시속도(Slow Order) 수
    Byte_t  NoOfSTL;                      // 출발반응등 수
    Byte_t  NoOfCpt;                      // 열차진입 방호장치(CPT) 수
    Byte_t  NoOfHeat;                     // Heat 수

    // ---------------------------------------------------------------
    Byte_t  NoOfRack;                     // Rack 수

    Byte_t  NoOfInCard;                   // 입력 Card 수 (빈 슬롯 제외한 실제 입력 카드 수)
    Byte_t  NoOfOutCard;                  // 출력 Card 수 (빈 슬롯 제외한 실제 출력 카드 수)
    Byte_t  NoOfModuleSig;                // 신호기 Card 수 (빈 슬롯 제외한 실제 신호기 카드 수)
    Byte_t  NoOfModuleSwh;                // 선로전환기 Card 수 (빈 슬롯 제외한 실제 선로전환기 카드 수)

    // ---------------------------------------------------------------
    Byte_t  AcrCntEis;                    // 연동논리부 정류기 수
    Byte_t  AcrCntAF;                     // AF 정류기 수
    Byte_t  AcrCntRelay;                  // 계전기랙 정류기 수
    Byte_t  AcrCntAto;                    // ATO/TWC 정류기 수
    Byte_t  AcrCntTs;                     // 속도코드 정류기 수

    // ---------------------------------------------------------------
    Byte_t  Spare[4];

    // ---------------------------------------------------------------
    struct DATA_PACKED_START  _EmgStop_t
    {
        Byte_t        Name[20];

        Byte_t        ArrSignal[MAX_EMG_STOP_SIGNAL];       // 장내 신호기
        Byte_t        DepSignal[MAX_EMG_STOP_SIGNAL];       // 출발 신호기
        Byte_t        TrackNo[MAX_EMG_STOP_TRACK];          // 비상정지 관련 궤도

        Byte_t        EmgOut;                               // bit0=1 : 비상정지 출력있음
                                                            // bit1=1 : 상선,하선 구분없음(즉, 전체비상정지)
        Byte_t        Spare[5];

    } DATA_PACKED_END  EmgStop[MAX_EMG_STOP][2];   // 비상정지
                                                   // [][0] : 상선, [][1] : 하선
                                                   // [0][0] : 상선,하선 구분없는 경우 사용

    struct DATA_PACKED_START  _EquipOut_t
    {
        Byte_t        OutHeater[MAX_OUT_HEATER];              // bit0=1 : Switch Heater 출력
        Byte_t        OutEL[MAX_OUT_EL];                      // bit0=1 : 전차선 출력 (대불공단)
        Byte_t        OutCtc;                                 // bit0=1 : CTC   모드시 출력
        Byte_t        OutLocal;                               // bit0=1 : Local 모드시 출력
        Byte_t        OutMain;                                // bit0=1 : 주계출력

        Byte_t        Spare2[10];
    } DATA_PACKED_END  EquipOut;

    Byte_t  Spare10[20];

} DATA_PACKED_END StationInfoType;


//--------------------------------------------------------------------
//      연동도표 정보 Type 
//--------------------------------------------------------------------
typedef struct DATA_PACKED_START _InterLockInfoType_t
{
    Byte_t  Name[20];

    struct DATA_PACKED_START _RouteInfo_t
    {
        Byte_t  SignalNo;             // 신호기 번호
        Byte_t  DepartureT;           // 출발궤도
        Byte_t  ArrivalT;             // 도착궤도 (착점)
        Byte_t  IsCallOnSig;          // bit0=1 : (무)유도등 ON [진로에 설정]
                                      //          주신호   유도등   OFF
                                      //          입환신호 무유도등 ON
    } DATA_PACKED_END RouteInfo;

    struct DATA_PACKED_START _Count_t
    {
        Byte_t  NoOfSwhLock;          // 쇄정 선로전환기 수
        Byte_t  NoOfExcept;           // 단조건      수
        Byte_t  NoOfSigLock;          // 쇄정 신호기 수
        Byte_t  NoOfCtlLock;          // 신호제어 궤도 수 (진로내쇄정 + 진로외쇄정)
        Byte_t  NoOfRteLock;          // 진로쇄정 궤도 수 (궤도 녹색 표시)
        Byte_t  NoOfAppLock;          // 접근쇄정 궤도 수
                                      // 0xFF : 접근궤도 점유상태 상관없이 신호취소시 항상 보류쇄정 동작
    } DATA_PACKED_END Count;

    struct DATA_PACKED_START _SwitchLock_t
    {
        Byte_t  SwitchNo;             // 선로전환기 번호

        Byte_t  Direction;            // 쇄정 방향(1:정위, 2:반위)
        Byte_t  LockKind;             // 쇄정 종류(진로/OverLap)
                                      // bit0 = 1 : 진로    쇄정
                                      // bit1 = 1 : Overlap 쇄정
                                      // bit2 = 1 : 진로설정시  일정시간동안 쇄정 (시간 값 : TimeValue)
                                      // bit3 = 1 : 착점도착 후 일정시간  후 해정, 착점궤도 복구시 해정          (시간 값 : TimeValue)
                                      // bit4 = 1 : 착점도착 후 일정시간  후 해정, 착점궤도 복구시에도 쇄정 유지 (시간 값 : TimeValue)

        Byte_t  ReleaseTrkNo;         // LockKind 값 bit0 = 1 or bit1 = 1 일 때 사용
                                      // 궤도 복구시 선로전환기 해정(Overlap) 하는 궤도

        Byte_t  TimeValue;            // LockKind 값 bit2 = 1 or bit3 = 1 or bit4 = 1 일 때 사용
                                      // 시간설정 값 (단위 : 초)

        Byte_t  Spare;

    } DATA_PACKED_END SwitchLock[NO_OF_LOCK_SWH];   // 쇄정 선로전환기

    struct DATA_PACKED_START _RouteLock_t
    {
        Byte_t  TrackNo;              // 궤도   번호

        Byte_t  Kind;                 // 쇄정 구분
                                      // 1 : 진로쇄정 궤도(TRACK_ROUTELOCK)
                                      // 2 : 접촉관계 쇄정(TRACK_TRACKLOCK)
                                      // 3 : 임의     쇄정(TRACK_TEMPLOCK) - DelTrack
                                      // 4 : 임의     쇄정(TRACK_SPECIALLOCK)

        Byte_t  RelGubun;             // bit0=0 : 궤도 낙하시 (무)유도와 신호기 모두 정지
                                      //      1 : 궤도 낙하시 무유도만 정지             (입환)
                                      // bit1=1 : 궤도 낙하시 유도만 현시 [취급시]      (주신호)
                                      // bit2=1 : 도착 후(후방궤도 복구) 설정 시간(TimeValue) 후 진로 해정

        Byte_t  ReleaseTrk;           // bit0=1 : 궤도 도착시 진로 해정(착점 이외의 궤도)
                                      //          착점궤도는 설정하지 않아도 착점 도착시 진로해정 동작

        Byte_t  TimeValue;            // 시간설정 값 (단위 : 초)
        Byte_t  Spare;

    } DATA_PACKED_END RouteLock[NO_OF_LOCK_TRK];    // 진로(구분) 쇄정 (궤도)

    struct DATA_PACKED_START _EquipLock_t
    {
        Byte_t  Kind;                 // 각 설비 구분 : 궤도('T'), 선로전환기('P'), 폐색('B'), 건널목('C'), 지장물('J'),....
        Byte_t  Gubun;                // kind 가 지장물인 경우 구분 : 낙석('F'), 보호('P')

        Byte_t  TblNo;                // 각 설비에 대한 인덱스 번호

        Byte_t  stateCond;            // 1 : 설비 낙하시 (무)유도와 신호기 모두 정지
                                      // 2 : 설비 낙하시 무유도만 정지      (입환)
                                      // 3 : 설비 낙하시 유도만 현시        (주신호)

                                      // 4 : 설비 여자일 때 신호정지

                                      // 5 : 설비가 선로전환기이면 정위가 아닐 때 신호정지
                                      // 6 : 설비가 선로전환기이면 반위가 아닐 때 신호정지
                                      // 7 : 설비가 선로전환기이면 쇄정상태일 때  신호정지

    } DATA_PACKED_END EquipLock[NO_OF_LOCK_EQUIP];  // 조건에 따른 진로의 신호기 정지

    struct DATA_PACKED_START _Exception_t
    {
        Byte_t  Kind;                 // 구분 Code('S', 'T', 'P', 'R')
        Byte_t  TblIdx;               // Table Index No
                                      //    Kind = 'S' 일 때 : 신호기 인덱스
                                      //    Kind = 'T' 일 때 : 궤도   인덱스

        Byte_t  SwitchNo;             // 선로전환기 번호 
        Byte_t  SwitchDir;            // 선로전환기 방향(1:정위, 2:반위)
    } DATA_PACKED_END Exception[NO_OF_EXCEPTION];   // 단조건

    // -------------------------------------------------------------
    Byte_t  SignalLock[MAX_SIGNAL_LOCK];          // 쇄정 신호기

    Byte_t  LeverLockTrack[MAX_LEVERLOCK_TRACK];  // 폐로쇄정 궤도 : 궤도 점유상태에서는 취급 및 현시 불가
                                                  //                 신호 현시 후 점유시는 진로 정상 유지

    Byte_t  ControlTrack[NO_OF_LOCK_TRK];         // 신호제어 및 철사쇄정(궤도)  (NO_OF_LOCK_TRK = 30)

    Byte_t  TtbDelTrack[MAX_TTB_DEL_TRACK];       // TTB상태에서 궤도단락시에도 신호는 현시 유지

    // -------------------------------------------------------------
    struct DATA_PACKED_START _ApproachLock_t
    {
        struct DATA_PACKED_START _ApproachTrack_t
        {
            Byte_t  AppTrack;          // 접근 및 보류쇄정(궤도)

            // 접근쇄정 단조건 : 궤도 단 선로전환기
            struct DATA_PACKED_START _ApproachSwitch_t
            {
                Byte_t  SwitchNo;      // 선로전환기 번호
                Byte_t  SwitchDir;     // 1:정위, 2:반위

                Word_t  RteNo;         // 쇄정 진로
                                       // 진로(RteNo)쇄정상태 && AppTrack의 궤도가 해당진로 쇄정시 접근쇄정 동작
                                       // 쇄정 진로설정 값이 있으면 선로전환기는 무시된다

                Byte_t  Spare[2];

            } DATA_PACKED_END  ApproachSwitch[MAX_APP_LOCK_SWITCH];  // 접근쇄정시 선로전환기 조건
        } DATA_PACKED_END  ApproachTrack[MAX_APP_LOCK];

        Byte_t  ReleaseTime;            // 접근/보류쇄정 시소 시간 (단위:초)

        Byte_t  Spare[2];

    } DATA_PACKED_END ApproachLock;   // 접근 및 보류쇄정

    struct DATA_PACKED_START _UhoInfo_t
    {
        Byte_t  FirstTrack;                  // 유효장 시작   궤도 (괄호의 시작   궤도)
        Byte_t  LastTrack;                   // 유효장 마지막 궤도 (괄호의 마지막 궤도)
        Byte_t  TrackNo[MAX_CNT_EF_TRACK];   // 유효장 궤도
        Byte_t  LockTime;                    // 유효장 해정 시간 (단위:초)

        Byte_t  UhoRelease;                  // bit0=1 : 유효장 해정 - 착점 궤도 도착시 유효장 해정
                                             //          유효장 착점궤도(유효장궤도) 끝까지 점유시 진로해정
    } DATA_PACKED_END UhoInfo;

    // -------------------------------------------------------------
    // 진로취급시 궤도[TrackNo] 점유상태에서 선로전환기[SwitchNo] 전환 불가
    //     진로는 쇄정, 신호기 정지상태
    //     궤도 복구시 선로전환기 전환 및 신호 현시 출력
    struct DATA_PACKED_START _SwitchCheck_t
    {
        Byte_t  TrackNo;              // 궤도 번호
        Byte_t  Spare;

        Byte_t  SwitchNo;             // 선로전환기 번호
        Byte_t  SwitchDir;            // 선로전환기 방향(1:정위, 2:반위, 0:방향 상관없음)
    } DATA_PACKED_END SwitchCheck[MAX_CHECK_SWITCH];

    struct DATA_PACKED_START _SpcState_t
    {
        Byte_t  SpcTrack;             // 타역궤도 점유시 신호정지

        Byte_t  SpcSignal;            // 타역 신호기
        Byte_t  SignalStat;           // bit0=0 : 정지시 신호정지
                                      // bit0=1 : 진행시 신호정지

        Byte_t  SpcSwitch;            // 타역 선로전환기
        Byte_t  SwitchStat;           // bit0=1 : 정위시 신호정지
                                      // bit1=1 : 반위시 신호정지

        Byte_t  SpcFaultIDX;          // 기타고장(F) 설비 인덱스
        Byte_t  SpcFaultStat;         // bit0=0 : 기타고장 설비 낙하시 신호정지
                                      // bit0=1 : 기타고장 설비 여자시 신호정지

        Byte_t  Spare[3];

    } DATA_PACKED_END SpcState[MAX_CNT_SPC_STATE];  // 타역설비의 조건에 따라 신호 정지 제어

    Byte_t   Spare10[20];

} DATA_PACKED_END InterLockInfoType;


//--------------------------------------------------------------------
//       진로 정보 Table Type
//--------------------------------------------------------------------
typedef struct DATA_PACKED_START _RouteInfoType_t
{
    struct DATA_PACKED_START _Kind_t
    {
        Byte_t  Special;         // bit0=1 : 유효장 진로
                                 // bit1=1 : 단조건 진로

        Byte_t  Direction;       // bit0=1 : 진로방향(Left:1,Right:0)
        Byte_t  IsTTB;           // bit0=1 : TTB 존재 여부
        Byte_t  IsBlock;         // bit0=1 : 구내폐색 존재 여부
        Byte_t  Arrive;          // bit0=1 : 장내 진로
        Byte_t  Depart;          // bit0=1 : 출발 진로
        Byte_t  Shunt;           // bit0=1 : 입환 진로
        Byte_t  IsLC;            // bit0=1 : 구내 건널목 포함여부

        Byte_t  JeonlaRevBlk;    // bit0=1 : 전라선 역방향 폐색
        Byte_t  ShuntSig;        // bit0=0 : 입환 신호기 전용
                                 // bit0=1 : 입환 표지, 신호기 공용 (입환공용)

    } DATA_PACKED_END Kind;

    struct DATA_PACKED_START _RouteFlag_t
    {
        Byte_t  MainSig;         // bit0=1 : 주신호 연결 (전방신호기 현시되어야 제어 가능)
        Byte_t  UdoSig;          // bit0=1 : (무)유도진로 [진로! 에 설정] - (무)유도 ON (유도진로)

        Byte_t  IOCheck;         // bit0=1 : 진로선별등 출력 있음(HU)
                                 // bit1=1 : 진로선별등 입력 있음(ULMR)

        Byte_t  Spare[5];

    } DATA_PACKED_END RouteFlag;

    // --------------------------------------------------------------------
    // bit0(R), bit1(G), bit2(Y), bit3(Y1)
    Byte_t  MaxSignal;                            // 최대현시 계열
    Byte_t  MinSignal;                            // 최소현시 계열
                                                  // 0x02 : 진행
                                                  // 0x04 : 주의
                                                  // 0x06 : 감속
                                                  // 0x0C : 경계

    Byte_t  FrontSignalNo;                        // 전방신호기 Index : 전방 신호기 현시에 따른 신호현시
                                                  // 전방신호기 현시일 때 현시, 전방신호기 정지일 때 정지
                                                  //   전방신호기 입환  이면 진행 현시
                                                  //   전방신호기 주신호이면 계열에 따른 현시
    Byte_t  RearSignalNo;                         // 후방신호기 Index

    Byte_t  TotRteSigNo[MAX_TOTRTE_SIG];          // 일괄(총괄) 진로인 경우 제어되는 신호기

    // --------------------------------------------------------------------
    Word_t  FrontLinkRteNo;                       // 전방신호 연계진로 : 전방 신호기 현시에 따른 신호현시 (FrontRteNo)
                                                  // 전방신호기 현시일 때 현시, 전방신호기 정지일 때 정지
                                                  //   전방신호기 입환  이면 진행 현시
                                                  //   전방신호기 주신호이면 계열에 따른 현시

    // --------------------------------------------------------------------
    Word_t  UdoRteNo;                             // (무)유도 진로 번호

    Word_t  BlockSigRteNo[MAX_HOME_BLOCK_ROUTE];  // 진로에 구내폐색 있음(0x01 == Kind.IsBlock) 일 때 : 구내폐색 진로
                                                  // 구내폐색 진로일 때 : 해당진로를 포함하는 진로

    Word_t  InhibitRteNo[MAX_INHIBIT_ROUTE];      // 대항진로 (MAX_INHIBIT_ROUTE = 20)
                                                  // 현시 및 쇄정 상태에서 대항진로는 취급 및 현시 불가

    // --------------------------------------------------------------------
    Byte_t  ArrBlockNo;                           // 장내폐색 번호
    Byte_t  DepBlockNo;                           // 출발폐색 번호

    // --------------------------------------------------------------------
    Byte_t  NotOutBlock;                          // bit0=1 : 진로 제어시 폐색 출력 없음
                                                  // 예) 구로역 : 폐색 K 에 대하여
                                                  //      진로 "2D - Z", "2D - P", "2D - LUP", "4D - E" 는 폐색 출력 없음

    // --------------------------------------------------------------------
    //IO_Position  OutHU;                           // 진로선별등 출력 포트(OutCard) : 값이 있을 때 출력 설정
    Byte_t  Spare[6];

    // --------------------------------------------------------------------
    // 자동 진로 제어
    struct DATA_PACKED_START _AutoRteCtrl_t
    {
        Byte_t  AutoMode;        // 1,2,3,...
                                 // 값이 있으면 화면에서 설정 클릭한 자동(회차)진로
                                 // 값이 없으면 조건에 대해서만 동작

        Word_t  PreRteNo;        // 이전에 제어한 자동진로번호
                                 // 값이 있으면 설정한 진로 다음에 해당진로 자동제어
                                 // 값이 없으면 자동진로 제어조건(CtrlCond)에 대해서만 동작

        Byte_t  Spare[4];

        struct DATA_PACKED_START _CtrlCond_t
        {
            Byte_t  CondTblNo;       // 설비 Index
            Byte_t  CondTblKind;     // 설비구분 (S, P, T, ...)
            Byte_t  CondTblState;    //   궤  도   : 낙하(0), 여자(1)
                                     // 선로전환기 : 정위(1), 반위(2)

            Byte_t  Spare;
        } DATA_PACKED_END CtrlCond[3];    // 자동진로 제어조건

        Byte_t  CtrlTime;        // 자동진로 제어 조건성립 후 진로제어 동작 시간 (단위 : 초)
                                 // 자동진로 제어 조건성립 후 설정 시간 이후 자동 진로제어 동작

    } DATA_PACKED_END AutoRteCtrl;    // 자동 진로 제어

    // --------------------------------------------------------------------
    Byte_t  Spare10[30];

} DATA_PACKED_END RouteInfoType;

//--------------------------------------------------------------------
//      궤도 정보 Type
//--------------------------------------------------------------------
typedef struct DATA_PACKED_START _TrackInfoType_t
{
    Byte_t  Name[20];

    struct DATA_PACKED_START _TrackInfoType_Kind_t
    {
        Byte_t  ApproachT;           // bit0=1 : 접근쇄정 궤도
        Byte_t  DetectorT;           // bit0=1 : 보류쇄정 궤도
        Byte_t  MainT;               // bit0=1 : 본선     궤도
        Byte_t  BlockT;              // bit0=1 : 폐색     궤도
        Byte_t  IncSignalT;          // bit0=1 : 신  호  기  포함 궤도
                                     // bit1=1 : 폐색신호기  포함 궤도
        Byte_t  IncSwitchT;          // bit0=1 : 선로전환기  포함 궤도
        Byte_t  FirstT;              // bit0=1 : 접 근 벨 설정 궤도
        Byte_t  LockT;               // bit0=1 : 폐로쇄정 구간 궤도

        Byte_t  CptTrack;            // bit0=1 : CPT 궤도

        Byte_t  SideTrack;           // bit0=1 : 궤도 낙하시 항상 장애(안전측선 또는 출구)
                                     // bit1=1 : 궤도장애 없음
                                     // bit2=1 : 궤도 이상복구 없음

        Byte_t  SpcTrack;            // bit0=1 : 타역 궤도

        Byte_t  VirtualTrk;          // bit0=1 : 가상궤도 (InCard 정보 없음)
                                     //          여자,낙하에 대해 진로현시 로직 동작 설정 
                                     // bit1=1 : 궤도점유로 진로 제어불가 설정 없음 (메시지 없음)

    } DATA_PACKED_END Kind;

    struct DATA_PACKED_START _TrkDelayTime_t
    {
        Byte_t  RecvTime;            // 0x00   : 복구 시소시간 2000 ms (default)
                                     // bit0=1 : 복구 시소시간    0 초 - 사용하지 않음
                                     // bit1=1 : 복구 시소시간  500 ms
                                     // bit2=1 : 복구 시소시간 1000 ms
                                     // bit3=1 : 복구 시소시간 1200 ms
                                     // bit4=1 : 복구 시소시간 1500 ms
                                     // bit5=1 : 복구 시소시간 1800 ms
                                     // bit6=1 : 복구 시소시간 2500 ms
                                     // bit7=1 : 복구 시소시간 3000 ms

        Byte_t  Spare[5];

    } DATA_PACKED_END TrkDelayTime;    // 궤도 시소시간

    struct DATA_PACKED_START _Equipment_t
    {
        Byte_t  NoOfSwitch;          // 선로전환기 수
        Byte_t  NoOfSignal;          // 신  호  기 수

        Byte_t  Spare1;
        Byte_t  Spare2;
    } DATA_PACKED_END Equipment;

    struct DATA_PACKED_START _Switch_t
    {
        Byte_t  SwitchNo;            // 선로전환기 No

        Byte_t  Spare[21];
    } DATA_PACKED_END Switch[NO_OF_SWITCH];      // 궤도에 포함된 선로전환기

    Byte_t  RightTrack;                          // Right 궤도 No
    Byte_t  LeftTrack;                           // Left  궤도 No

    Byte_t  IncSignal[MAX_INCSIG_TRACK];         // 궤도에 포함된 신호기 No.

    Byte_t  InBlockNo;                           // 궤도와 관련된 폐색표시
    Byte_t  CptNo;                               // 궤도와 관련된 Cpt 버튼

    Byte_t  TrnIdx;                              // 열번창 Table Index

    Byte_t  Spare[30];

    // ----------------------------------------------------------------
    struct DATA_PACKED_START _PlatForm_t
    {
        Byte_t  PlatFormNo;               // 플래폼 번호
        Byte_t  Spare;

        struct DATA_PACKED_START _PlatForm_Kind_t
        {
            Byte_t  PlatForm;    // bit0=1 : 플랫폼 궤도
            Byte_t  UpTrack;     // bit0=0 : 상선궤도
                                 // bit0=1 : 하선궤도
            Byte_t  Spare1;
            Byte_t  Spare2;

        } DATA_PACKED_END Kind;
    } DATA_PACKED_END PlatForm;

    Byte_t  Spare10[20];

} DATA_PACKED_END TrackInfoType;

//--------------------------------------------------------------------
//      신호기 정보 Type
//--------------------------------------------------------------------
typedef struct DATA_PACKED_START _SignalInfoType_t
{
    Byte_t  Name[20];

    struct DATA_PACKED_START _SignalInfoType_Kind_t
    {
        Byte_t  MainS;             // bit0=1 : 주   신호기
                                   // bit1=1 : 진로선별등 있음 (입력, 화면표시)

        Byte_t  ShuntD;            // bit0=1 : 입환 표지

        Byte_t  ShuntS;            // bit0=1 : 입환 신호기
                                   // default : 입환 HR출력과 무유도(HCR) 출력이 병렬로 구성(결선)
                                   // 무유도가 별개로 동작하는 경우 Out카드에 HCR 출력 구성
                                   // bit1=1 : Out카드에 무유도 출력 있음
                                   // bit2=1 : IN 카드에 무유도 입력 있음

        Byte_t  BlockS;            // bit0=1 : 폐색 신호기
        Byte_t  HomeBlockS;        // bit0=1 : 구내 폐색  신호기
        Byte_t  CallOnS;           // bit0=1 : 유도등포함 (주신호기)

        Byte_t  RepeatS;           // bit0=1 : 중계 신호기
                                   // bit1=1 : 중계 신호기 포함

        Byte_t  UmhoSig;           // bit0=1 : 엄호신호기
        Byte_t  IsTTB;             // bit0=1 : TTB 존재
        Byte_t  CptSignal;         // bit0=1 : CPT 신호기

        Byte_t  SpcSignal;         // bit0=1 : 타역 신호기

        Byte_t  SigDir;            // bit1=1 : 상행 신호기
                                   // bit2=1 : 하행 신호기
    } DATA_PACKED_END Kind;

    Byte_t  NoOfRoute;                    // 진로 수
    Byte_t  NoOfLight;                    // 신호 현시수(2:2현시,3:3현시,4:4현시,5:5현시)
    Byte_t  TrackNo;                      // 신호기 궤도

    Byte_t  RepeatSigNo;                  // 중계 신호기 번호 (중계 신호기 포함 일 때)
    Byte_t  FrontSignalNo;                // 전방 신호기 번호

    Byte_t  RearBlockAspect;              // 후방 폐색 현시 수(2:2현시,3:3현시,4:4현시,5:5현시)
                                          // 후방 폐색신호기가 없으면 설정하지 않는다

    Byte_t  Spare[5];

    // -------------------------------------------------------------
    struct DATA_PACKED_START _SignalOut_t
    {
        Byte_t  SigOutATS;         // bit0=1 : ATS 출력 있음
        Byte_t  SigOutRear;        // bit0=1 : 후방제어 폐색제어 출력 있음

        Byte_t  Spare[2];
    } DATA_PACKED_END SignalOut;

    Byte_t  SpareOut[26];
    Byte_t  SpareOutATS[24];
    Byte_t  SpareOutRear[24];

    // -------------------------------------------------------------
    Byte_t  Spare10[20];

} DATA_PACKED_END SignalInfoType;

//--------------------------------------------------------------------
//      선로전환기 정보 Type
//--------------------------------------------------------------------
typedef struct DATA_PACKED_START _SwitchInfoType_t
{
    Byte_t  Name[20];

    struct DATA_PACKED_START _SwitchInfoType_Kind_t
    {
        Byte_t  Single;                 // bit0=1 : 단동
        Byte_t  Double;                 // bit0=1 : 쌍동
                                        // bit1=1 : 메시지분리 없음 설정 - 사용하지 않음
        Byte_t  Triple;                 // bit0=1 : 삼동
        Byte_t  FourTime;               // bit0=1 : 사동
        Byte_t  Scissors;               // bit0=1 : 시서스

        Byte_t  Nose;                   // bit0=1 : 노스가동
                                        // bit1=1 : A호는 NS-AM,    B호는 노스가동 - 노스가동 쌍동 선로전환기인 경우 체크
                                        // bit2=1 : A호는 노스가동, B호는 NS-AM    - 노스가동 쌍동 선로전환기인 경우 체크
                                        //      bit1=0 && bit2=0 이면 A호 B호 모두 노스가동
                                        // bit6=1 : 분기히터 출력(포트) 있음 (GCPR) - OutCard
                                        // bit7=1 : MCR 출력(포트) 있음             - 사용하지 않음

        Byte_t  SpcSwitch;              // bit0=1 : 타역 선로전환기

        // 연동도표 인쇄시 사용 (예정)
        Byte_t  DirKind;                // bit0=1 : 정위 L방향,반위 R방향
                                        // bit1=1 : 정위 R방향,반위 L방향

        Byte_t  LocalOut;               // 선로전환기 로컬 설정시 출력 - (추가예정 : 현재 사용하지 않음)
                                        // bit0=1 : 로컬 설정 출력포트 WLR

        Byte_t  Spare;

    } DATA_PACKED_END Kind;

    Byte_t  SwhTrack[4];                // [0] : A 점 궤도 No
                                        // [1] : B 점 궤도 No
                                        // [2] : C 점 궤도 No
                                        // [3] : D 점 궤도 No

    Byte_t  Spare[6];
    Byte_t  SpareOut[18];
    Byte_t  Spare1[36];

    // -------------------------------------------------------------
    struct DATA_PACKED_START _SwitchLockEquip_t
    {
        struct DATA_PACKED_START _LockEquipStat_t
        {
            Byte_t         Kind;        // 구분 Code('S', 'T', 'R', 'B', ...)
            Word_t         TblNo;       // 각 설비에 대한 인덱스 번호

            Byte_t         EquipStat;   // 설비상태
                                        // 궤도 ('T')
                                        //     bit0=1 : 궤도 낙하상태이면 선로전환기 쇄정
                                        // 진로 ('R')
                                        //     bit0=1 : 진로 쇄정상태이면 선로전환기 쇄정
                                        // 폐색 ('B')
                                        //     bit0=1 : 폐색장내 황색이면 선로전환기 쇄정
                                        //     bit1=1 : 폐색장내 적색이면 선로전환기 쇄정
                                        //       0x03 : 폐색장내 황색 또는 적색이면 선로전환기 쇄정
                                        //     bit2=1 : 폐색출발 황색이면 선로전환기 쇄정
                                        //     bit3=1 : 폐색출발 적색이면 선로전환기 쇄정
                                        //       0x0C : 폐색출발 황색 또는 적색이면 선로전환기 쇄정
        } DATA_PACKED_END LockEquipStat[MAX_SWITCH_LOCK_EQUIP];

        Byte_t         SwitchDir;       // bit0=1 : 선로전환기 정위일 때 쇄정
                                        // bit1=1 : 선로전환기 반위일 때 쇄정
                                        //   0x03 : 선로전환기 정위, 반위 모두 쇄정
                                        // bit2=1 : 선로전환기 정위상태가 아니면 정위전환 후 쇄정

        Byte_t         LockTime;        // 쇄정시간 (초단위 : 0~255)
                                        // LockTime = 255 이면 계속 쇄정

        Byte_t         Spare[2];
    } DATA_PACKED_END SwitchLockEquip;   // 설비상태에 대한 선로전환기 쇄정

    // -------------------------------------------------------------
    struct DATA_PACKED_START _AutoSwitch_t
    {
        Byte_t         Kind;             // 구분 Code('S', 'T', 'P', 'B', ...)
        Byte_t         TblNo;            // 각 설비에 대한 인덱스 번호

        Byte_t         Lock;             // bit0=1 : 전환 후 쇄정 동작
        Byte_t         Spare1;
        Byte_t         Spare2;

        // 진로취급에 의해 반위상태에서 진로취소 또는 열차진입, 진로해정 후 (AutoSwhTm)초 후 자동 정위 전환동작
        // 단, 수동취급시에는 자동전환하지 않음
        Byte_t         AutoNormalCond;   // bit0=1
                                         //    설비가 신  호  기(S)일 때 : 현시되면 자동정위전환
                                         //    설비가   궤   도 (T)일 때 : 낙하되면 자동정위전환
                                         // bit1=1
                                         //    설비가 신  호  기(S)일 때 : 정지되면 자동정위전환
                                         //    설비가   궤   도 (T)일 때 : 여자되면 자동정위전환
                                         //    설비가 선로전환기(P)일 때 : 반위 전환되면 자동정위전환
                                         // bit7=1 : 설비상태 관계없이 자동전위 전환

        Byte_t         AutoSwhTm;        // 자동정위 전환에 대한 시간 설정 값 (단위:초)
                                         // 조건성립 후 설정 시간 이후 자동정위 전환제어 동작

        Byte_t         Spare3[3];

    } DATA_PACKED_END AutoSwitch;   // 자동 정위 전환

    Byte_t  Spare10[20];

} DATA_PACKED_END SwitchInfoType;

//--------------------------------------------------------------------
//      폐색 정보 Type
//--------------------------------------------------------------------
typedef struct DATA_PACKED_START _BlockTagInfoType_t
{
    Byte_t  Name[20];

    Byte_t  BlkKind;                  // 폐색 구분
                                      //  1 : 복선자동 : 5현시 default
                                      //  2 : 단선자동 : 3현시 default
                                      //  3 : 복선연동
                                      //  4 : 단선연동
                                      //  5 : 통표   폐색
                                      //  6 : 지하철 폐색(서울교통공사)
                                      //  7 : 대야   폐색
                                      //  8 : 의왕   폐색 : 5현시 default
                                      //  9 : 삼각선 폐색
                                      // 10 : 청량리 폐색
                                      // 11 : 양방향 폐색 (정방향출발 && 역방향장내)
                                      // 12 : 양방향 폐색 (정방향장내 && 역방향출발) : 3현시 default
                                      // 13 : 고속선 폐색

    Byte_t  BlockAspect;              // 폐색 현시 수 (2:2현시,3:3현시,4:4현시,5:5현시)
                                      // 현시 수에 따른 폐색 입력
                                      //     복선자동 3현시 : Y
                                      //     복선자동 5현시 : YY, Y, YG
                                      //     단선자동 3현시 : BR, DR
                                      //     단선자동 5현시 : BR, YY, Y, YG
                                      //     의왕   폐색    3현시 : HR, BHR, TR, TPSR, eHR
                                      //     의왕   폐색 4, 5현시 : HR, BHR, TR, TPSR
                                      //     양방향 폐색 (정방향장내 && 역방향출발) 3현시 : BR, DR
                                      //     양방향 폐색 (정방향장내 && 역방향출발) 5현시 : BR

    Byte_t  Spare1;

    //------------------------------------------------------------------
    struct DATA_PACKED_START _KindInfo_t
    {
        Byte_t    RevKind;            // Bit0=1 : 서울교통공사 3,4호선 역방향 폐색
                                      // Bit1=1 : 전라선 양방향 폐색 (Bit1=0 : 경부선 양방향 폐색)
                                      //          양방향 폐색인 경우 설정 (BlkKind=11 or 12 or 14)

        Byte_t    OutKind;            // Bit0=1 : BR, DR 모두 여자시 출발신호 진행 (단선자동 3현시)
                                      // Bit1=1 : BR, DR 모두 여자시 출발신호 주의 (단선자동 3현시)
                                      //   Bit0=0 && Bit1=0 이면 BR, DR 모두 여자시 출발신호 최소 현시(default)

        Byte_t    DispKind;           // bit0=1 : 복선자동폐색일 때 출  발 개통표시등 있음 (화면표시)
                                      //          지 하 철폐색일 때 정방향 개통표시등 있음 (화면표시)
                                      // Bit1=1 : 양방향폐색일 때 출발 적색점멸 있음(출발RF)
                                      // Bit2=1 : 양방향폐색일 때 출발 황색점멸 있음(출발YF):(정방향출발 && 역방향장내)
                                      // Bit3=1 : 양방향폐색일 때 장내 적색점멸 있음(장내RF)
                                      // Bit4=1 : 양방향폐색일 때 장내 황색점멸 있음(장내YF):(정방향장내 && 역방향출발)

        Byte_t    Spare[3];
    } DATA_PACKED_END KindInfo;

    struct DATA_PACKED_START _BlockBOthInfo_t
    {
        Byte_t    RevStartRed;        // Bit0=1 : 역방향 출발시(출발신호기 내방궤도 점유시) 폐색 적색 표시
                                      //          신호 쇄정구간의 궤도 복구이전까지 적색 유지
                                      // Bit1=1 : 역방향 출발 폐색 적색 표시 (적색점멸 없음)
                                      //          BLTR 낙하, BR 낙하일 때 출발폐색 적색점등
                                      //            (일반의 경우 BLTR, BR 낙하시 출발 적색 점멸)

        Byte_t    RevArrSig;          // Bit0=1 : 양방향 폐색 (정방향장내 && 역방향출발)일 때
                                      //            역방향 출발 황색 : 장내신호 취급 불가
                                      //            역방향 출발 적색 : 장내신호 취급 가능
                                      //            장내신호 현시상태에서 역방향출발 폐색취급 불가
                                      // Bit1=1 : 양방향 폐색 (정방향출발 && 역방향장내)일 때
                                      //            역방향장내 폐색 Y 또는 R 상태에서만 장내신호 취급 및 현시 가능
                                      //            역방향장내 폐색현시 && 장내신호 현시상태에서 신호내방 궤도 낙하시 폐색취소 취급 불가 (CNR 출력되지 않는다)

        Byte_t    DepSig;             // Bit0=1 : 양방향폐색(정방향출발 && 역방향장내) && 4현시출발 신호기일 때
                                      //            폐색 YY 여자 또는 진로의 착점궤도 여자이면 출발신호현시

        Byte_t    Spare[7];
    } DATA_PACKED_END BlockBOthInfo;

    //------------------------------------------------------------------
    Byte_t  DirKind;                                // (1 : 장내,  2 : 출발)
    Byte_t  ArrTrack[MAX_BLOCK_TRACK_ARR];          // 폐색 관련 궤도 (장내)
    Byte_t  DepTrack[MAX_BLOCK_TRACK_DEP];          // 폐색 관련 궤도 (출발, 개통)

    Byte_t  OppositeBlock;                          // 상대 폐색
    Byte_t  RearBlock;                              // 양방향 폐색이면  폐색번호

    Byte_t  UmhoSignal;                             // 엄호신호기 번호

    Byte_t  ArrivalSignal;                          // 폐색 장내 신호기
                                                    // 신호현시상태에서 출발폐색 취급 불가

    Byte_t  ArrFirstTrk;                            // 폐색관련 장내진로의 진로 첫 궤도(신호 내방궤도)
                                                    // 궤도 낙하상태에서 폐색 취급 불가

    Byte_t  DepLastTrk;                             // 폐색관련 출발진로의 착점궤도
                                                    // 궤도 낙하상태에서 폐색 취급 불가
                                                    // 궤도 낙하상태에서 ZR 출력 불가

    // 자동폐색  일 때        출발폐색 적색표시
    // 양방향폐색일 때 정방향 출발폐색 적색표시
    struct DATA_PACKED_START _BlockDepRed_t
    {
        Byte_t  DepRedKind;                             // 0x00(default) : 출발진로의 착점궤도 낙하일 때 적색표시
                                                        // bit0=1 : 궤도 상관없이 폐색 BLTR 낙하일 때 적색표시
                                                        // bit1=1 : 적색으로 표시하는 궤도 낙하일 때 적색표시
                                                        //          (DepBlkRedTrk에 궤도 설정해야 됨)
                                                        //          (폐색 BLTR 낙하/여자 상관없이 궤도 낙하시 적색 표시)

        Byte_t  DepBlkRedTrk[MAX_BLOCK_TRACK_DEPRED];   // 출발폐색을 적색으로 표시하는 궤도

    } DATA_PACKED_END BlockDepRed;

    Byte_t  Spare2;
    Byte_t  Spare3;

    //------------------------------------------------------------------
    Long_t  OutputTm;                               // 폐색 취급시 출력 최대 시간 (ms)
                                                    // default :  6000 ms (6초)
                                                    // (양방향 폐색) 역방향 출발 폐색 출력 : 12000 ms (12초)

    Long_t  CancelTm;                               // 폐색 취소 출력 최대 시간 (ms)
                                                    // default    :  6000 ms ( 6초)
                                                    // 고속선폐색 : 30000 ms (30초)

    Long_t  DelayTm;                                // 폐색 입력 변경 후 입력처리 대기시간 (ms)
                                                    // default :  2000 ms (2초)

    //------------------------------------------------------------------
    struct DATA_PACKED_START _BlockCond_t
    {
        Byte_t  Gubun;                // 동작 구분
                                      // 'C' : 폐색 CNR 출력
                                      // 'R' : 폐색 RR(ZR) 출력
                                      //       RR(ZR) 출력이후 조건성립시 출력유지, 조건해제 이전까지 출력 유지
                                      // 'E' : 기타 출력

        struct DATA_PACKED_START _OutCond_t
        {
            Byte_t  Kind;             // 설비 구분 : 신호기('S'), 궤도('T'), 선로전환기('P'),....

            Byte_t  TblNo;            // 각 설비에 대한 인덱스 번호
                                      // 설비가 궤도일 때는 여러개가 존재할 수 있다 (OR 조건)

            Byte_t  OnOffState;       // 설비 여자 또는 낙하일 때 동작
                                      //   bit0=1 : 여자 (bit0=0 : 낙하)
                                      // 설비가 선로전환기일 때 (bit0=1:정위, bit1=1:반위)
        } DATA_PACKED_END OutCond[MAX_BLOCK_OUT];

        IO_Position   OutPort;        // 출력 포트

        Byte_t  Spare[2];

    } DATA_PACKED_END BlockCond[MAX_BLOCK_COND];  // 동작에 대한 설비 조건

    Byte_t  Spare10[20];

} DATA_PACKED_END BlockTagInfoType;


//--------------------------------------------------------------------
//      건널목 고장 정보 Type
//--------------------------------------------------------------------
typedef struct DATA_PACKED_START _LevelCrossInfoType_t
{
    Byte_t  Name[20];

    Byte_t  Spare10[10];

} DATA_PACKED_END LevelCrossInfoType;


//--------------------------------------------------------------------
//      절연구간 정보 Type
//--------------------------------------------------------------------
// 한 개의 절연구간에 대해 동일한 인덱스로 1계, 2계, 운용 모두 생성
typedef struct DATA_PACKED_START _DeadSectionInfoType_t
{
    Byte_t         Name[20];    // 이름(상선, 하선)

    struct DATA_PACKED_START _Unit1_t
    {
        Byte_t         Name[10];    // 1계 이름 (Input)
        Byte_t         Spare[6];
    } Unit1;  // 1계

    struct DATA_PACKED_START _Unit2_t
    {
        Byte_t         Name[10];    // 2계 이름 (Input)
        Byte_t         Spare[6];
    } Unit2;  // 2계

    struct DATA_PACKED_START _FallLock_t
    {
        Byte_t         Name[10];    // 운용 이름 (Input)
        Byte_t         Spare[6];
    } Unit_Act;  // 운용 (Input 여자:1계 주계, Input 낙하:2계 주계)

    Byte_t         Spare10[10];

} DATA_PACKED_END DeadSectionInfoType;

//--------------------------------------------------------------------
//      지장물 정보 Type
//--------------------------------------------------------------------
// 한 개의 지장물에 대해 동일한 인덱스로 낙석, 보호 모두 생성
typedef struct DATA_PACKED_START _FallLockInfoType_t
{
    Byte_t         Name[20];    // 지장물 이름

    struct DATA_PACKED_START _FallLock_t
    {
        Byte_t         Name[20];    // 낙석 이름 (Input)
        Byte_t         Spare[6];
    } FallLock;  // 낙석

    struct DATA_PACKED_START _Proc_t
    {
        Byte_t         Name1[15];   // 보호 이름 (Input)
        Byte_t         Name2[15];

        Byte_t         Spare[6];
    } Proc;      // 보호

    struct DATA_PACKED_START _Release_t
    {
        Byte_t         Name[20];    // 표시 이름 (Input)
        Byte_t         Spare[6];
    } Release;   // 표시(해제) <=== 규격변경 이후 삭제(사용하지 않음)

    Byte_t         Spare10[10];

} DATA_PACKED_END FallLockInfoType;


//--------------------------------------------------------------------
//      Heater 정보 Type
//--------------------------------------------------------------------
// 한 개의 Heat에 대해 동일한 인덱스로 장애 알람 메시지 생성
typedef struct DATA_PACKED_START _HeatInfoType_t
{
    Byte_t         szHeatName[20];     // Heat 이름

    IO_Position    InputHeat;          // Heat Input (동작상태 Input)
                                       //   여자일 때 동작중

    IO_Position    InputFail;          // Fail Input(히터FAIL) - 규격개정 이후에 대해서만 입력 있음
                                       //   여자일 때 장애

    // 한 개의 Heat에 대해 최대 장애 알람 수 : MAX_CNT_HEATER_ALM
    struct DATA_PACKED_START _AlmData_t
    {
        Byte_t        szAlmMsg[20];    // 장애 알람 메시지
                                       // InputFail 여자 && InputAlarm 낙하 일 때 표시하는 알람 메시지

        IO_Position   InputAlarm;      // 장애에 대한 Input

    } AlmData[MAX_CNT_HEATER_ALM];  // 장애 메시지

    Byte_t         Spare10[10];

} DATA_PACKED_END HeatInfoType;

//--------------------------------------------------------------------
//      임시속도(Slow Order) 정보 Type
//--------------------------------------------------------------------
typedef struct DATA_PACKED_START _SlowOrderInfoType_t
{
    Byte_t         Name[20];      // 이름

    Byte_t         Type;          // bit0=1 : 인접역 임시속도

    Byte_t         TrackNo[MAX_SLOW_ORDER_TRACK];     // 임시속도 설정구간의 궤도
    Byte_t         SIgnalNo[MAX_SLOW_ORDER_SIGNAL];   // 임시속도 설정구간의 신호기

    Byte_t         Spare10[10];

} DATA_PACKED_END SlowOrderInfoType;

//--------------------------------------------------------------------
//      출발반응등 정보 Type
//--------------------------------------------------------------------
typedef struct DATA_PACKED_START _STLInfoType_t
{
    Byte_t         Name[20];      // 이름

    Byte_t         Spare10[20];

} DATA_PACKED_END STLInfoType;

//--------------------------------------------------------------------
//      끌림 감시장치 정보 Type
//--------------------------------------------------------------------
typedef struct DATA_PACKED_START _AttractionInfoType_t
{
    Byte_t         Name[20];      // 이름

    Byte_t         Spare10[20];

} DATA_PACKED_END AttractionInfoType;

//--------------------------------------------------------------------
//      기타 고장 정보 Type
//--------------------------------------------------------------------
typedef struct DATA_PACKED_START _FaultInfoType_t
{
    Byte_t         Name[20];      // 이름

    Byte_t         Type;          // 1 : 출발대용표시등

    Byte_t         Hide;          // bit0=1 : 화면 숨김

    Byte_t         Spare10[20];

} DATA_PACKED_END FaultInfoType;


//--------------------------------------------------------------------
//      CPT 정보 Type (열차진입 방호장치)
//--------------------------------------------------------------------
typedef struct DATA_PACKED_START _CPTInfoType_t
{
    char          CptName[20];    // CPT 이름

    Byte_t        CptNo;          // Index
    Byte_t        CptKind;        // bit0=1 : 출발용 - 신호기 점멸 표시
                                  // bit1=1 : 장내용 - 궤도 점유 표시 (비장애, 비점멸)

    IO_Position   CptInput;       // 입력 위치

    struct DATA_PACKED_START _CptInfo_t
    {
        Byte_t    SignalNo;       // CPT 관련 신호기 (폐색 신호기)
        Byte_t    TrackNo;        // CPT 관련 궤도 (폐색 궤도)
    } DATA_PACKED_END  CptEquip[MAX_CPT_EQUIP];

    Byte_t    spare2[10];
} DATA_PACKED_END  CPTInfoType;

//--------------------------------------------------------------------
//      소속역 정보 Type
//--------------------------------------------------------------------
typedef struct DATA_PACKED_START _DwellInfoType_t
{
    Byte_t  SubStnName[20];                         // 소속역 이름
    Byte_t  SubStnNo;                               // 소속역 번호
    Byte_t  PlatFormNo;                             // 플랫폼 번호

    struct DATA_PACKED_START _SubStnInfo_t
    {
        Byte_t  EndStation;  // bit0=1 : 종착역 구분(Dwell Lamp 없음)
        Byte_t  UpKind;      // 상선, 하선 구분 (0x00:상선, 0x01:하선)
        Byte_t  Spare1;
        Byte_t  Spare2;

    } DATA_PACKED_END  SubStnInfo;  // 역정보

    Byte_t  PlatFormTrk[MAX_DWELL_TRACK_FLATFORM];    // 플랫폼 궤도 인덱스
    Byte_t  PlatFormSig;                              // 정차등 제어시 현시상태 체크하는 신호기 인덱스
    Byte_t  TrainInfoNo;                              // 열차번호 창 인덱스

    Byte_t  EmgTrack[MAX_DWELL_TRACK_EMG];            // 비상정지 설정 궤도
    Byte_t  DepSignal;                                // 출발신호기 인덱스

    Byte_t  Spare[20];

} DATA_PACKED_END  DwellInfoType;


//--------------------------------------------------------------------
//      제어 건널목 정보 Type
//--------------------------------------------------------------------
typedef struct DATA_PACKED_START _LC_CTRL_INFO_TYPE_t
{
    Byte_t  Name1[10];
    Byte_t  Name2[10];

    Byte_t  LcNo;

    Byte_t  Status;      // bit 0 = 0 : 출력여자일 때 입력여자
                         //         1 : 출력여자일 때 입력낙하
                         // bit 1 = 1 : 부정입력 없음
                         // bit 2 = 1 : 부정출력 없음 - 사용하지 않음
                         // bit 3 = 1 : 동작불능 없음

    IO_Position    LC_Inp;    // 입력
    IO_Position    LC_Out;    // 출력

    Byte_t  Spare[10];
} DATA_PACKED_END  LC_CTRL_INFO_TYPE;


//====================================================================
//      DataBase Table
//====================================================================
// 파일 : 역이름_연동_데이터.bin
typedef struct DATA_PACKED_START _DBstruct_t
{
    StationInfoType           _STN_Info;                          // 역     정보 Table
    InterLockInfoType         _ILK_Info[MAX_ROUTE];               // 연동   정보 Table
    RouteInfoType             _RTE_Info[MAX_ROUTE];               // 진로   정보 Table
    TrackInfoType             _TRK_Info[MAX_TRACK];               // 궤도   정보 Table
    SignalInfoType            _SIG_Info[MAX_SIGNAL];              // 신호기 정보 Table
    SwitchInfoType            _SWH_Info[MAX_SWITCH];              // 전철기 정보 Table
    BlockTagInfoType          _BLK_Info[MAX_BLOCK];               // 폐색   정보 Table
    LevelCrossInfoType        _LCS_Info[MAX_LEVEL_CROSS];         // 건널목 정보 Table

    DeadSectionInfoType       _DeadSec_Info[MAX_DEAD_SECTION];    // 절연구간 정보 Table
    FallLockInfoType          _FallLck_Info[MAX_FALL_LOCK];       // 지장물 정보 Table
    HeatInfoType              _Heat_Info[MAX_CNT_HEATER];         // Heat 고장정보

    SlowOrderInfoType         _SO_Info[MAX_SLOW_ORDER];           // 임시속도(Slow Order)
    STLInfoType               _Stl_Info[MAX_STL];                 // 출발반응등
    AttractionInfoType        _Attract_Info[MAX_ATTRACT];         // 끌림 감시장치
    FaultInfoType             _Fault_Info[MAX_ETC_FAULT];         // 기타고장정보

    CPTInfoType               _CPT_Info[MAX_CPT_INFO];            // 열차진입 방지(CPT) Table
    DwellInfoType             _DWL_Info[MAX_DWELL];               // 소속역 정보 Table

    LC_CTRL_INFO_TYPE         _LC_CTRL_INFO[MAX_LEVELCROSS_CTRL]; // 제어건널목 정보 Table

} DATA_PACKED_END  DBstruct;


#if defined (_WIN32)
#  pragma pack (pop)
#endif


#endif  /* #ifndef __EI_IP_SB_STRUCT__H__ */
