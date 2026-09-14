#include "stdafx.h"
#pragma once
#ifndef __LOGICTYPEDEF__h__
#define __LOGICTYPEDEF__h__



#if defined (_WIN32)
#  pragma pack (push)
#  pragma pack (1)
#endif


//===========================================================================
#define MAX_TRACK_ERROR_CONDITION                      20
#define MAX_SWITCH_ERROR_CONDITION                     10
#define MAX_TRACK_FREE_COUNT                           5

//#define MAX_CNT_HEATER                                 20

//===========================================================================
// 궤도 장애 판단에 대한 파일(역이름_track_error.txt) Read 후 메모리 저장
typedef struct
{
	char   szTrkName[20];
	UCHAR  ChkTrackNo;     // 장애 판단에 대한 궤도 (DB의 Table Index)

	UINT   nCntCondTrk;    // CONDITION 에 대한 수

	struct {
		char   szOccTrkNm[20];
		UCHAR  OccTrackNo;     // 인접 점유 체크 궤도 (DB의 Table Index)

		UINT   nCntCondSwh;    // SWH 에 대한 수

		struct {
			char   szSwhName[20];
			UCHAR  SwitchNo;   // 선로전환기 (DB의 Table Index)
			UCHAR  SwhDir;     // 선로전환기 방향 (정위:1, 반위:2)
		} SWH[MAX_SWITCH_ERROR_CONDITION];

	} CONDITION[MAX_TRACK_ERROR_CONDITION];

	// CONDITION OK (OccTrack 낙하 && (SWH[0] OK 또는 SWH[1] OK 또는 SWH[2] OK......) 상태에서
	// 궤도(ChkTrackNo) 낙하시  정상 점유

} TRACK_ERROR_CHECK;

//-----------------------------------------------------------------------------
// CONDITION[].OccTrackNo 점유 상태에서 ChkTrackNo 복구시 정상 복구
// 궤도 정상 복구 판단
typedef struct
{
	char   szSwhName[20];
	UCHAR  SwitchNo;   // 선로전환기 (DB의 Table Index)
	UCHAR  SwhDir;     // 선로전환기 방향 (정위:1, 반위:2)
} SWH_COND;

typedef struct
{
	char   szTrkName[20];
	UCHAR  ChkTrackNo;     // 이상 복구 판단에 대한 궤도 (DB의 Table Index)

	UINT   nCntFreeTrk;

	struct {
		char   szFreeTrkNm[20];
		UCHAR  FreeTrackNo;     // 인접 복구 체크 궤도 (DB의 Table Index)
	} FREE_TRACK[MAX_TRACK_FREE_COUNT];


	UINT   nCntCondition;    // CONDITION 에 대한 수

	// CONDITION[0] : P23  23BTUR ETUR
	// CONDITION[0] : !P24 24TUR

	// CONDITION[0] : !P21 P22 21BTUR XTUR
	// CONDITION[1] : P21 !P22 21BTUR 4TUR
	// CONDITION[2] : P21  P22 4TUR 21ATUR
	struct {

		UINT   nCntCondTrk;    // TRACK 에 대한 수
		UINT   nCntCondSwh;    // SWH 에 대한 수

		struct {
			char   szSwhName[20];
			UCHAR  SwitchNo;   // 선로전환기 (DB의 Table Index)
			UCHAR  SwhDir;     // 선로전환기 방향 (정위:1, 반위:2)
		} SWH[MAX_SWITCH_ERROR_CONDITION];

		struct {
			char   szFreeTrkNm[20];
			UCHAR  FreeTrackNo;     // 인접 복구 체크 궤도 (DB의 Table Index)
		} TRACK[MAX_TRACK_FREE_COUNT];    // FREE_TRACK[]과 동일 내용

	} CONDITION[MAX_TRACK_ERROR_CONDITION];

	// CONDITION (TRACK[0].FreeTrackNo 복구 && TRACK[1].FreeTrackNo 복구 && .... 복구) 상태에서
	// 궤도(ChkTrackNo) 복구시  이상 복구

} TRACK_ABRECOVERY_CHECK;


//===========================================================================
typedef struct
{
	UINT    nKindNo;
	char    szFileName[256];
} LOGIC_KIND;    // 각 로직에 대한 기능별 파일 이름 설정

/*LOGIC_KIND        g_LogicKind[MAX_LOGIC_FILES] =
	{ { 0,                      _T("")                    },
	  { ID_LOGIC_NO_001_VRD,    IDS_LOGIC_NAME_001_VRD    },
	  { ID_LOGIC_NO_010_CMD,    IDS_LOGIC_NAME_010_CMD    },
	  { ID_LOGIC_NO_050_COMMON, IDS_LOGIC_NAME_050_COMMON },
	  { ID_LOGIC_NO_100_TRACK,  IDS_LOGIC_NAME_100_TRACK  },
	  { ID_LOGIC_NO_110_TRACK,  IDS_LOGIC_NAME_110_TRACK  },
	  { ID_LOGIC_NO_120_TRACK,  IDS_LOGIC_NAME_120_TRACK  },
	  { ID_LOGIC_NO_150_TRACK,  IDS_LOGIC_NAME_150_TRACK  },
	  { ID_LOGIC_NO_170_BLOCK,  IDS_LOGIC_NAME_170_BLOCK  },
	  { ID_LOGIC_NO_200_SWITCH, IDS_LOGIC_NAME_200_SWITCH },
	  { ID_LOGIC_NO_220_SWITCH, IDS_LOGIC_NAME_220_SWITCH },
	  { ID_LOGIC_NO_250_SWITCH, IDS_LOGIC_NAME_250_SWITCH },
	  { ID_LOGIC_NO_300_ROUTE,  IDS_LOGIC_NAME_300_ROUTE  },
	  { ID_LOGIC_NO_600_FAIL,   IDS_LOGIC_NAME_600_FAIL   },
	  { 0,                      _T("")                    }, };*/

	  //---------------------------------------------------------------------------
	  // 로직 변수 리스트
typedef struct
{
	unsigned short    nLogicIdx;         // 로직인덱스 번호 (0 ~ 65534)
	//  0  ~ 510  : reserved (연동장치 상태 등)
	// 511 ~ 8191 : 취급변수 (CMD)
	// 8292 ~ 65535 : IO, 상태 값, 제어 등
//char              szEquipName[20];   // 설비 이름
	char              szVarName[46];     // 로직변수 이름

	UINT              TimeValueDB;       // DB입력시 Time Value (단위: ms)

	UCHAR             Kind;              // 연동DB 값 : 'S', 'P', 'T', 'N',...
	UINT              TableIdx;          // 연동DB의 인덱스 번호
	UCHAR             BitNo;             // 연동DB의 BitNo

	struct {
		UCHAR    CardType;     // Card Type (1: IN, 2: OUT, 4:PDO)
		// 주의) DB에서 Card Type (1: IN, 2: OUT, 3:PDO)

		UCHAR    nRackNo;      // 랙번호 1~10
		UCHAR    nSlotNo;      // 슬롯번호 1~16
		//UCHAR    nCardNo;      // 카드번호 
		UCHAR    nPortNo;      // 포트번호 1~16
		UCHAR    attr;
		UCHAR    param;
	} CARDINFO;

	UCHAR   LogicKind;         // 로직 구분 : Input, Output, 궤도, 신호기 등 
	// _T('V') : VRD 속도코드제어장치VRD
	// _T('T') : 궤도
	// _T('P') : 선로전환기
	// _T('S') : 신호기
	// _T('L') : LMR
	// _T('R') : 진로
	// _T('N') : 역공통
	// _T('B') : 폐색
	// _T('C') : 건널목 (고장검지)
	// _T('c') : 건널목 (제어건널목)
	// _T('t') : 타역 궤도
	// _T('s') : 타역 신호기
	// _T('p') : 타역 선로전환기
	// _T('D') : 전차선 절연구간
	// _T('J') : 지장물
	// _T('K') : 출발반응등
	// _T('F') : 기타 고장
	// _T('W') : 소속역 정보
	// _T('H') : CPT
	// _T('h') : 히터
	// _T('E') : 연동장치 정보

	UINT    LogicType;         // 취급, 취소 등에 대한 구분
	// 1 : 입력 _T('I')
	// 2 : 출력 _T('O')
	// 4 : PDO  _T('P')
	//   궤  도   : UR, PrevUR, LOCK, OCC, FAIL, LSR, RSR
	// 신  호  기 : 
	// 선로전환기 : 

	UCHAR   TimerKind;
	// 2 : 낙하상태에서 조건 만족시 시간동안 delay 후 여자) ON
	// 3 : 여자상태에서 조건 만족시 시간동안 delay 후 낙하) OFF
	// 4 : 여자 조건일 때 delay 후 여자 ON-OFF
	//     낙하 조건일 때 delay 후 낙하 
	// 5 : 조건이 만족하는 동안 dleay 시간에 대한 여자와 낙하 반복 OSCI
	// 6 : 조건이 만족할 때 여자동작을 시작하고, delay 시간 후 낙하 TRIG

	UINT    TimeValue;         // Time Value (단위: ms) : 실제 동작시간

} LOGIC_VARIABLE;


//---------------------------------------------------------------------------
/*
# LDB 파일 구조
+----------------------------------------------+
| SymbolHead                                   |
+----------------------------------------------+
| Symbol_Information                           |
+----------------------------------------------+
| Symbol_Contents                              |
+----------------------------------------------+
*/

//typedef struct SymbolHead_t
//{
//	UINT    nSymbol_Count;
//	UINT    nCurIndex;
//} SymbolHead;
//
//
//typedef struct Symbol_Information_t
//{
//	UINT    nSymbol_Count;
//	UINT    Page_Max_Row;          // 최대 row
//	UINT    Page_Max_Col;          // 최대 col
//	UINT    Symbol_Max_Row;        // 비어있지 않은 심볼에 최대 row 
//	UINT    Symbol_Max_Col;        // 비어있지 않은 심볼에 최대 col 
//} Symbol_Information;
//
//typedef struct Symbol
//{
//	Gdiplus::Rect       rect;               // 심볼 영역
//	RECT                rect1;              // CRect 영역
//
//	Gdiplus::Color      inColor;            // 심볼 내부 컬러
//	Gdiplus::Color      outColor;           // 심볼 외부 컬러
//	Gdiplus::Color      edgeColor;          // 심볼 엣지 컬러
//
//	BOOL                bSelectflag;        // 심볼 선택 상태
//	BYTE                Contact;            // 접점 상태 (POSITIVE, NAGATIVE, _INPUT, _OUTPUT, NEUTRAL)
//
//	BYTE                LineType;           // 라인 타입
//	BYTE                childState;         // 자식 갯수 판단
//
//	UINT                Row;                // 열
//	UINT                Col;                // 행
//	UINT                page;               // 페이지 번호
//
//	UINT                TimerKind;
//	UINT                TimeValue;
//} SYMBOL;
//
//typedef struct Symbol_Data_t
//{
//	SYMBOL              Symbol;
//
//	//WCHAR               text[MAX_CHAR_BUFF];    // 텍스트
//	char                text[MAX_CHAR_BUFF];    // 텍스트
//} Symbol_Data;
//
//typedef struct Symbol_Contents_t
//{
//	SYMBOL              Symbol;
//
//	UINT                text_length;
//} Symbol_Contents;
//

//---------------------------------------------------------------------------


#if defined (_WIN32)
#  pragma pack (pop)
#endif


#endif  /* #ifndef __LOGICTYPEDEF__h__ */
