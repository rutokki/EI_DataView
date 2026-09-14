#include "stdafx.h"
#include "INCardGridInfo.h"
#include "GridColumnDefine.h"
#include "StructMainData.h"
#include "CommonUtils.h"

using namespace CommonUtil;

BEGIN_MESSAGE_MAP(INCardGridInfo, CBCGPGridCtrl)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//ON_REGISTERED_MESSAGE(BCGM_CHANGEVISUALMANAGER, &INCardGridInfo::OnChangeVisualManager)
END_MESSAGE_MAP()
INCardGridInfo::INCardGridInfo()
{
	EnableRowHeader(TRUE);
	EnableLineNumbers(TRUE);
	SetReadOnly(TRUE);
	SetVisualManagerColorTheme(TRUE, TRUE, TRUE);
	EnableColumnsResizeInClientArea(FALSE);
	EnableDragHeaderItems(FALSE);
	EnableMarkSortedColumn(FALSE);
	SetClearInplaceEditOnEnter(FALSE);

}

int INCardGridInfo::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPGridCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	auto IOCardGrid = GridColumnDefine::GetINCardColumnInfo();
	for (int i = 0; i < IOCardGrid.size(); i++) {
		InsertColumn(i, IOCardGrid[i].columnName, IOCardGrid[i].columnWidth);

		SetHeaderAlign(i, HDF_CENTER);
		SetColumnAlign(i, HDF_CENTER);
	}
	//LoadAllInCardData();
}

void INCardGridInfo::OnSize(UINT nType, int cx, int cy)
{
	CBCGPGridCtrl::OnSize(nType, cx, cy);
}

void INCardGridInfo::UpdateInCardData()
{
	RemoveAll();
	LoadAllInCardData();
	AdjustLayout();
}

void INCardGridInfo::LoadAllInCardData()
{
	// StructMainData 싱글톤 인스턴스 획득
	auto& mainData = StructMainData::GetInstance();

	// InCard 정보 조회
	// [구조체 변경] IN_CARDTABLE 이 RackNo/SlotNo + CardData[](포트별) 배열을 갖는 중첩 구조로 바뀜
	//              => 카드 1개(span의 원소 1개) 안에 최대 MAX_IO_CARD_PORT개의 포트가 들어있음
	auto inCardInfo = mainData.GetINCardInfo();
	if (inCardInfo.size() == 0) return;

	// 스테이션 정보 조회
	auto stationInfo = mainData.GetStationInfo();

	UINT nCardNo = 0;          // 논리적 카드 번호 (등록된 카드 순서)
	UINT nRow = 0;              // 그리드 행 번호

	// 전체 InCard(카드 단위) 순회
	for (auto& cardItem : inCardInfo) {
		// 카드 없음 체크
		if (cardItem.RackNo == 0) continue;

		nCardNo++;

		// 카드 번호 제한
		if (nCardNo > stationInfo.NoOfInCard) break;

		// 카드가 바뀔 때(첫 카드는 제외) 빈 줄 추가하여 카드 단위 구분
		if (nRow != 0) {
			CBCGPGridRow* pEmptyRow = CreateRow(GetColumnCount());
			AddRow(pEmptyRow, FALSE);
			nRow++;
		}

		// 카드 내 포트 순회 (Index : 1부터 사용, 0번째는 미사용)
		for (UINT nPort = 1; nPort < _countof(cardItem.CardData); nPort++) {
			auto& portItem = cardItem.CardData[nPort];

			// 미사용 포트는 건너뜀
			if (portItem.PortNo == 0) continue;

			// 그리드 행 증가
			nRow++;

			// 그리드에 데이터 추가
			CBCGPGridRow* pRow = CreateRow(GetColumnCount());

			// 각 컬럼 설정
			pRow->GetItem(0)->SetValue(cardItem.RackNo);                    // Rack No
			pRow->GetItem(1)->SetValue(cardItem.SlotNo);                    // Slot No
			pRow->GetItem(2)->SetValue((LONG)nCardNo);                      // Card No
			pRow->GetItem(3)->SetValue(portItem.PortNo);                    // Port No
			pRow->GetItem(4)->SetValue((LPCTSTR)GetName(portItem));         // 이름
			pRow->GetItem(5)->SetValue((LPCTSTR)GetBitName(portItem));      // 비트이름
			pRow->GetItem(6)->SetValue((LPCTSTR)GetKindName(portItem.Kind));// 종류
			pRow->GetItem(7)->SetValue(portItem.Idx);                       // Table Index
			pRow->GetItem(8)->SetValue(portItem.BitNo);                     // BitNo

			AddRow(pRow, FALSE);
		}
	}
}
//LRESULT INCardGridInfo::OnChangeVisualManager(WPARAM wParam, LPARAM lParam)
//{
//	// 기본 메시지 처리를 수행합니다.
//	Default();
//
//	// 💡 RightView 내부에 있는 그리드나 컨트롤의 테마를 여기서 갱신합니다!
//	// 예시: m_wndGrid.RedrawWindow(); 등 그리드 갱신 함수 호출
//	RedrawWindow();
//	// 뷰 전체 화면을 강제로 다시 그려서 색상 반영
//	Invalidate();
//	UpdateWindow();
//
//	return 0;
//}