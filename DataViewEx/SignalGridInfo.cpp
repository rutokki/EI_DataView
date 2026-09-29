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
	// [수정] 컬럼 인덱스를 GridColumnDefine::GetSignalColumnInfo() (0~21번) 와 1:1 로 맞춤.
	//  기존 코드는 13~16번에 값이 한 칸씩 밀려 들어가고(현시 수 칸에 진로 수, 신호기 궤도 칸에 현시 수 ...),
	//  존재하지 않는 22~26번 컬럼에 GetItem()->SetValue() 를 호출해 NULL 포인터 접근(크래시) 위험이 있었음.
	//   0:명칭  1~12:신호기 종류  13:현시 수  14:진로 수  15:신호기 궤도  16:전방신호기
	//   17:후방폐색현시  18:ATS출력  19:후방출력  20:중계 신호기  21:방향
	auto signalList = StructMainData::GetInstance().GetSignalInfo();
	for (auto& item : signalList) {
		if (item.Name[0] == 0 || item.Name[0] == 0xff) {
			continue;
		}
		CBCGPGridRow* pRow = CreateRow(GetColumnCount());
		// 신호기 이름 0번
		pRow->GetItem(0)->SetValue((LPCTSTR)CommonUtil::GetSafeString(item.Name, 20));

		// 신호기 종류 1번 ~ 12번
		SetSignalType(pRow, item);

		// 현시 수 13번 (2:2현시,3:3현시,4:4현시,5:5현시)
		pRow->GetItem(13)->SetValue(item.NoOfLight);
		// 진로 수 14번
		pRow->GetItem(14)->SetValue(item.NoOfRoute);
		// 신호기 궤도 15번
		pRow->GetItem(15)->SetValue((LPCTSTR)CommonUtil::GetDBNameByNumber(item.TrackNo, GetDBNameByNum::TrackIdx));
		// 전방 신호기 16번
		pRow->GetItem(16)->SetValue((LPCTSTR)CommonUtil::GetDBNameByNumber(item.FrontSignalNo, GetDBNameByNum::SignalIdx));
		// 후방 폐색 현시 수 17번 (후방 폐색신호기가 없으면 0)
		if (item.RearBlockAspect != 0) pRow->GetItem(17)->SetValue(item.RearBlockAspect);
		// ATS 출력 있음 18번
		if (item.SignalOut.SigOutATS & 0x01) pRow->GetItem(18)->SetValue(_T("O"));
		// 후방제어 폐색제어 출력 있음 19번
		if (item.SignalOut.SigOutRear & 0x01) pRow->GetItem(19)->SetValue(_T("O"));
		// 중계 신호기 번호 20번 (중계 신호기 포함일 때)
		if (item.RepeatSigNo != 0 && item.RepeatSigNo != 0xFF)
			pRow->GetItem(20)->SetValue((LPCTSTR)CommonUtil::GetDBNameByNumber(item.RepeatSigNo, GetDBNameByNum::SignalIdx));
		// 방향 21번 (SigDir bit1=1 : 상행, bit2=1 : 하행)
		CString strDir;
		if (item.Kind.SigDir & SignalInfo::SigDir)  strDir = _T("상행");
		if (item.Kind.SigDir & SignalInfo::SigDir2) strDir += strDir.IsEmpty() ? _T("하행") : _T("/하행");
		pRow->GetItem(21)->SetValue((LPCTSTR)strDir);
		SetDebugIdx(pRow, (int)(&item - signalList.data())); // [DEBUG-IDX] _SIG_Info 배열 인덱스

		AddRow(pRow, FALSE);
	}
}

void SignalGridInfo::SetSignalType(CBCGPGridRow* pRow, SignalInfoType& sigItem)
{
	// [수정] 배열 순서(i + 1) == 컬럼 번호(1~12) 가 되도록 GridColumnDefine 의 "신호기 종류" 칼럼 순서와 맞춤.
	//  기존에는 입환 신호기의 무유도 IN/OUT 비트, 상행/하행 비트가 중간에 끼어 있어
	//  폐색/구내폐색/유도등/중계 ... 가 모두 한 칸 이상 밀려 다른 칼럼에 표시되었음.
	SGBitCheckInfo signalList[] = {
		{&sigItem.Kind.MainS, SignalInfo::MAINSBIT1},      //  1 주 신호기
		{&sigItem.Kind.ShuntD, SignalInfo::SHUNTD},        //  2 입환 표지
		{&sigItem.Kind.ShuntS, SignalInfo::SHUNTS},        //  3 입환 신호기
		{&sigItem.Kind.BlockS, SignalInfo::BLOCKS},        //  4 폐색 신호기
		{&sigItem.Kind.HomeBlockS, SignalInfo::HOMEBLOCKS},//  5 구내 폐색 신호기
		{&sigItem.Kind.CallOnS, SignalInfo::CALLONS},      //  6 유도등포함(주 신호기)
		{&sigItem.Kind.RepeatS, SignalInfo::REPEATS},      //  7 중계 신호기
		{&sigItem.Kind.RepeatS, SignalInfo::REPEATS2},     //  8 중계 신호기 포함
		{&sigItem.Kind.UmhoSig, SignalInfo::UMHOSIG},      //  9 엄호 신호기
		{&sigItem.Kind.IsTTB, SignalInfo::ISTTB},          // 10 TTB 존재
		{&sigItem.Kind.CptSignal, SignalInfo::CPTSIGNAL},  // 11 CPT 신호기
		{&sigItem.Kind.SpcSignal, SignalInfo::SPCSIGNAL},  // 12 타역 신호기
	};
	static_assert(_countof(signalList) == MAIN_SIGNAL_TYPE, "signalList 와 MAIN_SIGNAL_TYPE 개수 불일치");

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

	// 부가 정보는 셀 색상만으로는 알 수 없으므로 글자가 보이도록 표시
	auto SetExtraText = [&](int nCol, const CString& strText)
	{
		CBCGPGridItem* pItem = pRow->GetItem(nCol);
		if (pItem == nullptr) return;
		pItem->SetValue((LPCTSTR)strText);
		pItem->SetTextColor(RGB(0, 0, 0));
		pItem->SetBackgroundColor(RGB(80, 205, 80));
	};

	// 주 신호기 bit1 : 진로 선별등 있음
	// (기존에는 "선별등o" 를 쓴 뒤 bit0 루프에서 "O" 로 덮어써 표시되지 않았음)
	if (sigItem.Kind.MainS & SignalInfo::MAINSBIT2)
	{
		SetExtraText(1, _T("선별등"));
	}
	// 입환 신호기 bit1 : OUT 카드 무유도 출력 / bit2 : IN 카드 무유도 입력
	if (sigItem.Kind.ShuntS & (SignalInfo::SHUNTS1 | SignalInfo::SHUNTS2))
	{
		CString strShunt = _T("무유도");
		if (sigItem.Kind.ShuntS & SignalInfo::SHUNTS1) strShunt += _T(" OUT");
		if (sigItem.Kind.ShuntS & SignalInfo::SHUNTS2) strShunt += _T(" IN");
		SetExtraText(3, strShunt);
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
	for (int j = 18; j <= 19; j++) arrSignalOutPutCols.Add(j); // [수정] ATS출력/후방출력 (기존 17~21 은 실제 출력 칼럼과 어긋나 있었음)
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