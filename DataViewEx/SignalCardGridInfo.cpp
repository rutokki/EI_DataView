#include "stdafx.h"
#include "SignalCardGridInfo.h"
#include "GridColumnDefine.h"
#include "StructMainData.h"
#include "CommonUtils.h"

using namespace CommonUtil;

BEGIN_MESSAGE_MAP(SignalCardGridInfo, CustomBCGGridCtrl)
	ON_WM_SIZE()
	ON_WM_CREATE()
END_MESSAGE_MAP()

SignalCardGridInfo::SignalCardGridInfo()
{
	EnableRowHeader(TRUE);
	EnableLineNumbers(TRUE);
	CustomBCGGridCtrl::InitGridControl();
}

int SignalCardGridInfo::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CustomBCGGridCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	auto IOCardGrid = GridColumnDefine::GetSignalCardColumnInfo();
	for (int i = 0; i < IOCardGrid.size(); i++) {
		InsertColumn(i, IOCardGrid[i].columnName, IOCardGrid[i].columnWidth);

		SetHeaderAlign(i, HDF_CENTER);
		SetColumnAlign(i, HDF_CENTER);
	}
	return 0;
}
void SignalCardGridInfo::OnSize(UINT nType, int cx, int cy)
{
	CBCGPGridCtrl::OnSize(nType, cx, cy);
}

void SignalCardGridInfo::LoadAllSignalCardData()
{
	//  SIG_CARDTABLE 이 RackNo/SlotNo + CardData[](포트별) 배열을 갖는 중첩 구조로 바뀜
	//  
	auto SignalCardInfo = StructMainData::GetInstance().GetSignalCardInfo();

	if (SignalCardInfo.size() == 0) return;

	auto stationInfo = StructMainData::GetInstance().GetStationInfo();

	UINT nCardNo = 0;          // 논리적 카드 번호 (등록된 카드 순서)
	UINT nRow = 0;              // 그리드 행 번호
	CString strCardKind = _T("");

	// 전체 OutCard(카드 단위) 순회
	for (auto& cardItem : SignalCardInfo) {
		// 카드 없음 체크
		if (cardItem.RackNo == 0) continue;

		nCardNo++;

		// 카드 번호 제한 (증가 직후 체크 - 넘으면 종료)
		if (nCardNo > stationInfo.NoOfModuleSig) break;

		// 카드가 바뀔 때(첫 카드는 제외) 빈 줄 추가
		if (nRow != 0) {
			CBCGPGridRow* pEmptyRow = CreateRow(GetColumnCount());
			AddRow(pEmptyRow, FALSE);
			nRow++;
		}
		if (cardItem.CardKind == 3) {
			strCardKind = _T("4등형 신호기(주신호기, 구내폐색)");
		}
		else if (cardItem.CardKind == 4) {
			strCardKind = _T("2등형 신호기(입환 표지, 신호기)");
		}
		else {
			strCardKind = _T("");
		}
		// 카드 내 포트 순회 (Index : 1부터 사용, 0번째는 미사용)
		for (UINT nPort = 1; nPort < _countof(cardItem.CardData); nPort++) {
			auto& portItem = cardItem.CardData[nPort];

			// 미사용 포트는 건너뜀
			if (portItem.PortNo == 0) continue;

			// 그리드 행 증가
			nRow++;

			CBCGPGridRow* pRow = CreateRow(GetColumnCount());

			// [수정] 컬럼 정의(GridColumnDefine::GetSignalCardColumnInfo, 8개: RackNo/SlotNo/CardNo/PortNo/
			// 신호기 카드 종류/이름/표찰이름/Table Index)와 어긋나 있던 컬럼 인덱스를 맞춤.
			// 기존 코드는 4~8번에 값이 한 칸씩 밀려 들어가고, 존재하지 않는 8번 컬럼에
			// (LPCTSTR)portItem.Idx (Byte_t를 문자열 포인터로 캐스팅) 를 대입해 접근 위반 위험이 있었음.
			pRow->GetItem(0)->SetValue(cardItem.RackNo);              // 랙번호
			pRow->GetItem(1)->SetValue(cardItem.SlotNo);               // 슬롯번호
			pRow->GetItem(2)->SetValue((LONG)nCardNo);              // 카드번호
			pRow->GetItem(3)->SetValue(portItem.PortNo);                // 포트번호
			pRow->GetItem(4)->SetValue((LPCTSTR)strCardKind); // 신호기 카드 종류 (3->4등형 신호기, 4->2등형 신호기)
			pRow->GetItem(5)->SetValue((LPCTSTR)GetName(portItem));     // 이름
			pRow->GetItem(6)->SetValue((LPCTSTR)GetTagName(portItem));  // 표찰이름
			pRow->GetItem(7)->SetValue(portItem.Idx);                   // Table Index (숫자 그대로, 문자열 캐스팅 금지)

			AddRow(pRow, FALSE);
		}
	}
}

void SignalCardGridInfo::UpdateSignalCardData()
{
	RemoveAll();
	LoadAllSignalCardData();
	AdjustLayout();
}
