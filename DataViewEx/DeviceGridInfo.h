#pragma once
#include "CustomBCGGridCtrl.h"
enum GroupType {
	GROUP_BLOCK = 0, // 폐색
	GROUP_CROSSING, // 건널목
	GROUP_HEATER, //히터
	GROUP_OBSTRUCTION // 지장물
};
enum class BlockKind : Byte_t
{
	DoubleAuto_5Aspect = 1,  // 복선자동 5현시(YY, Y, YG)
	SingleAuto_3Aspect = 2,  // 단선자동 3현시(BR, DR)
	DoubleInterlocking = 3,  // 복선연동
	SingleInterlocking = 4,  // 단선연동
	TokenBlock = 5,  // 통표 폐색
	SubwayBlock = 6,  // 지하철 폐색(서울교통공사)
	DaeyaBlock = 7,  // 대야   폐색
	UiwangBlock = 8,  // 의왕   폐색 : 5현시 default (3현시도 있음 - GetBlcokAspectFromByte 참고)
	TriangleBlock = 9,  // 삼각선 폐색
	Cheongnyangni_Mangu = 10, // 청량리 폐색 (EI_IP_DBStruct_Typedef.h 원본 주석 기준. 청량리-망우 구간을 가리키는 것으로 보임)
	BiDirectional = 11, // 양방향 폐색       (정방향출발 && 역방향장내)
	BiDirectionalRev = 12,// 12 : 양방향 폐색 (정방향장내 && 역방향출발) : 3현시 default
	HighSpeedBlock = 13,  // 고속선 폐색
	// [추가] EI_IP_DBStruct_Typedef.h의 BlkKind 최상단 목록(1~13)에는 없지만, 같은 파일의
	// KindInfo.RevKind 주석에 "양방향 폐색인 경우 설정 (BlkKind=11 or 12 or 14)"라고 14가
	// 언급되어 있고, DeviceGridInfo.cpp의 LoadAllData()도 14를 양방향 폐색으로 처리하고 있음.
	// 다만 14의 정확한 명칭/세부 구분은 구조체 어디에도 정의되어 있지 않아 확인이 필요함.
	BiDirectionalEtc = 14 // 양방향 폐색 계열로 추정 (정확한 명칭 미확인 - 구조체 원본에 14 자체가 이름 없이 언급만 됨)
};
// [삭제] 기존 enum class BlockAspect(1~8)는 실제 DB 필드 BlockTagInfoType::BlockAspect와
// 값 체계가 전혀 다른, 잘못된 모델이었음. 원본 구조체 주석(EI_IP_DBStruct_Typedef.h)에 따르면
// item.BlockAspect는 이 enum이 아니라 그냥 "현시 개수"(2/3/4/5)를 담은 순수 숫자 값이고,
// 실제 신호 입력 이름(Y / BR,DR / YY,Y,YG 등)은 BlkKind와 조합해야만 정해짐.
// 그래서 이 enum은 제거하고, GetBlcokAspectFromByte(BlkKind, 현시개수)로 통합함.
class DeviceGridInfo : public CustomBCGGridCtrl
{
public:
	DeviceGridInfo();
	virtual ~DeviceGridInfo();

	CString GetBlockKindFromByte(Byte_t byte);
	// [수정] item.BlockAspect(현시 개수 2~5)는 BlkKind와 조합해야 실제 신호 입력 이름을 알 수
	// 있으므로, blkKind를 함께 받도록 시그니처 변경. (기존엔 BlockAspect 값을 엉뚱한 enum으로
	// 캐스팅해서 거의 모든 경우에 잘못된 텍스트를 보여주고 있었음)
	CString GetBlcokAspectFromByte(Byte_t blkKind, Byte_t aspectCount);
	//virtual void AddSignalRow();
	//BOOL FillSubitemsByCountry(CBCGPGridRow* pParent, LPCTSTR lpzsCountry);
	virtual int GetExtraHierarchyOffset() const
	{
		return 0; // 강제로 여백 소멸!
	}
public:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	void LoadAllData();
	CString FormatByteArray(const Byte_t* pArr, int nSize);
	void OnSize(UINT nType, int cx, int cy);
	//CString GetSafeString(const Byte_t* pRawData, int nMaxLength);
	CString FormatIOPosition(IO_Position pos);
	//void AddBlockRow();
	CString GetBlockInfoString(BlockTagInfoType* pData);
	CString FormatTrackNamesFromByteArray(const Byte_t* pArr, int nSize);
	CString GetLCListInfoString(LC_CTRL_INFO_TYPE* pData);
	CString GetTrackName(CString TrackIndex);
public:
	void UpdateDeviceData();
	void PrintGrid(LPCTSTR lpszDocName = NULL);          // 인쇄 실행 함수
	void PrintPreviewGrid();                             // 인쇄 미리보기 함수
protected:


public:
	afx_msg void OnDestroy();

	virtual void OnDrawGroupRow(CDC* pDC, CBCGPGridRow* pRow, const CRect& rect);
	// CBCGPGridCtrl 인쇄 가상 함수 오버라이드
	//virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) override;
	//virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo) override;
	//virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) override;

	//// 머리글/바닥글 커스텀 (선택 사항)
	//virtual void OnPrintPageHeader(CDC* pDC, CPrintInfo* pInfo) override;
	//virtual void OnPrintPageFooter(CDC* pDC, CPrintInfo* pInfo) override;
};

