#include "stdafx.h"
#include "TrackGridInfo.h"
#include "GridColumnDefine.h"
#include "StructMainData.h"
#include "CommonUtils.h"

using namespace CommonUtil;
struct TKBitCheckInfo {
	UCHAR* pValue; // 변수의 주소
	UINT   bit;    // 체크할 비트 상수
};
BEGIN_MESSAGE_MAP(TrackGridInfo, CBCGPGridCtrl)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

TrackGridInfo::TrackGridInfo()
{
	m_Column.SetOwnerList(this);
	CustomBCGGridCtrl::InitGridControl();
	EnableRowHeader(TRUE);
	EnableLineNumbers(TRUE);
}
CString TrackGridInfo::GetSafeString(const unsigned char* pRawData, int nMaxLength)
{
	if (pRawData == nullptr || nMaxLength <= 0)
		return _T("");

	// unsigned char*를 const char*로 명시적 변환
	const char* pCharData = reinterpret_cast<const char*>(pRawData);

	// CStringA를 사용하여 nMaxLength만큼만 읽어들임 (바이너리 데이터 안전 처리)
	CStringA strAnsi(pCharData, nMaxLength);

	// 유니코드 프로젝트라면 자동으로 유니코드로 변환됨
	return CString(strAnsi);
}
void TrackGridInfo::LoadAllTrackData()
{
	auto trackList = StructMainData::GetInstance().GetTrackInfo();

	for (auto& item : trackList) {
		if (item.Name[0] == 0 || item.Name[0] == 0xFF) {
			continue;
		}
		CBCGPGridRow* pRow = CreateRow(GetColumnCount());
		pRow->GetItem(0)->SetValue((LPCTSTR)GetSafeString(item.Name, 20)); // 궤도 명
		SetItemType(pRow, item); // 1~17번 칼럼 궤도 타입
		pRow->GetItem(19)->SetValue((LPCTSTR)GetSafeString(trackList[item.LeftTrack].Name, 20)); //left
		pRow->GetItem(20)->SetValue((LPCTSTR)GetSafeString(trackList[item.RightTrack].Name, 20)); // right
		CString closeSignal = GetDBNameByArray(item.IncSignal, TrackIdx);
		pRow->GetItem(21)->SetValue((LPCTSTR)closeSignal);// 관련 신호기
		pRow->GetItem(22)->SetValue((LPCTSTR)LoadAllSwitches(item.Switch, NO_OF_SWITCH)); // 선로전환기
		pRow->GetItem(23)->SetValue(item.Equipment.NoOfSignal); // 신호기 수
		pRow->GetItem(24)->SetValue((item.Equipment.NoOfSwitch)); // 선로전환기 수
		pRow->GetItem(25)->SetValue((LPCTSTR)GetBlockName(item.InBlockNo)); // 폐색

		pRow->GetItem(26)->SetValue((LPCTSTR)GetAppDelayTime(item.TrkDelayTime.RecvTime)); // 복구Delay

		AddRow(pRow, FALSE);
	}
}

void TrackGridInfo::UpdateTrackData()
{
	RemoveAllData();
	LoadAllTrackData();
	AdjustLayout();
}

CString TrackGridInfo::GetBlockName(Byte_t blockNo)
{
	auto blockList = StructMainData::GetInstance().GetBlockInfo();
	return 	GetSafeString(blockList[blockNo].Name, 20);
}

CString TrackGridInfo::GetAppDelayTime(Byte_t& time)
{
	//Byte_t  RecvTime;            // 0x00   : 복구 시소시간 2000 ms (default)
 //                              // bit0=1 : 복구 시소시간    0 초 - 사용하지 않음
 //                              // bit1=1 : 복구 시소시간  500 ms
 //                              // bit2=1 : 복구 시소시간 1000 ms
 //                              // bit3=1 : 복구 시소시간 1200 ms
 //                              // bit4=1 : 복구 시소시간 1500 ms
 //                              // bit5=1 : 복구 시소시간 1800 ms
 //                              // bit6=1 : 복구 시소시간 2500 ms
 //                               //bit7=1 : 복구 시소시간 3000 ms
	int nTime = 0;
	if (time == TrackInfo::APP_TIME_DEFAULT) nTime = 2000;
	//if (time & TrackInfo::APP_TIME_0MS_BIT0)    nTime = 0;
	else if (time & TrackInfo::APP_TIME_500MS_BIT1)  nTime = 500;
	else if (time & TrackInfo::APP_TIME_1000MS_BIT2) nTime = 1000;
	else if (time & TrackInfo::APP_TIME_1200MS_BIT3) nTime = 1200;
	else if (time & TrackInfo::APP_TIME_1500MS_BIT4) nTime = 1500;
	else if (time & TrackInfo::APP_TIME_1800MS_BIT5) nTime = 1800;
	else if (time & TrackInfo::APP_TIME_2500MS_BIT6) nTime = 2500;
	else if (time & TrackInfo::APP_TIME_3000MS_BIT7) nTime = 3000;

	// 2. CString으로 변환하여 반환
	CString strResult;
	strResult.Format(_T("%d ms"), nTime);
	return strResult;
}

void TrackGridInfo::SetItemType(CBCGPGridRow* pRow, TrackInfoType& trackItem)
{
	TKBitCheckInfo checkList[] = {
	{ &trackItem.Kind.ApproachT,   TrackInfo::APPROACH_T_BIT0},
	{ &trackItem.Kind.DetectorT,   TrackInfo::DETECTOR_T_BIT0},
	{ &trackItem.Kind.MainT,       TrackInfo::MAIN_T_BIT0 },
	{ &trackItem.Kind.BlockT,      TrackInfo::BLOCK_T_BIT0},
	{ &trackItem.Kind.IncSignalT,  TrackInfo::INC_SIGNAL_T_BIT0 },
	{ &trackItem.Kind.IncSignalT,  TrackInfo::INC_SIGNAL_T_BIT1 },
	{ &trackItem.Kind.IncSwitchT,  TrackInfo::INC_SWITCH_T_BIT0 },
	{ &trackItem.Kind.FirstT,      TrackInfo::FIRST_T_BIT0},
	{ &trackItem.Kind.LockT,       TrackInfo::LOCK_T_BIT0},
	{ &trackItem.Kind.CptTrack,    TrackInfo::CPT_TRACK_BIT0 },
	{ &trackItem.Kind.SideTrack,   TrackInfo::SIDE_TRACK_BIT0},
	{ &trackItem.Kind.SideTrack,   TrackInfo::SIDE_TRACK_BIT1},
	{ &trackItem.Kind.SideTrack,   TrackInfo::SIDE_TRACK_BIT2},
	{ &trackItem.Kind.SpcTrack,    TrackInfo::SPC_TRACK_BIT0},
	{ &trackItem.Kind.VirtualTrk,  TrackInfo::VIRTUAL_TRK_BIT0},
	{ &trackItem.Kind.VirtualTrk,  TrackInfo::VIRTUAL_TRK_BIT1},
		{&trackItem.PlatForm.Kind.UpTrack, TrackInfo::PLATFORM_UP_BIT0},
		{&trackItem.PlatForm.Kind.UpTrack, TrackInfo::PLATFORM_DOWN_BIT0}
	};
	for (int i = 0; i < MAX_TRACK_TYPE; i++) {
		if (*checkList[i].pValue & checkList[i].bit) {
			// 특정 열(Column)의 셀 색상을 변경
			// 예를 들어 각 타입이 1번 열부터 시작한다고 가정하면 i + 1
			CBCGPGridItem* pItem = pRow->GetItem(i + 1);
			if (pItem != nullptr) {
				pItem->SetValue(_T("O")); // 표시 문자 설정
				pItem->SetTextColor(RGB(80, 205, 80)); // 배경색 글자)
				pItem->SetBackgroundColor(RGB(80, 205, 80)); // 녹색으로 설정
				//pItem->SetvisualMan(TRUE); // 색상 적용 활성화
			}
		}
	}
}

CString TrackGridInfo::LoadAllSwitches(const _TrackInfoType_t::_Switch_t* pSwitchArray, int nCount)
{
	CString strResult = _T("");
	bool bFirst = true;
	auto switchList = StructMainData::GetInstance().GetSwitchInfo();
	// 구조체 배열 전체를 순회
	for (int i = 0; i < nCount; i++) {
		// 각 구조체 안의 SwitchNo를 가져옴
		Byte_t nSwitchNo = pSwitchArray[i].SwitchNo;

		// 0이나 0xFF 같은 더미 데이터는 제외 (원치 않으시면 이 줄을 지우세요)
		if (nSwitchNo == 0 || nSwitchNo == 0xFF) continue;

		CString strNum;
		strNum.Format(GetSafeString(switchList[nSwitchNo].Name, 20));

		if (!bFirst) {
			strResult += _T(", ");
		}

		strResult += strNum;
		bFirst = false;
	}

	return strResult;
}

CString TrackGridInfo::GetSignalName(Byte_t* pSignalArray, int nSize)
{
	CString strTotalNames = _T("");
	bool bFound = false;
	auto SignalList = StructMainData::GetInstance().GetSignalInfo();
	for (int i = 0; i < nSize; i++) {
		Byte_t nSigNo = pSignalArray[i];

		if (nSigNo == 0 || nSigNo == 0xFF) continue;
		if (nSigNo >= SignalList.size()) continue;

		CString strName = GetSafeString(SignalList[nSigNo].Name, 20);

		if (bFound) {
			strTotalNames += _T(", ");
		}
		strTotalNames += strName;
		bFound = true;
	}

	// 하나도 없으면 "없음" 혹은 공백 반환
	return strTotalNames;
}

CRect TrackGridInfo::OnGetHeaderRect(CDC* pDC, const CRect& rectDraw) {
	CRect rect = CBCGPGridCtrl::OnGetHeaderRect(pDC, rectDraw);
	// 2단 헤더를 위해 높이 확장
	rect.bottom = rect.top + (rect.Height() * m_Column.GetHeaderLineCount());
	return rect;
}

void TrackGridInfo::OnDrawHeader(CDC* pDC) {
	m_Column.PrepareDrawHeader();
	CBCGPGridCtrl::OnDrawHeader(pDC);
}

void TrackGridInfo::OnPrintHeader(CDC* pDC, CPrintInfo* pInfo) {
	m_Column.PreparePrintHeader();
	CBCGPGridCtrl::OnPrintHeader(pDC, pInfo);
}

void TrackGridInfo::OnPosSizeChanged() {
	CBCGPGridCtrl::OnPosSizeChanged();
	m_Column.ReposHeaderItems();
}

int TrackGridInfo::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	if (CBCGPGridCtrl::OnCreate(lpCreateStruct) == -1) return -1;

	m_Column.SetHeaderLineCount(2);

	//상세 헤더 병합 설정 (2번~15번 컬럼)
	CArray<int, int> arrDetailCols;
	CArray<int, int> arrCountCols;
	for (int i = 1; i <= 18; i++) arrDetailCols.Add(i);
	for (int j = 23; j <= 24; j++) arrCountCols.Add(j);
	CArray<int, int> arrDetailLines;
	arrDetailLines.Add(0); // 상단 그룹 헤더는 0번 라인

	// 그룹 추가: 궤도 종류 상세정보
	m_Column.AddHeaderItem(&arrDetailCols, &arrDetailLines, 2, _T("궤도 종류"), HDF_CENTER, -1);
	m_Column.AddHeaderItem(&arrCountCols, &arrDetailLines, -1, _T("Count"), HDF_CENTER, -1);
	auto TrackColumn = GridColumnDefine::GetTrackColumnInfo();
	// 1. 컬럼 추가
	for (int i = 0; i < (int)TrackColumn.size(); i++) {
		InsertColumn(i, TrackColumn[i].columnName, TrackColumn[i].columnWidth);
		//m_Column.SetColumnWidthAutoSize(i, TRUE);
		SetHeaderAlign(i, HDF_CENTER);
		SetColumnAlign(i, HDF_CENTER);
	}

	//LoadAllTrackData();
	AdjustLayout();
	return 0;
}
void TrackGridInfo::OnSize(UINT nType, int cx, int cy)
{
	CBCGPGridCtrl::OnSize(nType, cx, cy);

	if (GetSafeHwnd() == NULL || cx <= 0 || cy <= 0) return;

	// 컬럼의 현재 유효 너비를 계산
	// 컬럼들이 0으로 찌그러지지 않게 강제로 너비를 할당
	//int nColCount = GetColumnCount();
	//if (nColCount > 0)
	//{
	//	// 예시: 1/N 등분 (혹은 GridColumnDefine에서 정의한 비율을 사용하세요)
	//	int nTotalWidth = cx - 20; // 스크롤바 감안 여유
	//	int nWidth = nTotalWidth / nColCount;

	//	for (int i = 0; i < nColCount; i++)
	//	{
	//		SetColumnWidth(i, nWidth);
	//	}
	//}
	m_Column.ReposHeaderItems(); // 다시 계산된 정보를 바탕으로 위치 재배치

	// 5. 마지막으로 전체 그리드 재조정
	AdjustLayout();
	Invalidate(); // 강제 다시 그리기
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
void TrackGridInfo::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// ★ 그리드 내부이므로 접근 가능: pInfo가 NULL이 되는 것을 방지
	if (pInfo != nullptr)
	{
		m_PrintParams.m_pPrintInfo = pInfo;
	}

	CBCGPGridCtrl::OnBeginPrinting(pDC, pInfo);
}

void TrackGridInfo::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	CBCGPGridCtrl::OnPrint(pDC, pInfo);
}

void TrackGridInfo::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	CBCGPGridCtrl::OnEndPrinting(pDC, pInfo);
}