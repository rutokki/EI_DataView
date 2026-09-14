#include "stdafx.h"
#include "OUTCardGridInfo.h"
#include "GridColumnDefine.h"
#include "StructMainData.h"
#include "CommonUtils.h"

using namespace CommonUtil;
BEGIN_MESSAGE_MAP(OUTCardGridInfo, CustomBCGGridCtrl)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()
OUTCardGridInfo::OUTCardGridInfo()
{
	EnableRowHeader(TRUE);
	EnableLineNumbers(TRUE);
	CustomBCGGridCtrl::InitGridControl();
}

int OUTCardGridInfo::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CustomBCGGridCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	auto IOCardGrid = GridColumnDefine::GetOUTCardColumnInfo();
	for (int i = 0; i < IOCardGrid.size(); i++) {
		InsertColumn(i, IOCardGrid[i].columnName, IOCardGrid[i].columnWidth);

		SetHeaderAlign(i, HDF_CENTER);
		SetColumnAlign(i, HDF_CENTER);
	}
	//LoadAllOutCardData();
}

void OUTCardGridInfo::OnSize(UINT nType, int cx, int cy)
{
	CBCGPGridCtrl::OnSize(nType, cx, cy);
}

void OUTCardGridInfo::LoadAllOutCardData()
{
	// [구조체 변경] OUT_CARDTABLE 이 RackNo/SlotNo + CardData[](포트별) 배열을 갖는 중첩 구조로 바뀜
	//              => 카드 1개(span의 원소 1개) 안에 최대 MAX_IO_CARD_PORT개의 포트가 들어있음
	auto outCardInfo = StructMainData::GetInstance().GetOUTCardInfo();

	if (outCardInfo.size() == 0) return;

	auto stationInfo = StructMainData::GetInstance().GetStationInfo();

	UINT nCardNo = 0;          // 논리적 카드 번호 (등록된 카드 순서)
	UINT nRow = 0;              // 그리드 행 번호

	// 전체 OutCard(카드 단위) 순회
	for (auto& cardItem : outCardInfo) {
		// 카드 없음 체크
		if (cardItem.RackNo == 0) continue;

		nCardNo++;

		// 카드 번호 제한 (증가 직후 체크 - 넘으면 종료)
		if (nCardNo > stationInfo.NoOfOutCard) break;

		// 카드가 바뀔 때(첫 카드는 제외) 빈 줄 추가
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

			CBCGPGridRow* pRow = CreateRow(GetColumnCount());

			pRow->GetItem(0)->SetValue(cardItem.RackNo);              // 랙번호
			pRow->GetItem(1)->SetValue(cardItem.SlotNo);               // 슬롯번호
			pRow->GetItem(2)->SetValue((LONG)nCardNo);              // 카드번호
			pRow->GetItem(3)->SetValue(portItem.PortNo);                // 포트번호 카드당 포트번호 16개

			// 카드 종류
			pRow->GetItem(4)->SetValue((LPCTSTR)GetName(portItem));     // 이름
			pRow->GetItem(5)->SetValue((LPCTSTR)GetBitName(portItem));  // 표찰이름

			// 종류 판별
			pRow->GetItem(6)->SetValue((LPCTSTR)GetKindName(portItem.Kind));// 종류

			// 테이블 번호
			int tableIdx = portItem.Idx;
			pRow->GetItem(7)->SetValue(tableIdx); //인덱스
			pRow->GetItem(8)->SetValue(portItem.BitNo); //비트번호
			CString strOutKind = GetOUTKInd(portItem.Kind, portItem.PORTOUT.OutKind);
			CString strOutGubun = GetOutGubun(portItem.PORTOUT.OutKind, portItem.PORTOUT.OutGubun); // 설비 KInd 별 출력 
			strOutKind.AppendFormat(_T(" : ") + strOutGubun);
			pRow->GetItem(9)->SetValue((LPCTSTR)strOutKind);

			AddRow(pRow, FALSE);
		}
	}
}
void OUTCardGridInfo::UpdateOutCardData()
{
	RemoveAll();
	LoadAllOutCardData();
	AdjustLayout();
}

//
//CString OUTCardGridInfo::GetOutKindName(const OUT_CARDTABLE::_CardData_t& portItem)
//{
//	const auto& portOut = portItem.PORTOUT;
//	if (portOut.OutKind == 0) return CString();
//
//	CString kindStr;
//	CString gubunStr;
//
//	switch (portItem.Kind)
//	{
//	case 'S': // 신호기 : OutKind 1(ATS출력)/2(후방폐색 제어출력)
//		switch (portOut.OutKind)
//		{
//		case 1: kindStr = _T("신호기 ATS 출력"); break;
//		case 2: kindStr = _T("신호기 후방폐색 제어출력"); break;
//		default: kindStr = _T(""); break;
//		}
//		switch (portOut.OutGubun) {
//		case 1: gubunStr = _T("YY"); break;
//		case 2: gubunStr = _T("Y"); break;
//		case 3: gubunStr = _T("YG"); break;
//		case 4: gubunStr = _T("G"); break;
//		default: gubunStr = _T(""); break;
//		}
//		break;
//
//	case 'E': // 비상정지/주계/운영모드 : OutKind 3/5/6
//		switch (portOut.OutKind)
//		{
//		case 3: // 비상정지 출력
//			kindStr = _T("비상정지 출력");
//			switch (portOut.OutGubun) {
//			case 1: gubunStr = _T("상선"); break;
//			case 2: gubunStr = _T("하선"); break;
//			default: gubunStr = _T(""); break;
//			}
//			break;
//		case 5: // 주계 출력
//			kindStr = _T("주계 출력");
//			switch (portOut.OutGubun) {
//			case 1: gubunStr = _T("1계 출력"); break;
//			case 2: gubunStr = _T("2계 출력"); break;
//			default: gubunStr = _T(""); break;
//			}
//			break;
//		case 6: // 운영모드 출력
//			kindStr = _T("운영모드 출력");
//			switch (portOut.OutGubun) {
//			case 1: gubunStr = _T("Local 출력"); break;
//			case 2: gubunStr = _T("CTC 출력"); break;
//			default: gubunStr = _T(""); break;
//			}
//			break;
//		default:
//			kindStr = _T("");
//			break;
//		}
//		break;
//
//	case 'Y': // 임시속도 : OutKind 4
//		if (portOut.OutKind == 4)
//		{
//			kindStr = _T("임시속도 출력");
//			gubunStr = (portOut.OutGubun == 1) ? _T("인접역 임시속도") : _T("자기역 임시속도");
//		}
//		else
//		{
//			kindStr = _T("기타(Kind=Y)");
//		}
//		break;
//
//	default:
//		// [주의] Switch Heater(7), 전차선(8) 출력은 구조체 주석상 대응되는 Kind 표기가 없어
//		//        포트 Kind 와 무관하게 OutKind 값만으로 판별함
//		switch (portOut.OutKind)
//		{
//		case 7: kindStr = _T("Switch Heater 출력"); break;
//		case 8: kindStr = _T("전차선 출력"); break;
//		default: kindStr = _T(""); break;
//		}
//		break;
//	}
//
//	if (!gubunStr.IsEmpty())
//	{
//		CString result;
//		result.Format(_T("%s(%s)"), (LPCTSTR)kindStr, (LPCTSTR)gubunStr);
//		return result;
//	}
//	return kindStr;
//}