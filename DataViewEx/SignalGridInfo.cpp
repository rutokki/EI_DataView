#include "stdafx.h"
#include "SignalGridInfo.h"
#include "GridColumnDefine.h"
#include "StructMainData.h"
#include "CommonUtils.h"

using namespace CommonUtil;



BEGIN_MESSAGE_MAP(SignalGridInfo, CustomBCGGridCtrl)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

SignalGridInfo::SignalGridInfo() {
	m_Column.SetOwnerList(this);
	CustomBCGGridCtrl::InitGridControl();
	EnableRowHeader(TRUE);
	EnableLineNumbers(TRUE);
}

void SignalGridInfo::LoadAllSignal()
{
	auto signalList = StructMainData::GetInstance().GetSignalInfo();
	for (auto& item : signalList) {
		if (item.Name[0] == 0 || item.Name[0] == 0xff) {
			continue;
		}
		CBCGPGridRow* pRow = CreateRow(GetColumnCount());
		// 신호기 이름 0번
		pRow->GetItem(0)->SetValue((LPCTSTR)CommonUtil::GetSafeString(item.Name, 20));

		// 신호기 타입 1번 ~ 12번
		SetSignalType(pRow, item);
		// 신호기 TTB 13번
		if (item.Kind.IsTTB & SignalInfo::ISTTB)
		{
			pRow->GetItem(13)->SetValue(_T("T"));
		}
		// 신호기 진로 수 14번
		pRow->GetItem(14)->SetValue(item.NoOfRoute);
		// 신호기 현시 수 15번
		pRow->GetItem(15)->SetValue(item.NoOfLight);
		// 신호기 궤도명 16번
		CString strTrackNames = CommonUtil::GetDBNameByNumber(item.TrackNo, GetDBNameByNum::TrackIdx);
		pRow->GetItem(16)->SetValue((LPCTSTR)strTrackNames);

		// [구조체 변경] item.OutHR / item.OutSignal[] 는 현재 EI_IP_DBStruct_Typedef.h 의
		//              SignalInfoType 에 없는 필드라 제외함
		//              (SignalOut 이 SigOutATS/SigOutRear 플래그만 갖는 구조로 바뀌었고,
		//               출력 카드 위치(IO_Position) 정보는 더 이상 SignalInfoType 에 없음)
		// 신호기 HR 17번
		// 신호기 G 18번

		pRow->GetItem(22)->SetValue((LPCTSTR)CommonUtil::GetDBNameByNumber(item.RepeatSigNo, GetDBNameByNum::SignalIdx));
		// 신호기 Y 19번
		// 신호기 착점 20번
		// 신호기 유도 21번
		// 신호기 상대 22번

		// [추가] 구조체에는 있으나 기존 코드에서 빠져있던 필드들 (23~26번 컬럼 신규 추가)
		// 전방 신호기 23번
		pRow->GetItem(23)->SetValue((LPCTSTR)CommonUtil::GetDBNameByNumber(item.FrontSignalNo, GetDBNameByNum::SignalIdx));
		// 후방 폐색 현시 수 24번 (2:2현시,3:3현시,4:4현시,5:5현시)
		pRow->GetItem(24)->SetValue(item.RearBlockAspect);
		// ATS 출력 있음 25번
		if (item.SignalOut.SigOutATS & 0x01) pRow->GetItem(25)->SetValue(_T("O"));
		// 후방제어 폐색제어 출력 있음 26번
		if (item.SignalOut.SigOutRear & 0x01) pRow->GetItem(26)->SetValue(_T("O"));

		AddRow(pRow, FALSE);
	}
}

void SignalGridInfo::SetSignalType(CBCGPGridRow* pRow, SignalInfoType& sigItem)
{
	SGBitCheckInfo signalList[] = {
		{&sigItem.Kind.MainS, SignalInfo::MAINSBIT1}, // 주 신호기
		{&sigItem.Kind.ShuntD, SignalInfo::SHUNTD}, // 입환표지
		{&sigItem.Kind.ShuntS, SignalInfo::SHUNTS}, // 입환 신호기 bit0 
		{&sigItem.Kind.ShuntS, SignalInfo::SHUNTS1}, // OUT 카드 무유도 출력
		{&sigItem.Kind.ShuntS, SignalInfo::SHUNTS2}, // IN 카드 무유도 출력
		{&sigItem.Kind.BlockS, SignalInfo::BLOCKS}, // 폐색 신호기
		{&sigItem.Kind.HomeBlockS, SignalInfo::HOMEBLOCKS}, // 구내 폐색 신호기
		{&sigItem.Kind.CallOnS, SignalInfo::CALLONS}, // 유도등포함(주 신호기)
		{&sigItem.Kind.RepeatS, SignalInfo::REPEATS}, // 중계신호기
		{&sigItem.Kind.RepeatS, SignalInfo::REPEATS2}, // 중계신호기 포함
		{&sigItem.Kind.UmhoSig, SignalInfo::UMHOSIG}, // 엄호신호기
		{&sigItem.Kind.IsTTB, SignalInfo::ISTTB}, // TTB 존재
		{&sigItem.Kind.CptSignal, SignalInfo::CPTSIGNAL}, // CPT 신호기
		{&sigItem.Kind.SpcSignal, SignalInfo::SPCSIGNAL}, // 타역 신호기
		{&sigItem.Kind.SigDir, SignalInfo::SigDir}, // 상행 
		{&sigItem.Kind.SigDir, SignalInfo::SigDir2} // 하행
	};
	if (sigItem.Kind.MainS & SignalInfo::MAINSBIT2)
	{
		pRow->GetItem(1)->SetValue(_T("선별등o"));
	}
	if (sigItem.Kind.MainS & SignalInfo::SHUNTS1) {

	}
	if (sigItem.Kind.MainS & SignalInfo::SHUNTS2)
	{

	}
	//pRow->GetItem(1)->SetValue(_T("선별등o"));
	for (int i = 0; i < MAIN_SIGNAL_TYPE; i++) {
		if (*signalList[i].pValue & signalList[i].bit) {

			CBCGPGridItem* pItem = pRow->GetItem(i + 1);
			if (pItem != nullptr) {
				pItem->SetValue(_T("O"));
				pItem->SetTextColor((RGB(80, 205, 80)));
				pItem->SetBackgroundColor((RGB(80, 205, 80)));
			}
		}
	}
}


CRect SignalGridInfo::OnGetHeaderRect(CDC* pDC, const CRect& rectDraw)
{
	CRect rect = CBCGPGridCtrl::OnGetHeaderRect(pDC, rectDraw);
	// 2단 헤더를 위해 높이 확장
	rect.bottom = rect.top + (rect.Height() * m_Column.GetHeaderLineCount());
	return rect;
}

void SignalGridInfo::OnDrawHeader(CDC* pDC)
{
	m_Column.PrepareDrawHeader();
	CBCGPGridCtrl::OnDrawHeader(pDC);
}

void SignalGridInfo::OnPrintHeader(CDC* pDC, CPrintInfo* pInfo)
{
	m_Column.PreparePrintHeader();
	CBCGPGridCtrl::OnPrintHeader(pDC, pInfo);
}

void SignalGridInfo::OnPosSizeChanged()
{
	CBCGPGridCtrl::OnPosSizeChanged();
	m_Column.ReposHeaderItems();
}

void SignalGridInfo::UpdateSignalData()
{
	RemoveAll();
	LoadAllSignal();
	AdjustLayout();
}

int SignalGridInfo::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CustomBCGGridCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_Column.SetHeaderLineCount(2);

	//상세 헤더 병합 설정 (2번~15번 컬럼)
	CArray<int, int> arrSignalTypeCols;
	CArray<int, int> arrSignalOutPutCols;
	for (int i = 1; i <= 12; i++) arrSignalTypeCols.Add(i);
	for (int j = 17; j <= 21; j++) arrSignalOutPutCols.Add(j);
	CArray<int, int> arrDetailSignalTypeLines;
	CArray<int, int> arrDetailSignalOutPutLines;
	arrDetailSignalTypeLines.Add(0); // 상단 그룹 헤더는 0번 라인
	arrDetailSignalOutPutLines.Add(0);
	// 그룹 추가: 궤도 종류 상세정보
	m_Column.AddHeaderItem(&arrSignalTypeCols, &arrDetailSignalTypeLines, 2, _T("신호기 종류"), HDF_CENTER, -1);
	m_Column.AddHeaderItem(&arrSignalOutPutCols, &arrDetailSignalOutPutLines, -1, _T("신호기 출력"), HDF_CENTER, -1);

	auto TrackColumn = GridColumnDefine::GetSignalColumnInfo();
	// 1. 컬럼 추가
	for (int i = 0; i < (int)TrackColumn.size(); i++) {
		InsertColumn(i, TrackColumn[i].columnName, TrackColumn[i].columnWidth);

		SetHeaderAlign(i, HDF_CENTER);
		SetColumnAlign(i, HDF_CENTER);
	}

	//LoadAllSignal();
	AdjustLayout();
	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	return 0;
}

void SignalGridInfo::OnSize(UINT nType, int cx, int cy)
{
	CBCGPGridCtrl::OnSize(nType, cx, cy);

	m_Column.ReposHeaderItems(); // 다시 계산된 정보를 바탕으로 위치 재배치

	// 5. 마지막으로 전체 그리드 재조정
	AdjustLayout();
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}