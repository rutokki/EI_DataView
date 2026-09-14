#ifndef __LOGICDEFINE__h__
#define __LOGICDEFINE__h__


#if defined (_WIN32)
#  pragma pack (push)
#  pragma pack (1)
#endif


//===========================================================================
#define LOGIC_LINE_TYPE0                           0x00 // 
#define LOGIC_LINE_TYPE1                           0x01 // ㅏ
#define LOGIC_LINE_TYPE2                           0x02 // ㅜ
#define LOGIC_LINE_TYPE3                           0x03 // +
#define LOGIC_LINE_TYPE4                           0x04 // ㅗ
#define LOGIC_LINE_TYPE5                           0x05 // ㅡ
#define LOGIC_LINE_TYPE6                           0x06 // ㄴ
#define LOGIC_LINE_TYPE7                           0x07 // ┘
#define LOGIC_LINE_TYPE8                           0x08 // ㅓ
#define LOGIC_LINE_TYPE9                           0x09 // ㄱ
#define LOGIC_LINE_TYPE10                          0x10 // ㅣ
#define LOGIC_LINE_TYPE11                          0x11 // ┌


//------------------------------------------------------------
enum RELAY_TYPE { POSITIVE, NAGATIVE, _INPUT, _OUTPUT, NEUTRAL };

enum LINE_TYPE 
{
    LINE_TYPE0  = 0x00,
    LINE_TYPE1  = 0x01, // ㅏ
    LINE_TYPE2  = 0x02, // ㅜ
    LINE_TYPE3  = 0x03, // +
    LINE_TYPE4  = 0x04, // ㅗ
    LINE_TYPE5  = 0x05, // ㅡ
    LINE_TYPE6  = 0x06, // ㄴ
    LINE_TYPE7  = 0x07, // ┘
    LINE_TYPE8  = 0x08, // ㅓ
    LINE_TYPE9  = 0x09, // ㄱ
    LINE_TYPE10 = 0x10, // ㅣ
    LINE_TYPE11 = 0x11  // ┌
};

//===========================================================================
//------------------------------------------------------------
#define IDS_FILE_NAME_DB                           _T("%s_연동_데이터.bin")      // 연동 DB 파일
#define IDS_FILE_NAME_DB_ALL                       _T("*_연동_데이터.bin")
#define IDS_FILE_NAME_IOCARD                       _T("%s_IOCard.bin")           // IO Card (역이름_IOCard.bin)
#define IDS_FILE_NAME_LOGIC_DB                     _T("%s_연동_데이터_폐색.bin") // 로직생성시에만 사용하는 조건
#define IDS_FILE_NAME_LOGIC_DB_ALL                 _T("*_연동_데이터_폐색.bin")
#define IDS_FILE_NAME_TRACK_ERROR                  _T("%s_track_error.Txt")      // 궤도 정상 점유상태 조건

#define IDS_FILE_NAME_RELAY_VARIABLE               _T("%s_1RelayVariable.Txt")   // 로직변수 리스트 (역이름_RelayVariable.Txt)
#define IDS_FILE_NAME_RELAY_VARIABLE_ALL           _T("*_1RelayVariable.Txt")
#define IDS_FILE_NAME_RELAY_IOLIST                 _T("%s_2RelayIoList.Txt")     // 로직 IO 리스트
#define IDS_FILE_NAME_RELAY_IOLIST_ALL             _T("*_2RelayIoList.Txt")
#define IDS_FILE_NAME_RELAY_INDEX                  _T("%s_3RelayIndex.Txt")      // 로직변수 인덱스
//#define IDS_FILE_NAME_ROMDB                        _T("%s_ROM_ilf.bin")        // 전체 RomDB파일 (연동 DB + 로직)
#define IDS_FILE_NAME_LOGIC_VARIABLE               _T("%s_LogicVariable.Dat")    // 로직변수 이름 리스트
#define IDS_FILE_NAME_LOGIC_VARIABLE_ALL           _T("*_LogicVariable.Dat")
#define IDS_FILE_NAME_LC_LOGIC_VARIABLE            _T("%s_LogicVariable_LC.Dat") // 제어건널목 로직변수 이름 리스트
#define IDS_FOLDER_LOGIC_FILES                     _T("Data_Logic")              // Logic파일에 대한 폴더 이름
#define IDS_FOLDER_DB_FILES                        _T("Data_DB")                 // DB파일에 대한 폴더 이름
#define IDS_FOLDER_XML_FILES                       _T("Data_Xml")                // XML파일에 대한 폴더 이름
#define IDS_LOGIC_RELAY_TXT                        _T("Txt")                     // 로직 계전기 리스트 저장 파일 확장자
#define IDS_LOGIC_FILE_TXT                         _T("LDat")                    // 로직 내용 저장 파일 확장자
#define IDS_LOGIC_FILE_EXT                         _T("LDBX")                    // 로직파일 확장자
#define IDS_LOGIC_ERROR_FILE_NAME                  _T("%s_LogicError.Txt")       // 로직생성 실패시 에러내용

//------------------------------------------------------------
#define ID_LOGIC_NO_001_VRD                        001
#define ID_LOGIC_NO_010_CMD                        010
#define ID_LOGIC_NO_030_ROUTE_SET                  030
#define ID_LOGIC_NO_050_COMMON                     050
#define ID_LOGIC_NO_080_INPUT                      80
#define ID_LOGIC_NO_100_TRACK                      100
#define ID_LOGIC_NO_110_TRACK                      0    // 110
#define ID_LOGIC_NO_120_TRACK                      0    // 120
#define ID_LOGIC_NO_150_TRACK                      0    // 150
#define ID_LOGIC_NO_170_BLOCK                      170
#define ID_LOGIC_NO_200_SWITCH                     200
#define ID_LOGIC_NO_210_SWITCH_SPC                 210
#define ID_LOGIC_NO_220_SWITCH                     0    // 220
#define ID_LOGIC_NO_250_SWITCH                     0    // 250
#define ID_LOGIC_NO_300_SIGNAL                     300
#define ID_LOGIC_NO_300_SIGNAL_NAME                300
#define ID_LOGIC_NO_300_SIGNAL_SPC                 310
#define ID_LOGIC_NO_350_ROUTE                      350
#define ID_LOGIC_NO_360_ROUTE_TIMELOCK             360
#define ID_LOGIC_NO_370_ROUTE_NAME                 370
#define ID_LOGIC_NO_600_LC_FAIL                    600
#define ID_LOGIC_NO_610_DEAD_SECTION               610
#define ID_LOGIC_NO_620_FALL_LOCK                  620
#define ID_LOGIC_NO_630_CPT                        630
#define ID_LOGIC_NO_640_STL                        640
#define ID_LOGIC_NO_650_FAULT                      650
#define ID_LOGIC_NO_800_OUTPUT                     800
#define ID_LOGIC_NO_850_TRACK_DISPLAY              850
#define ID_LOGIC_NO_900_LC_CONTROL                 900


#define IDS_LOGIC_NAME_001_VRD                     _T("001-VRD,EIS상태,통신상태")
#define IDS_LOGIC_NAME_010_CMD                     _T("010-취급명령,운전모드")
#define IDS_LOGIC_NAME_030_ROUTE_SET               _T("030-ROUTE-SET")
#define IDS_LOGIC_NAME_050_COMMON                  _T("050-역공통")
#define IDS_LOGIC_NAME_080_INPUT                   _T("080-입력UR")
#define IDS_LOGIC_NAME_100_TRACK                   _T("100-TRACK")                  // _T("100-TRACK(UR,LSR,RSR,LOCK,FAIL)")
#define IDS_LOGIC_NAME_110_TRACK                   _T("")                           // _T("110-TRACK(LSR,RSR,비상,정상,BELL)")
#define IDS_LOGIC_NAME_120_TRACK                   _T("")                           // _T("120-TRACK(LSR,RSR,BELL)")
#define IDS_LOGIC_NAME_150_TRACK                   _T("")                           // _T("150-TRACK(표찰)")
#define IDS_LOGIC_NAME_170_BLOCK                   _T("170-BLOCK")                  // _T("170-BLOCK(장내,출발,RR,CNR)")
#define IDS_LOGIC_NAME_200_SWITCH                  _T("200-SWITCH")                 // _T("200-SWITCH(UR,FAIL,Position,단조건)")
#define IDS_LOGIC_NAME_210_SWITCH_SPC              _T("200-SWITCH(타역)")
#define IDS_LOGIC_NAME_220_SWITCH                  _T("")                           // _T("220-SWITCH 연동취급(NUR,RUR,ALockP)")
#define IDS_LOGIC_NAME_250_SWITCH                  _T("")                           // _T("250-SWITCH 단동취급(WLR,WR-N,WR-R출력)")
#define IDS_LOGIC_NAME_300_SIGNAL                  _T("300-SIGNAL")
#define IDS_LOGIC_NAME_300_SIGNAL_NAME             _T("300-SIGNAL(%s)")             // _T("300-SIGNAL(신호기이름)")
#define IDS_LOGIC_NAME_310_SIGNAL_SPC              _T("300-SIGNAL(타역)")
#define IDS_LOGIC_NAME_350_ROUTE                   _T("350-ROUTE(CR,OPP)")
#define IDS_LOGIC_NAME_350_ROUTE_ANYTHING          _T("350-ROUTE*")
#define IDS_LOGIC_NAME_360_ROUTE_TIME_LOCK         _T("360-ROUTE(TimeLock)")
#define IDS_LOGIC_NAME_370_ROUTE_NAME              _T("370-ROUTE(%s)")              // _T("370-ROUTE(진로이름)")
#define IDS_LOGIC_NAME_370_ROUTE_ANYTHING          _T("370-ROUTE*")
#define IDS_LOGIC_NAME_600_LC_FAIL                 _T("600-건널목,폐색신호기장애")  // 건널목,폐색신호기,히터
#define IDS_LOGIC_NAME_610_DEAD_SECTION            _T("")                           // _T("610-전차선 절연구간")
#define IDS_LOGIC_NAME_620_FALL_LOCK               _T("")                           // _T("620-지장물")
#define IDS_LOGIC_NAME_630_CPT                     _T("630-FAULT,STL,CPT")          // _T("630-열차진입 방호장치(CPT)")
#define IDS_LOGIC_NAME_640_STL                     _T("")                           // _T("640-출발반응등")
#define IDS_LOGIC_NAME_650_FAULT                   _T("")                           // _T("650-기타고장정보")
#define IDS_LOGIC_NAME_800_OUTPUT                  _T("800-출력OUT")
#define IDS_LOGIC_NAME_800_TRACK_DISPLAY           _T("850-궤도표시, 메시지")
#define IDS_LOGIC_NAME_900_LC_CONTROL              _T("900-건널목제어")



//===========================================================================
#define MAX_CHAR_BUFF                              100
#define MAX_BUFF                                   50

#define MAX_LOGIC_CHAR_BUFFER                      1024

#define MAX_LOGIC_FILES                            28      // 로직 파일 최대 수

#define ARRAY_NO_VRD                               1
#define ARRAY_NO_CMD                               2
#define ARRAY_NO_ROUTE_SET                         3
#define ARRAY_NO_COMMON                            4
#define ARRAY_NO_INPUT_080                         5
#define ARRAY_NO_TRACK_100                         6
#define ARRAY_NO_TRACK_110                         7
#define ARRAY_NO_TRACK_120                         8
#define ARRAY_NO_TRACK_150                         9
#define ARRAY_NO_BLOCK                             10
#define ARRAY_NO_SWITCH_200                        11
#define ARRAY_NO_SWITCH_SPC_210                    12
#define ARRAY_NO_SWITCH_220                        13
#define ARRAY_NO_SWITCH_250                        14
#define ARRAY_NO_SIGNAL                            15
#define ARRAY_NO_SIGNAL_SPC                        16
#define ARRAY_NO_ROUTE_350                         17
#define ARRAY_NO_ROUTE_360                         18
#define ARRAY_NO_ROUTE_370                         19
#define ARRAY_NO_LC_FAIL                           20
#define ARRAY_NO_DEAD_SECTION                      21
#define ARRAY_NO_FALL_LOCK                         22
#define ARRAY_NO_CPT                               23
#define ARRAY_NO_OUTPUT_800                        24
#define ARRAY_NO_DISPLAY_MSG_850                   25
#define ARRAY_NO_STL                               26
#define ARRAY_NO_FAULT                             27


//------------------------------------------------------------
#define ID_PROGRESS_OPERATOR                       0x1F

#define ID_PROGRESS_NO_NAME                        0x00
#define ID_PROGRESS_READ                           0x01
#define ID_PROGRESS_WRITE                          0x02
#define ID_PROGRESS_MAKE_LOGIC                     0x04
#define ID_PROGRESS_PATH_NAME                      0x08
#define ID_PROGRESS_REMOVE                         0x10
#define ID_PROGRESS_FAIL                           0x80

#define ID_PROGRESS_SPACE_LEVEL0                   0x00
#define ID_PROGRESS_SPACE_LEVEL1                   0x10
#define ID_PROGRESS_SPACE_LEVEL2                   0x20
#define ID_PROGRESS_SPACE_LEVEL3                   0x40

//===========================================================================
// 신호기 출력에 대한 Bit
#define MAX_OUT_SIG_R                              0x01
#define MAX_OUT_SIG_G                              0x02
#define MAX_OUT_SIG_Y                              0x04
#define MAX_OUT_SIG_Y1                             0x08

//===========================================================================
#define COLOR_LINE_GRID1                           RGB(224, 224, 224);
#define COLOR_LINE_GRID2                           RGB(255, 224, 224);
#define COLOR_LINE_GRID3                           RGB(127, 127, 127);
#define COLOR_LINE_REVERSE                         RGB(128,   0,  64);
#define COLOR_LINE_WIRE                            RGB(  0,   0,   0);
#define COLOR_LINE_WIRE_WHITE                      RGB(255, 255, 255);
#define COLOR_LINE_TRIANGLE                        RGB(  0,   0, 255);
#define COLOR_LINE_POSITIVE                        RGB(  0,   0, 255);
#define COLOR_LINE_POSITIVE_WHITE                  RGB(255, 255, 255);
#define COLOR_LINE_NEGATIVE                        RGB(255,   0,   0);
#define COLOR_LINE_GROUP                           RGB(255,   0,   0);
#define COLOR_LINE_COPY                            RGB(255,   0,   0);
#define COLOR_LINE_CONTACT2                        RGB(255, 255, 255);

#define COLOR_LINE_GRID                            RGB(224, 224, 224)
#define COLOR_TEXT_NUMBER                          RGB(128, 128, 128)
#define COLOR_CURSOR                               RGB( 64,  64, 255)
#define COLOR_CURSOR_SELECT                        RGB(128, 128,  64)
#define COLOR_CONTACT                              RGB(128,   0,   0)
#define COLOR_GROUP_GREEN                          RGB(  0, 128,   0)
#define COLOR_SEARCH                               RGB(255,   0,   0)
#define COLOR_ROOT                                 RGB(128,   0, 255)
#define COLOR_SYMBOL                               RGB(  0, 200,   0)

//===========================================================================
#define IDT_THREAD_EXIT_WAIT                       3000u       // thread 종료시 wait 시간
#define IDT_THREAD_EXIT_WAIT_1000                  1000u
#define IDT_THREAD_EXIT_WAIT_500                   500u
#define IDT_THREAD_EXIT_WAIT_300                   300u

#define IDT_THREAD_SLEEP_10                        10u
#define IDT_THREAD_SLEEP_20                        20u
#define IDT_THREAD_SLEEP_30                        30u
#define IDT_THREAD_SLEEP_40                        40u
#define IDT_THREAD_SLEEP_50                        50u
#define IDT_THREAD_SLEEP_100                       100u
#define IDT_THREAD_SLEEP_200                       200u
#define IDT_THREAD_SLEEP_500                       500u
#define IDT_THREAD_SLEEP_1000                      1000u
#define IDT_THREAD_SLEEP_2000                      2000u
#define IDT_THREAD_SLEEP_5000                      5000u
#define IDT_THREAD_SLEEP_10000                     10000u
#define IDT_THREAD_SLEEP_20000                     20000u

//------------------------------------------------------------



//===========================================================================
// LOGIC_VARIABLE.LogicKind 값에 대한 정의
#define LOGIC_KIND_VRD                             INP_VRD              // _T('V')    // VRD
#define LOGIC_KIND_TRACK                           INP_TRACK            // _T('T')    // 궤도
#define LOGIC_KIND_SWITCH                          INP_SWITCH           // _T('P')    // 선로전환기
#define LOGIC_KIND_SIGNAL                          INP_SIGNAL           // _T('S')    // 신호기
#define LOGIC_KIND_LMR                             INP_LMR              // _T('L')    // LMR
#define LOGIC_KIND_ROUTE                           INP_ROUTE_SELECT     // _T('R')    // 진로선별등
#define LOGIC_KIND_STATION                         INP_STATION          // _T('N')    // 역공통
#define LOGIC_KIND_BLOCK                           INP_BLOCK            // _T('B')    // 폐색
#define LOGIC_KIND_LEVEL_CROSS                     INP_LEVEL_CROSS      // _T('C')    // 건널목 (고장검지)
#define LOGIC_KIND_LEVEL_CONTROL                   INP_LEVEL_CONTROL    // _T('c')    // 건널목 (제어건널목)
#define LOGIC_KIND_SPC_TRACK                       INP_SPC_TRACK        // _T('t')    // 타역 궤도
#define LOGIC_KIND_SPC_SIGNAL                      INP_SPC_SIGNAL       // _T('s')    // 타역 신호기
#define LOGIC_KIND_SPC_SWITCH                      INP_SPC_SWITCH       // _T('p')    // 타역 선로전환기

#define LOGIC_KIND_DEAD_SECTION                    INP_DEAD_SECTION     // _T('D')    // 전차선 절연구간
#define LOGIC_KIND_FALL_LOCK                       INP_FALL_LOCK        // _T('J')    // 지장물

#define LOGIC_KIND_STL_INFO                        INP_STL_INFO         // _T('K')    // 출발반응등
#define LOGIC_KIND_ETC_FAULT                       INP_ETC_FAULT        // _T('F')    // 기타 고장
#define LOGIC_KIND_DWL_INFO                        INP_DWL_INFO         // _T('W')    // 소속역 정보

#define LOGIC_KIND_CPT                             INP_CPT_INFO         // _T('H')    // 열차진입 방지(CPT)
#define LOGIC_KIND_HEAT                            INP_HEAT_INFO        // _T('h')    // 히터

#define LOGIC_KIND_ALM_INFO                        INP_ALM_INFO         // _T('A')    // 알람 메시지 - Logic에서만 사용

#define LOGIC_KIND_EIS                             _T('E')

#define LOGIC_KIND_COMMAND                         _T('M')    // 모드에 따른 취급 구분

#define LOGIC_KIND_NOTHING                         _T('X')


//------------------------------------------------------------
#define LOGIC_NOTHING_STRING                       _T("##ERROR##")
#define LOGIC_NOTHING_STRING_UNICHAR               "##ERROR##"


//===========================================================================
// BiNo 값에 대한 정의

//------------------------------------------------------------
// 궤도 (LOGIC_KIND_TRACK)
#define LOGIC_BITNO_TRACK_IN                      0

//------------------------------------------------------------
// 신호기 (LOGIC_KIND_SIGNAL)
#define LOGIC_BITNO_SIGNAL_YR                     0
#define LOGIC_BITNO_SIGNAL_GR                     1
#define LOGIC_BITNO_SIGNAL_HR                     4
#define LOGIC_BITNO_SIGNAL_ULMR                   6    // 진로선별등
#define LOGIC_BITNO_SIGNAL_SHR                    7    // (무)유도

//------------------------------------------------------------
// LMR (LOGIC_KIND_LMR)
#define LOGIC_BITNO_LMR_GM                        0
#define LOGIC_BITNO_LMR_GA                        1
#define LOGIC_BITNO_LMR_YM                        2
#define LOGIC_BITNO_LMR_YA                        3
#define LOGIC_BITNO_LMR_Y1M                       4
#define LOGIC_BITNO_LMR_Y1A                       5
#define LOGIC_BITNO_LMR_RM                        6
#define LOGIC_BITNO_LMR_RA                        7

#define LOGIC_BITNO_LMR_GLMR                      0    // 중계신호기(GLMR)
#define LOGIC_BITNO_LMR_CLMR                      6    // 중계신호기(CLMR)

//------------------------------------------------------------
// 선로전환기 (LOGIC_KIND_SWITCH)
#define LOGIC_BITNO_SWITCH_KR_N                   0     // 단동(쌍동) 또는 메시지분리 A호 NK
#define LOGIC_BITNO_SWITCH_KR_R                   1     // 단동(쌍동) 또는 메시지분리 A호 RK
#define LOGIC_BITNO_SWITCH_BKR_N                  2     // 메시지분리 B호 NK
#define LOGIC_BITNO_SWITCH_BKR_R                  3     // 메시지분리 B호 RK
#define LOGIC_BITNO_SWITCH_WLR                    11
#define LOGIC_BITNO_SWITCH_WR_N                   12
#define LOGIC_BITNO_SWITCH_WR_R                   13
#define LOGIC_BITNO_SWITCH_PKR_N                  14    // 첨단   단동 또는 A호 KR-N
#define LOGIC_BITNO_SWITCH_PKR_R                  15    // 첨단   단동 또는 A호 KR-R
#define LOGIC_BITNO_SWITCH_FKR_N                  16    // 크로싱 단동 또는 A호 KR-N
#define LOGIC_BITNO_SWITCH_FKR_R                  17    // 크로싱 단동 또는 A호 KR-R
#define LOGIC_BITNO_SWITCH_PHPR                   18
#define LOGIC_BITNO_SWITCH_BPKR_N                 19    // 첨단   B호 KR-N
#define LOGIC_BITNO_SWITCH_BPKR_R                 20    // 첨단   B호 KR-R
#define LOGIC_BITNO_SWITCH_BFKR_N                 21    // 크로싱 B호 KR-N
#define LOGIC_BITNO_SWITCH_BFKR_R                 22    // 크로싱 B호 KR-R

//------------------------------------------------------------
// 폐색 (LOGIC_KIND_BLOCK)
// 복선자동
#define LOGIC_BITNO_BLOCK_DOUBLE_ABS_YY           0
#define LOGIC_BITNO_BLOCK_DOUBLE_ABS_Y            1
#define LOGIC_BITNO_BLOCK_DOUBLE_ABS_YG           2

// 단선자동
#define LOGIC_BITNO_BLOCK_SINGLE_ABS_DIR          0
#define LOGIC_BITNO_BLOCK_SINGLE_ABS_BR           1
#define LOGIC_BITNO_BLOCK_SINGLE_ABS_DR           2
#define LOGIC_BITNO_BLOCK_SINGLE_ABS_BLTR         3
#define LOGIC_BITNO_BLOCK_SINGLE_ABS_YY           4
#define LOGIC_BITNO_BLOCK_SINGLE_ABS_Y            5
#define LOGIC_BITNO_BLOCK_SINGLE_ABS_YG           6

// 연동폐색
#define LOGIC_BITNO_BLOCK_REL_ATPSR               0
#define LOGIC_BITNO_BLOCK_REL_AFR                 1
#define LOGIC_BITNO_BLOCK_REL_AEHAR               2
#define LOGIC_BITNO_BLOCK_REL_ABOR                3
#define LOGIC_BITNO_BLOCK_REL_DTPSR               4
#define LOGIC_BITNO_BLOCK_REL_DFR                 5
#define LOGIC_BITNO_BLOCK_REL_DEHAR               6
#define LOGIC_BITNO_BLOCK_REL_DBOR                7

// 지하철
#define LOGIC_BITNO_BLOCK_SUBWAY_START            0
#define LOGIC_BITNO_BLOCK_SUBWAY_OPEN             1
#define LOGIC_BITNO_BLOCK_SUBWAY_ARR              2
#define LOGIC_BITNO_BLOCK_SUBWAY_SIG              3

// 대야
#define LOGIC_BITNO_BLOCK_DAEYA_ARR_Y             0
#define LOGIC_BITNO_BLOCK_DAEYA_ARR_R             1
#define LOGIC_BITNO_BLOCK_DAEYA_DEP_Y             2
#define LOGIC_BITNO_BLOCK_DAEYA_DEP_R             3
#define LOGIC_BITNO_BLOCK_DAEYA_TPSR              4

// 의왕
#define LOGIC_BITNO_BLOCK_EUIWANG_HR              0
#define LOGIC_BITNO_BLOCK_EUIWANG_BHR             1
#define LOGIC_BITNO_BLOCK_EUIWANG_TR              2
#define LOGIC_BITNO_BLOCK_EUIWANG_TPSR            3
#define LOGIC_BITNO_BLOCK_EUIWANG_eHR             4

// 양방향 폐색 (정방향출발 && 역방향장내)
#define LOGIC_BITNO_BLOCK_BOTH1_YY                0
#define LOGIC_BITNO_BLOCK_BOTH1_Y                 1
#define LOGIC_BITNO_BLOCK_BOTH1_YG                2
#define LOGIC_BITNO_BLOCK_BOTH1_BIT3              3
#define LOGIC_BITNO_BLOCK_BOTH1_RR                4
#define LOGIC_BITNO_BLOCK_BOTH1_ZR                5
#define LOGIC_BITNO_BLOCK_BOTH1_ZDIR              6
#define LOGIC_BITNO_BLOCK_BOTH1_BLTR              7

// 양방향 폐색 (정방향장내 && 역방향출발)
#define LOGIC_BITNO_BLOCK_BOTH2_oRR               0
#define LOGIC_BITNO_BLOCK_BOTH2_oDR               1
#define LOGIC_BITNO_BLOCK_BOTH2_CNR               2
#define LOGIC_BITNO_BLOCK_BOTH2_BIT3              3
#define LOGIC_BITNO_BLOCK_BOTH2_oZR               4
#define LOGIC_BITNO_BLOCK_BOTH2_iRR               5
#define LOGIC_BITNO_BLOCK_BOTH2_iRDIR             6
#define LOGIC_BITNO_BLOCK_BOTH2_BLTR              7

// 고속선폐색
#define LOGIC_BITNO_BLOCK_EXPRESS_YY              0
#define LOGIC_BITNO_BLOCK_EXPRESS_Y               1
#define LOGIC_BITNO_BLOCK_EXPRESS_YG              2

//------------------------------------------------------------
// 공통 (LOGIC_KIND_STATION)
#define LOGIC_BITNO_STATION_VRD1                  1    // SYSVRD1/I   -> 1계VRD/I
#define LOGIC_BITNO_STATION_VRD2                  2    // SYSVRD2/I   -> 2계VRD/I
#define LOGIC_BITNO_STATION_VRD_EMG               3    // 비상 VRD
#define LOGIC_BITNO_STATION_CTC_EMG               4    // 비상 CTC

#define LOGIC_BITNO_STATION_N1                    8
#define LOGIC_BITNO_STATION_N1_X1                 9
#define LOGIC_BITNO_STATION_N2                    10
#define LOGIC_BITNO_STATION_N2_X2                 11
#define LOGIC_BITNO_STATION_UPS                   12
#define LOGIC_BITNO_STATION_UPS_AC                13
#define LOGIC_BITNO_STATION_ACR                   14    // 정류기 (14 ~ 33)

#define LOGIC_BITNO_STATION_CHARGE                34    // 축전지

#define LOGIC_BITNO_STATION_FUSE                  35    // 계전기랙 FUSE
#define LOGIC_BITNO_STATION_FUSE_EIS              36    // 연동논리부 FUSE (36 ~ 55)
#define LOGIC_BITNO_STATION_FUSE_EIS1             56    // 연동논리부 1계 FUSE
#define LOGIC_BITNO_STATION_FUSE_EIS2             57    // 연동논리부 2계 FUSE

#define LOGIC_BITNO_STATION_P_SOUTH               58
#define LOGIC_BITNO_STATION_P_NORTH               59
#define LOGIC_BITNO_STATION_DOOR                  60
#define LOGIC_BITNO_STATION_SHUNT_OFF             61

//#define LOGIC_BITNO_STATION_HEATER_SOUTH          62    // 히터남
//#define LOGIC_BITNO_STATION_HEATER_NORTH          63    // 히터북
//#define LOGIC_BITNO_STATION_HEATER_SOUTH_FAIL     64    // 히터남 Fail Message
//#define LOGIC_BITNO_STATION_HEATER_NORTH_FAIL     65    // 히터북 Fail Message
//#define LOGIC_BITNO_STATION_HEATER2_SOUTH         66
//#define LOGIC_BITNO_STATION_HEATER2_NORTH         67
//#define LOGIC_BITNO_STATION_HEATER2_SOUTH_FAIL    68
//#define LOGIC_BITNO_STATION_HEATER2_NORTH_FAIL    69
//#define LOGIC_BITNO_STATION_HEATER3_SOUTH         70
//#define LOGIC_BITNO_STATION_HEATER3_NORTH         71
//#define LOGIC_BITNO_STATION_HEATER3_SOUTH_FAIL    72
//#define LOGIC_BITNO_STATION_HEATER3_NORTH_FAIL    73

//#define LOGIC_BITNO_STATION_COMMAND_SUCCESS       201    // _T("취급/OK")

//------------------------------------------------------------
// 히터 (LOGIC_KIND_HEAT)
#define LOGIC_BITNO_HEATER_ACT                      0    // 히터 동작 입력(출력에 대한 입력 Feedback)
#define LOGIC_BITNO_HEATER_FAIL                     1    // 히터 장애
#define LOGIC_BITNO_HEATER_MESSAGE_FAIL             11   // 히터 장애 메시지 (BitNo : 11 ~ 20)

//------------------------------------------------------------
// 전차선 절연구간 (LOGIC_KIND_DEAD_SECTION)
#define LOGIC_BITNO_DEAD_SECTION_IN                 0

//------------------------------------------------------------
// 지장물검지 (LOGIC_KIND_FALL_LOCK)
#define LOGIC_BITNO_FALL_LOCK_FALL                  0    // 낙석
//#define LOGIC_BITNO_FALL_LOCK_PROC                  1    // 보호
#define LOGIC_BITNO_FALL_LOCK_PROC                  0    // 보호

//------------------------------------------------------------
// 건널목고장 (LOGIC_KIND_LEVEL_CROSS)
#define LOGIC_BITNO_LEVEL_CROSS_IN                  0

//------------------------------------------------------------
// 제어건널목 (LOGIC_KIND_LEVEL_CONTROL)
#define LOGIC_BITNO_LEVEL_CONTROL_IN                0

//------------------------------------------------------------
// 출발반응등 STL (LOGIC_KIND_STL_INFO)
#define LOGIC_BITNO_STL_IN                          0

//------------------------------------------------------------
// 기타고장 (LOGIC_KIND_ETC_FAULT)
#define LOGIC_BITNO_ETC_FAULT_IN                    0

//------------------------------------------------------------
// 열차진입장지 (LOGIC_KIND_CPT)
#define LOGIC_BITNO_CPT_IN                          0


//===========================================================================
// LOGIC_VARIABLE.LogicType 값에 대한 정의

#define LOGIC_TYPE_INPUT                           1    // 입력 _T('I')
#define LOGIC_TYPE_OUTPUT                          2    // 출력 _T('O')
#define LOGIC_TYPE_PDO                             4    // PDO  _T('P')

//---------------------------------------------------------------
// 취급 (LOGIC_VARIABLE.LogicKind=LOGIC_KIND_COMMAND) : _T('M')
//---------------------------------------------------------------
#define LOGIC_TYPE_CMD_LOCAL                       11    // 표시제어부 Local 취급 _T("CMD/LOCAL")
#define LOGIC_TYPE_CMD_CTC                         12    // 표시제어부 CTC   취급 _T("CMD/CTC")
#define LOGIC_TYPE_CMD_RC                          13    // 표시제어부 RC    취급 _T("CMD/RC")

//---------------------------------------------------------------
// EIS (LOGIC_VARIABLE.LogicKind=LOGIC_KIND_EIS) : _T('E')
//---------------------------------------------------------------
#define LOGIC_TYPE_EIS_CMD_LOCAL                   11
#define LOGIC_TYPE_EIS_CMD_RC                      12
#define LOGIC_TYPE_EIS_CMD_CTC                     13

//#define LOGIC_TYPE_EIS_FAIL                        13
#define LOGIC_TYPE_EIS_PB                          14

#define LOGIC_TYPE_EIS_INIT_CHECK                  16    // 기동시 체크 확인 (기동시 장애 없음 설정 Flag)
#define LOGIC_TYPE_EIS_INIT_CHECK_UR               17


// SYSTEM 상태 (1계)
#define LOGIC_TYPE_EIS_WORKER1                     21

#define LOGIC_TYPE_EIS_WORKER1_ILC_HEALTH          23
#define LOGIC_TYPE_EIS_WORKER1_ILC_ACTIVE          24

#define LOGIC_TYPE_EIS_WORKER1_CS_DATA_LINE        27
#define LOGIC_TYPE_EIS_WORKER1_CA_DATA_LINE        28
#define LOGIC_TYPE_EIS_WORKER1_IOC0_DATA_LINE      29
#define LOGIC_TYPE_EIS_WORKER1_IOC1_DATA_LINE      30
#define LOGIC_TYPE_EIS_WORKER1_IOC2_DATA_LINE      31
#define LOGIC_TYPE_EIS_WORKER1_IOC3_DATA_LINE      32
#define LOGIC_TYPE_EIS_WORKER1_IOC4_DATA_LINE      33
#define LOGIC_TYPE_EIS_WORKER1_IOC5_DATA_LINE      34
#define LOGIC_TYPE_EIS_WORKER1_IOC6_DATA_LINE      35
#define LOGIC_TYPE_EIS_WORKER1_IOC7_DATA_LINE      36
#define LOGIC_TYPE_EIS_WORKER1_IOC8_DATA_LINE      37
#define LOGIC_TYPE_EIS_WORKER1_IOC9_DATA_LINE      38

// SYSTEM 상태 (2계)
#define LOGIC_TYPE_EIS_WORKER2                     45

#define LOGIC_TYPE_EIS_WORKER2_ILC_HEALTH          47
#define LOGIC_TYPE_EIS_WORKER2_ILC_ACTIVE          48

#define LOGIC_TYPE_EIS_WORKER2_CS_DATA_LINE        51
#define LOGIC_TYPE_EIS_WORKER2_CA_DATA_LINE        52
#define LOGIC_TYPE_EIS_WORKER2_IOC0_DATA_LINE      53
#define LOGIC_TYPE_EIS_WORKER2_IOC1_DATA_LINE      54
#define LOGIC_TYPE_EIS_WORKER2_IOC2_DATA_LINE      55
#define LOGIC_TYPE_EIS_WORKER2_IOC3_DATA_LINE      56
#define LOGIC_TYPE_EIS_WORKER2_IOC4_DATA_LINE      57
#define LOGIC_TYPE_EIS_WORKER2_IOC5_DATA_LINE      58
#define LOGIC_TYPE_EIS_WORKER2_IOC6_DATA_LINE      59
#define LOGIC_TYPE_EIS_WORKER2_IOC7_DATA_LINE      60
#define LOGIC_TYPE_EIS_WORKER2_IOC8_DATA_LINE      61
#define LOGIC_TYPE_EIS_WORKER2_IOC9_DATA_LINE      62

// 통신
#define LOGIC_TYPE_EIS_LCC1_LINEA                  69
#define LOGIC_TYPE_EIS_LCC1_LINEB                  70
#define LOGIC_TYPE_EIS_LCC2_LINEA                  71
#define LOGIC_TYPE_EIS_LCC2_LINEB                  72
#define LOGIC_TYPE_EIS_LCC1_LINEA_ACT              73
#define LOGIC_TYPE_EIS_LCC1_LINEB_ACT              74
#define LOGIC_TYPE_EIS_LCC2_LINEA_ACT              75
#define LOGIC_TYPE_EIS_LCC2_LINEB_ACT              76
#define LOGIC_TYPE_EIS_CTC_LINEA                   77
#define LOGIC_TYPE_EIS_CTC_LINEB                   78
#define LOGIC_TYPE_EIS_RC_LINEA                    79
#define LOGIC_TYPE_EIS_RC_LINEB                    80
#define LOGIC_TYPE_EIS_LMC_LINEA                   81
#define LOGIC_TYPE_EIS_LMC_LINEB                   82


//---------------------------
// 운전모드 - 상태
#define LOGIC_TYPE_EIS_MODE_LOCAL_ACT              85    // Local Mode
#define LOGIC_TYPE_EIS_MODE_LOCAL_REQ              86    // Local FLASH
#define LOGIC_TYPE_EIS_MODE_CTC_ACT                87    // CTC   Mode
#define LOGIC_TYPE_EIS_MODE_CTC_REQ                88    // CTC   FLASH
#define LOGIC_TYPE_EIS_MODE_RC_ACT                 89    // RC    Mode
#define LOGIC_TYPE_EIS_MODE_RC_REQ                 90    // RC    FLASH

//---------------------------
// 운전모드 취급
#define LOGIC_TYPE_EIS_MODE_CMD_LOCAL              93     // LOCAL 취급
#define LOGIC_TYPE_EIS_MODE_CMD_LOCAL_EMG          94     // LOCAL 비상 취급
#define LOGIC_TYPE_EIS_MODE_CMD_LOCAL_CANCEL       95     // Local 취소 취급

#define LOGIC_TYPE_EIS_MODE_CMD_CTC                97     // CTC   취급
#define LOGIC_TYPE_EIS_MODE_CMD_CTC_EMG            98     // CTC   비상 취급
#define LOGIC_TYPE_EIS_MODE_CMD_CTC_CANCEL         99     // CTC   취소 취급

#define LOGIC_TYPE_EIS_MODE_CMD_RC                 101    // RC    취급
#define LOGIC_TYPE_EIS_MODE_CMD_RC_EMG             102    // RC    비상 취급
#define LOGIC_TYPE_EIS_MODE_CMD_RC_CANCEL          103    // RC    취소 취급

//---------------------------
// 원격제어 통신
#define LOGIC_TYPE_EIS_RCC1_LINEA                  115
#define LOGIC_TYPE_EIS_RCC1_LINEB                  116
#define LOGIC_TYPE_EIS_RCC2_LINEA                  117
#define LOGIC_TYPE_EIS_RCC2_LINEB                  118
#define LOGIC_TYPE_EIS_RCC1_LINEA_ACT              119
#define LOGIC_TYPE_EIS_RCC1_LINEB_ACT              120
#define LOGIC_TYPE_EIS_RCC2_LINEA_ACT              121
#define LOGIC_TYPE_EIS_RCC2_LINEB_ACT              122


//---------------------------
// 표시제어부 상태
#define LOGIC_TYPE_EIS_LCC1_ACT                    201

#define LOGIC_TYPE_EIS_LCC2_ACT                    206


//---------------------------------------------------------------
// VRD (LOGIC_VARIABLE.LogicKind=LOGIC_KIND_VRD) : _T('V')
//---------------------------------------------------------------
#define LOGIC_TYPE_VRD_CMD_LOCAL                   11    // 표시제어부 취급 (RUN/CMD)
#define LOGIC_TYPE_VRD_CMD_RC                      12
#define LOGIC_TYPE_VRD_CMD_CTC                     13


#define LOGIC_TYPE_VRD_PB                          14
#define LOGIC_TYPE_VRD_FAIL                        15

#define LOGIC_TYPE_VRD_INPUT                       21

#define LOGIC_TYPE_VRD_OUTPUT                      26

//#define LOGIC_TYPE_VRD_UR_ON                       31
//#define LOGIC_TYPE_VRD_UR_OFF                      32

#define LOGIC_TYPE_RUN                             41
#define LOGIC_TYPE_VRD_EMG                         42    // 비상 RUN (Input)

#define LOGIC_TYPE_VRD_ACT                         43    // RUN/ACT
#define LOGIC_TYPE_RUN1_ACT                        44    // 1계RUN/ACT
#define LOGIC_TYPE_RUN2_ACT                        45    // 2계RUN/ACT

#define LOGIC_TYPE_VRD_ERROR                       51    // RUN/FAIL
#define LOGIC_TYPE_SYSVRD1_ACT                     52    // 1계VRD/ACT
#define LOGIC_TYPE_SYSVRD2_ACT                     53    // 2계VRD/ACT
#define LOGIC_TYPE_SYSVRD_ALIVE                    54    // VRD/ACT

#define LOGIC_TYPE_NOT_EXPECT_INPUT                61    // _T("부정 입력")
#define LOGIC_TYPE_NOT_EXPECT_OUTPUT               62    // _T("부정 출력")

//---------------------------------------------------------------
// Station (LOGIC_VARIABLE.LogicKind=LOGIC_KIND_STATION) : _T('N')
//---------------------------------------------------------------
#define LOGIC_TYPE_STATION_CMD_PB_LOCAL            11    // 표시제어부 취급
#define LOGIC_TYPE_STATION_CMD_CB_LOCAL            12    // 표시제어부 취소 취급
//#define LOGIC_TYPE_STATION_CMD_HEATER_PB_LOC       13    // 표시제어부 히터 취급
//#define LOGIC_TYPE_STATION_CMD_HEATER_CB_LOC       14    // 표시제어부 히터 취소 취급
//#define LOGIC_TYPE_STATION_CMD_HEATER_SOUTH_PB_LOC       13    // 표시제어부 히터 남 취급
//#define LOGIC_TYPE_STATION_CMD_HEATER_SOUTH_CB_LOC       14    // 표시제어부 히터 남 취소 취급
//#define LOGIC_TYPE_STATION_CMD_HEATER_NORTH_PB_LOC 15    // 표시제어부 히터 북 취급
//#define LOGIC_TYPE_STATION_CMD_HEATER_NORTH_CB_LOC 16    // 표시제어부 히터 북 취소 취급
#define LOGIC_TYPE_STATION_CMD_SIGSTOP_PB_LOCAL    17    // 표시제어부 일괄정지 취급
#define LOGIC_TYPE_STATION_CMD_SIGSTOP_CB_LOCAL    18    // 표시제어부 일괄정지 취소 취급
#define LOGIC_TYPE_STATION_CMD_SHUNTOFF_PB_LOCAL   19    // 표시제어부 입환소등 취급
#define LOGIC_TYPE_STATION_CMD_SHUNTOFF_CB_LOCAL   20    // 표시제어부 입환소등 취소 취급

#define LOGIC_TYPE_STATION_CMD_PB_RC               21    // RC 취급
#define LOGIC_TYPE_STATION_CMD_CB_RC               22
//#define LOGIC_TYPE_STATION_CMD_HEATER_SOUTH_PB_RC  23
//#define LOGIC_TYPE_STATION_CMD_HEATER_SOUTH_CB_RC  24
//#define LOGIC_TYPE_STATION_CMD_HEATER_NORTH_PB_RC  25
//#define LOGIC_TYPE_STATION_CMD_HEATER_NORTH_CB_RC  26
#define LOGIC_TYPE_STATION_CMD_SIGSTOP_PB_RC       27
#define LOGIC_TYPE_STATION_CMD_SIGSTOP_CB_RC       28
#define LOGIC_TYPE_STATION_CMD_SHUNTOFF_PB_RC      29
#define LOGIC_TYPE_STATION_CMD_SHUNTOFF_CB_RC      30

#define LOGIC_TYPE_STATION_CMD_PB_CTC              31    // CTC 취급
#define LOGIC_TYPE_STATION_CMD_CB_CTC              32
//#define LOGIC_TYPE_STATION_CMD_HEATER_SOUTH_PB_CTC 33
//#define LOGIC_TYPE_STATION_CMD_HEATER_SOUTH_CB_CTC 34
//#define LOGIC_TYPE_STATION_CMD_HEATER_NORTH_PB_CTC 35
//#define LOGIC_TYPE_STATION_CMD_HEATER_NORTH_CB_CTC 36
#define LOGIC_TYPE_STATION_CMD_SIGSTOP_PB_CTC      37
#define LOGIC_TYPE_STATION_CMD_SIGSTOP_CB_CTC      38
#define LOGIC_TYPE_STATION_CMD_SHUNTOFF_PB_CTC     39
#define LOGIC_TYPE_STATION_CMD_SHUNTOFF_CB_CTC     40

//#define LOGIC_TYPE_STATION_HEATER_PB               51
//#define LOGIC_TYPE_STATION_HEATER_CB               52

//#define LOGIC_TYPE_STATION_HEATER_SOUTH_PB         53
//#define LOGIC_TYPE_STATION_HEATER_SOUTH_CB         54
//#define LOGIC_TYPE_STATION_HEATER_NORTH_PB         55
//#define LOGIC_TYPE_STATION_HEATER_NORTH_CB         56

#define LOGIC_TYPE_STATION_SIGSTOP_PB              57    // 신호기 일괄정지
#define LOGIC_TYPE_STATION_SIGSTOP_CB              58

#define LOGIC_TYPE_STATION_SHUNTOFF_PB             59    // 입환소등 취급
#define LOGIC_TYPE_STATION_SHUNTOFF_CB             60
#define LOGIC_TYPE_STATION_SHUNTOFF_OUT_UR_OFF     61    // 입환소등 출력 최대 시간

#define LOGIC_TYPE_STATION_SHUNTOFF_NOT_ON         62    // 입환소등 동작불능
//#define LOGIC_TYPE_STATION_SHUNTOFF_NOT_OFF        63

#define LOGIC_TYPE_STATION_FAIL                    71

#define LOGIC_TYPE_STATION_INPUT                   81    // 공통설비 입력
//#define LOGIC_TYPE_STATION_INPUT_HEAT_NORTH        82
//#define LOGIC_TYPE_STATION_INPUT_HEAR_NORTH_FAIL   83
//#define LOGIC_TYPE_STATION_INPUT_HEAR_SOUTH        84
//#define LOGIC_TYPE_STATION_INPUT_HEAR_SOUTH_FAIL   85
//#define LOGIC_TYPE_STATION_INPUT_SHUNT_OFF         86    // 입환R/I (입력)

//#define LOGIC_TYPE_STATION_OUTPUT_HEAT_NORTH       87    // 히터 북/O (출력)
//#define LOGIC_TYPE_STATION_OUTPUT_HEAT_SOUTH       88    // 히터 남/O (출력)
#define LOGIC_TYPE_STATION_OUTPUT_SHUNT_OFF        89    // 입환R/O (출력)

//#define LOGIC_TYPE_STATION_UR_ON                   91
//#define LOGIC_TYPE_STATION_UR_OFF                  92

// 설비상태
#define LOGIC_TYPE_STATION_SIGNAL_FAIL             101    // 신호기 고장
#define LOGIC_TYPE_STATION_SWITCH_FAIL             102    // 전철기 고장
#define LOGIC_TYPE_STATION_TRACK_FAIL              103    // 궤도   고장
#define LOGIC_TYPE_STATION_UPS_ACT                 104    // UPS    정상
#define LOGIC_TYPE_STATION_ACR_ACT                 105    // 정류기 정상
#define LOGIC_TYPE_STATION_CHARGE_ACT              106    // 축전지 정상
#define LOGIC_TYPE_STATION_FUSE_ACT                107    // Fuse   정상

#define LOGIC_TYPE_STATION_N1_ACT                  108    // 한전전원 사용
#define LOGIC_TYPE_STATION_N1_GOOD                 109    // 한전전원 GOOD
#define LOGIC_TYPE_STATION_N1_FAIL                 110    // 한전전원 고장
#define LOGIC_TYPE_STATION_N2_ACT                  111    // 철도전원 사용
#define LOGIC_TYPE_STATION_N2_GOOD                 112    // 철도전원 GOOD
#define LOGIC_TYPE_STATION_N2_FAIL                 113    // 철도전원 고장

#define LOGIC_TYPE_STATION_DOOR_CLOSE              114    // 출입문 Close

#define LOGIC_TYPE_STATION_P_SOUTH_ACT             115    // P남 정상
#define LOGIC_TYPE_STATION_P_NORTH_ACT             116    // P북 정상

//#define LOGIC_TYPE_STATION_HEATER_NORTH_OUT        117    // 히터 북 출력 - 황색
//#define LOGIC_TYPE_STATION_HEATER_NORTH_ACT        118    // 히터 북 동작 (입력 여자 - 주황색)
//#define LOGIC_TYPE_STATION_HEARTH_NORTH_FAIL       119    // 히터 북 고장
//#define LOGIC_TYPE_STATION_HEATER_SOUTH_OUT        120    // 히터 남 출력 - 황색
//#define LOGIC_TYPE_STATION_HEATER_SOUTH_ACT        121    // 히터 남 동작 (입력 여자 - 주황색)
//#define LOGIC_TYPE_STATION_HEARTH_SOUTH_FAIL       122    // 히터 남 고장

//#define LOGIC_TYPE_STATION_HEATER_NORTH_YELLOW     123
//#define LOGIC_TYPE_STATION_HEATER_NORTH_ORANGE     124
//#define LOGIC_TYPE_STATION_HEATER_NORTH_RED        125
//#define LOGIC_TYPE_STATION_HEATER_SOUTH_YELLOW     126
//#define LOGIC_TYPE_STATION_HEATER_SOUTH_ORANGE     127
//#define LOGIC_TYPE_STATION_HEATER_SOUTH_RED        128

//#define LOGIC_TYPE_STATION_HEATER_INPUT_UR         121   // 히터/UR
//#define LOGIC_TYPE_STATION_HEATER_OUTPUT           122   // 히터/O (출력)

//#define LOGIC_TYPE_STATION_HEATER_ACT              123
//#define LOGIC_TYPE_STATION_HEATER_FAIL             124
//#define LOGIC_TYPE_STATION_HEATER_MESSAGE_IN       125
//#define LOGIC_TYPE_STATION_HEATER_MESSAGE          126
//#define LOGIC_TYPE_STATION_HEATER_ERROR            127

#define LOGIC_TYPE_STATION_SIGNAL_NOT_EXPECTED_IN  131    // 신호기 부정입력 _T("SIG/부정입력")
#define LOGIC_TYPE_STATION_SIGNAL_NOT_EXPECTED_OUT 132    // 신호기 부정출력
#define LOGIC_TYPE_STATION_SWITCH_NOT_EXPECTED_IN  133    // 전철기 부정입력 _T("SWH/부정입력")
#define LOGIC_TYPE_STATION_SWITCH_NOT_EXPECTED_OUT 134    // 전철기 부정출력

#define LOGIC_TYPE_STATION_SIG_ALLSTOP_ACT         136    // 신호기 일괄정지

#define LOGIC_TYPE_STATION_SHUNT_OFF_ACT           137    // 입환소등 ACT
#define LOGIC_TYPE_STATION_SHUNT_OFF_INPUT         138    // 입환소등 Input
#define LOGIC_TYPE_STATION_SHUNT_OFF_OUT           139    // 입환소등 Out

#define LOGIC_TYPE_STATION_SIG_SHUNT_ALL_STOP      141    // 입환신호 전체 정지상태
#define LOGIC_TYPE_STATION_RTE_SHUNT_LOCK          142    // 입환진로 쇄정

#define LOGIC_TYPE_STATION_SIG_SHUNT_STOP          150
#define LOGIC_TYPE_STATION_SIG_SHUNT_STOP1         151    // 입환정지 (1그룹) : 15개
#define LOGIC_TYPE_STATION_SIG_SHUNT_STOP2         152    // 입환정지 (2그룹) : 15개
#define LOGIC_TYPE_STATION_SIG_SHUNT_STOP3         153    // 입환정지 (3그룹) : 15개
#define LOGIC_TYPE_STATION_SIG_SHUNT_STOP4         154    // 입환정지 (4그룹) : 15개
#define LOGIC_TYPE_STATION_SIG_SHUNT_STOP5         155    // 입환정지 (5그룹) : 15개
#define LOGIC_TYPE_STATION_SIG_SHUNT_STOP6         156    // 입환정지 (6그룹) : 15개
#define LOGIC_TYPE_STATION_SIG_SHUNT_STOP7         157    // 입환정지 (7그룹) : 15개
#define LOGIC_TYPE_STATION_SIG_SHUNT_STOP8         158    // 입환정지 (8그룹) : 15개
#define LOGIC_TYPE_STATION_SIG_SHUNT_STOP9         159    // 입환정지 (9그룹) : 15개


#define LOGIC_TYPE_STATION_UNKNOWN_UINT            300    // 정의되지 않은 설비 (301 ~ 400)


//---------------------------------------------------------------
// 궤도 (LOGIC_VARIABLE.LogicKind=LOGIC_KIND_TRACK) : _T('T')
//---------------------------------------------------------------
#define LOGIC_TYPE_TRACK_CMD_LOCAL                 11    // 구분진로 비상해정 취급
//#define LOGIC_TYPE_TRACK_CMD_RC                    12
//#define LOGIC_TYPE_TRACK_CMD_CTC                   13

#define LOGIC_TYPE_TRACK_PB                        14

#define LOGIC_TYPE_TRACK_CMD_ABNORMAL              15    // 이상복구 해정취급
#define LOGIC_TYPE_TRACK_PB_ABNORMAL               16

#define LOGIC_TYPE_TRACK_INPUT                     21

#define LOGIC_TYPE_TRACK_UR_ON                     31    // 입력 여자 시소 (2000ms)
#define LOGIC_TYPE_TRACK_UR_OFF                    32
#define LOGIC_TYPE_TRACK_UR_PREV                   33    // PreUR
#define LOGIC_TYPE_TRACK_OLD_UR                    34
#define LOGIC_TYPE_TRACK_OLD_OCC                   35
//#define LOGIC_TYPE_TRACK_OLD_FREE                  34    // UR_ON에 대한 이전 상태
//#define LOGIC_TYPE_TRACK_OLD_OCC                   35    // 이전 점유 상태 (저장)
//#define LOGIC_TYPE_TRACK_OLD_FAIL                  36    // 이전 장애 상태 (저장)

#define LOGIC_TYPE_TRACK_OCCUPIED                  41    // 점유 (정상점유)
#define LOGIC_TYPE_TRACK_OCCUPIED_UR               42    // 궤도 낙하 시소시간 (200ms ~ 3000ms)

#define LOGIC_TYPE_TRACK_ROUTELOCK                 43    // 진로내 궤도쇄정 (TLS쇄정 또는 TRS쇄정)
#define LOGIC_TYPE_TRACK_TRACKLOCK                 44    // 진로외 궤도쇄정 (접촉한계)
#define LOGIC_TYPE_TRACK_LEVERLOCK                 45    // 폐로쇄정

#define LOGIC_TYPE_TRACK_FAILURE                   47    // 궤도 상태(장애)
//#define LOGIC_TYPE_TRACK_FAILURE_ROUTE             48    // 진로쇄정 후 궤도장애 묶음

#define LOGIC_TYPE_TRACK_GROUP                     51    // 진로에 대한 쇄정 상태 (/GROUP)
#define LOGIC_TYPE_TRACK_NORMAL_RECOVERY           52    // 궤도 정상 복구 (/복구)
#define LOGIC_TYPE_TRACK_ABNORMAL_RECOVERY         53    // 궤도 이상 복구 (/이상)
#define LOGIC_TYPE_TRACK_APP_OCC                   54    // 진로내 접근쇄정중 궤도낙하 (/APP-OCC)
//#define LOGIC_TYPE_TRACK_ABRECOVERY_ROUTE          55    // 진로현시 후 궤도 이상 복구 (/R이상)
//#define LOGIC_TYPE_TRACK_ABRECOVERY_MESSAGE        56    // 궤도 이상 복구 메시지(/이상MSG)
//#define LOGIC_TYPE_TRACK_NORMAL_RECOVERY_ROUTE     57    // 궤도 쇄정상태에서 정상 복구 (/R복구)

//#define LOGIC_TYPE_TRACK_EMG_RELEASE_UR            60    // 이상복구 해정 Timer (/UR해정)
#define LOGIC_TYPE_TRACK_EMG                       61    // 구분진로 비상해정 취급 가능 상태 (/비상)
#define LOGIC_TYPE_TRACK_EMG_RELEASE               62    // 구분진로 비상해정 Timer (/해정)

#define LOGIC_TYPE_TRACK_LSR                       63    // Left  Lock (TLS)
#define LOGIC_TYPE_TRACK_RSR                       64    // Right Lock (TRS)

#define LOGIC_TYPE_TRACK_ABNOR_RELEASE             65    // 이상복구 해정 Timer (/해정-ABNOR)

#define LOGIC_TYPE_TRACK_APP_BELL                  66    // 접근벨
#define LOGIC_TYPE_TRACK_APP_LOCK                  67

#define LOGIC_TYPE_TRACK_LSR_RTE                   68    // LSR에 대한 진로쇄정 조건 (L진로)
#define LOGIC_TYPE_TRACK_RSR_RTE                   69    // RSR에 대한 진로쇄정 조건 (R진로)

//#define LOGIC_TYPE_TRACK_RTE_SR_LEFT               71    // Left  방향 진로에 대한 SR 낙하(SR-LEFT)
//#define LOGIC_TYPE_TRACK_RTE_SR_RIGHT              72    // Right 방향 진로에 대한 SR 낙하(SR-RIGHT)

#define LOGIC_TYPE_TRACK_MOVE_LEFT                 73    // 폐색궤도 이동방향 Left  - 접근벨 설정시 사용
#define LOGIC_TYPE_TRACK_MOVE_RIGHT                74    // 폐색궤도 이동방향 Right - 접근벨 설정시 사용

#define LOGIC_TYPE_TRACK_GREEN                     77    // 녹색
#define LOGIC_TYPE_TRACK_YELLOW                    78    // 황색
//#define LOGIC_TYPE_TRACK_RED                       79    // 적색

//#define LOGIC_TYPE_TRACK_RTE_SR_LEFT_GROUP         80
//#define LOGIC_TYPE_TRACK_RTE_SR_LEFT_GROUP1        81    // Left  방향 진로에 대한 SR-LEFT GROUP1
//#define LOGIC_TYPE_TRACK_RTE_SR_LEFT_GROUP2        82
//#define LOGIC_TYPE_TRACK_RTE_SR_LEFT_GROUP3        83
//#define LOGIC_TYPE_TRACK_RTE_SR_LEFT_GROUP4        84
//#define LOGIC_TYPE_TRACK_RTE_SR_LEFT_GROUP5        85

//#define LOGIC_TYPE_TRACK_RTE_SR_RIGHT_GROUP        90
//#define LOGIC_TYPE_TRACK_RTE_SR_RIGHT_GROUP1       91    // Right 방향 진로에 대한 SR-RIGHT GROUP1
//#define LOGIC_TYPE_TRACK_RTE_SR_RIGHT_GROUP2       92
//#define LOGIC_TYPE_TRACK_RTE_SR_RIGHT_GROUP3       93
//#define LOGIC_TYPE_TRACK_RTE_SR_RIGHT_GROUP4       94
//#define LOGIC_TYPE_TRACK_RTE_SR_RIGHT_GROUP5       95

//---------------------------------------------------------------
//#define LOGIC_TYPE_TRACK_RTELOCK_ROUTE             100   // 진로내 궤도를 쇄정하는 진로    (궤도/R쇄정(진로) :   100 + 진로인덱스)
//#define LOGIC_TYPE_TRACK_TRKLOCK_ROUTE             2000  // 진로외 궤도를 쇄정하는 진로    (궤도/T쇄정(진로) :  2000 + 진로인덱스)
//#define LOGIC_TYPE_TRACK_RECOVERY_ROUTE            4000  // 진로에 대한 궤도 정상복구      (궤도/R복구(진로) :  4000 + 진로인덱스)
//#define LOGIC_TYPE_TRACK_ABFREE_ROUTE              6000  // 진로에 대한 궤도 이상복구      (궤도/R이상(진로) :  6000 + 진로인덱스)
//#define LOGIC_TYPE_TRACK_RELEASE_ROUTE             8000  // 진로에 대한 궤도 이상복구 해정 (궤도/해정(진로)  :  8000 + 진로인덱스)
//#define LOGIC_TYPE_TRACK_FAIL_ROUTE                10000 // 진로쇄정 후 궤도장애           (궤도/장애(진로)  : 10000 + 진로인덱스)

//---------------------------------------------------------------
// 신호기 (LOGIC_VARIABLE.LogicKind=LOGIC_KIND_SIGNAL) : _T('S')
//---------------------------------------------------------------
//#define LOGIC_TYPE_SIGNAL_CMD_PB_LOCAL             11    // 표시제어부 신호 취급
//#define LOGIC_TYPE_SIGNAL_CMD_RB_LOCAL             12    // 표시제어부 신호 재취급
//#define LOGIC_TYPE_SIGNAL_CMD_CB_LOCAL             13    // 표시제어부 신호 취소 취급
#define LOGIC_TYPE_SIGNAL_CMD_TTB_LOCAL            14    // 표시제어부 TTB 취급
#define LOGIC_TYPE_SIGNAL_CMD_TCB_LOCAL            15    // 표시제어부 TTB 취소취급
//#define LOGIC_TYPE_SIGNAL_CMD_SHR_ON_LOCAL         16    // 표시제어부 (무)유도 취급
//#define LOGIC_TYPE_SIGNAL_CMD_SHR_OFF_LOCAL        17    // 표시제어부 (무)유도 취소 취급

//#define LOGIC_TYPE_SIGNAL_CMD_PB_RC                18    // RC 취급
//#define LOGIC_TYPE_SIGNAL_CMD_RB_RC                19
//#define LOGIC_TYPE_SIGNAL_CMD_CB_RC                20
//#define LOGIC_TYPE_SIGNAL_CMD_TTB_RC               21
//#define LOGIC_TYPE_SIGNAL_CMD_TCB_RC               22
//#define LOGIC_TYPE_SIGNAL_CMD_SHR_ON_RC            23
//#define LOGIC_TYPE_SIGNAL_CMD_SHR_OFF_RC           24

//#define LOGIC_TYPE_SIGNAL_CMD_PB_CTC               25    // CTC 취급
//#define LOGIC_TYPE_SIGNAL_CMD_RB_CTC               26
//#define LOGIC_TYPE_SIGNAL_CMD_CB_CTC               27
//#define LOGIC_TYPE_SIGNAL_CMD_TTB_CTC              28
//#define LOGIC_TYPE_SIGNAL_CMD_TCB_CTC              29
//#define LOGIC_TYPE_SIGNAL_CMD_SHR_ON_CTC           30
//#define LOGIC_TYPE_SIGNAL_CMD_SHR_OFF_CTC          31


#define LOGIC_TYPE_SIGNAL_PB                       41    // 취급
#define LOGIC_TYPE_SIGNAL_RB                       42    // 재취급
#define LOGIC_TYPE_SIGNAL_CB                       43    // 취소
//#define LOGIC_TYPE_SIGNAL_FAIL                     44

#define LOGIC_TYPE_SIGNAL_PB_TTB                   45    // TTB 취급
#define LOGIC_TYPE_SIGNAL_CB_TCB                   46    // TTB 취소취급
//#define LOGIC_TYPE_SIGNAL_PB_SHR                   47    // (무)유도 취급
//#define LOGIC_TYPE_SIGNAL_CB_SHR                   48    // (무)유도 취소취급

#define LOGIC_TYPE_SIGNAL_INPUT                    51
//#define LOGIC_TYPE_SIGNAL_INPUT_HR                 52
//#define LOGIC_TYPE_SIGNAL_INPUT_YR                 53
//#define LOGIC_TYPE_SIGNAL_INPUT_GR                 54
//#define LOGIC_TYPE_SIGNAL_INPUT_SHR                55

#define LOGIC_TYPE_SIGNAL_OUTPUT                   56

#define LOGIC_TYPE_SIG_UR_ON                       61
#define LOGIC_TYPE_SIG_UR_OFF                      62

#define LOGIC_TYPE_SIG_HR_UR_ON                    63   // HR   입력
#define LOGIC_TYPE_SIG_YR_UR_ON                    64   // YR   입력
#define LOGIC_TYPE_SIG_GR_UR_ON                    65   // GR   입력
#define LOGIC_TYPE_SIG_HCR_UR_ON                   66   // 입환신호기 입력
#define LOGIC_TYPE_SIG_SHR_UR_ON                   67   // 유도신호기 입력
#define LOGIC_TYPE_SIG_ULMR_UR_ON                  68   // 진로선별등 ULMR 입력 (ULMR/UR)

//#define LOGIC_TYPE_SIG_HR_UR_OFF                   
//#define LOGIC_TYPE_SIG_YR_UR_OFF                   69
//#define LOGIC_TYPE_SIG_GR_UR_OFF                   70
//#define LOGIC_TYPE_SIG_HCR_UR_OFF                  71
//#define LOGIC_TYPE_SIG_SHR_UR_OFF                  72

#define LOGIC_TYPE_SIG_HR_OUT_UR_OFF               73
#define LOGIC_TYPE_SIG_YR_OUT_UR_OFF               74
#define LOGIC_TYPE_SIG_GR_OUT_UR_OFF               75
#define LOGIC_TYPE_SIG_HCR_OUT_UR_OFF              76
#define LOGIC_TYPE_SIG_SHR_OUT_UR_OFF              77

#define LOGIC_TYPE_SIG_ULMR_ON                     78    // 진로선별등 현시 (선별등ON)
#define LOGIC_TYPE_SIG_ULMR_FAIL                   79    // 진로선별등 장애 (선별등FAIL)

#define LOGIC_TYPE_SIGNAL_ON                       80    // 신호 정지 이외 현시 상태
#define LOGIC_TYPE_SIGNAL_RED                      81    // 정지
#define LOGIC_TYPE_SIGNAL_GREEN                    82    // 진행
#define LOGIC_TYPE_SIGNAL_YELLOW                   83    // 주의
#define LOGIC_TYPE_SIGNAL_YELLOW1                  84    //
#define LOGIC_TYPE_SIGNAL_YG                       85    // 감속
#define LOGIC_TYPE_SIGNAL_YY                       86    // 경계
#define LOGIC_TYPE_SIGNAL_HCR_ON                   87    // 입환신호기 현시 (HCR)
#define LOGIC_TYPE_SIGNAL_CALLON_ON                88    // 유도신호기 현시 (SHR)
//#define LOGIC_TYPE_SIGNAL_REPEAT_ON                89    // 중계 신호기 현시 (RELS)
#define LOGIC_TYPE_SIGNAL_REPEAT_ON_GO             89    // 중계 신호기 진행 현시
#define LOGIC_TYPE_SIGNAL_REPEAT_ON_LIMIT          90    // 중계 신호기 제한 현시

#define LOGIC_TYPE_SIGNAL_TTB                      91    // TTB Set
#define LOGIC_TYPE_SIGNAL_REQUEST                  92    // REQ(진로에 REQ와 동일)
#define LOGIC_TYPE_SIGNAL_APPLOCK                  93    // 접근 쇄정
#define LOGIC_TYPE_SIGNAL_DELLLOCK                 94    // 보류 쇄정

#define LOGIC_TYPE_SIGNAL_LOCK                     95    // 신호기 쇄정
#define LOGIC_TYPE_SIGNAL_ONOFF                    96    // 신호현시후 정지출력상태 (장애)

#define LOGIC_TYPE_SIGNAL_REPEAT_MSG_GO_OFF        97    // 중계 신호기 진행소등 메시지
#define LOGIC_TYPE_SIGNAL_REPEAT_MSG_GO_ON         98    // 중계 신호기 진행복구 메시지
#define LOGIC_TYPE_SIGNAL_REPEAT_MSG_LIMIT_OFF     99    // 중계 신호기 제한소등 메시지
#define LOGIC_TYPE_SIGNAL_REPEAT_MSG_LIMIT_ON      100   // 중계 신호기 제한복구 메시지
#define LOGIC_TYPE_SIGNAL_REPEAT_MSG_STOP_OFF      101   // 중계 신호기 정지소등 메시지
#define LOGIC_TYPE_SIGNAL_REPEAT_MSG_STOP_ON       102   // 중계 신호기 정지복구 메시지

#define LOGIC_TYPE_SIGNAL_FAILURE                  111    // 고장 (FAIL-S)
#define LOGIC_TYPE_SIGNAL_FAIL_HR                  112    // FAIL HR
#define LOGIC_TYPE_SIGNAL_FAIL_YR                  113    // FAIL YR
#define LOGIC_TYPE_SIGNAL_FAIL_GR                  114    // FAIL GR
#define LOGIC_TYPE_SIGNAL_FAIL_REPEAT_ON           115    // 중계 신호기 현시에서 장애
#define LOGIC_TYPE_SIGNAL_FAIL_REPEAT_RED          116    // 중계 신호기 정지에서 장애
#define LOGIC_TYPE_SIGNAL_FAIL_FLASH               117    // 신호기 점멸 표시

#define LOGIC_TYPE_SIGNAL_OUT_HR                   120    // HR   출력 - OUT Card
#define LOGIC_TYPE_SIGNAL_OUT_YR                   121    // YR   출력 - OUT Card
#define LOGIC_TYPE_SIGNAL_OUT_GR                   122    // GR   출력 - OUT Card
#define LOGIC_TYPE_SIGNAL_OUT_HCR                  123    // 입환신호기 출력 - OUT Card
#define LOGIC_TYPE_SIGNAL_OUT_SHR                  124    // 유도신호기 출력 - OUT Card

#define LOGIC_TYPE_SIGNAL_OUT_COND                 125    // 신호기 출력 조건 OK
#define LOGIC_TYPE_SIGNAL_OUT_COND_SWH             126    // 선로전환기에 대한 신호기 출력 조건 OK
#define LOGIC_TYPE_SIGNAL_OUT_COND_TRACK           127    // 궤도에       대한 신호기 출력 조건 OK
#define LOGIC_TYPE_SIGNAL_OUT_COND_BLOCK           128    // 폐색에       대한 신호기 출력 조건 OK
#define LOGIC_TYPE_SIGNAL_OUT_COND_TOT_RTE         129    // 총괄진로에   대한 신호기 출력 조건 OK
#define LOGIC_TYPE_SIGNAL_OUT_COND_TRACK_SHUNTD    130    // 입환공용 표지에 대한 궤도출력 조건 OK

#define LOGIC_TYPE_SIGNAL_OUT_COND_SHUNTSIG        131   // 입환공용 신호 출력 조건 OK

#define LOGIC_TYPE_SIG_OUT_GO                      132   // 진행(G)  출력
#define LOGIC_TYPE_SIG_OUT_SLOW                    133   // 감속(YG) 출력
#define LOGIC_TYPE_SIG_OUT_ATTENTION               134   // 주의(Y)  출력
#define LOGIC_TYPE_SIG_OUT_WARNING                 135   // 경계(YY) 출력
#define LOGIC_TYPE_SIG_OUT_STOP                    136   // 정지(R)  출력
#define LOGIC_TYPE_SIG_OUT_HCR                     137   // 입환신호기(HCR) 출력
#define LOGIC_TYPE_SIG_OUT_CALLON                  138   // 유도 신호 출력

// 출력시 UR(Timer) 2초 설정 후 입력이 없으면 장애 처리
#define LOGIC_TYPE_SIG_OUT_UR_GO                   141   // 진행(G)  출력 UR
#define LOGIC_TYPE_SIG_OUT_UR_SLOW                 142   // 감속(YG) 출력 UR
#define LOGIC_TYPE_SIG_OUT_UR_ATTENTION            143   // 주의(Y)  출력 UR
#define LOGIC_TYPE_SIG_OUT_UR_WARNING              144   // 경계(YY) 출력 UR
#define LOGIC_TYPE_SIG_OUT_UR_STOP                 145   // 정지(R)  출력 UR
#define LOGIC_TYPE_SIG_OUT_UR_HCR                  146   // 입환신호기(HCR) 출력 UR
#define LOGIC_TYPE_SIG_OUT_UR_CALLON               147   // 유도 신호 출력 UR

#define LOGIC_TYPE_SIG_OUT_UR_STOP_CHK             148   // 정지(R)  출력 후 진로/FAIL 체크를 위한 Timer

#define LOGIC_TYPE_SIG_OUT_UR_OFF_GO               151   // 진행(G)  출력차단 후 wait Timer
#define LOGIC_TYPE_SIG_OUT_UR_OFF_SLOW             152   // 감속(YG) 출력차단 후 wait Timer
#define LOGIC_TYPE_SIG_OUT_UR_OFF_ATTENTION        153   // 주의(Y)  출력차단 후 wait Timer
#define LOGIC_TYPE_SIG_OUT_UR_OFF_WARNING          154   // 경계(YY) 출력차단 후 wait Timer
#define LOGIC_TYPE_SIG_OUT_UR_OFF_STOP             155   // 정지(R)  출력차단 후 wait Timer
#define LOGIC_TYPE_SIG_OUT_UR_OFF_HCR              156   // 입환신호기(HCR) 출력차단 후 wait Timer
#define LOGIC_TYPE_SIG_OUT_UR_OFF_CALLON           157   // 유도 신호 출력차단 후 wait Timer

#define LOGIC_TYPE_SIG_OUT_CANCEL_UR_OFF           158   // 출력차단(취소) UR (차단UR-OFF)

// 신호기 정지 상태(현시후 정지 원인) - 재취급 필요
//#define LOGIC_TYPE_SIG_TRACKOCCUPY                 161   // 궤도   점유에 의한 장애
//#define LOGIC_TYPE_SIG_FAIL_TRACK                  162   // 궤도   고장
//#define LOGIC_TYPE_SIG_SWITCHOOC                   163   // 전철기 불일치
//#define LOGIC_TYPE_SIG_FAIL_SIGNAL                 164   // 신호기 고장
//#define LOGIC_TYPE_SIG_FAIL_POWER                  165   // 정전후 복구
//#define LOGIC_TYPE_SIG_FAIL_BLOCK                  166   // 폐색   이상

//#define LOGIC_TYPE_SIG_NOT_EXPECTED_INPUT          171   // 신호기 부정입력  _T("%s/S부정입력")
//#define LOGIC_TYPE_SIG_NOT_EXPECTED_IN_HR          172
//#define LOGIC_TYPE_SIG_NOT_EXPECTED_IN_YR          173
//#define LOGIC_TYPE_SIG_NOT_EXPECTED_IN_GR          174
//#define LOGIC_TYPE_SIG_NOT_EXPECTED_IN_HCR         175   // 입환신호기 HCR
//#define LOGIC_TYPE_SIG_NOT_EXPECTED_IN_SHR         176   // (무)유도

//#define LOGIC_TYPE_SIG_NOT_EXPECTED_OUTPUT         181   // 신호기 부정출력
//#define LOGIC_TYPE_SIG_NOT_EXPECTED_OUT_HR         182
//#define LOGIC_TYPE_SIG_NOT_EXPECTED_OUT_YR         183
//#define LOGIC_TYPE_SIG_NOT_EXPECTED_OUT_GR         184
//#define LOGIC_TYPE_SIG_NOT_EXPECTED_OUT_HCR        185   // 입환신호기 HCR
//#define LOGIC_TYPE_SIG_NOT_EXPECTED_OUT_SHR        186   // (무)유도

//#define LOGIC_TYPE_SIG_OUT_OLD_GO                  191   // 진행(G)  OLD출력
//#define LOGIC_TYPE_SIG_OUT_OLD_SLOW                192   // 감속(YG) OLD출력
//#define LOGIC_TYPE_SIG_OUT_OLD_ATTENTION           193   // 주의(Y)  OLD출력
//#define LOGIC_TYPE_SIG_OUT_OLD_WARNING             194   // 경계(YY) OLD출력
//#define LOGIC_TYPE_SIG_OUT_OLD_STOP                195   // 정지(R)  OLD출력
//#define LOGIC_TYPE_SIG_OUT_OLD_HCR                 196   // 입환신호기(HCR) OLD출력
//#define LOGIC_TYPE_SIG_OUT_OLD_CALLON              197   // 유도 신호 OLD출력

#define LOGIC_TYPE_SIGNAL_OUT_OLD_HR               201    // HR   출력 - OUT Card
#define LOGIC_TYPE_SIGNAL_OUT_OLD_YR               202    // YR   출력 - OUT Card
#define LOGIC_TYPE_SIGNAL_OUT_OLD_GR               203    // GR   출력 - OUT Card
#define LOGIC_TYPE_SIGNAL_OUT_OLD_HCR              204    // 입환신호기 출력 - OUT Card
#define LOGIC_TYPE_SIGNAL_OUT_OLD_SHR              205    // 유도신호기 출력 - OUT Card

#define LOGIC_TYPE_SIG_NOT_EXPECTED_UR_HR          211    // HR/부정입력
#define LOGIC_TYPE_SIG_NOT_EXPECTED_UR_YR          212    // YR/부정입력
#define LOGIC_TYPE_SIG_NOT_EXPECTED_UR_GR          213    // GR/부정입력
//#define LOGIC_TYPE_SIG_NOT_EXPECTED_UR_HCR         214   // 입환신호기 HCR
//#define LOGIC_TYPE_SIG_NOT_EXPECTED_UR_SHR         215   // (무)유도

//#define LOGIC_TYPE_SIGNAL_RTE_SIG_ON               1000   // 진로에 대한 신호기 현시      (신호기/(진로)현시 : 1000 + 진로인덱스)
//#define LOGIC_TYPE_SIGNAL_RTE_SHR_ON               3000   // 주신호 진로에 대한 유도 현시 (신호기/(진로)SHR  : 3000 + 진로인덱스)

//---------------------------------------------------------------
// 신호기 LMR 상태 (LOGIC_VARIABLE.LogicKind=LOGIC_KIND_LMR) : _T('L')
//---------------------------------------------------------------
#define LOGIC_TYPE_SIG_LMR_INPUT                   11
#define LOGIC_TYPE_SIG_LMR_INPUT_GM                12
#define LOGIC_TYPE_SIG_LMR_INPUT_GA                13
#define LOGIC_TYPE_SIG_LMR_INPUT_YM                14
#define LOGIC_TYPE_SIG_LMR_INPUT_YA                15
#define LOGIC_TYPE_SIG_LMR_INPUT_Y1M               16
#define LOGIC_TYPE_SIG_LMR_INPUT_Y1A               17
#define LOGIC_TYPE_SIG_LMR_INPUT_RM                18
#define LOGIC_TYPE_SIG_LMR_INPUT_RA                19
#define LOGIC_TYPE_SIG_LMR_INPUT_GLMR              21    // 중계 GLMR
#define LOGIC_TYPE_SIG_LMR_INPUT_CLMR              22
//#define LOGIC_TYPE_SIG_LMR_INPUT_SHR               23    // 유도

//#define LOGIC_TYPE_SIG_LMR_OUTPUT                  26

#define LOGIC_TYPE_SIG_LMR_UR_ON                   31
#define LOGIC_TYPE_SIG_LMR_UR_OFF                  32

#define LOGIC_TYPE_SIG_LMR_UR_ON_GM                33    // Timer
#define LOGIC_TYPE_SIG_LMR_UR_ON_GA                34
#define LOGIC_TYPE_SIG_LMR_UR_ON_YM                35
#define LOGIC_TYPE_SIG_LMR_UR_ON_YA                36
#define LOGIC_TYPE_SIG_LMR_UR_ON_Y1M               37
#define LOGIC_TYPE_SIG_LMR_UR_ON_Y1A               38
#define LOGIC_TYPE_SIG_LMR_UR_ON_RM                39
#define LOGIC_TYPE_SIG_LMR_UR_ON_RA                40
#define LOGIC_TYPE_SIG_LMR_UR_ON_GLMR              41    // 중계 GLMR
#define LOGIC_TYPE_SIG_LMR_UR_ON_CLMR              42
//#define LOGIC_TYPE_SIG_LMR_UR_ON_SHR               43    // 유도

//#define LOGIC_TYPE_SIG_LMR_UR_OFF_GM               44
//#define LOGIC_TYPE_SIG_LMR_UR_OFF_GA               45
//#define LOGIC_TYPE_SIG_LMR_UR_OFF_YM               46
//#define LOGIC_TYPE_SIG_LMR_UR_OFF_YA               47
//#define LOGIC_TYPE_SIG_LMR_UR_OFF_Y1M              48
//#define LOGIC_TYPE_SIG_LMR_UR_OFF_Y1A              49
//#define LOGIC_TYPE_SIG_LMR_UR_OFF_RM               50
//#define LOGIC_TYPE_SIG_LMR_UR_OFF_RA               51
//#define LOGIC_TYPE_SIG_LMR_UR_OFF_GLMR             52    // 중계 GLMR
//#define LOGIC_TYPE_SIG_LMR_UR_OFF_CLMR             53
//#define LOGIC_TYPE_SIG_LMR_UR_OFF_SHR              54    // 유도


#define LOGIC_TYPE_SIG_LMR_UR_ON_CHECK_GM          61
#define LOGIC_TYPE_SIG_LMR_UR_ON_CHECK_GA          62
#define LOGIC_TYPE_SIG_LMR_UR_ON_CHECK_YM          63
#define LOGIC_TYPE_SIG_LMR_UR_ON_CHECK_YA          64
#define LOGIC_TYPE_SIG_LMR_UR_ON_CHECK_Y1M         65
#define LOGIC_TYPE_SIG_LMR_UR_ON_CHECK_Y1A         66
#define LOGIC_TYPE_SIG_LMR_UR_ON_CHECK_RM          67
#define LOGIC_TYPE_SIG_LMR_UR_ON_CHECK_RA          68

//#define LOGIC_TYPE_SIG_LMR_CHK_ON_GLMR             55    // GLMR/ON-CHK
//#define LOGIC_TYPE_SIG_LMR_CHK_ON_YLMR             56
//#define LOGIC_TYPE_SIG_LMR_CHK_ON_Y1LMR            57
//#define LOGIC_TYPE_SIG_LMR_CHK_ON_RLMR             58
//#define LOGIC_TYPE_SIG_LMR_CHK_ON_CLMR             59

//#define LOGIC_TYPE_SIG_LMR_FAIL                    71    // LMR  고장
#define LOGIC_TYPE_SIG_LMR_FAIL_GREEN              72    // 진행 고장
#define LOGIC_TYPE_SIG_LMR_FAIL_GM                 73    // 주심 단심
#define LOGIC_TYPE_SIG_LMR_FAIL_GA                 74    // 부심 단심
#define LOGIC_TYPE_SIG_LMR_FAIL_YELLOW             75
#define LOGIC_TYPE_SIG_LMR_FAIL_YM                 76
#define LOGIC_TYPE_SIG_LMR_FAIL_YA                 77
#define LOGIC_TYPE_SIG_LMR_FAIL_YELLOW1            78
#define LOGIC_TYPE_SIG_LMR_FAIL_Y1M                79
#define LOGIC_TYPE_SIG_LMR_FAIL_Y1A                80
#define LOGIC_TYPE_SIG_LMR_FAIL_RED                81
#define LOGIC_TYPE_SIG_LMR_FAIL_RM                 82
#define LOGIC_TYPE_SIG_LMR_FAIL_RA                 83
//#define LOGIC_TYPE_SIG_LMR_FAIL_GLMR               84    // 중계 GLMR 고장
//#define LOGIC_TYPE_SIG_LMR_FAIL_CLMR               85
#define LOGIC_TYPE_SIG_LMR_FAIL_REPEAT             86    // 중계 LMR 고장
//#define LOGIC_TYPE_SIG_LMR_FAIL_SHR                87    // 유도

#define LOGIC_TYPE_SIG_LMR_FAILSTOP                88    // LMR장애로 신호정지

#define LOGIC_TYPE_SIG_LMR_FAIL_UR_GLMR            91
#define LOGIC_TYPE_SIG_LMR_FAIL_UR_YLMR            92
#define LOGIC_TYPE_SIG_LMR_FAIL_UR_RLMR            93
#define LOGIC_TYPE_SIG_LMR_FAIL_UR_Y1LMR           94

#define LOGIC_TYPE_SIG_LMR_INPUT_ON_GLMR           95    // 입력 GLMR 여자 (GLMR/ON : GMLMR 또는 GALMR)
#define LOGIC_TYPE_SIG_LMR_INPUT_ON_YLMR           96
#define LOGIC_TYPE_SIG_LMR_INPUT_ON_RLMR           97
#define LOGIC_TYPE_SIG_LMR_INPUT_ON_Y1LMR          98

#define LOGIC_TYPE_SIG_LMR_FAIL_UR_GMLMR           101   // Gm/FAIL-UR
#define LOGIC_TYPE_SIG_LMR_FAIL_UR_YMLMR           102   // Ym/FAIL-UR
#define LOGIC_TYPE_SIG_LMR_FAIL_UR_RMLMR           103   // Rm/FAIL-UR
#define LOGIC_TYPE_SIG_LMR_FAIL_UR_Y1MLMR          104   // Y1m/FAIL-UR

//#define LOGIC_TYPE_SIG_LMR_Gm                      91    // G  주심 ON
//#define LOGIC_TYPE_SIG_LMR_Ga                      92    // G  부심 ON
//#define LOGIC_TYPE_SIG_LMR_Ym                      93    // Y  주심 ON
//#define LOGIC_TYPE_SIG_LMR_Ya                      94    // Y  부심 ON
//#define LOGIC_TYPE_SIG_LMR_Y1m                     95    // Y1 주심 ON
//#define LOGIC_TYPE_SIG_LMR_Y1a                     96    // Y1 부심 ON
//#define LOGIC_TYPE_SIG_LMR_Rm                      97    // R  주심 ON
//#define LOGIC_TYPE_SIG_LMR_Ra                      98    // R  부심 ON
//#define LOGIC_TYPE_SIG_LMR_GLMR                    99    // 중계 신호기 ON
//#define LOGIC_TYPE_SIG_LMR_CLMR                    100

#define LOGIC_TYPE_SIG_LMR_NOT_EXPECTED_IN         111   // LMR 부정입력
#define LOGIC_TYPE_SIG_LMR_NOT_EXPECTED_IN_GM      112   // G주심 부정입력
#define LOGIC_TYPE_SIG_LMR_NOT_EXPECTED_IN_GA      113   // G부심 부정입력
#define LOGIC_TYPE_SIG_LMR_NOT_EXPECTED_IN_YM      114
#define LOGIC_TYPE_SIG_LMR_NOT_EXPECTED_IN_YA      115
#define LOGIC_TYPE_SIG_LMR_NOT_EXPECTED_IN_Y1M     116
#define LOGIC_TYPE_SIG_LMR_NOT_EXPECTED_IN_Y1A     117
#define LOGIC_TYPE_SIG_LMR_NOT_EXPECTED_IN_RM      118
#define LOGIC_TYPE_SIG_LMR_NOT_EXPECTED_IN_RA      119
#define LOGIC_TYPE_SIG_LMR_NOT_EXPECTED_IN_GLMR    120   // 중계 GLMR 부정입력
#define LOGIC_TYPE_SIG_LMR_NOT_EXPECTED_IN_CLMR    121

// 메시지 : 주심, 부심 단심
#define LOGIC_TYPE_SIG_LMR_FAIL_MSG_GLMR           131   // G/FM주부심
#define LOGIC_TYPE_SIG_LMR_FAIL_MSG_GmLMR          132
//#define LOGIC_TYPE_SIG_LMR_FAIL_MSG_GaLMR          133
#define LOGIC_TYPE_SIG_LMR_FAIL_MSG_YLMR           134   // Y/FM주부심
#define LOGIC_TYPE_SIG_LMR_FAIL_MSG_YmLMR          135
//#define LOGIC_TYPE_SIG_LMR_FAIL_MSG_YaLMR          136
#define LOGIC_TYPE_SIG_LMR_FAIL_MSG_Y1LMR          137   // Y1/FM주부심
#define LOGIC_TYPE_SIG_LMR_FAIL_MSG_Y1mLMR         138
//#define LOGIC_TYPE_SIG_LMR_FAIL_MSG_Y1aLMR         139
#define LOGIC_TYPE_SIG_LMR_FAIL_MSG_RLMR           140   // R/FM주부심
#define LOGIC_TYPE_SIG_LMR_FAIL_MSG_RmLMR          141
//#define LOGIC_TYPE_SIG_LMR_FAIL_MSG_RaLMR          142

// 메시지 : 주심, 부심 동시점등
#define LOGIC_TYPE_SIG_LMR_MA_MSG_GLMR             146   // G/MA주부심
#define LOGIC_TYPE_SIG_LMR_MA_MSG_YLMR             147   // Y/MA주부심
#define LOGIC_TYPE_SIG_LMR_MA_MSG_Y1LMR            148   // Y1/MA주부심
#define LOGIC_TYPE_SIG_LMR_MA_MSG_RLMR             149   // R/MA주부심

// 메시지 : 현시불능
//#define LOGIC_TYPE_SIG_LMR_FAIL_SIGON_MSG_R        151   // 메시지 : 정지 현시 불능
#define LOGIC_TYPE_SIG_LMR_FAIL_SIGON_MSG_YY       152   // 메시지 : 경계 현시 불능
#define LOGIC_TYPE_SIG_LMR_FAIL_SIGON_MSG_Y        153   // 메시지 : 주의 현시 불능
#define LOGIC_TYPE_SIG_LMR_FAIL_SIGON_MSG_YG       154   // 메시지 : 감속 현시 불능
#define LOGIC_TYPE_SIG_LMR_FAIL_SIGON_MSG_G        155   // 메시지 : 진행 현시 불능

//------------------------------------
// 고장검지 LMR 1회선
#define LOGIC_TYPE_SIG_LMR_ONE_UR_ON_G             201   // GLMR 여자 (GLMR/UR)
#define LOGIC_TYPE_SIG_LMR_ONE_UR_ON_Y             202
#define LOGIC_TYPE_SIG_LMR_ONE_UR_ON_Y1            203
#define LOGIC_TYPE_SIG_LMR_ONE_UR_ON_R             204

//#define LOGIC_TYPE_SIG_LMR_ONE_UR_OFF_G            206
//#define LOGIC_TYPE_SIG_LMR_ONE_UR_OFF_Y            207
//#define LOGIC_TYPE_SIG_LMR_ONE_UR_OFF_Y1           208
//#define LOGIC_TYPE_SIG_LMR_ONE_UR_OFF_R            209

#define LOGIC_TYPE_SIG_LMR_ONE_NOT_EXPECTED_IN_G   211   // G LMR 부정입력
#define LOGIC_TYPE_SIG_LMR_ONE_NOT_EXPECTED_IN_Y   212
#define LOGIC_TYPE_SIG_LMR_ONE_NOT_EXPECTED_IN_Y1  213
#define LOGIC_TYPE_SIG_LMR_ONE_NOT_EXPECTED_IN_R   214

#define LOGIC_TYPE_SIG_LMR_ONE_FAIL_MSG_GLMR       216   // 메시지 : G 전구 단심
#define LOGIC_TYPE_SIG_LMR_ONE_FAIL_MSG_YLMR       217
#define LOGIC_TYPE_SIG_LMR_ONE_FAIL_MSG_Y1LMR      218
#define LOGIC_TYPE_SIG_LMR_ONE_FAIL_MSG_RLMR       219

#define LOGIC_TYPE_SIG_LMR_UR_ON_CHECK_GLMR        221
#define LOGIC_TYPE_SIG_LMR_UR_ON_CHECK_YLMR        222
#define LOGIC_TYPE_SIG_LMR_UR_ON_CHECK_Y1LMR       223
#define LOGIC_TYPE_SIG_LMR_UR_ON_CHECK_RLMR        224


//---------------------------------------------------------------
// 선로전환기 (LOGIC_VARIABLE.LogicKind=LOGIC_KIND_SWITCH) : _T('P')
//---------------------------------------------------------------
#define LOGIC_TYPE_SWITCH_CMD                      11

#define LOGIC_TYPE_SWITCH_CMD_NORMAL_LOCAL         12    // 표시제어부 정위 취급
#define LOGIC_TYPE_SWITCH_CMD_REVERSE_LOCAL        13    // 표시제어부 반위 취급

#define LOGIC_TYPE_SWITCH_CMD_NORMAL_RC            14    // RC 취급
#define LOGIC_TYPE_SWITCH_CMD_REVERSE_RC           15

#define LOGIC_TYPE_SWITCH_CMD_NORMAL_CTC           16    // CTC 취급
#define LOGIC_TYPE_SWITCH_CMD_REVERSE_CTC          17


#define LOGIC_TYPE_SWITCH_PB                       22
#define LOGIC_TYPE_SWITCH_CB                       23
#define LOGIC_TYPE_SWITCH_PB_NORMAL                24
#define LOGIC_TYPE_SWITCH_PB_REVERSE               25

#define LOGIC_TYPE_SWITCH_PB_NOT_MOV_NORMAL        26    // 메시지 : 선로전환기 전환중으로 정위 취급불가
#define LOGIC_TYPE_SWITCH_PB_NOT_MOV_REVERSE       27    // 메시지 : 선로전환기 전환중으로 반위 취급불가

//#define LOGIC_TYPE_SWITCH_FAIL                     31

#define LOGIC_TYPE_SWITCH_INPUT                    41
#define LOGIC_TYPE_SWITCH_OUTPUT                   42

#define LOGIC_TYPE_SWITCH_UR_ON                    51
#define LOGIC_TYPE_SWITCH_UR_OFF                   52
#define LOGIC_TYPE_SWITCH_UR_WLR                   53    // WLR/UR-OFF
#define LOGIC_TYPE_SWITCH_UR_WLR_CHK               54    // WLR/UR-CHK : WLR 동작불능 체크시간
#define LOGIC_TYPE_SWITCH_UR_WRN                   55    // WR-N/UR-OFF
#define LOGIC_TYPE_SWITCH_UR_WRR                   56    // WR-R/UR-OFF
#define LOGIC_TYPE_SWITCH_UR_WRN_CHK               57    // WR-N/UR-CHK : WR-N 동작불능 체크시간
#define LOGIC_TYPE_SWITCH_UR_WRR_CHK               58    // WR-R/UR-CHK : WR-R 동작불능 체크시간

#define LOGIC_TYPE_SWITCH_UR_ILK                   61    // 연동취급에 의해 출력 후 TIMER 설정 (UR연동)
//#define LOGIC_TYPE_SWITCH_SET_ILK                  62    // 연동취급에 의해 순차출력 (순차연동)
//#define LOGIC_TYPE_SWITCH_URN_ILK                  62    // 연동취급(정위)
//#define LOGIC_TYPE_SWITCH_URR_ILK                  63    // 연동취급(반위)

#define LOGIC_TYPE_SWITCH_NORMAL                   71    // 정위
#define LOGIC_TYPE_SWITCH_REVERSE                  72    // 반위

#define LOGIC_TYPE_SWITCH_LOCK                     73
#define LOGIC_TYPE_SWITCH_LOCK_ROUTE               74    // 진로    쇄정
#define LOGIC_TYPE_SWITCH_LOCK_OVERLAP             75    // OverLap 쇄정
#define LOGIC_TYPE_SWITCH_LOCK_DEAD                76    // 철사    쇄정

#define LOGIC_TYPE_SWITCH_LOCK_ROUTE_OVERLAP       77    // 진로 쇄정 또는 OverLap 쇄정

#define LOGIC_TYPE_SWITCH_REL_ROUTE_LOCK_UR        78    // 진로설정시 일정시간동안 쇄정 (진로설정상태에서 일정시간 후 해정)
#define LOGIC_TYPE_SWITCH_REL_ROUTE_ARR_UR         79    // 착점도착 후 일정시간 후 해정

//#define LOGIC_TYPE_SWITCH_FAILURE                  81    // 고장
#define LOGIC_TYPE_SWITCH_OOC                      82    // 불일치
#define LOGIC_TYPE_SWITCH_OOC_WR                   83    // WR 방향을 모르는 상태의 불일치
#define LOGIC_TYPE_SWITCH_OOC_NORMAL               84    // 정위 불일치
#define LOGIC_TYPE_SWITCH_OOC_REVERSE              85    // 반위 불일치
#define LOGIC_TYPE_SWITCH_MOVING                   86    // 전환중
#define LOGIC_TYPE_SWITCH_OOC_UR_CHK               87    // OOC-UR : 불일치 체크시간
#define LOGIC_TYPE_SWITCH_OOC_WLR_OUT              88    // 연동취급 && WLR 출력 후 불일치

#define LOGIC_TYPE_SWITCH_OUT_WLR                  91    // WLR  출력 - OUT Card
#define LOGIC_TYPE_SWITCH_OUT_WRN                  92    // 정위 출력 - PDO Card
#define LOGIC_TYPE_SWITCH_OUT_WRR                  93    // 반위 출력 - PDO Card

#define LOGIC_TYPE_SWITCH_MOVING_NORMAL            94    // 정위 전환중
#define LOGIC_TYPE_SWITCH_MOVING_REVERSE           95    // 반위 전환중

#define LOGIC_TYPE_SWITCH_MOVING_UR                96    // 전환완료 Timer (전환중으로 취급불가 설정을 위한 Timer)

#define LOGIC_TYPE_SWITCH_SET_MOVE_NORMAL          97    // 정위 전환 설정 (연동)
#define LOGIC_TYPE_SWITCH_SET_MOVE_REVERSE         98    // 반위 전환 설정 (연동)
#define LOGIC_TYPE_SWITCH_SET_UR_OFF               99    // 연동취급 후 종료 Timer

#define LOGIC_TYPE_SWITCH_OUT_MCR                  101   // 노스가동 출력 - OUT Card
#define LOGIC_TYPE_SWITCH_OUT_GCPR                 102
#define LOGIC_TYPE_SWITCH_OUT_ApMCR                103   // 단동 또는 쌍동A호
#define LOGIC_TYPE_SWITCH_OUT_AfMCR                104   // 단동 또는 쌍동A호
#define LOGIC_TYPE_SWITCH_OUT_BpMCR                105   // 쌍동B호
#define LOGIC_TYPE_SWITCH_OUT_BfMCR                106   // 쌍동B호

//#define LOGIC_TYPE_SWITCH_OUT_PB                   111   // 단동취급에 대한 출력
//#define LOGIC_TYPE_SWITCH_OUT_ILK                  112   // 연동취급에 대한 출력
//#define LOGIC_TYPE_SWITCH_OUT_COMD_N               113   // 단동취급에 대한 출력 (정위)
//#define LOGIC_TYPE_SWITCH_OUT_COMD_R               114   // 단동취급에 대한 출력 (반위)
//#define LOGIC_TYPE_SWITCH_OUT_ILK_N                115   // 연동취급에 대한 출력 (정위)
//#define LOGIC_TYPE_SWITCH_OUT_ILK_R                116   // 연동취급에 대한 출력 (반위)

#define LOGIC_TYPE_SWITCH_OUT_COND                 117   // 선로전환기 출력 조건 OK

#define LOGIC_TYPE_SWITCH_OOC_COND_WRN             118   // 정위 불일치에 대한 WR 조건
#define LOGIC_TYPE_SWITCH_OOC_COND_WRR             119   // 반위 불일치에 대한 WR 조건

#define LOGIC_TYPE_SWITCH_IN_WLR                   121   // WLR  출력후 입력상태
#define LOGIC_TYPE_SWITCH_IN_WRN                   122   // 정위 출력후 입력상태
#define LOGIC_TYPE_SWITCH_IN_WRR                   123   // 반위 출력후 입력상태
#define LOGIC_TYPE_SWITCH_IN_KRN                   124   // KR-N        입력상태
#define LOGIC_TYPE_SWITCH_IN_KRR                   125   // KR-R        입력상태

#define LOGIC_TYPE_SWITCH_CHECK_NORMAL             126
#define LOGIC_TYPE_SWITCH_CHECK_REVERSE            127
#define LOGIC_TYPE_SWITCH_CHECK_WLR_NOT_EXPECTED   128    // WLR 부정입력 Check Timer

#define LOGIC_TYPE_SWITCH_WLR_NOT_INPUT            131    // 메시지 : WLR 동작불능 _T("%s/WLR입력불능") - WLR 입력정보 불능
#define LOGIC_TYPE_SWITCH_WR_NORMAL_NOT_INPUT      132    // 메시지 : WR 정위 동작불능
#define LOGIC_TYPE_SWITCH_WR_REVERSE_NOT_INPUT     133    // 메시지 : WR 반위 동작불능

#define LOGIC_TYPE_SWITCH_DELAY_OFF_WLR            134

//#define LOGIC_TYPE_SWITCH_NOT_EXPECTED_INPUT       141    // 선로전환기 부정입력  _T("%s/P부정입력")
#define LOGIC_TYPE_SWITCH_NOT_EXPECTED_IN_WLR      142
#define LOGIC_TYPE_SWITCH_NOT_EXPECTED_IN_WRN      143
#define LOGIC_TYPE_SWITCH_NOT_EXPECTED_IN_WRR      144
#define LOGIC_TYPE_SWITCH_NOT_EXPECTED_OUTPUT      145    // 선로전환기 부정출력  _T("%s/P부정출력")
#define LOGIC_TYPE_SWITCH_NOT_EXPECTED_OUT_WLR     146
#define LOGIC_TYPE_SWITCH_NOT_EXPECTED_OUT_WRN     147
#define LOGIC_TYPE_SWITCH_NOT_EXPECTED_OUT_WRR     148

#define LOGIC_TYPE_SWITCH_NOT_EXPECTED_UR_WLR      151    // 선로전환기 WLR/부정입력UR
#define LOGIC_TYPE_SWITCH_NOT_EXPECTED_UR_WRN      152    // 선로전환기 WR-N/부정입력UR
#define LOGIC_TYPE_SWITCH_NOT_EXPECTED_UR_WRR      153    // 선로전환기 WR-R/부정입력UR


#define LOGIC_TYPE_SWITCH_DIR_CHECK_NORMAL         161    // NS-AM 또는 노스가동 단동 정위방향 체크 (DIR-N)
#define LOGIC_TYPE_SWITCH_DIR_CHECK_REVERSE        162    // NS-AM 또는 노스가동 단동 반위방향 체크 (DIR-R)
//#define LOGIC_TYPE_SWITCH_DIR_CHECK_A_NORMAL       163    // 표시분리 A호 또는 노스가동 A호 정위방향 체크 (A/DIR-N)
//#define LOGIC_TYPE_SWITCH_DIR_CHECK_A_REVERSE      164    // 표시분리 A호 또는 노스가동 A호 반위방향 체크 (A/DIR-R)
//#define LOGIC_TYPE_SWITCH_DIR_CHECK_B_NORMAL       165    // 표시분리 B호 또는 노스가동 B호 정위방향 체크 (B/DIR-N)
//#define LOGIC_TYPE_SWITCH_DIR_CHECK_B_REVERSE      166    // 표시분리 B호 또는 노스가동 B호 반위방향 체크 (B/DIR-R)

//-----------------------------------------------------
// 노스가동(MJ81) 첨단, 크로싱
#define LOGIC_TYPE_SWITCH_OOC_P                    201    // 단동 첨단불일치 - 정위,반위
#define LOGIC_TYPE_SWITCH_OOC_P_NORMAL             202    // 단동 첨단불일치 - 정위
#define LOGIC_TYPE_SWITCH_OOC_P_REVERSE            203    // 단동 첨단불일치 -,반위
#define LOGIC_TYPE_SWITCH_OOC_F                    204    // 단동 크로싱불일치 - 정위,반위
#define LOGIC_TYPE_SWITCH_OOC_F_NORMAL             205    // 단동 크로싱불일치 - 정위
#define LOGIC_TYPE_SWITCH_OOC_F_REVERSE            206    // 단동 크로싱불일치 - 반위

#define LOGIC_TYPE_SWITCH_MJ81_P_NORMAL            207    // 첨단   정위
#define LOGIC_TYPE_SWITCH_MJ81_P_REVERSE           208    // 첨단   반위
#define LOGIC_TYPE_SWITCH_MJ81_F_NORMAL            209    // 크로싱 정위
#define LOGIC_TYPE_SWITCH_MJ81_F_REVERSE           210    // 크로싱 반위

#define LOGIC_TYPE_SWITCH_OOC_AP                   221    // 쌍동 A호 첨단불일치 - 정위,반위
#define LOGIC_TYPE_SWITCH_OOC_AP_NORMAL            222    // 쌍동 A호 첨단불일치 - 정위
#define LOGIC_TYPE_SWITCH_OOC_AP_REVERSE           223    // 쌍동 A호 첨단불일치 - 반위
#define LOGIC_TYPE_SWITCH_OOC_AF                   224    // 쌍동 A호 크로싱불일치 - 정위,반위
#define LOGIC_TYPE_SWITCH_OOC_AF_NORMAL            225    // 쌍동 A호 크로싱불일치 - 정위
#define LOGIC_TYPE_SWITCH_OOC_AF_REVERSE           226    // 쌍동 A호 크로싱불일치 - 반위

#define LOGIC_TYPE_SWITCH_OOC_BP                   227    // 쌍동 B호 첨단불일치 - 정위,반위
#define LOGIC_TYPE_SWITCH_OOC_BP_NORMAL            228    // 쌍동 B호 첨단불일치 - 정위
#define LOGIC_TYPE_SWITCH_OOC_BP_REVERSE           229    // 쌍동 B호 첨단불일치 - 반위
#define LOGIC_TYPE_SWITCH_OOC_BF                   230    // 쌍동 B호 크로싱불일치 - 정위,반위
#define LOGIC_TYPE_SWITCH_OOC_BF_NORMAL            231    // 쌍동 B호 크로싱불일치 - 정위
#define LOGIC_TYPE_SWITCH_OOC_BF_REVERSE           232    // 쌍동 B호 크로싱불일치 - 반위

#define LOGIC_TYPE_SWITCH_MJ81_AP_NORMAL           233    // 첨단   A호 정위
#define LOGIC_TYPE_SWITCH_MJ81_AP_REVERSE          234    // 첨단   A호 반위
#define LOGIC_TYPE_SWITCH_MJ81_AF_NORMAL           235    // 크로싱 A호 정위
#define LOGIC_TYPE_SWITCH_MJ81_AF_REVERSE          236    // 크로싱 A호 반위

#define LOGIC_TYPE_SWITCH_MJ81_BP_NORMAL           237    // 첨단   B호 정위
#define LOGIC_TYPE_SWITCH_MJ81_BP_REVERSE          238    // 첨단   B호 반위
#define LOGIC_TYPE_SWITCH_MJ81_BF_NORMAL           239    // 크로싱 B호 정위
#define LOGIC_TYPE_SWITCH_MJ81_BF_REVERSE          240    // 크로싱 B호 반위

//-----------------------------------------------------
// 표시분리 선로전환기
#define LOGIC_TYPE_SWITCH_DIST_A_NORMAL            301    // 표시분리 A호 정위
#define LOGIC_TYPE_SWITCH_DIST_A_REVERSE           302    // 표시분리 A호 반위
#define LOGIC_TYPE_SWITCH_DIST_B_NORMAL            303    // 표시분리 B호 정위
#define LOGIC_TYPE_SWITCH_DIST_B_REVERSE           304    // 표시분리 B호 반위

#define LOGIC_TYPE_SWITCH_DIST_A_LOCK              305    // 표시분리 A호 쇄정
#define LOGIC_TYPE_SWITCH_DIST_B_LOCK              306    // 표시분리 B호 쇄정

#define LOGIC_TYPE_SWITCH_DIST_OOC_A_WR            311   // 표시분리 A호 WR 방향을 모르는 상태의 불일치
#define LOGIC_TYPE_SWITCH_DIST_OOC_B_WR            312   // 표시분리 B호 WR 방향을 모르는 상태의 불일치
#define LOGIC_TYPE_SWITCH_DIST_OOC_A_NORMAL        313   // 표시분리 A호 정위 불일치
#define LOGIC_TYPE_SWITCH_DIST_OOC_A_REVERSE       314   // 표시분리 A호 반위 불일치
#define LOGIC_TYPE_SWITCH_DIST_OOC_B_NORMAL        315   // 표시분리 B호 정위 불일치
#define LOGIC_TYPE_SWITCH_DIST_OOC_B_REVERSE       316   // 표시분리 B호 반위 불일치

#define LOGIC_TYPE_SWITCH_DIST_OOC_A_COND_WRN      317    // 표시분리 A호 정위 불일치에 대한 WR 조건
#define LOGIC_TYPE_SWITCH_DIST_OOC_A_COND_WRR      318    // 표시분리 A호 반위 불일치에 대한 WR 조건
#define LOGIC_TYPE_SWITCH_DIST_OOC_B_COND_WRN      319    // 표시분리 B호 정위 불일치에 대한 WR 조건
#define LOGIC_TYPE_SWITCH_DIST_OOC_B_COND_WRR      320    // 표시분리 B호 반위 불일치에 대한 WR 조건

#define LOGIC_TYPE_SWITCH_DIST_A_LOCK_ROUTE        321    // 표시분리 A호 진로    쇄정
#define LOGIC_TYPE_SWITCH_DIST_B_LOCK_ROUTE        322    // 표시분리 B호 진로    쇄정
#define LOGIC_TYPE_SWITCH_DIST_A_LOCK_OVERLAP      323    // 표시분리 A호 Overlap 쇄정
#define LOGIC_TYPE_SWITCH_DIST_B_LOCK_OVERLAP      324    // 표시분리 B호 Overlap 쇄정
#define LOGIC_TYPE_SWITCH_DIST_A_LOCK_DEAD         325    // 표시분리 A호 철사    쇄정
#define LOGIC_TYPE_SWITCH_DIST_B_LOCK_DEAD         326    // 표시분리 B호 철사    쇄정


//-----------------------------------------------------
//#define LOGIC_TYPE_SWITCH_FAIL_IN_ROUTE            1000  // 진로에서 선로전환기 장애 (선로전환기/장애(진로) : 1000 + 진로인덱스)
//#define LOGIC_TYPE_SWITCH_RTELOCK_IN_ROUTE         3000  // 선로전환기를 진로쇄정하는 진로 (선로전환기/R쇄정(진로) : 3000 + 진로인덱스)
//#define LOGIC_TYPE_SWITCH_TRKLOCK_IN_ROUTE         5000  // 선로전환기를 진로쇄정하는 진로 (선로전환기/T쇄정(진로)

//-----------------------------------------------------
#define LOGIC_TYPE_SWITCH_DANCASE_SIGNAL           7000    // 단조건 (선로전환기/DAN신호기) : 7000 + 신호기인덱스
#define LOGIC_TYPE_SWITCH_DANCASE_SIGNAL_END       7999
#define LOGIC_TYPE_SWITCH_DANCASE_TRACK            8000    // 단조건 (선로전환기/DAN궤도)   : 8000 + 궤도인덱스
#define LOGIC_TYPE_SWITCH_DANCASE_TRACK_END        8999
//#define LOGIC_TYPE_SWITCH_DANCASE_SIGNAL_STOP      9000    // 단조건 (선로전환기/DAN신호기-R) : 9000 + 신호기인덱스

#define LOGIC_TYPE_SWITCH_DANCASE_ROUTE_LOCKT     10000    // (선로전환기/DAN진로-LCK) 진로취급시 설정시간동안 쇄정[선로전환기 단 00초]
#define LOGIC_TYPE_SWITCH_DANCASE_ROUTE_RELEASET  11000    // (선로전환기/DAN진로-REL) 진로도착 후 설정 후 해정 [[선로전환기 단 00초]]


//---------------------------------------------------------------
// 진로 (LOGIC_VARIABLE.LogicKind=LOGIC_KIND_ROUTE) : _T('R')
//---------------------------------------------------------------
#define LOGIC_TYPE_RTE_CMD_PB_LOCAL                11    // 표시제어부 진로 취급
#define LOGIC_TYPE_RTE_CMD_RB_LOCAL                12    // 표시제어부 진로 재취급
#define LOGIC_TYPE_RTE_CMD_CB_LOCAL                13    // 표시제어부 진로 취소 취급
#define LOGIC_TYPE_RTE_CMD_TTB_PB_LOCAL            14    // 표시제어부 진로 TTB 취급
#define LOGIC_TYPE_RTE_CMD_TCB_CB_LOCAL            15    // 표시제어부 진로 TTB 취소 취급
#define LOGIC_TYPE_RTE_CMD_UDO_ON_LOCAL            16    // 표시제어부 (무)유도 진로 취급
#define LOGIC_TYPE_RTE_CMD_UDO_OFF_LOCAL           17    // 표시제어부 (무)유도 진로 취소 취급

#define LOGIC_TYPE_RTE_CMD_PB_RC                   21    // RC 취급
#define LOGIC_TYPE_RTE_CMD_RB_RC                   22
#define LOGIC_TYPE_RTE_CMD_CB_RC                   23
#define LOGIC_TYPE_RTE_CMD_TTB_PB_RC               24
#define LOGIC_TYPE_RTE_CMD_TCB_CB_RC               25
#define LOGIC_TYPE_RTE_CMD_UDO_ON_RC               26
#define LOGIC_TYPE_RTE_CMD_UDO_OFF_RC              27

#define LOGIC_TYPE_RTE_CMD_PB_CTC                  31    // CTC 취급
#define LOGIC_TYPE_RTE_CMD_RB_CTC                  32
#define LOGIC_TYPE_RTE_CMD_CB_CTC                  33
#define LOGIC_TYPE_RTE_CMD_TTB_PB_CTC              34
#define LOGIC_TYPE_RTE_CMD_TCB_CB_CTC              35
#define LOGIC_TYPE_RTE_CMD_UDO_ON_CTC              36
#define LOGIC_TYPE_RTE_CMD_UDO_OFF_CTC             37

#define LOGIC_TYPE_RTE_PB                          41    // 취급
#define LOGIC_TYPE_RTE_RB                          42    // 재취급
#define LOGIC_TYPE_RTE_CB                          43    // 취소
//#define LOGIC_TYPE_RTE_TTB_PB                      44    // TTB 취급
//#define LOGIC_TYPE_RTE_TCB_CB                      45    // TTB 취소
//#define LOGIC_TYPE_RTE_PB_UDO_ON                   46    // (무)유도 진로 취급
//#define LOGIC_TYPE_RTE_CB_UDO_OFF                  47    // (무)유도 진로 취소 취급

//#define LOGIC_TYPE_RTE_PCB                         48    // 진로 취소   가능상태
//#define LOGIC_TYPE_RTE_PRB                         49    // 진로 재취급 가능상태

//#define LOGIC_TYPE_RTE_UR_ON                       51
//#define LOGIC_TYPE_RTE_UR_OFF                      52
//#define LOGIC_TYPE_RTE_PB_UR_ON                    53    // 취급UR-ON
//#define LOGIC_TYPE_RTE_CB_UR_ON                    54    // 취소UR-ON

//#define LOGIC_TYPE_RTE_INHIBIT_CANCEL              55    // 진로 취소불가상태

#define LOGIC_TYPE_RTE_SET                         60    // 진로/SET

//#define LOGIC_TYPE_RTE_REQUEST                     61    // 진로 요구 (진로/REQ)
//#define LOGIC_TYPE_RTE_REQUEST_CANCEL              62    // 진로     취소 요구상태

//#define LOGIC_TYPE_RTE_SHZR                        63    // 입환 무유도 진로에서 표지현시 가능
//#define LOGIC_TYPE_RTE_SZR                         64    // (무)유도 현시가능
#define LOGIC_TYPE_RTE_CR                          65    // 진로취급 가능상태 (진로조사)
#define LOGIC_TYPE_RTE_SR                          66    // 진로     요구상태 (진로 취급시 여자, 신호 현시 전까지 계속 여자)
#define LOGIC_TYPE_RTE_ZR                          67    // 신호현시 가능상태 (진로내 궤도,선로전화기 쇄정 후 여자) : 여자 후 HR 출력

#define LOGIC_TYPE_RTE_SETLOCK                     68    // 진로 쇄정 상태[진로/LOCK] - 신호 현시
#define LOGIC_TYPE_RTE_SETRETRY                    69    // 진로 이상 상태(신호 ON)
//#define LOGIC_TYPE_RTE_SETLOCK_SHUNT               70    // 입환공용일 때 표지진로 쇄정상태

#define LOGIC_TYPE_RTE_EMG                         71    // 궤도 구분진로 비상해정 취급 가능 상태 [진로/EMG]
#define LOGIC_TYPE_RTE_ZR_OCCP                     72    // 진로내의 궤도 낙하 상태
#define LOGIC_TYPE_RTE_FREET                       73    // 유효장내 궤도 점유 && 유효장 후방궤도 복구 상태 [진로/FreeT]

#define LOGIC_TYPE_RTE_APP_OCCP                    74    // 접근쇄정중 궤도 낙하
#define LOGIC_TYPE_RTE_APP_OCC_CALLON              75    // 접근쇄정중 무유도만OFF 궤도 낙하

//#define LOGIC_TYPE_RTE_PREV_FAIL                   76    // 진로 취급 이전 궤도 낙하, 선로전환기 불일치

#define LOGIC_TYPE_RTE_SH_OCCP                     77    // 입환공용진로에서 무유도만 Off궤도 한 개 이상 낙하 (SH-OCCP)

//#define LOGIC_TYPE_RTE_SIG_ON                      78    // 진로에 대한 신호기 현시
//#define LOGIC_TYPE_RTE_SHR_ON                      79    // 주신호 진로에 대한 유도 현시

#define LOGIC_TYPE_RTE_SETTTB                      80    // 진로 TTB Set 상태

#define LOGIC_TYPE_RTE_APP_SET                     81    // 진로 접근(보류)쇄정 시작설정 (APP-START)
#define LOGIC_TYPE_RTE_APP_LOCK                    82    // 진로 접근쇄정상태(APP)
#define LOGIC_TYPE_RTE_APP_UR                      83
#define LOGIC_TYPE_RTE_APP_UR_OFF                  84

#define LOGIC_TYPE_RTE_DELLOCK                     85    // 진로 보류쇄정상태(APPD)
//#define LOGIC_TYPE_RTE_DELLOCK_UR                  86
//#define LOGIC_TYPE_RTE_DELLOCK_UR_OFF              87

#define LOGIC_TYPE_RTE_UHO                         88    // 유효장
#define LOGIC_TYPE_RTE_UHO_UR                      89
#define LOGIC_TYPE_RTE_UHO_UR_OFF                  90
#define LOGIC_TYPE_RTE_UHO_ALARM                   91    // 유효장 시작, 종료 알람

#define LOGIC_TYPE_RTE_FAIL_TRACK                  92    // 진로현시 후 (진로 내,외)궤도 장애
//#define LOGIC_TYPE_RTE_FAIL_TRACK_RTELOCK          93    // 진로현시 후 (진로 내)   궤도 장애

#define LOGIC_TYPE_RTE_FAIL_SWITCH                 94    // 진로현시 후 (진로쇄정, Overlap) 선로전환기 불일치
//#define LOGIC_TYPE_RTE_FAIL_SWITCH_RTELOCK         95    // 진로현시 후 (진로쇄정)          선로전환기 불일치

//#define LOGIC_TYPE_RTE_EMGRECOVERY_TRACK           96    // 진로쇄정 상태에서 궤도 비상해정 취급 (T비상해정) - 모든 궤도
//#define LOGIC_TYPE_RTE_ABRECOVERY_TRACK            97    // 진로쇄정 상태에서 궤도 이상복구 (T이상복구)
//#define LOGIC_TYPE_RTE_ABRECOVERY_TRACK_ROUTE      98    // 진로쇄정 상태에서 궤도 이상복구 (궤도이상)

#define LOGIC_TYPE_RTE_APP_END                     99    // 접근,보류 쇄정 종료
#define LOGIC_TYPE_RTE_UHO_END                     100   // 유효장 종료

//----------------------------------------------------
// 진로제어 상태
//#define LOGIC_TYPE_RTE_SELECTION                   101    // 진로   선별중
//#define LOGIC_TYPE_RTE_SWCONTROL                   102    // 전철기 전환중(선별완료)
//#define LOGIC_TYPE_RTE_ROUTECHECK                  103    // 진로   조사중(전철기 전환중)
//#define LOGIC_TYPE_RTE_LIGHTSIGNAL                 104    // 신호기 제어중(진로쇄정 완료)
//#define LOGIC_TYPE_RTE_ENDCONTROL                  105    // 진로   제어완료

#define LOGIC_TYPE_RTE_CHANGING                    106    // 주신호 무유도 to 신호현시
#define LOGIC_TYPE_RTE_CHANGING_SH                 107    // 입환신호 유도에서 표지현시로 변경중

#define LOGIC_TYPE_RTE_ABRECOVERY_LOCK_TRACK       108    // 진로내 궤도 1개 이상 이상복구 상태(진로/T이상)
#define LOGIC_TYPE_RTE_RELEASE_TRACK               109    // 진로내 궤도 1개 이상 해정 상태(진로/T해정)
#define LOGIC_TYPE_RTE_ABNORMAL_LOCK_TRACK         110    // 진로 내 궤도 비정상 복구(진로/AbNorT)

//----------------------------------------------------
// 진로 비정상 상태
#define LOGIC_TYPE_RTE_FAIL                        111    // 진로/FAIL

//#define LOGIC_TYPE_RTE_FAIL_RTELOCK                112    // 진로쇄정 후 장애(/FailLock)

//#define LOGIC_TYPE_RTE_FAIL_TRACK_EMG              113    // 궤도 비상(/FailEmgT)
//#define LOGIC_TYPE_RTE_FAIL_TRACK_OCCUPY           114    // 궤도 장애
//#define LOGIC_TYPE_RTE_FAIL_TRACK_ABFREE           115    // 궤도 이상 복구(/FailAbT)
//#define LOGIC_TYPE_RTE_FAIL_SWITCHOOC              116    // 전철기 불일치
//#define LOGIC_TYPE_RTE_FAIL_SWITCHFAIL             117    // 전철기 불일치
//#define LOGIC_TYPE_RTE_FAIL_SIGNAL                 118    // 신호기 고장
//#define LOGIC_TYPE_RTE_FAIL_POWER                  119    // 정전후 복구
//#define LOGIC_TYPE_RTE_FAIL_BLOCK                  120    // 폐색 이상

//----------------------------------------------------
//#define LOGIC_TYPE_RTE_COND_BLOCK                  121   // 진로이름/BLK
//#define LOGIC_TYPE_RTE_COND_SWITCH                 122   // 진로이름/SWH
#define LOGIC_TYPE_RTE_COND_DAN                    123   // 진로이름/단조건
//#define LOGIC_TYPE_RTE_COND_OPPRTE                 124   // 진로이름/OppRte
//#define LOGIC_TYPE_RTE_COND_TRKLOCK                125   // 진로이름/TROCK
//#define LOGIC_TYPE_RTE_COND_TRKOCC                 126   // 진로이름/TROCC

//----------------------------------------------------
// 원격제어(집중화)에 진로 SIGON 설정 : 진로의 궤도 황색 표시
// 진로/FAIL
// 접근쇄정 상태
// 접근쇄정 상태 중 궤도낙하
// LMR장애로 신호정지
#define LOGIC_TYPE_RTE_RC_SIGON                    127    // 진로이름/SIGON

//----------------------------------------------------
#define LOGIC_TYPE_RTE_OPP_INHIBIT                 131
#define LOGIC_TYPE_RTE_OPP_SWITCH                  132
#define LOGIC_TYPE_RTE_OPP_TRACK                   133
#define LOGIC_TYPE_RTE_OPP_SIGNAL                  134
#define LOGIC_TYPE_RTE_OPP_BLOCK_DEP               135
#define LOGIC_TYPE_RTE_OPP_BLOCK_ARR               136

//----------------------------------------------------
//#define LOGIC_TYPE_RTE_ZR_LOCK_TRK_ALL             200
//#define LOGIC_TYPE_RTE_ZR_LOCK_TRK                 201    // ZR 설정시 체크하는 궤도 상태 (201 ~ 499)
//#define LOGIC_TYPE_RTE_ZR_LOCK_SWH_ALL             500
//#define LOGIC_TYPE_RTE_ZR_LOCK_SWH                 501    // ZR 설정시 체크하는 선로전환기 상태 (301 ~ 799)

#define LOGIC_TYPE_RTE_REVERSE_TRACK_ALL           800
//#define LOGIC_TYPE_RTE_REVERSE_TRACK               801    // 진로쇄정 궤도에 대한 퇴행(801 ~ 1099)

#define LOGIC_TYPE_RTE_NOT_SIGON_TRACK_ALL         1100     // 메시지 : 궤도점유로 제어불가 (1101 ~ 1399)
#define LOGIC_TYPE_RTE_NOT_SIGON_TRACK_END         1399

//---------------------------------------------------------------
// 폐색 (LOGIC_VARIABLE.LogicKind=LOGIC_KIND_BLOCK) : _T('B')
//---------------------------------------------------------------
#define LOGIC_TYPE_BLK_FAIL                        11

#define LOGIC_TYPE_BLK_FAIL_COMMON                 12    // FAIL-COMMON (/F-COMM)
#define LOGIC_TYPE_BLK_FAIL_CHECK                  13    // FAIL-CHECK (/F-CHK)

#define LOGIC_TYPE_BLK_INPUT_ALL_OFF               14    // 폐색입력 모두 낙하

#define LOGIC_TYPE_BLK_MSG_COMPLETE                16    // 메시지 : 폐색수속완료
#define LOGIC_TYPE_BLK_MSG_FAIL                    17    // 메시지 : 폐색수속실패

#define LOGIC_TYPE_BLK_DEP_BLTR_RC                 18    // 원격제어 PC로 Bltr 낙하상태 정보 전송

//----------------------------------------------------
// 폐색 제어 상태
#define LOGIC_TYPE_BLK_CMD_CANCEL_LOCAL            22    // 표시제어부 취소취급
#define LOGIC_TYPE_BLK_CMD_START_LOCAL             23    // 표시제어부 연동폐색 START 취급
#define LOGIC_TYPE_BLK_CMD_HOME_LOCAL              24    // 표시제어부 연동폐색 HOME 취급
#define LOGIC_TYPE_BLK_CMD_OPEN_LOCAL              25    // 표시제어부 연동폐색 OPEN 취급
#define LOGIC_TYPE_BLK_CMD_ARR_REQ_LOCAL           26    // 표시제어부 장내취급
#define LOGIC_TYPE_BLK_CMD_ARR_CANCEL_LOCAL        27    // 표시제어부 장내취소취급
#define LOGIC_TYPE_BLK_CMD_DEP_REQ_LOCAL           28    // 표시제어부 출발취급

#define LOGIC_TYPE_BLK_CMD_CANCEL_RC               29    // RC
#define LOGIC_TYPE_BLK_CMD_START_RC                30
#define LOGIC_TYPE_BLK_CMD_HOME_RC                 31
#define LOGIC_TYPE_BLK_CMD_OPEN_RC                 32
#define LOGIC_TYPE_BLK_CMD_ARR_REQ_RC              33
#define LOGIC_TYPE_BLK_CMD_ARR_CANCEL_RC           34
#define LOGIC_TYPE_BLK_CMD_DEP_REQ_RC              35

#define LOGIC_TYPE_BLK_CMD_CANCEL_CTC              36    // CTC
#define LOGIC_TYPE_BLK_CMD_START_CTC               37
#define LOGIC_TYPE_BLK_CMD_HOME_CTC                38
#define LOGIC_TYPE_BLK_CMD_OPEN_CTC                39
#define LOGIC_TYPE_BLK_CMD_ARR_REQ_CTC             40
#define LOGIC_TYPE_BLK_CMD_ARR_CANCEL_CTC          41
#define LOGIC_TYPE_BLK_CMD_DEP_REQ_CTC             42

#define LOGIC_TYPE_BLK_PB_CANCEL                   51    // 폐색 취소취급
#define LOGIC_TYPE_BLK_PB_START                    52    // 폐색 출발취급
#define LOGIC_TYPE_BLK_PB_HOME                     53
#define LOGIC_TYPE_BLK_PB_OPEN                     54

#define LOGIC_TYPE_BLK_PB_ARR_REQ                  55    // 역방향 장내취급
#define LOGIC_TYPE_BLK_PB_ARR_CANCEL               56    // 역방향 취소취급
#define LOGIC_TYPE_BLK_PB_DEP_REQ                  57    // 역방향 출발취급

#define LOGIC_TYPE_BLK_UR_ON                       58
#define LOGIC_TYPE_BLK_UR_OFF                      59

#define LOGIC_TYPE_BLK_INPUT                       61
#define LOGIC_TYPE_BLK_OUTPUT                      62


//----------------------------------------------------
// 연동폐색
#define LOGIC_TYPE_BLK_REL_INPUT_ATSR              71
#define LOGIC_TYPE_BLK_REL_INPUT_AFR               72
#define LOGIC_TYPE_BLK_REL_INPUT_AEHAR             73
#define LOGIC_TYPE_BLK_REL_INPUT_ABOR              74
#define LOGIC_TYPE_BLK_REL_INPUT_DTSR              75
#define LOGIC_TYPE_BLK_REL_INPUT_DFR               76
#define LOGIC_TYPE_BLK_REL_INPUT_DEHAR             77
#define LOGIC_TYPE_BLK_REL_INPUT_DBOR              78

#define LOGIC_TYPE_BLK_REL_OUTPUT_START            79
#define LOGIC_TYPE_BLK_REL_OUTPUT_HOME             80
#define LOGIC_TYPE_BLK_REL_OUTPUT_OPEN             81
#define LOGIC_TYPE_BLK_REL_OUTPUT_CANCEL           82

//#define LOGIC_TYPE_BLK_REL_ARR_GRAY                83
#define LOGIC_TYPE_BLK_REL_ARR_YELLOW              84
#define LOGIC_TYPE_BLK_REL_ARR_YF                  85
#define LOGIC_TYPE_BLK_REL_ARR_RED                 86
#define LOGIC_TYPE_BLK_REL_ARR_RF                  87

//#define LOGIC_TYPE_BLK_REL_DEP_GRAY                88
#define LOGIC_TYPE_BLK_REL_DEP_YELLOW              89
#define LOGIC_TYPE_BLK_REL_DEP_YF                  90
#define LOGIC_TYPE_BLK_REL_DEP_RED                 91
#define LOGIC_TYPE_BLK_REL_DEP_RF                  92

#define LOGIC_TYPE_BLK_REL_ARR_UR_ATPSR            93
#define LOGIC_TYPE_BLK_REL_ARR_UR_AFR              94
#define LOGIC_TYPE_BLK_REL_ARR_UR_AEHAR            95
#define LOGIC_TYPE_BLK_REL_ARR_UR_ABOR             96
#define LOGIC_TYPE_BLK_REL_DEP_UR_DTPSR            97
#define LOGIC_TYPE_BLK_REL_DEP_UR_DFR              98
#define LOGIC_TYPE_BLK_REL_DEP_UR_DEHAR            99
#define LOGIC_TYPE_BLK_REL_DEP_UR_DBOR             100

#define LOGIC_TYPE_BLK_REL_UR_START                101
#define LOGIC_TYPE_BLK_REL_UR_HOME                 102
#define LOGIC_TYPE_BLK_REL_UR_OPEN                 103
#define LOGIC_TYPE_BLK_REL_UR_CANCEL               104

#define LOGIC_TYPE_BLK_REL_UR_OUT                  105    // 폐색 출력 시간

#define LOGIC_TYPE_BLK_REL_PB_SUCCESS              106
#define LOGIC_TYPE_BLK_REL_PB_FAILURE              107

//----------------------------------------------------
// 대야폐색
#define LOGIC_TYPE_BLK_DEAYA_INPUT_ARR_Y           111
#define LOGIC_TYPE_BLK_DEAYA_INPUT_ARR_R           112
#define LOGIC_TYPE_BLK_DEAYA_INPUT_DEP_Y           113
#define LOGIC_TYPE_BLK_DEAYA_INPUT_DEP_R           114
#define LOGIC_TYPE_BLK_DEAYA_INPUT_TPSR            115

#define LOGIC_TYPE_BLK_DEAYA_OUTPUT_START          116
#define LOGIC_TYPE_BLK_DEAYA_OUTPUT_HOME           117
#define LOGIC_TYPE_BLK_DEAYA_OUTPUT_OPEN           118
#define LOGIC_TYPE_BLK_DEAYA_OUTPUT_CANCEL_DEP     119
#define LOGIC_TYPE_BLK_DEAYA_OUTPUT_CANCEL_ARR     120

//#define LOGIC_TYPE_BLK_DEAYA_ARR_GRAY              121
#define LOGIC_TYPE_BLK_DEAYA_ARR_YELLOW            122
#define LOGIC_TYPE_BLK_DEAYA_ARR_YF                123
#define LOGIC_TYPE_BLK_DEAYA_ARR_RED               124
#define LOGIC_TYPE_BLK_DEAYA_ARR_RF                125

//#define LOGIC_TYPE_BLK_DEAYA_DEP_GRAY              126
#define LOGIC_TYPE_BLK_DEAYA_DEP_YELLOW            127
#define LOGIC_TYPE_BLK_DEAYA_DEP_YF                128
#define LOGIC_TYPE_BLK_DEAYA_DEP_RED               129
#define LOGIC_TYPE_BLK_DEAYA_DEP_RF                130

#define LOGIC_TYPE_BLK_DEAYA_UR_ARR_Y              131
#define LOGIC_TYPE_BLK_DEAYA_UR_ARR_R              132
#define LOGIC_TYPE_BLK_DEAYA_UR_DEP_Y              133
#define LOGIC_TYPE_BLK_DEAYA_UR_DEP_R              134
#define LOGIC_TYPE_BLK_DEAYA_UR_TPSR               135

#define LOGIC_TYPE_BLK_DEAYA_UR_OUT                136    // 폐색 출력 시간

//----------------------------------------------------
// 지하철 폐색
#define LOGIC_TYPE_BLK_SUBWAY_INPUT_START          141     // 출발
#define LOGIC_TYPE_BLK_SUBWAY_INPUT_OPEN           142     // 개통
#define LOGIC_TYPE_BLK_SUBWAY_INPUT_ARR            143     // 장내
#define LOGIC_TYPE_BLK_SUBWAY_INPUT_SIG            144     // 신호
//#define LOGIC_TYPE_BLK_SUBWAY_INPUT_ARR            145

#define LOGIC_TYPE_BLK_SUBWAY_UR_START             146
#define LOGIC_TYPE_BLK_SUBWAY_UR_OPEN              147
#define LOGIC_TYPE_BLK_SUBWAY_UR_ARR               148
#define LOGIC_TYPE_BLK_SUBWAY_UR_SIG               149

//#define LOGIC_TYPE_BLK_SUBWAY_ARR_GRAY             151
#define LOGIC_TYPE_BLK_SUBWAY_ARR_YELLOW           152
#define LOGIC_TYPE_BLK_SUBWAY_ARR_GREEN            153
#define LOGIC_TYPE_BLK_SUBWAY_ARR_RED              154

//#define LOGIC_TYPE_BLK_SUBWAY_DEP_GRAY             156
#define LOGIC_TYPE_BLK_SUBWAY_DEP_YELLOW           157
#define LOGIC_TYPE_BLK_SUBWAY_DEP_GREEN            158
#define LOGIC_TYPE_BLK_SUBWAY_DEP_RED              159

#define LOGIC_TYPE_SUBWAY_NORMAL_DIR_LIGHT_ACT     161    // 정방향개통표시등 동작(/ACT)

//----------------------------------------------------
// 복선 자동 폐색
#define LOGIC_TYPE_BLK_ABS_DOUBLE_INPUT_YY         171
#define LOGIC_TYPE_BLK_ABS_DOUBLE_INPUT_Y          172
#define LOGIC_TYPE_BLK_ABS_DOUBLE_INPUT_YG         173

//#define LOGIC_TYPE_BLK_ABS_DOUBLE_DEP_LIGHT_INPUT  176    // 출발개통표시등 입력(/I)
//#define LOGIC_TYPE_BLK_ABS_DOUBLE_DEP_LIGHT_ON     177    // 출발개통표시등 입력 여자(/ON)
#define LOGIC_TYPE_BLK_ABS_DOUBLE_DEP_LIGHT_ACT    178    // 출발개통표시등 동작(/ACT)

#define LOGIC_TYPE_BLK_ABS_DOUBLE_UR_YY            184
#define LOGIC_TYPE_BLK_ABS_DOUBLE_UR_Y             185
#define LOGIC_TYPE_BLK_ABS_DOUBLE_UR_YG            186

#define LOGIC_TYPE_BLK_ABS_DOUBLE_UR_OFF_YY        187
#define LOGIC_TYPE_BLK_ABS_DOUBLE_UR_OFF_Y         188
#define LOGIC_TYPE_BLK_ABS_DOUBLE_UR_OFF_YG        189

//----------------------------------------------------
// 단선 자동폐색
#define LOGIC_TYPE_BLK_ABS_SINGLE_INPUT_DIR        201
#define LOGIC_TYPE_BLK_ABS_SINGLE_INPUT_BR         202
#define LOGIC_TYPE_BLK_ABS_SINGLE_INPUT_DR         203
#define LOGIC_TYPE_BLK_ABS_SINGLE_INPUT_BLTR       204
#define LOGIC_TYPE_BLK_ABS_SINGLE_INPUT_YY         205
#define LOGIC_TYPE_BLK_ABS_SINGLE_INPUT_Y          206
#define LOGIC_TYPE_BLK_ABS_SINGLE_INPUT_YG         207

#define LOGIC_TYPE_BLK_ABS_SINGLE_OUTPUT_RR        208
#define LOGIC_TYPE_BLK_ABS_SINGLE_OUTPUT_CNR       209

//#define LOGIC_TYPE_BLK_ABS_SINGLE_ARR_GRAY         210
#define LOGIC_TYPE_BLK_ABS_SINGLE_ARR_YELLOW       211
#define LOGIC_TYPE_BLK_ABS_SINGLE_ARR_RED          212

//#define LOGIC_TYPE_BLK_ABS_SINGLE_DEP_GRAY         213
#define LOGIC_TYPE_BLK_ABS_SINGLE_DEP_YELLOW       214
#define LOGIC_TYPE_BLK_ABS_SINGLE_DEP_RED          215

#define LOGIC_TYPE_BLK_ABS_SINGLE_UR_CNR           216    // 폐색 취소 출력 시간
#define LOGIC_TYPE_BLK_ABS_SINGLE_UR_iDIR          217
#define LOGIC_TYPE_BLK_ABS_SINGLE_UR_oBR           218
#define LOGIC_TYPE_BLK_ABS_SINGLE_UR_oDR           219
#define LOGIC_TYPE_BLK_ABS_SINGLE_UR_BLTR          220
#define LOGIC_TYPE_BLK_ABS_SINGLE_UR_YY            221
#define LOGIC_TYPE_BLK_ABS_SINGLE_UR_Y             222
#define LOGIC_TYPE_BLK_ABS_SINGLE_UR_YG            223

//#define LOGIC_TYPE_BLK_ABS_SINGLE_UR_OFF_DIR       226
#define LOGIC_TYPE_BLK_ABS_SINGLE_UR_OFF_BR        227
#define LOGIC_TYPE_BLK_ABS_SINGLE_UR_OFF_DR        228
//#define LOGIC_TYPE_BLK_ABS_SINGLE_UR_OFF_BLTR      229
#define LOGIC_TYPE_BLK_ABS_SINGLE_UR_OFF_YY        230
#define LOGIC_TYPE_BLK_ABS_SINGLE_UR_OFF_Y         231
#define LOGIC_TYPE_BLK_ABS_SINGLE_UR_OFF_YG        232

#define LOGIC_TYPE_BLK_ABS_SINGLE_DEP_YELLOW_ZR    233    // 출발YZR

//----------------------------------------------------
// 의왕폐색
#define LOGIC_TYPE_BLK_EUIWANG_INPUT_HR            241
#define LOGIC_TYPE_BLK_EUIWANG_INPUT_BHR           242
#define LOGIC_TYPE_BLK_EUIWANG_INPUT_TR            243
#define LOGIC_TYPE_BLK_EUIWANG_INPUT_TPSR          244
#define LOGIC_TYPE_BLK_EUIWANG_INPUT_eHR           245

#define LOGIC_TYPE_BLK_EUIWANG_OUTPUT_START        251
#define LOGIC_TYPE_BLK_EUIWANG_OUTPUT_CNR          252

//#define LOGIC_TYPE_BLK_EUIWANG_ARR_GRAY            253
#define LOGIC_TYPE_BLK_EUIWANG_ARR_YELLOW          254
#define LOGIC_TYPE_BLK_EUIWANG_ARR_YF              255
#define LOGIC_TYPE_BLK_EUIWANG_ARR_RED             256
#define LOGIC_TYPE_BLK_EUIWANG_ARR_RF              257

//#define LOGIC_TYPE_BLK_EUIWANG_DEP_GRAY            258
#define LOGIC_TYPE_BLK_EUIWANG_DEP_YELLOW          259
#define LOGIC_TYPE_BLK_EUIWANG_DEP_YF              260
#define LOGIC_TYPE_BLK_EUIWANG_DEP_RED             261
#define LOGIC_TYPE_BLK_EUIWANG_DEP_RF              262

#define LOGIC_TYPE_BLK_EUIWANG_UR_HR               263
#define LOGIC_TYPE_BLK_EUIWANG_UR_BHR              264
#define LOGIC_TYPE_BLK_EUIWANG_UR_TR               265
#define LOGIC_TYPE_BLK_EUIWANG_UR_TPSR             266
#define LOGIC_TYPE_BLK_EUIWANG_UR_eHR              267

#define LOGIC_TYPE_BLK_EUIWANG_UR_OUT              268    // 폐색 출력 시간
#define LOGIC_TYPE_BLK_EUIWANG_UR_OUT_CNR          269    // 폐색 취소 출력 시간

//----------------------------------------------------
// 양방향폐색
#define LOGIC_TYPE_BLK_BOTH_INPUT_YY               271
#define LOGIC_TYPE_BLK_BOTH_INPUT_Y                272
#define LOGIC_TYPE_BLK_BOTH_INPUT_YG               273
#define LOGIC_TYPE_BLK_BOTH_INPUT_oRR              274
#define LOGIC_TYPE_BLK_BOTH_INPUT_iZR              275
#define LOGIC_TYPE_BLK_BOTH_INPUT_iZDIR            276
#define LOGIC_TYPE_BLK_BOTH_INPUT_BLTR             277

#define LOGIC_TYPE_BLK_BOTH_INPUT_oBR              281
#define LOGIC_TYPE_BLK_BOTH_INPUT_oDR              282
#define LOGIC_TYPE_BLK_BOTH_INPUT_CNR              283
#define LOGIC_TYPE_BLK_BOTH_INPUT_oZR              284
#define LOGIC_TYPE_BLK_BOTH_INPUT_iRR              285
#define LOGIC_TYPE_BLK_BOTH_INPUT_iRDir            286

////#define LOGIC_TYPE_BLK_BOTH_INPUT_iRR_OLD          287

#define LOGIC_TYPE_BLK_BOTH_OUTPUT_ZDIR            291
#define LOGIC_TYPE_BLK_BOTH_OUTPUT_RR              292
#define LOGIC_TYPE_BLK_BOTH_OUTPUT_CNR             293
#define LOGIC_TYPE_BLK_BOTH_OUTPUT_ZR              294
#define LOGIC_TYPE_BLK_BOTH_OUTPUT_OCCR            295

#define LOGIC_TYPE_BLK_BOTH_DEP_RED_COND           300

//#define LOGIC_TYPE_BLK_BOTH_ARR_GRAY               301
#define LOGIC_TYPE_BLK_BOTH_ARR_YELLOW             302
#define LOGIC_TYPE_BLK_BOTH_ARR_YF                 303
#define LOGIC_TYPE_BLK_BOTH_ARR_RED                304
#define LOGIC_TYPE_BLK_BOTH_ARR_RF                 305

//#define LOGIC_TYPE_BLK_BOTH_DEP_GRAY               306
#define LOGIC_TYPE_BLK_BOTH_DEP_YELLOW             307
#define LOGIC_TYPE_BLK_BOTH_DEP_YF                 308
#define LOGIC_TYPE_BLK_BOTH_DEP_RED                309
#define LOGIC_TYPE_BLK_BOTH_DEP_RF                 310

#define LOGIC_TYPE_BLK_BOTH_UR_YY                  311
#define LOGIC_TYPE_BLK_BOTH_UR_Y                   312
#define LOGIC_TYPE_BLK_BOTH_UR_YG                  313
#define LOGIC_TYPE_BLK_BOTH_UR_oRR                 314
#define LOGIC_TYPE_BLK_BOTH_UR_iZR                 315
#define LOGIC_TYPE_BLK_BOTH_UR_iZDIR               316
#define LOGIC_TYPE_BLK_BOTH_UR_BLTR                317

#define LOGIC_TYPE_BLK_BOTH_UR_oBR                 321
#define LOGIC_TYPE_BLK_BOTH_UR_oDR                 322
#define LOGIC_TYPE_BLK_BOTH_UR_CNR                 323
#define LOGIC_TYPE_BLK_BOTH_UR_oZR                 324
#define LOGIC_TYPE_BLK_BOTH_UR_iRR                 325
#define LOGIC_TYPE_BLK_BOTH_UR_iRDir               326

#define LOGIC_TYPE_BLK_BOTH_UR_OUT                 327    // 폐색 출력 시간
#define LOGIC_TYPE_BLK_BOTH_UR_OUT_CNR             328    // 폐색 취소 출력 시간

#define LOGIC_TYPE_BLK_BOTH_UR_OFF_YY              331
#define LOGIC_TYPE_BLK_BOTH_UR_OFF_Y               332
#define LOGIC_TYPE_BLK_BOTH_UR_OFF_YG              333
//#define LOGIC_TYPE_BLK_BOTH_UR_OFF_oRR             334
//#define LOGIC_TYPE_BLK_BOTH_UR_OFF_iZR             335
//#define LOGIC_TYPE_BLK_BOTH_UR_OFF_iZDIR           336
#define LOGIC_TYPE_BLK_BOTH_UR_OFF_BLTR            337

#define LOGIC_TYPE_BLK_BOTH_UR_OFF_oBR             341
#define LOGIC_TYPE_BLK_BOTH_UR_OFF_oDR             342
//#define LOGIC_TYPE_BLK_BOTH_UR_OFF_CNR             343
//#define LOGIC_TYPE_BLK_BOTH_UR_OFF_oZR             344
//#define LOGIC_TYPE_BLK_BOTH_UR_OFF_iRR             345
//#define LOGIC_TYPE_BLK_BOTH_UR_OFF_iRDir           346

#define LOGIC_TYPE_BLK_BOTH_PB_IMPOSSIBLE          351
#define LOGIC_TYPE_BLK_BOTH_PB_SUCCESS             352
#define LOGIC_TYPE_BLK_BOTH_PB_FAILURE             353

//----------------------------------------------------
// 통표 폐색
//#define LOGIC_TYPE_BLK_TONGPTO_ARR_GRAY            371
#define LOGIC_TYPE_BLK_TONGPTO_ARR_YELLOW          372
#define LOGIC_TYPE_BLK_TONGPTO_ARR_YF              373
#define LOGIC_TYPE_BLK_TONGPTO_ARR_RED             374
#define LOGIC_TYPE_BLK_TONGPTO_ARR_RF              375

//#define LOGIC_TYPE_BLK_TONGPTO_DEP_GRAY            376
#define LOGIC_TYPE_BLK_TONGPTO_DEP_YELLOW          377
#define LOGIC_TYPE_BLK_TONGPTO_DEP_YF              378
#define LOGIC_TYPE_BLK_TONGPTO_DEP_RED             379
#define LOGIC_TYPE_BLK_TONGPTO_DEP_RF              380

//----------------------------------------------------
// 삼각선 폐색
//#define LOGIC_TYPE_BLK_TRI_ARR_GRAY                391
#define LOGIC_TYPE_BLK_TR_ARR_YELLOW               392
#define LOGIC_TYPE_BLK_TR_ARR_YF                   393
#define LOGIC_TYPE_BLK_TR_ARR_RED                  394
#define LOGIC_TYPE_BLK_TR_ARR_RF                   395

//#define LOGIC_TYPE_BLK_TR_DEP_GRAY                 396
#define LOGIC_TYPE_BLK_TR_DEP_YELLOW               397
#define LOGIC_TYPE_BLK_TR_DEP_YF                   398
#define LOGIC_TYPE_BLK_TR_DEP_RED                  399
#define LOGIC_TYPE_BLK_TR_DEP_RF                   400

//----------------------------------------------------
// 청량리
//#define LOGIC_TYPE_BLK_CRRI_ARR_GRAY               411
#define LOGIC_TYPE_BLK_CRRI_ARR_YELLOW             412
#define LOGIC_TYPE_BLK_CRRI_ARR_YF                 413
#define LOGIC_TYPE_BLK_CRRI_ARR_RED                414
#define LOGIC_TYPE_BLK_CRRI_ARR_RF                 415

//#define LOGIC_TYPE_BLK_CRRI_DEP_GRAY               416
#define LOGIC_TYPE_BLK_CRRI_DEP_YELLOW             417
#define LOGIC_TYPE_BLK_CRRI_DEP_YF                 418
#define LOGIC_TYPE_BLK_CRRI_DEP_RED                419
#define LOGIC_TYPE_BLK_CRRI_DEP_RF                 420

//----------------------------------------------------
// 고속선 폐색
#define LOGIC_TYPE_BLK_EXPRESS_INPUT_YY            511
#define LOGIC_TYPE_BLK_EXPRESS_INPUT_Y             512
#define LOGIC_TYPE_BLK_EXPRESS_INPUT_YG            513

#define LOGIC_TYPE_BLK_EXPRESS_OUTPUT_CNR          516

#define LOGIC_TYPE_BLK_EXPRESS_UR_CNR              521    // 폐색 취소 출력 시간

#define LOGIC_TYPE_BLK_EXPRESS_UR_YY               531
#define LOGIC_TYPE_BLK_EXPRESS_UR_Y                532
#define LOGIC_TYPE_BLK_EXPRESS_UR_YG               533

#define LOGIC_TYPE_BLK_EXPRESS_UR_OFF_YY           536
#define LOGIC_TYPE_BLK_EXPRESS_UR_OFF_Y            537
#define LOGIC_TYPE_BLK_EXPRESS_UR_OFF_YG           538

//#define LOGIC_TYPE_BLK_EXPRESS_ARR_GRAY          541
#define LOGIC_TYPE_BLK_EXPRESS_ARR_YELLOW          542
#define LOGIC_TYPE_BLK_EXPRESS_ARR_RED             543

//#define LOGIC_TYPE_BLK_EXPRESS_DEP_GRAY          546
#define LOGIC_TYPE_BLK_EXPRESS_DEP_YELLOW          547
#define LOGIC_TYPE_BLK_EXPRESS_DEP_RED             548


//---------------------------------------------------------------
// 건널목 (고장검지) (LogicVariable.LogicKind = LOGIC_KIND_LEVEL_CROSS) : _T('C')
//---------------------------------------------------------------
#define LOGIC_TYPE_LCS_FAIL                        15

#define LOGIC_TYPE_LCS_INPUT                       21
#define LOGIC_TYPE_LCS_OUTPUT                      22

#define LOGIC_TYPE_LCS_UR_ON                       31
#define LOGIC_TYPE_LCS_UR_OFF                      32


//---------------------------------------------------------------
// 히터 (LogicVariable.LogicKind = LOGIC_KIND_HEAT) : _T('h')
//---------------------------------------------------------------
#define LOGIC_TYPE_HEATER_CMD_PB                   11
#define LOGIC_TYPE_HEATER_CMD_CB                   12

#define LOGIC_TYPE_HEATER_PB                       21
#define LOGIC_TYPE_HEATER_CB                       22

#define LOGIC_TYPE_HEATER_OUTPUT                   31   // 히터/O (출력)

#define LOGIC_TYPE_HEATER_INPUT                    36   // 히터/I
#define LOGIC_TYPE_HEATER_INPUT_FAIL               37   // 히터FAIL/I

#define LOGIC_TYPE_HEATER_INPUT_MESSAGE            41   // 히터MSG/I  (41 ~ 50)

//#define LOGIC_TYPE_HEATER_INPUT_UR                 51   // 히터/UR
//#define LOGIC_TYPE_HEATER_INPUT_UR_OFF             52   // 히터/UR-OFF
#define LOGIC_TYPE_HEATER_OUTPUT_UR_OFF            53   // 히터/출력UR-OFF
#define LOGIC_TYPE_HEATER_OUTPUT_CB_UR_OFF         54   // 히터/차단UR-OFF
//#define LOGIC_TYPE_HEATER_INPUT_ON_CHKUR           55   // 히터/ONUR-CHK

#define LOGIC_TYPE_HEATER_FAIL_UR_OFF              56   // 히터/장애UR-OFF

#define LOGIC_TYPE_HEATER_ACT                      61
#define LOGIC_TYPE_HEATER_FAIL                     62
#define LOGIC_TYPE_HEATER_ERROR                    63
#define LOGIC_TYPE_HEATER_NOT_EXPECTED_INPUT       64
#define LOGIC_TYPE_HEATER_STANDBY                  65

//#define LOGIC_TYPE_HEATER_MESSAGE_IN               70
#define LOGIC_TYPE_HEATER_MESSAGE                  71   // 알람 메시지 (71 ~ 80)


//---------------------------------------------------------------
// 제어 건널목 (LogicVariable.LogicKind = LOGIC_KIND_LEVEL_CONTROL) : _T('c')
//---------------------------------------------------------------
//#define LOGIC_TYPE_LC_CTRL_CMD_PB                  11
//#define LOGIC_TYPE_LC_CTRL_CMD_CB                  12
//#define LOGIC_TYPE_LC_CTRL_PB                      13
//#define LOGIC_TYPE_LC_CTRL_CB                      14

#define LOGIC_TYPE_LC_CTRL_FAIL                    15

#define LOGIC_TYPE_LC_CTRL_INPUT                   21
#define LOGIC_TYPE_LC_CTRL_OUTPUT                  22

#define LOGIC_TYPE_LC_CTRL_UR_ON                   31
#define LOGIC_TYPE_LC_CTRL_UR_OFF                  32

#define LOGIC_TYPE_LC_CTRL_NOT_EXPEC_INPUT         41    // 부정입력 (한 개 이상 부정입력)
#define LOGIC_TYPE_LC_CTRL_NOT_EXPEC_OUTPUT        42    // 부정출력 (한 개 이상 부정출력)

#define LOGIC_TYPE_LC_CTRL_NOT_EXPECTED_INPUT      43    // 부정입력 (각각에 대한 부정입력)
#define LOGIC_TYPE_LC_CTRL_NOT_EXPECTED_OUTPUT     44    // 부정출력 (각각에 대한 부정출력)

#define LOGIC_TYPE_LC_CTRL_INPUT_UR                51    // 입력 여자UR
#define LOGIC_TYPE_LC_CTRL_INPUT_OFF_UR            52    // 입력 낙하UR
#define LOGIC_TYPE_LC_CTRL_OUTPUT_UR               53    // 출력UR
#define LOGIC_TYPE_LC_CTRL_OUTPUT_OFF_UR           54    // 출력차단UR

#define LOGIC_TYPE_LC_CTRL_NOT_INPUT_FOR_OUT       61    // 동작 불능 (출력에 대한 입력없음)

//---------------------------------------------------------------
// 절연구간 (LogicVariable.LogicKind = LOGIC_KIND_DEAD_SECTION) : _T('D')
//---------------------------------------------------------------
#define LOGIC_TYPE_DEAD_SECTION_FAIL               15

#define LOGIC_TYPE_DEAD_SECTION_INPUT              21
#define LOGIC_TYPE_DEAD_SECTION_OUTPUT             22

#define LOGIC_TYPE_DEAD_SECTION_UR_ON              31
#define LOGIC_TYPE_DEAD_SECTION_UR_OFF             32

#define LOGIC_TYPE_DEAD_SECTION_UR_STAT            41     // 운영 Input
#define LOGIC_TYPE_DEAD_SECTION_UR_SEC1            42     // 1계 Input
#define LOGIC_TYPE_DEAD_SECTION_UR_SEC2            43     // 2계 Input

#define LOGIC_TYPE_DEAD_SECTION_RECOVERY_SEC1      51     // 1계 정상
#define LOGIC_TYPE_DEAD_SECTION_RECOVERY_SEC2      52     // 2계 정상
#define LOGIC_TYPE_DEAD_SECTION_ACTIVE_SEC1        53     // 1계 Main
#define LOGIC_TYPE_DEAD_SECTION_ACTIVE_SEC2        54     // 2계 Main

//---------------------------------------------------------------
// 지장물 (LogicVariable.LogicKind = LOGIC_KIND_FALL_LOCK) : _T('J')
//---------------------------------------------------------------
#define LOGIC_TYPE_FALL_LOCK_CMD_PB                11

#define LOGIC_TYPE_FALL_LOCK_FAIL                  15

#define LOGIC_TYPE_FALL_LOCK_INPUT                 21
#define LOGIC_TYPE_FALL_LOCK_OUTPUT                22

#define LOGIC_TYPE_FALL_LOCK_UR_ON                 31
#define LOGIC_TYPE_FALL_LOCK_UR_OFF                32

#define LOGIC_TYPE_FALL_LOCK_UR_FALL               41     // 낙석 Input - 지장물검지, 장애(적색)
#define LOGIC_TYPE_FALL_LOCK_UR_PROC               42     // 보호 Input - 보호해제 취급   (적색)

// 규격 변경 이전
#define LOGIC_TYPE_FALL_LOCK_RECOVERY_FALL         51     // 낙석 복구
#define LOGIC_TYPE_FALL_LOCK_RECOVERY_PROC         52     // 보호 복구
#define LOGIC_TYPE_FALL_LOCK_RELEASE               53     // 헤제

// 규격 변경 이후
#define LOGIC_TYPE_FALL_LOCK_DETECT                61     // 지장물
#define LOGIC_TYPE_FALL_LOCK_RELEASE_PROC          62     // 보호해제
#define LOGIC_TYPE_FALL_LOCK_DETECT_WARN           63     // 지장물경보
#define LOGIC_TYPE_FALL_LOCK_DETECT_WARN_STOP      64     // 경보확인

//---------------------------------------------------------------
// ETC Fault (LogicVariable.LogicKind = LOGIC_KIND_ETC_FAULT) : _T('F')
//---------------------------------------------------------------
#define LOGIC_TYPE_ETC_FAULT_INPUT                 21

#define LOGIC_TYPE_ETC_FAULT_UR_ON                 31
#define LOGIC_TYPE_ETC_FAULT_UR_OFF                32

#define LOGIC_TYPE_ETC_FAULT_ON                    41     // 입력 여자

#define LOGIC_TYPE_ETC_DEP_INDICATOR_LIGHT_INPUT   51     // 출발대용표시등 입력(/I)
#define LOGIC_TYPE_ETC_DEP_INDICATOR_LIGHT_ON      52     // 출발대용표시등 입력 여자(/ON)
#define LOGIC_TYPE_ETC_DEP_INDICATOR_LIGHT_ACT     53     // 출발대용표시등 동작(/ACT)

//---------------------------------------------------------------
// STL (LogicVariable.LogicKind = LOGIC_KIND_STL_INFO) : _T('K')
//---------------------------------------------------------------
#define LOGIC_TYPE_STL_CMD_PB                      11
#define LOGIC_TYPE_STL_CMD_CB                      12
#define LOGIC_TYPE_STL_PB                          13
#define LOGIC_TYPE_STL_CB                          14

#define LOGIC_TYPE_STL_INPUT                       21
#define LOGIC_TYPE_STL_OUTPUT                      22

#define LOGIC_TYPE_STL_UR_ON                       31
#define LOGIC_TYPE_STL_UR_OFF                      32

#define LOGIC_TYPE_STL_ON                          41     // ON (항색점등)
#define LOGIC_TYPE_STL_FALSH                       42     // 점멸

//---------------------------------------------------------------
// CPT (LogicVariable.LogicKind = LOGIC_KIND_CPT) : _T('H')
//---------------------------------------------------------------
#define LOGIC_TYPE_CPT_INPUT                       21

#define LOGIC_TYPE_CPT_UR_ON                       31
#define LOGIC_TYPE_CPT_UR_OFF                      32

#define LOGIC_TYPE_CPT_ON                          41
#define LOGIC_TYPE_CPT_RED                         42
#define LOGIC_TYPE_CPT_FALSH_RED                   43

//---------------------------------------------------------------
// Dwell (LogicVariable.LogicKind = LOGIC_KIND_DWL_INFO) : _T('W')
//---------------------------------------------------------------
#define LOGIC_TYPE_DWL_CMD_PB                      11
#define LOGIC_TYPE_DWL_CMD_CB                      12
#define LOGIC_TYPE_DWL_PB                          13
#define LOGIC_TYPE_DWL_CB                          14

#define LOGIC_TYPE_DWL_INPUT                       21
#define LOGIC_TYPE_DWL_OUTPUT                      22

#define LOGIC_TYPE_DWL_UR_ON                       31
#define LOGIC_TYPE_DWL_UR_OFF                      32

#define LOGIC_TYPE_DWL_ON                          41
#define LOGIC_TYPE_DWL_FALSH                       42


//---------------------------------------------------------------
// Alarm (LogicVariable.LogicKind = LOGIC_KIND_ALM_INFO) : _T('A')
//---------------------------------------------------------------
// [진로이름] [궤도이름] 궤도 점유로 접근쇄정 중지
#define LOGIC_TYPE_ALM_RTEAPP_STOP_TRACK           41    // ALM[진로]+궤도



//===============================================================
// LOGIC_VARIABLE.TimerKind 값에 대한 정의
// 각각에 delay 시간 설정 해야됨
#define LOGIC_TIMER_KIND_DELAY_ON                  2      // UR-ON  (낙하상태에서 조건 만족시 시간동안 delay 후 여자)
#define LOGIC_TIMER_KIND_DELAY_OFF                 3      // UR-OFF (조건 만족하는 동안 여자 후
                                                          //         조건 만족하지 않을때 delay 후 낙하)
#define LOGIC_TIMER_KIND_DELAY_ON_OFF              4      // UR-CHK = UR-ON + UR-OFF
                                                          //      여자 조건일 때 : delay 후 여자
                                                          //      낙하 조건일 때 : delay 후 낙하
#define LOGIC_TIMER_KIND_OSCILLATOR                5      // 조건이 만족하는 동안 dleay 시간에 대한 여자와 낙하 반복
#define LOGIC_TIMER_KIND_TRIGER                    6      // 조건이 만족할 때 여자동작을 시작하고, delay 시간 후 낙하

#if 0
     +---------------------------------------------+
     |                                             |
-----+                                             +-------------- <CONDITION>


       t       +-----------------------------------+
      <=======>|                                   |
---------------+                                   +-------------- [DELAYED_ON]

     +------------------------------------------------------+
     |                                              <======>|
-----+                                               t      +----- [DELAYED_OFF]

       t       +--------------------------------------------+
      <=======>|                                    <======>|
---------------+                                     t      +----- [DELAYED_ON_OFF]

     +---------+ t       +---------+ t       +-----+
     |<=======>|<=======>|<=======>|<=======>|<====|
-----+ t       +---------+ t       +---------+     +-------------- [OSCILLATOR]

     +---------+
     |<=======>|
-----+ t       +-------------------------------------------------- [TRIGGER (rising edge)]
#endif

//===============================================================
// LOGIC_VARIABLE.TimeValue 값에 대한 정의
// Time Value (단위 : ms)

//---------------------------------------------------------------
// 시간보정 : 보정시간을 감소하여 설정

// 취급시 취급변수(CMD) 1.5~2초 동안 여자후 낙하됨
// 취급에 의한 출력유지후 차단시(LOGIC_TIMER_KIND_DELAY_OFF) 보정시간 감소하여 설정
#define LOGIC_TIMER_VALUE_ADJUST_COMMAND           1500u

// 궤도 시간 보정

// 선로전환기 시간 보정
#define LOGIC_TIMER_VALUE_ADJUST_SWITCH_OUT        2000u

// 신호기 시간 보정

// 진로 시간 보정
#define LOGIC_TIMER_VALUE_ADJUST_ROUTE_APP         2000u
#define LOGIC_TIMER_VALUE_ADJUST_ROUTE_EF          4000u

// 폐색 시간 보정
#define LOGIC_TIMER_VALUE_ADJUST_BLOCK_DEALY_OFF   200u

//---------------------------------------------------------------
#define LOGIC_TIMER_VALUE_EIS_INIT_CHECK_UR_ON     5000u

#define LOGIC_TIMER_VALUE_SHUNTOFF_OUT_UR_OFF      6000u    // 입환소등/UR-OFF
#define LOGIC_TIMER_VALUE_SHUNTOFF_NOT_UR_OFF      3000u    // 입환소등/동작불능

//---------------------------------------------------------------
// 궤도 복구
#define LOGIC_TIMER_VALUE_TRACK_RECOVERY_0         0u
#define LOGIC_TIMER_VALUE_TRACK_RECOVERY_500       500u
#define LOGIC_TIMER_VALUE_TRACK_RECOVERY_1000      1000u
#define LOGIC_TIMER_VALUE_TRACK_RECOVERY_1200      1200u
#define LOGIC_TIMER_VALUE_TRACK_RECOVERY_1500      1500u
#define LOGIC_TIMER_VALUE_TRACK_RECOVERY_1800      1800u
#define LOGIC_TIMER_VALUE_TRACK_RECOVERY_2000      2000u
#define LOGIC_TIMER_VALUE_TRACK_RECOVERY_2500      2500u
#define LOGIC_TIMER_VALUE_TRACK_RECOVERY_3000      3000u

// 궤도 낙하
#define LOGIC_TIMER_VALUE_TRACK_OCCUPY_0           0u
#define LOGIC_TIMER_VALUE_TRACK_OCCUPY_200         200u
#define LOGIC_TIMER_VALUE_TRACK_OCCUPY_500         500u
#define LOGIC_TIMER_VALUE_TRACK_OCCUPY_1000        1000u
#define LOGIC_TIMER_VALUE_TRACK_OCCUPY_1200        1200u
#define LOGIC_TIMER_VALUE_TRACK_OCCUPY_1500        1500u
#define LOGIC_TIMER_VALUE_TRACK_OCCUPY_2000        2000u
#define LOGIC_TIMER_VALUE_TRACK_OCCUPY_2500        2500u
#define LOGIC_TIMER_VALUE_TRACK_OCCUPY_3000        3000u

#define LOGIC_TIMER_VALUE_TRACK_UR_OFF             400u

#define LOGIC_TIMER_VALUE_TRACK_RELEASE_UR_OFF     2000u    // 비상해정 취급 후 Timer (for Message)
#define LOGIC_TIMER_VALUE_TRACK_ABNOR_UR_OFF       2000u    // 이상복구 해정 취급 후 Timer

//#define LOGIC_TIMER_VALUE_TRACK_ROUTE_LOCK_UR_OFF  200u     // 궤도/R쇄정(진로)


//---------------------------------------------------------------
// 선로전환기
#define LOGIC_TIMER_VALUE_SWITCH_UR_ON             400u

#define LOGIC_TIMER_VALUE_SWITCH_ILKPB             2000u   // 연동PB

#define LOGIC_TIMER_VALUE_SWITCH_UR_WLR            12000u  // WLR 출력시간
//#define LOGIC_TIMER_VALUE_SWITCH_UR_WR             3000u   // WR-N, WR-R 출력시간
//#define LOGIC_TIMER_VALUE_SWITCH_UR_WR             1000u   // WR-N, WR-R 출력시간 (WR-N/UR-OFF, WR-R/UR-OFF)
#define LOGIC_TIMER_VALUE_SWITCH_UR_WR             1200u   // WR-N, WR-R 출력시간 (WR-N/UR-OFF, WR-R/UR-OFF)

#define LOGIC_TIMER_VALUE_SWITCH_UR_WLR_CHK        3000u   // WLR 동작불능 판단시간        (WLR/UR-CHK)
#define LOGIC_TIMER_VALUE_SWITCH_UR_WR_CHK         1000u   // WR-N, WR-R 동작불능 판단시간 (WR-N/UR-CHK, WR-R/UR-CHK)

#define LOGIC_TIMER_VALUE_SWITCH_UR_OOC_CHK        7000u   // 불일치 판단시간

#define LOGIC_TIMER_VALUE_SWITCH_UR_MOV_CHK        1000u   // 전환완료 Timer (전환중으로 취급불가 설정을 위한 Timer)


#define LOGIC_TIMER_VALUE_SWITCH_ILK_MAX           500u
#define LOGIC_TIMER_VALUE_SWITCH_ILK_MIN           250u
#define LOGIC_TIMER_VALUE_SWITCH_UR_ILK            350u    // 선로전환기 순차 출력 시간 간격 (/UR연동)

#define LOGIC_TIMER_VALUE_SWITCH_SET_MOVE          3000u
//#define LOGIC_TIMER_VALUE_SWITCH_SET_MOVE          1000u
#define LOGIC_TIMER_VALUE_SWITCH_SET_UR_OFF        2000u

#define LOGIC_TIMER_VALUE_SWITCH_UR_DELAY_OFF_WLR  2000u

//#define LOGIC_TIMER_VALUE_SWITCH_WLR_NOT_EXPECTED  1000u   // WLR 부정입력 체크 시간

//#define LOGIC_TIMER_VALUE_SWITCH_NOT_EXPEDTED_IN   600u    // WLR 부정입력UR Timer
#define LOGIC_TIMER_VALUE_SWITCH_NOT_EXPEDTED_IN   900u    // WLR 부정입력UR Timer (WLR/부정입력)

//---------------------------------------------------------------
// 신호기
#define LOGIC_TIMER_VALUE_SIG_SPB_UR_OFF           2000u

#define LOGIC_TIMER_VALUE_SIG_OUT_UR_ON            600u
#define LOGIC_TIMER_VALUE_SIG_OUT_UR_OFF           2000u
//#define LOGIC_TIMER_VALUE_SIG_HR_UR_ON             600u
//#define LOGIC_TIMER_VALUE_SIG_HR_UR_OFF            2000u
//#define LOGIC_TIMER_VALUE_SIG_YR_UR_ON             600u
//#define LOGIC_TIMER_VALUE_SIG_YR_UR_OFF            2000u
//#define LOGIC_TIMER_VALUE_SIG_GR_UR_ON             600u
//#define LOGIC_TIMER_VALUE_SIG_GR_UR_OFF            2000u


//#define LOGIC_TIMER_VALUE_SIG_IN_HR_UR_ON          1000u    // HR, YR, GR (/UR)
//#define LOGIC_TIMER_VALUE_SIG_IN_SHR_UR_ON         1000u    // 유도
#define LOGIC_TIMER_VALUE_SIG_IN_HR_UR_ON          600u    // HR, YR, GR (/UR)
#define LOGIC_TIMER_VALUE_SIG_IN_SHR_UR_ON         600u    // 유도

//#define LOGIC_TIMER_VALUE_SIG_IN_HR_UR_OFF         1000u    // HR, YR, GR (/UR-OFF)
//#define LOGIC_TIMER_VALUE_SIG_IN_SHR_UR_OFF        1000u    // 유도
#define LOGIC_TIMER_VALUE_SIG_IN_HR_UR_OFF         600u    // HR, YR, GR (/UR-OFF)
#define LOGIC_TIMER_VALUE_SIG_IN_SHR_UR_OFF        600u    // 유도

#define LOGIC_TIMER_VALUE_SIG_IN_INDICATOR_UR_ON   600u    // 진로선볋등 (/UR)

//#define LOGIC_TIMER_VALUE_SIG_NOT_EXPEDTED_IN      600u     // HR,YR,GR 부정입력UR Timer
#define LOGIC_TIMER_VALUE_SIG_NOT_EXPEDTED_IN      900u     // HR,YR,GR 부정입력UR Timer

#define LOGIC_TIMER_VALUE_SIG_OUT_WAIT_UR_ON       3000u    // 신호 출력 후 입력 여자 대기 Time (UR출력)
// 시뮬에서 경계 출력 후 LMR입력까지 2.5초 이상 소요되어 사용하지 않는다
//#define LOGIC_TIMER_VALUE_SIG_OUT_WAIT_UR_ON       2500u    // 신호 출력 후 입력 여자 대기 Time (UR출력)

#define LOGIC_TIMER_VALUE_SIG_CANCEL_WAIT_UR_OFF   2000u    // 신호 취소 후 입력 낙하 대기 Time (UROFF)

#define LOGIC_TIMER_VALUE_SIG_OUT_UR_STOP_CHK      700u     // 정지(R) 출력 후 진로/FAIL 체크를 위한 Time

#define LOGIC_TIMER_VALUE_SIG_OUT_APSECT_UR        2000u

#define LOGIC_TIMER_VALUE_SIG_OUTIN_OFF_WAIT_UR    1200u

#define LOGIC_TIMER_VALUE_SIG_ONOFF                1000u    // 현시 후 정지

//---------------------------------------------------------------
// LMR
//#define LOGIC_TIMER_VALUE_SIG_IN_MLMR_UR_ON        1000u    // 주심
//#define LOGIC_TIMER_VALUE_SIG_IN_ALMR_UR_ON        1000u    // 부심
#define LOGIC_TIMER_VALUE_SIG_IN_MLMR_UR_ON        600u    // 주심
#define LOGIC_TIMER_VALUE_SIG_IN_ALMR_UR_ON        600u    // 부심

//#define LOGIC_TIMER_VALUE_SIG_IN_MLMR_UR_OFF       1000u    // 주심LMR m/UR-OFF
//#define LOGIC_TIMER_VALUE_SIG_IN_ALMR_UR_OFF       1000u    // 부심LMR a/UR-OFF
#define LOGIC_TIMER_VALUE_SIG_IN_MLMR_UR_OFF       600u    // 주심LMR m/UR-OFF
#define LOGIC_TIMER_VALUE_SIG_IN_ALMR_UR_OFF       600u    // 부심LMR a/UR-OFF

#define LOGIC_TIMER_VALUE_SIG_IN_LMR_UR_ONCHK      1000u    // LMR/UR-CHK

#define LOGIC_TIMER_VALUE_SIG_IN_MLMR_UR_NOT_IN    2500u    // LMR/부정입력


#define LOGIC_TIMER_VALUE_SIG_IN_CLMR_UR_ON        400u     // 유도
#define LOGIC_TIMER_VALUE_SIG_IN_CLMR_UR_OFF       400u

//#define LOGIC_TIMER_VALUE_SIG_FAIL_LMR_UR          500u      // LMR/FAIL-UR
#define LOGIC_TIMER_VALUE_SIG_FAIL_LMR_UR          700u      // LMR/FAIL-UR (G, Y, R, Y1)

//#define LOGIC_TIMER_VALUE_SIG_FAIL_MLMR_CHK_UR     300u      // FAIL-UR (Gm, Ym, Rm, Y1m)
#define LOGIC_TIMER_VALUE_SIG_FAIL_MLMR_CHK_UR     700u      // FAIL-UR (Gm, Ym, Rm, Y1m)

// 중계신호기
#define LOGIC_TIMER_VALUE_SIG_REPEAT_UR_ON         400u      // GLMR/UR-ON, CLMR/UR-ON

// 폐색신호기
#define LOGIC_TIMER_VALUE_SIG_BLOCK_UR_ON          400u


//---------------------------------------------------------------
// 진로
#define LOGIC_TIMER_VALUE_ROUTE_CANCEL_UR_ON       600u    // 진로 취소

#define LOGIC_TIMER_VALUE_ROUTE_APP_UR_ON          400u
#define LOGIC_TIMER_VALUE_ROUTE_APP_UR_OFF         1000u
#define LOGIC_TIMER_VALUE_ROUTE_APP_UR_END         2000u

#define LOGIC_TIMER_VALUE_ROUTE_UHYO_UR_ON         400u
#define LOGIC_TIMER_VALUE_ROUTE_UHYO_UR_OFF        1000u
#define LOGIC_TIMER_VALUE_ROUTE_UHYO_UR_END        2000u

#define LOGIC_TIMER_VALUE_ROUTE_RELEASE_TRACK      600u    // 진로/T해정

//---------------------------------------------------------------
// 폐색
//#define LOGIC_TIMER_VALUE_BLOCK_

#define LOGIC_TIMER_VALUE_BLOCK_OUTPUT             6000u     // 폐색 출력 최대 시간 (ms)
#define LOGIC_TIMER_VALUE_BLOCK_OUTPUT_BOTH1       6000u     // 역방향 장내 폐색 출력 최대 시간 (ms)
#define LOGIC_TIMER_VALUE_BLOCK_OUTPUT_BOTH2       12000u    // 역방향 출발 폐색 출력 최대 시간 (ms)

//#define LOGIC_TIMER_VALUE_BLOCK_DEALY              2000u     // 폐색 입력 변경 후 입력처리 대기시간 (ms)

#define LOGIC_TIMER_VALUE_BLOCK_DEALY_ON           2000u     // 폐색/UR
//#define LOGIC_TIMER_VALUE_BLOCK_DEALY_ON           1200u     // 폐색/UR
//#define LOGIC_TIMER_VALUE_BLOCK_DEALY_OFF          2000u     // 폐색/UR-OFF
#define LOGIC_TIMER_VALUE_BLOCK_DEALY_OFF          1200u     // 폐색/UR-OFF (BR <-> DR 변경시간)

//#define LOGIC_TIMER_VALUE_BLOCK_CANCEL             6000u     // 폐색 취소 출력 최대 시간 (ms) - 단선자동, 의왕
#define LOGIC_TIMER_VALUE_BLOCK_CANCEL             2000u     // 폐색 취소 출력 최대 시간 (ms) - 단선자동, 의왕
#define LOGIC_TIMER_VALUE_BLOCK_CANCEL_BOTH1       6000u     // 역방향 장내 폐색 취소 출력 최대 시간 (ms)
#define LOGIC_TIMER_VALUE_BLOCK_CANCEL_EXP         30000u    // 고속선 폐색 취소 출력 최대 시간 (ms)


//---------------------------------------------------------------
// 고장정보 (건널목)
//#define LOGIC_TIMER_VALUE_FAULT_


//---------------------------------------------------------------
// 전차선 절연구간
#define LOGIC_TIMER_VALUE_DEAD_SECTION_STAT        200u
#define LOGIC_TIMER_VALUE_DEAD_SECTION_SEC1        200u
#define LOGIC_TIMER_VALUE_DEAD_SECTION_SEC2        200u


//---------------------------------------------------------------
// 지장물 검지
#define LOGIC_TIMER_VALUE_FALL_LOCK_FALL           200u
#define LOGIC_TIMER_VALUE_FALL_LOCK_PROC           200u

//---------------------------------------------------------------
// 기타 고장

//---------------------------------------------------------------
// STL

//---------------------------------------------------------------
// 열차진입방지(CPT)
//#define LOGIC_TIMER_VALUE_CPT_

//---------------------------------------------------------------
// 히터
//#define LOGIC_TIMER_VALUE_HEAT_

#define LOGIC_TIMER_VALUE_HEAT_INPUT_WAIT           3000u    // 히터 출력     후 입력 여자 Wait Time
#define LOGIC_TIMER_VALUE_HEAT_INPUT_OFF_WAIT       3000u    // 히터 출력차단 후 입력 낙하 Wait Time
#define LOGIC_TIMER_VALUE_HEAT_INPUT_ON_CHK         2000u    // 히터 가동 동작불능 체크 Time

#define LOGIC_TIMER_VALUE_HEAT_FAIL_WAIT            7000u    // 히터 출력 후 장애판단까지 Wait Time (분선반 Timer 계전기)

//---------------------------------------------------------------
// DWELL INFO

//---------------------------------------------------------------
// 제어건널목
//#define LOGIC_TIMER_VALUE_LC_CTRL_

#define LOGIC_TIMER_VALUE_LC_CTRL_ON_UR             2000u
#define LOGIC_TIMER_VALUE_LC_CTRL_OFF_UR            2000u    // 낙하UR

#define LOGIC_TIMER_VALUE_LC_CTRL_OUT_UR            2000u    // 출력UR
#define LOGIC_TIMER_VALUE_LC_CTRL_OUT_OFF_UR        3000u    // 출력차단UR (부정입력 처리시간)

//---------------------------------------------------------------

#if defined (_WIN32)
#  pragma pack (pop)
#endif


#endif  /* #ifndef __LOGICDEFINE__h__ */
